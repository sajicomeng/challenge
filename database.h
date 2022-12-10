#ifndef DATABASE_H
#define DATABASE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sqlite3.h"

#define CREATE_LOG_TABLE_QUERY "CREATE TABLE IF NOT EXISTS Log(id INTEGER PRIMARY KEY AUTOINCREMENT, "\
    "intValue INTEGER, strValue TEXT, date TEXT);"
#define INSERT_INTEGER_VALUE_QUERY "INSERT INTO Log (intValue, date) VALUES(?, ?)"
#define INSERT_STRING_VALUE_QUERY "INSERT INTO Log (strValue, date) VALUES(?, ?)"
#define SELECT_INTEGERS_QUERY "SELECT intValue, date from Log where intValue is not null"
#define SELECT_COUNT_INTEGERS_QUERY "SELECT count(*) from Log where intValue is not null"

int openDB();
int createTable();
int insertInteger(int value, char* date);
int insertString(char* value, char* date);
void printListOfIntegers();
int getNumberOfIntValues();

#ifdef __cplusplus
}
#endif
#endif // DATABASE_H
