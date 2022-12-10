#include "print_thread.h"
#include "database.h"

static HANDLE thread = 0;
static bool threadIsRunning = false;
bool startThread(){
    threadIsRunning = true;
    thread = CreateThread(NULL, 0, ThreadFunc, NULL, 0, NULL);
    return thread;
}

void stopThread(){
    threadIsRunning = false;
}

DWORD WINAPI ThreadFunc(void* data) {
    while(threadIsRunning){
        Sleep(2 * 60 * 1000); // two minutes
        printListOfIntegers();
    }
    return 0;
}
