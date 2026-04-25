#include "database_manager.h"
#include <format>

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
        this->connection = std::make_unique<pqxx::connection>(connection_string);

        Logger::getInstance().info("Connection initialized", connection_string.c_str());

    } catch (std::exception& e) {
        Logger::getInstance().error("Error in initializing connection: {}", e.what());
    }

    return *this;
}