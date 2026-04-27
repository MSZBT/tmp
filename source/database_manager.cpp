#include "database_manager.h"

#include <format>
#include <fstream>

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
            std::string request = "CREATE TABLE IF NOT EXISTS _migrations(id SERIAL, file_name VARCHAR(255), applied_at TIMESTAMP DEFAULT NOW())";
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

void DatabaseManager::execute(std::string &mig_name, std::string &mig_type)
{
    Logger::getInstance().info("Request: {} [{}]", mig_name, mig_type);

    auto connection = this->connection;

    try
    {
        auto request_file = std::ifstream(std::format("{}/migrations/{}.{}.sql", PROJECT_DIR, mig_name, mig_type));

        if (request_file)
        {
            std::string request((std::istreambuf_iterator<char>(request_file)), std::istreambuf_iterator<char>());

            try
            {
                pqxx::work transaction(*connection);

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

                        std::string migr_requets = std::format("INSERT INTO _migrations(file_name) VALUES ({})", mig_name);

                        transaction.exec(migr_requets);
                        transaction.exec(request);

                    } else {
                        Logger::getInstance().error("No such transaction");
                    }
                }

                transaction.commit();
            }
            catch (std::exception &e)
            {
                Logger::getInstance().error("Error with transaction: {}", e.what());
            }
        }
        else
        {
            Logger::getInstance().error("No such migration file");
        }
    }
    catch (std::exception &e)
    {
        Logger::getInstance().error("Error {}", e.what());
    }
}