#ifndef MIGR_DATABASE_MANAGER_H
#define MIGR_DATABASE_MANAGER_H

#ifndef PROJECT_DIR
#define PROJECT_DIR "."
#endif

#include "logger.h"
#include <pqxx/pqxx>
#include <string>
#include <vector>

class DatabaseManager {
public:
    static DatabaseManager &getInstance() {
        static DatabaseManager dbinstance;
        return dbinstance;
    }

    DatabaseManager& initialize(const std::string &d_name, const std::string &u_name, const std::string &p_name);

    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;


    void execute(std::string &mig_name, std::string &mig_type);

    void close_connection();

    std::shared_ptr<pqxx::connection> get_connection();

    static std::vector<std::string> get_modified_tables(const std::string& request);

    void show_status();

private:

    std::string database_name;
    std::string user;
    std::string password;

    std::shared_ptr<pqxx::connection> connection;

    DatabaseManager();
};

#endif //MIGR_DATABASE_MANAGER_H