#ifndef DATABASE_H
#define DATABASE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sqlite3.h"

#define CREATE_LOG_TABLE_QUERY "CREATE TABLE IF NOT EXISTS Log(id INTEGER PRIMARY KEY AUTOINCREMENT, "\
    "intValue INTEGER, strValue TEXT, date TEXT);"
#define INSERT_INTEGER_VALUE_QUERY "INSERT INTO Log (intValue, date) VALUES(?, ?)"

int openDB();
int createTable();
int insertInteger(int value, char* date);

#ifdef __cplusplus
}
#endif
#endif // DATABASE_H
