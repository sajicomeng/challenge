#ifndef PRINT_THREAD_H
#define PRINT_THREAD_H
#ifdef __cplusplus
extern "C" {
#endif
#include <windows.h>

bool startThread();
DWORD WINAPI ThreadFunc(void* data);
void stopThread();

#ifdef __cplusplus
}
#endif

#endif // PRINT_THREAD_H
