#include "db_demo.h"
#include <stdio.h>
#include <string.h>

int db_open(db_handle_t *h, const char *path)
{
    return sqlite3_open(path, &h->db);
}

void db_close(db_handle_t *h)
{
    if (h->db) {
        sqlite3_close(h->db);
        h->db = NULL;
    }
}

int db_create_table(db_handle_t *h)
{
    const char *sql =
        "CREATE TABLE IF NOT EXISTS users ("
        "  id   INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  name TEXT    NOT NULL,"
        "  age  INTEGER NOT NULL"
        ");";
    return sqlite3_exec(h->db, sql, NULL, NULL, NULL);
}

int db_insert_user(db_handle_t *h, const char *name, int age)
{
    sqlite3_stmt *stmt = NULL;
    int rc = sqlite3_prepare_v2(h->db,
        "INSERT INTO users (name, age) VALUES (?, ?);", -1, &stmt, NULL);
    if (rc != SQLITE_OK) return rc;

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, age);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (rc == SQLITE_DONE) ? SQLITE_OK : rc;
}

int db_get_user(db_handle_t *h, int id, db_user_t *out)
{
    sqlite3_stmt *stmt = NULL;
    int rc = sqlite3_prepare_v2(h->db,
        "SELECT id, name, age FROM users WHERE id = ?;", -1, &stmt, NULL);
    if (rc != SQLITE_OK) return rc;

    sqlite3_bind_int(stmt, 1, id);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        out->id  = sqlite3_column_int(stmt, 0);
        snprintf(out->name, sizeof(out->name), "%s",
                 (const char *)sqlite3_column_text(stmt, 1));
        out->age = sqlite3_column_int(stmt, 2);
        rc = SQLITE_OK;
    } else {
        rc = SQLITE_NOTFOUND;
    }

    sqlite3_finalize(stmt);
    return rc;
}

int db_count_users(db_handle_t *h)
{
    sqlite3_stmt *stmt = NULL;
    int rc = sqlite3_prepare_v2(h->db,
        "SELECT COUNT(*) FROM users;", -1, &stmt, NULL);
    if (rc != SQLITE_OK) return -1;

    rc = sqlite3_step(stmt);
    int count = (rc == SQLITE_ROW) ? sqlite3_column_int(stmt, 0) : -1;
    sqlite3_finalize(stmt);
    return count;
}

int db_delete_user(db_handle_t *h, int id)
{
    sqlite3_stmt *stmt = NULL;
    int rc = sqlite3_prepare_v2(h->db,
        "DELETE FROM users WHERE id = ?;", -1, &stmt, NULL);
    if (rc != SQLITE_OK) return rc;

    sqlite3_bind_int(stmt, 1, id);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (rc == SQLITE_DONE) ? SQLITE_OK : rc;
}
