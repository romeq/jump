//
// Created by toke019 on 8.10.2021.
//

#ifndef JUMP_DATABASE_H
#define JUMP_DATABASE_H

#define DB_DIR "$HOME/.config/jump"
#define DB_FILENAME "config.db"

sqlite3    *init_db();
int         find_record_by_alias(const char *, sqlite3 *, char*);
int         reset_table(sqlite3 *);
int         delete_record_by_alias(const char *, sqlite3 *);
int         alias_exists(const char *, sqlite3 *);
int         new_path(const char *, const char *, sqlite3 *);

#endif //JUMP_DATABASE_H
