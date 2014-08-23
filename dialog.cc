#include <CoreFoundation/CoreFoundation.h>
#include <nan.h>

using v8::Local;
using v8::Array;
using v8::Object;
using v8::String;
using v8::Value;

CFMutableArrayRef convert_checkboxes(Local<Array>, int);

class DialogWorker : public NanAsyncWorker {
  public:
    DialogWorker(NanCallback *callback, CFUserNotificationRef un, int boxCount, int timeout)
      : NanAsyncWorker(callback), un(un), boxCount(boxCount), timeout(timeout) {}
    ~DialogWorker() {}

    // v8 stuff is not available
    void Execute () {
      error = CFUserNotificationReceiveResponse(un, timeout, &flags);
    }

    // v8 stuff is available.
    void HandleOKCallback () {
      NanScope();
      if (error != 0) {
        CFUserNotificationCancel(un);

        Local<Value> argv[] = {
          NanNew<String>("timeout")
        };
        callback->Call(1, argv);
        return;
      }

      bool dismissal = (flags & 0x3) == 0;

      if (boxCount == 0) {
        Local<Value> argv[] = {
          NanNull(),
          NanNew<v8::Boolean>(dismissal)
        };
        callback->Call(2, argv);

      } else {
        Local<Array> boxes = NanNew<Array>(boxCount);

        for (int i = 0; i < boxCount; i++) {
          bool checkbox = (flags & CFUserNotificationCheckBoxChecked(i)) != 0;
          boxes->Set(i, NanNew<v8::Boolean>(checkbox));
        }

        Local<Value> argv[] = {
          NanNull(),
          NanNew<v8::Boolean>(dismissal),
          boxes
        };
        callback->Call(3, argv);
      }
    };

  private:
    CFUserNotificationRef un;
    CFOptionFlags flags;
    int boxCount;
    int timeout;
    SInt32 error;
};

/**
 * Show takes an options object and a callback. It then displays a dialog box
 * to the user. Calling the callback when it is done.
 *
 * Object options: has 3 possible properties.
 *   'title' is a string that is the title of the dialog box.
 *   'message' is a string that is the message of the dialog box.
 *   'checkboxes' is an array of strings for the dialog to show (optional)
 *
 * Function callback: is a function that takes 6 args, the first is whether
 *                    the 'ok' button is pressed. The next 5 are the states of
 *                    the checkboxes in the same order as were passed in.
 */

NAN_METHOD(Show) {
  Local<Object> options(args[0]->ToObject());
  String::Utf8Value title(options->Get(String::NewSymbol("title"))->ToString());
  char *c_title = *title;
  String::Utf8Value msg(options->Get(String::NewSymbol("msg"))->ToString());
  char *c_msg = *msg;
  Local<Array> checkboxes = Local<Array>::Cast(options->Get(String::NewSymbol("checkboxes")));
  Local<Value> timeoutThing = (options->Get(String::NewSymbol("timeout")));
  int timeout = 0;
  if (!timeoutThing->IsUndefined() && timeoutThing->IsNumber()) {
    timeout = timeoutThing->Int32Value();
  }

  int checkbox_len = 0;
  if (!checkboxes->IsUndefined()) {
    checkbox_len = checkboxes->Length();
  }

  SInt32 err = 0;

  // Create base dictionary with title and message
  CFMutableDictionaryRef dict = CFDictionaryCreateMutable(NULL, 0,
                                                          &kCFTypeDictionaryKeyCallBacks,
                                                          &kCFTypeDictionaryValueCallBacks);
  CFDictionarySetValue(dict, kCFUserNotificationAlertHeaderKey,
                       CFStringCreateWithCString(NULL, c_title, kCFStringEncodingUTF8));
  CFDictionarySetValue(dict, kCFUserNotificationAlertMessageKey,
                       CFStringCreateWithCString(NULL, c_msg, kCFStringEncodingUTF8));

  // Checkbox array
  if (checkbox_len > 0) {
    CFMutableArrayRef array = convert_checkboxes(checkboxes, checkbox_len);
    CFDictionarySetValue(dict, kCFUserNotificationCheckBoxTitlesKey, array);
    CFRelease(array);
  }

  // Set the flags
  CFOptionFlags flags = kCFUserNotificationPlainAlertLevel;

  // Display the dialog
  CFUserNotificationRef un = CFUserNotificationCreate(NULL, 0, flags, &err, dict);

  // Cast the callback and pass it to the queue worker.
  NanCallback *callback = new NanCallback(args[1].As<v8::Function>());
  if (!callback->IsEmpty()) {
    NanAsyncQueueWorker(new DialogWorker(callback, un, checkbox_len, timeout));
  }

  NanReturnUndefined();
}

void Init(v8::Handle<Object> exports) {
  exports->Set(NanNew("show"), NanNew<v8::FunctionTemplate>(Show)->GetFunction());
}

NODE_MODULE(dialog, Init)

CFMutableArrayRef convert_checkboxes(Local<Array> checkboxes, int len) {
  CFMutableArrayRef array = CFArrayCreateMutable(NULL, 0, NULL);
  for (int i = 0; i < len; i++) {
    String::Utf8Value box(checkboxes->Get(i)->ToString());
    char *c_box = *box;
    CFArrayAppendValue(array, CFStringCreateWithCString(NULL, c_box, kCFStringEncodingUTF8));
  }
  return array;
}
