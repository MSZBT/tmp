#ifndef MIGR_DATABASE_MANAGER_H
#define MIGR_DATABASE_MANAGER_H

#ifndef PROJECT_DIR
#define PROJECT_DIR "."
#endif

#include <pqxx/pqxx>


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