#include "database_manager.h"

#include <format>
#include <fstream>
#include <regex>

DatabaseManager::DatabaseManager()
{
    this->database_name = std::string("Creating database manager");
}

DatabaseManager &DatabaseManager::initialize(const std::string &d_name, const std::string &u_name,
                                             const std::string &p_name)
{

    Logger::getInstance().info("Trying to initialize database manager");

    this->database_name = d_name;
    this->user = u_name;
    this->password = p_name;

    Logger::getInstance().info("Trying to make a database connection");

    try
    {
        const std::string connection_string =
            std::format("dbname={} user={} password={}", this->database_name, this->user, this->password);
        this->connection = std::make_shared<pqxx::connection>(connection_string);

        Logger::getInstance().info("Connection initialized", connection_string.c_str());

        Logger::getInstance().info("Looking for a migration accounting table: _migrations", connection_string.c_str());

        pqxx::work transaction(*this->connection);
        {
            std::string request = "CREATE TABLE IF NOT EXISTS _migrations(id SERIAL, file_name VARCHAR(255), table_name VARCHAR(255), applied_at TIMESTAMP DEFAULT NOW())";
            transaction.exec(request);

            transaction.commit();
        }

    }
    catch (std::exception &e)
    {
        Logger::getInstance().error("Error in initializing connection: {}", e.what());
    }

    return *this;
}

std::shared_ptr<pqxx::connection> DatabaseManager::get_connection()
{
    return this->connection;
}

void DatabaseManager::close_connection()
{
    Logger::getInstance().info("Closing connection");
    try
    {
        this->connection->close();
    }
    catch (std::exception &e)
    {
        Logger::getInstance().error("Error closing connection: {}", e.what());
    }
}

std::vector<std::string> DatabaseManager::get_modified_tables(const std::string& request) {
    std::vector<std::string> modified_tables;
    std::regex r(R"((?:TABLE|INTO|DELETE\s+FROM|UPDATE|FROM|INSERT\s+INTO|JOIN|TRUNCATE\s+TABLE|ALTER\s+TABLE)\s+(?:IF\s+NOT\s+EXISTS\s+)?(\w+))");

    auto begin = std::sregex_iterator(request.begin(), request.end(), r);
    auto end = std::sregex_iterator();

    for (auto it = begin; it != end; ++it) {
        modified_tables.push_back((*it)[1].str());
        Logger::getInstance().info("Modified table: {}", (*it)[1].str());
    }

    return modified_tables;
}

void DatabaseManager::execute(std::string &mig_name, std::string &mig_type)
{
    Logger::getInstance().info("Request: {} [{}]", mig_name, mig_type);

    auto con = this->connection;

    try
    {
        auto request_file = std::ifstream(std::format("{}/migrations/{}.{}.sql", PROJECT_DIR, mig_name, mig_type));

        if (request_file)
        {
            std::string request((std::istreambuf_iterator<char>(request_file)), std::istreambuf_iterator<char>());

            auto modified_tables = get_modified_tables(request);

            {
                pqxx::work transaction(*con);

                // for (auto &table : modified_tables) {
                //     std::string check_table = std::format("SELECT table_name FROM information_schema.tables WHERE table_name='{}'", table);
                //
                //     auto check_table_res = transaction.exec(check_table);
                //
                //     if (check_table_res.empty()) {
                //         std::string delete_table_request = std::format("DELETE FROM _migrations WHERE table_name='{}'", table);
                //     }
                //
                // }


                std::string check_request = std::format("SELECT file_name FROM _migrations WHERE file_name='{}'", mig_name);

                auto check_transaction_result = transaction.exec(check_request);

                if (!check_transaction_result.empty()) {
                    Logger::getInstance().error("ALREADY IN");

                    if (mig_type == "up") {
                        Logger::getInstance().error("Transaction already exists");

                    } else {

                        Logger::getInstance().info("Executing transaction");

                        std::string migr_requets = std::format("DELETE FROM _migrations WHERE file_name='{}'", mig_name);

                        transaction.exec(migr_requets);
                        transaction.exec(request);
                    }

                } else {
                    Logger::getInstance().error("EMPTY");
                    if (mig_type == "up") {

                        Logger::getInstance().info("Executing transaction");

                        for (auto &table : modified_tables) {
                            std::string migr_requst = std::format("INSERT INTO _migrations(file_name, table_name) VALUES ('{}', '{}')", mig_name, table);
                            transaction.exec(migr_requst);
                        }

                        transaction.exec(request);

                    } else {
                        Logger::getInstance().error("No such transaction");
                    }
                }

                Logger::getInstance().info("CHECK FOR EXISTING");

                for (auto &table : modified_tables) {
                    std::string check_table = std::format("SELECT table_name FROM information_schema.tables WHERE table_name='{}'", table);

                    auto check_table_res = transaction.exec(check_table);

                    Logger::getInstance().info("{} - REQUST: {}", check_table, check_table_res.empty());

                    if (check_table_res.empty()) {
                        std::string delete_table_request = std::format("DELETE FROM _migrations WHERE table_name='{}'", table);
                        transaction.exec(delete_table_request);
                    }

                }

                transaction.commit();
            }

        }
        else
        {
            Logger::getInstance().error("No such migration file");
        }
    }
    catch (const pqxx::undefined_table& e) {
        std::string msg = e.what();

        size_t start = msg.find('"');
        size_t end = msg.rfind('"');
        std::string table = msg.substr(start + 1, end - start - 1);

        Logger::getInstance().error("Undefined table {}", table.c_str());

    }
    catch (std::exception &e)
    {
        Logger::getInstance().error("Error {}", e.what());
    }
}