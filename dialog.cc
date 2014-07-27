#include <nan.h>
#include <CoreFoundation/CoreFoundation.h>

using namespace v8;

NAN_METHOD(Show) {
	String::Utf8Value title(args[0]->ToString());
	char *c_title = *title;
	String::Utf8Value msg(args[1]->ToString());
	char *c_msg = *msg;	
	SInt32 nRes = 0;
	CFUserNotificationRef pDlg = NULL;
	const void* keys[] = {
		kCFUserNotificationAlertHeaderKey,
		kCFUserNotificationAlertMessageKey
	};
	const void* vals[] = {
		CFStringCreateWithCString(NULL, c_title, kCFStringEncodingUTF8),
		CFStringCreateWithCString(NULL, c_msg, kCFStringEncodingUTF8)
	};
	CFDictionaryRef dict = CFDictionaryCreate(0, keys, vals,
	                                          sizeof(keys)/sizeof(*keys),
	                                          &kCFTypeDictionaryKeyCallBacks,
	                                          &kCFTypeDictionaryValueCallBacks);
	pDlg = CFUserNotificationCreate(kCFAllocatorDefault, 0,
	                                kCFUserNotificationPlainAlertLevel,
	                                &nRes, dict);
	NanReturnUndefined();
}

void Init(Handle<Object> exports) {
  exports->Set(NanNew("show"), NanNew<FunctionTemplate>(Show)->GetFunction());
}

NODE_MODULE(dialog, Init)
