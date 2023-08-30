#ifdef _WIN32

#include "thread_win32.h"
#include "types.h"

Thread thread_create(void *(*entry_point)(void *), void *arg) {
    return CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) entry_point, arg, 0, NULL);
}

void thread_join(Thread thread, usize *result_holder) {
    WaitForSingleObject(thread, INFINITE);
    DWORD exit_code;
    GetExitCodeThread(thread, &exit_code);
    if (result_holder != 0) {
        *result_holder = exit_code;
    }
}

void thread_exit(usize result) {
    ExitThread(result);
}

void thread_sleep(u64 millis) {
    Sleep(millis);
}

#endif
