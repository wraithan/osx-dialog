#include <CoreFoundation/CoreFoundation.h>
#include "dialog-worker.h"

using v8::Local;
using v8::Array;
using v8::String;
using v8::Value;

// no v8 stuff available.
void DialogWorker::Execute () {
  error = CFUserNotificationReceiveResponse(un, timeout, &flags);
}

// v8 stuff is available.
void DialogWorker::HandleOKCallback () {
  NanScope();
  // An 'error' occured, only known one is when a timeout happens.
  if (error != 0) {
    CFUserNotificationCancel(un);

    Local<Value> argv[] = {
      NanNew<String>("timeout")
    };
    callback->Call(1, argv);
    return;
  }

  // Pull off what button was pressed when it was dismissed, we only support the
  // default 'ok' button for now.
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
}