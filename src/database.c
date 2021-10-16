//
// Created by toke019 on 8.10.2021.
//

#include <string.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

#include <wordexp.h>
#include <sqlite3.h>
#include <errno.h>

#include "../include/extern.h"
#include "../include/utils.h"
#include "../include/database.h"

int         validate_DB_existence(char *, char *);
int         get_db_path(char *, char *);
int         populate_with_defaults(sqlite3 *);
sqlite3*    get_connection(char *, char *);

int
init_db(struct DBHandler *dest_controller) {
    sqlite3 *db = NULL;
    char db_path[PATH_MAX_LENGTH];
    char cfg_full_path[PATH_MAX_LENGTH];

    get_db_path(db_path, cfg_full_path);

    if ((db = get_connection(db_path, cfg_full_path)) == NULL)
        return -1;

    if (populate_with_defaults(db) < 0)
        return -1;

    dest_controller->db_connection = db;
    return EXIT_SUCCESS;
}

sqlite3*
get_connection(char *dir_path, char *file_path)
{
    if (validate_DB_existence(dir_path, file_path) < 0) {
        print_err("Database doesn't exist!");
        return NULL;
    }

    sqlite3 *db = NULL;
    if (sqlite3_open(file_path, &db) != SQLITE_OK) {
        print_err("Database connection couldn't be opened.");
        return NULL;
    }

    return db;
}

int
populate_with_defaults(sqlite3 *controller)
{
    if (controller == NULL) {
        print_err("No database controller received");
        return -1;
    }
    char *sql_statement = "CREATE TABLE IF NOT EXISTS paths("\
        "alias VARCHAR(255) NOT NULL,"\
        "path VARCHAR(255) NOT NULL"\
    ")";
    char *errmsg;
    int err;
    err = sqlite3_exec(controller, sql_statement, NULL, 0, &errmsg);

    if (err != SQLITE_OK) {
        fprintf(stdout, "(%s) Unable to create table in database\n",
                "ERROR");

        sqlite3_free(errmsg);
        return -1;
    }

    return 0;
}

int
validate_DB_existence(char *config_dir, char *config_full_path)
{
    struct stat cfg_dir_stat = {0};
    if (stat(config_dir, &cfg_dir_stat) == -1) {
        errno = 0;
        if (mkdir(config_dir, 0755) > 0) {
            switch (errno) {
                case EACCES:
                    print_err("Unable to create the config folder: Permission denied");
                    break;
                case EEXIST:
                    print_err("Unable to create the config folder: Already exists");
                    break;
                default:
                    print_err("Unable to create a config folder, sorry!");
                    break;
            }
            return -1;
        }
    }

    struct stat cfg_full_stat = {0};
    if (stat(config_full_path, &cfg_full_stat) == -1) {
        if (fopen(config_full_path, "w") == NULL) {
            print_err("Unable to create a new database, sorry! ");
            return -1;
        }
    }

    return 0;
}

int
get_db_path(char *dest_dir, char *dest_full)
{
    int     standard_exists;
    char    config_home[PATH_MAX_LENGTH];
    char   *xdg_config_home;
    char    config_full[PATH_MAX_LENGTH];

    xdg_config_home = getenv("XDG_CONFIG_HOME");
    standard_exists = xdg_config_home != NULL &&
        strlen(xdg_config_home) > 3;

    if (standard_exists == 1) {
        strlcpy(config_home, xdg_config_home, PATH_MAX_LENGTH - 1);
    }
    else {
        wordexp_t p;
        wordexp( DB_DIR , &p, 0 );
        strlcpy(config_home, *p.we_wordv, PATH_MAX_LENGTH-1);
        wordfree(&p);
    }

    if (strncmp(&config_home[strlen(config_home)], "/", 1)!=0) {
        strlcat(config_home, "/", PATH_MAX_LENGTH);
    }

    strlcpy(config_full, config_home, sizeof(config_full) - 1);
    strlcat(config_full, DB_FILENAME, sizeof(config_full) - 1);

    strlcpy(dest_dir,  config_home, PATH_MAX_LENGTH - 1);
    strlcpy(dest_full, config_full, PATH_MAX_LENGTH - 1);

    return 0;
}