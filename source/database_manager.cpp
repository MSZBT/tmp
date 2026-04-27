#include "database_manager.h"
#include <format>
#include <fstream>

DatabaseManager::DatabaseManager() {
    this->database_name = std::string("Creating database manager");
}

DatabaseManager& DatabaseManager::initialize(const std::string &d_name, const std::string &u_name, const std::string &p_name) {

    Logger::getInstance().info("Trying to initialize database manager");

    this->database_name = d_name;
    this->user = u_name;
    this->password = p_name;

    Logger::getInstance().info("Trying to make a database connection");

    try {
        const std::string connection_string = std::format("dbname={} user={} password={}", this->database_name, this->user, this->password);
        this->connection = std::make_shared<pqxx::connection>(connection_string);

        Logger::getInstance().info("Connection initialized", connection_string.c_str());

    } catch (std::exception& e) {
        Logger::getInstance().error("Error in initializing connection: {}", e.what());
    }

    return *this;
}

std::shared_ptr<pqxx::connection> DatabaseManager::get_connection() {
    return this->connection;
}

void DatabaseManager::close_connection() {
    this->connection->close();
}

void DatabaseManager::execute(std::string &mig_name, std::string &mig_type) {
    Logger::getInstance().info("{} [{}]", mig_name, mig_type);

    //auto connection = this->get_connection().get();
    auto connection = this->connection;

    try {
        auto request_file = std::ifstream(std::format("{}/migrations/{}.{}.sql", PROJECT_DIR, mig_name, mig_type));

        if (request_file) {
            Logger::getInstance().info("Request reading");
            std::string request((std::istreambuf_iterator<char>(request_file)),std::istreambuf_iterator<char>());

            Logger::getInstance().info("Starting transaction", mig_name, mig_type);

            try {
                pqxx::work transaction(*connection);
                transaction.exec(request);

                transaction.commit();

            } catch (std::exception& e) {
                Logger::getInstance().error("Error with transaction: {}", e.what());
            }

        } else {
            Logger::getInstance().error("No such migration file");
        }
    } catch (std::exception& e) {
        Logger::getInstance().error("Error {}", e.what());
    }
}