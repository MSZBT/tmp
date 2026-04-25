#include "database_manager.h"
#include <format>

DatabaseManager::DatabaseManager() {
    this->database_name = std::string("DATABASE MANAGER INIZIALAZED");

}

DatabaseManager& DatabaseManager::initialize(std::string d_name, std::string u_name, std::string p_name) {

    Logger::getInstance().info("GET A ARGUMENTS");
    this->database_name = d_name;
    this->user = u_name;
    this->password = p_name;

    Logger::getInstance().info("TRYING TI CONNECT");
    try {
        const std::string connection_string = std::format("dbname={} user={} password={}", this->database_name, this->user, this->password);
        //this->connection = pqxx::connection(connection_string);
        this->connection = std::make_unique<pqxx::connection>(connection_string);

        Logger::getInstance().info("GOT A CONNECTION TO DB");
    } catch (std::exception& e) {
        Logger::getInstance().info("Error in initializing connection", e.what());
    }

    return *this;
}