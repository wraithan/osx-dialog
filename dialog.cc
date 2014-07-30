#include <nan.h>
#include <CoreFoundation/CoreFoundation.h>

using namespace v8;

CFMutableArrayRef convert_checkboxes(Local<Array>, int);

NAN_METHOD(Show) {
    Local<Object> options(args[0]->ToObject());
    String::Utf8Value title(options->Get(String::NewSymbol("title"))->ToString());
    char *c_title = *title;
    String::Utf8Value msg(options->Get(String::NewSymbol("msg"))->ToString());
    char *c_msg = *msg;
    Local<Array> checkboxes = Local<Array>::Cast(options->Get(String::NewSymbol("checkboxes")));
    int checkbox_len = checkboxes->Length();

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
    if (!checkboxes->IsUndefined() && checkbox_len > 0) {
        CFMutableArrayRef array = convert_checkboxes(checkboxes, checkbox_len);
        CFDictionarySetValue(dict, kCFUserNotificationCheckBoxTitlesKey, array);
        CFRelease(array);
    }

    // Set the flags
    CFOptionFlags flags = kCFUserNotificationPlainAlertLevel;

    // Display the dialog
    CFUserNotificationRef un = CFUserNotificationCreate( NULL, 0.0, flags, &err, dict );

    NanReturnUndefined();
}

void Init(Handle<Object> exports) {
    exports->Set(NanNew("show"), NanNew<FunctionTemplate>(Show)->GetFunction());
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