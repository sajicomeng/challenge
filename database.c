#include "database.h"

static sqlite3 *db;

int openDB(){
    if (sqlite3_open("database.sqlite", &db) == SQLITE_OK){
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
    int res = 0;
    sqlite3_stmt* query;
    if (sqlite3_prepare_v2(db, INSERT_INTEGER_VALUE_QUERY, -1, &query, 0) == SQLITE_OK){
        sqlite3_bind_int(query, 1, value);
        sqlite3_bind_text(query, 2, date, strlen(date), 0);
        if (sqlite3_step(query) == SQLITE_DONE)
            res = 1;
    }
    sqlite3_finalize(query);
    return res;
}

int insertString(char* value, char* date){
    int res = 0;
    sqlite3_stmt* query;
    if (sqlite3_prepare_v2(db, INSERT_INTEGER_VALUE_QUERY, -1, &query, 0) == SQLITE_OK){
        sqlite3_bind_text(query, 1, value,  strlen(value), 0);
        sqlite3_bind_text(query, 2, date, strlen(date), 0);
        if (sqlite3_step(query) == SQLITE_DONE)
            res = 1;
    }
    sqlite3_finalize(query);
    return res;
}


