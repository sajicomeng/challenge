#include "database.h"
#include "synchapi.h"

static sqlite3 *db;
static CRITICAL_SECTION ctlSec;

void swap(int* xp, int* yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void swapStr(unsigned char** xp,unsigned char** yp){
    unsigned char * temp = *xp;
    *xp = *yp;
    *yp = temp;
}

// A function to implement bubble sort
void bubbleSort(int arr[], int n, unsigned char* charArr[])
{
    int i, j;
    for (i = 0; i < n - 1; i++)

        // Last i elements are already in place
        for (j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1])
            {
                swap(&arr[j], &arr[j + 1]);
                swapStr(&charArr[j], &charArr[j+1]);
            }
}

int openDB(){
    if (sqlite3_open("database.sqlite", &db) == SQLITE_OK){
        InitializeCriticalSection(&ctlSec);
        return 1;
    }
    return 0;
}

int createTable(){
    int res = 0;
    sqlite3_stmt* query;
    if (sqlite3_prepare_v2(db, CREATE_LOG_TABLE_QUERY, -1, &query, 0) == SQLITE_OK){
        if (sqlite3_step(query) == SQLITE_DONE)
            res = 1;
    }
    sqlite3_finalize(query);
    return res;
}

int insertInteger(int value, char* date){
    EnterCriticalSection(&ctlSec);
    int res = 0;
    sqlite3_stmt* query;
    if (sqlite3_prepare_v2(db, INSERT_INTEGER_VALUE_QUERY, -1, &query, 0) == SQLITE_OK){
        sqlite3_bind_int(query, 1, value);
        sqlite3_bind_text(query, 2, date, strlen(date), 0);
        if (sqlite3_step(query) == SQLITE_DONE)
            res = 1;
    }
    sqlite3_finalize(query);
    LeaveCriticalSection(&ctlSec);
    return res;
}

int insertString(char* value, char* date){
    EnterCriticalSection(&ctlSec);
    int res = 0;
    sqlite3_stmt* query;
    if (sqlite3_prepare_v2(db, INSERT_INTEGER_VALUE_QUERY, -1, &query, 0) == SQLITE_OK){
        sqlite3_bind_text(query, 1, value,  strlen(value), 0);
        sqlite3_bind_text(query, 2, date, strlen(date), 0);
        if (sqlite3_step(query) == SQLITE_DONE)
            res = 1;
    }
    sqlite3_finalize(query);
    LeaveCriticalSection(&ctlSec);
    return res;
}

int getNumberOfIntValues(){
    EnterCriticalSection(&ctlSec);
    int res = 0;
    sqlite3_stmt* query;
    if (sqlite3_prepare_v2(db, SELECT_COUNT_INTEGERS_QUERY, -1, &query, 0) == SQLITE_OK){
        if (sqlite3_step(query) == SQLITE_ROW){
            res = sqlite3_column_int(query, 0);
        }

    }
    sqlite3_finalize(query);
    LeaveCriticalSection(&ctlSec);
    return res;
}

void printListOfIntegers(){
    int count = getNumberOfIntValues();
    int intArray[count];
    unsigned char* dateArray[count];
    int index = 0;
    EnterCriticalSection(&ctlSec);
    int res = 0;
    sqlite3_stmt* query;
    if (sqlite3_prepare_v2(db, SELECT_INTEGERS_QUERY, -1, &query, 0) == SQLITE_OK){
        while (sqlite3_step(query) == SQLITE_ROW){
            intArray[index] = sqlite3_column_int(query, 0);
            unsigned char *tmp = sqlite3_column_text(query, 1);
            dateArray[index] = (unsigned char *)malloc(strlen(tmp)+1);
            memcpy(dateArray[index], tmp, strlen(tmp));
            dateArray[index][strlen(tmp)] = '\0';
            index++;
        }
    }
    sqlite3_finalize(query);
    LeaveCriticalSection(&ctlSec);
    bubbleSort(intArray, count, dateArray);
    printf("----------------\n");
    printf("intValue\tdate\n");
    for (int i=0; i<count; i++){
        printf("%d\t%s\n", intArray[i], dateArray[i]);
        free(dateArray[i]);
    }
}

