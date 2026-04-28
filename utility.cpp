#include "utility.h"

int main(int argc, char** argv) {

    std::shared_ptr<UtArg> utilits_param;

    try {
        utilits_param = std::make_shared<UtArg>(argc, argv);
    } catch (std::exception &e) {
        Logger::getInstance().error("Exception: {}", e.what());
        return 1;
    }

    if (!utilits_param->check_database_options()) {
        Logger::getInstance().error("Uncorrected arguments");
        return 1;
    }

    if (utilits_param->help_flag) {
        std::string line(100, '=');
        std::string line_thin(100, '_');

        std::cout << "" << std::endl;
        std::cout << line << std::endl;
        std::cout << "" << std::endl;
        std::cout << "MEOW MAZAFAKA" << std::endl;

        std::cout << line_thin << std::endl;
        std::cout << line << std::endl;

        return 0;
    }

    auto &database_manager = DatabaseManager::getInstance().initialize(utilits_param->get_database_name(), utilits_param->get_user_name(), utilits_param->get_user_password());

    if (database_manager.get_connection() != nullptr) {

        for (auto &migration_name : utilits_param->migration_names) {
            try {
                database_manager.execute(migration_name, utilits_param->migration_type);
            } catch (std::exception &e) {
                Logger::getInstance().error("Exception: {}", e.what());
            }
        }

        database_manager.close_connection();
    } else {
        Logger::getInstance().error("No connection");
        return 1;
    }

    return 0;
}