#ifndef DB_DEMO_H
#define DB_DEMO_H

#include <sqlite3.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct {
    int    id;
    char   name[128];
    int    age;
} db_user_t;

typedef struct {
    sqlite3 *db;
} db_handle_t;

int  db_open(db_handle_t *h, const char *path);
void db_close(db_handle_t *h);

int  db_create_table(db_handle_t *h);
int  db_insert_user(db_handle_t *h, const char *name, int age);
int  db_get_user(db_handle_t *h, int id, db_user_t *out);
int  db_count_users(db_handle_t *h);
int  db_delete_user(db_handle_t *h, int id);

#endif
