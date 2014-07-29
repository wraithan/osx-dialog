#include <nan.h>
#include <CoreFoundation/CoreFoundation.h>

using namespace v8;

NAN_METHOD(Show) {
    Local<Object> options(args[0]->ToObject());
    String::Utf8Value title(options->Get(String::NewSymbol("title"))->ToString());
    char *c_title = *title;
    String::Utf8Value msg(options->Get(String::NewSymbol("msg"))->ToString());
    char *c_msg = *msg;
    String::Utf8Value box1(options->Get(String::NewSymbol("box1"))->ToString());
    char *c_box1 = *box1;
    String::Utf8Value box2(options->Get(String::NewSymbol("box2"))->ToString());
    char *c_box2 = *box2;

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
    CFMutableArrayRef array = CFArrayCreateMutable(NULL, 0, NULL);
    CFArrayAppendValue(array, CFStringCreateWithCString(NULL, c_box1, kCFStringEncodingUTF8));
    CFArrayAppendValue(array, CFStringCreateWithCString(NULL, c_box2, kCFStringEncodingUTF8));
    CFDictionarySetValue(dict, kCFUserNotificationCheckBoxTitlesKey, array);
    CFRelease(array);

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
