#ifndef MIGR_DATABASE_MANAGER_H
#define MIGR_DATABASE_MANAGER_H

#ifndef PROJECT_DIR
#define PROJECT_DIR "."
#endif

#include "logger.h"
#include <pqxx/pqxx>

class DatabaseManager {
public:
    static DatabaseManager& getInstance() {
        static DatabaseManager dbinstance;
        return dbinstance;
    }

    DatabaseManager& initialize(const std::string &d_name, const std::string &u_name, const std::string &p_name);

    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;



    //void execute_changes();

private:

    std::string database_name;
    std::string user;
    std::string password;

    std::unique_ptr<pqxx::connection> connection;

    DatabaseManager();
};

/*
pqxx::connection con("dbname=test_database user=postgres password=masia");


try {
    pqxx::work txn(con);

    txn.exec(
        request
    );

    txn.commit();
} catch (std::exception& e) {
    std::cout << e.what() << std::endl;
}

con.close();
*/
#endif //MIGR_DATABASE_MANAGER_H