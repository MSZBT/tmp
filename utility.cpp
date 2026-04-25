#include "utility.h"

int main(int argc, char** argv) {

    auto log = Logger::getInstance();


    auto utilits_param = std::make_shared<UtArg>(argc, argv);
    utilits_param->parse();


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

    log.info("TRYING IN DATABASE");

    auto &database_manager = DatabaseManager::getInstance().initialize(utilits_param->get_database_name(), utilits_param->get_user_name(), utilits_param->get_user_password());

    //auto& database_connecrion = DatabaseManager::getInstance();

    for (auto &name : utilits_param->migration_names) {
        std::cout << name << std::endl;
    }


    return 0;
}