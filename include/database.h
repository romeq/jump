//
// Created by toke019 on 8.10.2021.
//

#ifndef MCD_DATABASE_H
#define MCD_DATABASE_H

#define DB_DIR "$HOME/.config/mcd"
#define DB_FILENAME "config.db"

struct DBHandler {
    sqlite3    *db_connection;
    int         status;
};

int init_db(struct DBHandler *);

#endif //MCD_DATABASE_H
