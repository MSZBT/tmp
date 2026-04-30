#include "utility.h"

int main(int argc, char** argv) {

    std::shared_ptr<UtArg> utilits_param;

    try {
        utilits_param = std::make_shared<UtArg>(argc, argv);
    } catch (std::exception &e) {
        Logger::getInstance().error("Exception: {}", e.what());
        return 1;
    }

    if (utilits_param->help_flag) {
        std::string line(100, '=');
        std::string line_thin(100, '_');

        std::cout << "" << std::endl;
        std::cout << line << std::endl;
        std::cout << "" << std::endl;
        std::cout << "[ MIGR ] - A utility for managing migrations in a Postgres database" << std::endl;
        std::cout << "" << std::endl;
        std::cout << "!!!   To use it you need to have ./migrations directory in your project tree" << std::endl;
        std::cout << "!!!   Migration file name structure [NAME].[up/down].sql" << std::endl;
        std::cout << "" << std::endl;
        std::cout << line_thin << std::endl;
        std::cout << "" << std::endl;
        std::cout << "  -l, --list       Command list - ignoring another flags" << std::endl;
        std::cout << "  -s, --status     Show a list of applied migrations - ignoring another flags" << std::endl;
        std::cout << "  -f, --fileconf   Configuration from file '.migr_conf.json' (database, username, password)" << std::endl;
        std::cout << "  -d, --database   Database name input : example --database=database" << std::endl;
        std::cout << "  -u, --username   Username input      : example --username=username" << std::endl;
        std::cout << "  -p, --password   Password database   : example --password=password" << std::endl;
        std::cout << "  -a, --action     Type of migrations  : example --action=up/down" << std::endl;
        std::cout << "  -m, --migrations Names of migrations : example --migrations name, [name2]" << std::endl;


        std::cout << line << std::endl;

        return 0;
    }

    if (!utilits_param->check_database_options()) {
        Logger::getInstance().error("Uncorrected arguments: try to write --list to see utility`s flags");
        return 1;
    }


    auto &database_manager = DatabaseManager::getInstance().initialize(utilits_param->get_database_name(), utilits_param->get_user_name(), utilits_param->get_user_password());

    if (utilits_param->status_flag) {
        database_manager.show_status();
        return 0;
    }

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