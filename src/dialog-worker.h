#include <nan.h>

class DialogWorker : public NanAsyncWorker {
  private:
    CFUserNotificationRef un;
    CFOptionFlags flags;
    int boxCount;
    int timeout;
    SInt32 error;
  public:
    DialogWorker(NanCallback *callback, CFUserNotificationRef un, int boxCount, int timeout)
      : NanAsyncWorker(callback), un(un), boxCount(boxCount), timeout(timeout) {}
    ~DialogWorker() {}
    void Execute();
    void HandleOKCallback();
};