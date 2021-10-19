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

int         find_record_by_path(const char *, sqlite3 *, char *);
int         validate_DB_existence(char *, char *);
int         get_db_path(char *, char *);
int         populate_with_defaults(sqlite3 *);
int         execute_query(const char *, sqlite3 *);
sqlite3*    get_connection(char *, char *);

sqlite3*
init_db() {
    sqlite3 *db = NULL;
    char db_path[PATH_MAX_LENGTH];
    char cfg_full_path[PATH_MAX_LENGTH];

    if (get_db_path(db_path, cfg_full_path) < 0)
        return NULL;

    if ((db = get_connection(db_path, cfg_full_path)) == NULL)
        return NULL;

    if (populate_with_defaults(db) < 0)
        return NULL;

    return db;
}

int
show_all(sqlite3 *controller, sqlite3_callback callback)
{
    if (controller == NULL) {
        print_err("Function passed null pointer, exit");
        return -1;
    }

    int rc;
    const char *sql = "SELECT * FROM config;";

    rc = sqlite3_exec(controller,sql, callback, 0, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "(%s) Unable to execute query: %s\n",
                "ERROR", sqlite3_errmsg(controller));

        return -1;
    }

    return 0;
}

int
new_path(const char *alias, const char *path, sqlite3 *controller)
{
    char db_path[PATH_MAX_LENGTH];
    int alias_exists = 0;

    int alias_err = find_record_by_alias(alias, controller, db_path);
    if (alias_err < 0) {
        print_err("Unable to resolve alias");
        return -1;
    }
    else if (alias_err == SQLITE_ROW) {
        alias_exists = 1;
    }

    // if exact record exists -> abort;
    if (strncmp(db_path, path, PATH_MAX_LENGTH) == 0) {
        print_err("Alias with that path already exists!");
        return -1;
    }

    // if path exists under another alias -> inform user
    char alias_db[ALIAS_MAX_LENGTH];

    int path_err = find_record_by_path(path, controller, alias_db);
    if (path_err < 0) {
        print_err("Unable to search database for that path!");
        return -1;
    }
    else if (path_err == SQLITE_ROW) {
        fprintf(stderr, "That path already exists under \"%s\" alias!\n", alias_db);
        return -1;
    }

    if (alias_exists == 1) {
        printf("Update information: "
               "[ \x1B[34m%s\x1B[0m -> \x1B[32m%s\x1B[0m ]\n",
               db_path, path);
        if (confirm("Want to proceed?") == 1) {
            int rc;
            sqlite3_stmt *statement;
            const char *query = "UPDATE config SET path = ? WHERE alias = ?";

            rc = sqlite3_prepare_v2(controller, query, -1, &statement, 0);

            if (rc != SQLITE_OK) {
                fprintf(stderr, "(%s) Unable to prepare statement: %s\n",
                        "ERROR", sqlite3_errmsg(controller));
                return -1;
            }

            sqlite3_bind_text(statement, 1, path, -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(statement, 2, alias, -1, SQLITE_TRANSIENT);

            rc = sqlite3_step(statement);
            if (rc != SQLITE_DONE) {
                printf("(%s) Operation failed: %s\n",
                       "ERROR", sqlite3_errmsg(controller));
                sqlite3_finalize(statement);
                return -1;
            }

            sqlite3_finalize(statement);
        } else {
            fprintf(stderr, "Alrighty then, nothing done. See you!\n");
            return -1;
        }
    } else {
        int rc;
        sqlite3_stmt *statement;
        const char *query = "INSERT INTO config(alias, path) VALUES (?, ?)";
        rc = sqlite3_prepare_v2(controller, query, -1, &statement, 0);

        if (rc != SQLITE_OK) {
            fprintf(stderr, "(%s) Unable to prepare statement: %s\n",
                    "ERROR", sqlite3_errmsg(controller));
            return -1;
        }

        sqlite3_bind_text(statement, 1, alias, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(statement, 2, path, -1, SQLITE_TRANSIENT);

        rc = sqlite3_step(statement);
        if (rc != SQLITE_DONE) {
            printf("(%s) Operation failed: %s\n",
                   "ERROR", sqlite3_errmsg(controller));
            sqlite3_finalize(statement);
            return -1;
        }

        sqlite3_finalize(statement);
    }

    return 0;
}

// search records by path
int
find_record_by_path(const char *path, sqlite3 *controller, char *dest)
{
    int rc;
    sqlite3_stmt *statement;
    const char *query = "SELECT alias FROM config WHERE path = ?";

    if (strlen(path) > 256) {
        print_err("Too long path");
        return -1;
    }

    rc = sqlite3_prepare_v2(controller, query, -1, &statement, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "(%s) Unable to prepare statement: %s\n",
                "ERROR", sqlite3_errmsg(controller));

        return -1;
    }
    sqlite3_bind_text(statement, 1, path, -1, NULL);

    switch (sqlite3_step(statement)) {
        case SQLITE_DONE:
            sqlite3_finalize(statement);
            return SQLITE_DONE;
        case SQLITE_ROW:
            strlcpy(dest, (const char *) sqlite3_column_text(statement, 0), PATH_MAX_LENGTH - 1);
            sqlite3_finalize(statement);
            return SQLITE_ROW;
        default:
            printf("(%s) Execution failed: %s\n", "ERROR",
                   sqlite3_errmsg(controller));
            sqlite3_finalize(statement);
            return -1;
    }
}


// search records by alias
int
find_record_by_alias(const char *keyword, sqlite3 *controller, char *dest)
{
    int rc;
    sqlite3_stmt *statement;
    const char *query = "SELECT path FROM config WHERE alias = ?";

    if (strlen(keyword) > 128) {
        print_err("Too long keyword");

        return -1;
    }

    rc = sqlite3_prepare_v2(controller, query, -1, &statement, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "(%s) Unable to prepare statement: %s\n",
                "ERROR", sqlite3_errmsg(controller));

        return -1;
    }

    sqlite3_bind_text(statement, 1, keyword, -1, NULL);

    switch (sqlite3_step(statement)) {
        case SQLITE_DONE:
            sqlite3_finalize(statement);
            return SQLITE_DONE;
        case SQLITE_ROW:
            strlcpy(dest, (const char *) sqlite3_column_text(statement, 0), PATH_MAX_LENGTH - 1);
            sqlite3_finalize(statement);
            return SQLITE_ROW;
        default:
            printf("(%s) Execution failed: %s\n", "ERROR",
                   sqlite3_errmsg(controller));
            sqlite3_finalize(statement);
            return -1;
    }
}

int
delete_record_by_alias(const char *keyword, sqlite3 *controller)
{
    if (alias_exists(keyword, controller) == 0) {
        print_err("Alias not found, can not delete");
        return -1;
    }

    int rc;
    sqlite3_stmt *statement;
    const char *query = "DELETE FROM config WHERE alias = ?";

    rc = sqlite3_prepare_v2(controller, query, -1, &statement,0 );

    if (rc != SQLITE_OK) {
        fprintf(stderr, "(%s) Unable to prepare statement: %s\n",
                "ERROR", sqlite3_errmsg(controller));
        return -1;
    }

    sqlite3_bind_text(statement, 1, keyword, -1, NULL);

    rc = sqlite3_step(statement);
    if (rc != SQLITE_DONE) {
        printf("(%s) Execution failed: %s\n",
               "ERROR", sqlite3_errmsg(controller));

        sqlite3_finalize(statement);
        return -1;
    }

    sqlite3_finalize(statement);

    return 0;
}

// returns connection to database
sqlite3*
get_connection(char *dir_path, char *file_path)
{

    sqlite3 *db = NULL;
    if (validate_DB_existence(dir_path, file_path) < 0)
        return NULL;

    if (sqlite3_open(file_path, &db) != SQLITE_OK) {
        print_err("Database connection couldn't be opened.");
        return NULL;
    }

    return db;
}

// reset database; delete all records
// it does not ask confirmation, so please do that yourself.
int
reset_table(sqlite3 *controller)
{
    const char *sql_query = "DELETE FROM config;";
    if (execute_query(sql_query, controller) < 0) {
        return -1;
    }

    return 0;
}

// Executes single query
int
execute_query(const char *query, sqlite3 *controller)
{
    if (controller == NULL) {
        print_err("No database controller received");
        return -1;
    }

    char *errmsg;
    int err;
    err = sqlite3_exec(controller, query, NULL, 0, &errmsg);

    if (err != SQLITE_OK) {
        fprintf(stderr, "(%s) Unable to create table in database: %s\n",
                "ERROR", errmsg);

        sqlite3_free(errmsg);
        return -1;
    }
    sqlite3_free(errmsg);
    return 0;
}

int
alias_exists(const char *keyword, sqlite3 *controller)
{
    int rc;
    sqlite3_stmt *statement;
    const char *query = "SELECT path FROM config WHERE alias = ?";

    if (strlen(keyword) > 64) {
        print_err("Too long keyword");
        return -1;
    }
    rc = sqlite3_prepare_v2(controller, query, -1, &statement, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "(%s) Unable to prepare statement: %s\n",
                "ERROR", sqlite3_errmsg(controller));

        return -1;
    }

    sqlite3_bind_text(statement, 1, keyword, -1, NULL);

    rc = sqlite3_step(statement);
    if (rc != SQLITE_DONE && rc != SQLITE_ROW) {
        printf("(%s) Execution failed: %s\n", "ERROR",
               sqlite3_errmsg(controller));
        sqlite3_finalize(statement);
        return -1;
    } else if (rc == SQLITE_DONE) {
        return 0;
    } else {
        return 1;
    }
}

// Creates table if it does not exist already; e.g. setup database
int
populate_with_defaults(sqlite3 *controller)
{
    char *sql_statement = "CREATE TABLE IF NOT EXISTS config("\
                                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                "alias VARCHAR(255) NOT NULL,"\
                                "path VARCHAR(255) NOT NULL,"\
                                "UNIQUE(alias)"\
                            ")";

    if (execute_query(sql_statement, controller) < 0) {
        fprintf(stderr, "Unable to execute query (@%s): %s\n",
                __func__, sqlite3_errmsg(controller));
        return -1;
    }

    return 0;
}

// Create configuration files and folders, if they don't exist.
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
                    return -1;
                case EEXIST:
                    print_err("Unable to create the config folder: Already exists");
                    return -1;
                default:
                    print_err("Unable to create a config folder, sorry!");
                    return -1;
            }
        }
    }

    struct stat cfg_full_stat = {0};
    if (stat(config_full_path, &cfg_full_stat) == -1) {
        if (fopen(config_full_path, "w") == NULL) {
            print_err("Unable to create a new database, sorry!");
            return -1;
        }
    }

    return 0;
}

int
get_db_path(char *dest_dir, char *dest_full)
{
    int     standard_exists;
    char    xdg_config_home[PATH_MAX_LENGTH];
    char    config_home[PATH_MAX_LENGTH];
    char    config_full[PATH_MAX_LENGTH];

    char *env_get = getenv("XDG_CONFIG_HOME");

    standard_exists = env_get != NULL && strlen(env_get) > 0;

    if (standard_exists == 1) {
        strlcpy(xdg_config_home, env_get, PATH_MAX_LENGTH - 1);

        struct stat xdg_standard_exists = {0};
        if (stat(xdg_config_home, &xdg_standard_exists) == -1) {
            printf("Custom directory: %s\n", xdg_config_home);
            if (confirm("Preferred config directory was given, but not found. Do you want to create it now?") == 0) {
                return -1;
            }
            else {
                if (mkdir(xdg_config_home, 0755) < 0) {
                    print_err("Unable to create config directory. Do you have correct rights, and does parent directories for it exist?");
                    return -1;
                }
            }
        }

        if (strncmp(&xdg_config_home[strlen(xdg_config_home)-1], "/", PATH_MAX_LENGTH)) {
            strlcat(xdg_config_home, "/", PATH_MAX_LENGTH - 1);
        }
        strlcat(xdg_config_home, "jump/", PATH_MAX_LENGTH - 1);
        strlcpy(config_home, xdg_config_home, PATH_MAX_LENGTH - 1);
    }

    else {
        wordexp_t p;
        wordexp( DB_DIR , &p, 0 );
        strlcpy(config_home, *p.we_wordv, PATH_MAX_LENGTH-1);
        wordfree(&p);
    }

    if (strncmp(&config_home[strlen(config_home)-1], "/", 1) != 0) {
        strlcat(config_home, "/", PATH_MAX_LENGTH);
    }

    strlcpy(config_full, config_home, sizeof(config_full) - 1);
    strlcat(config_full, DB_FILENAME, sizeof(config_full) - 1);

    strlcpy(dest_dir,  config_home, PATH_MAX_LENGTH - 1);
    strlcpy(dest_full, config_full, PATH_MAX_LENGTH - 1);

    return 0;
}