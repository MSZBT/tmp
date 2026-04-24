#include "parsing.h"

UtArg::UtArg(int argc, char** argv) {
    this->app = std::make_shared<CLI::App>();
    this->configurate();

    this->argc = argc;
    this->argv = argv;
}

void UtArg::configurate() {
    this->app->add_flag("-c,--commands", this->help_flag, "command list");

    this->app->add_option("-d,--dbname", this->database_name, "database name input")->required();

    this->app->add_option("-u,--username", this->user_name, "username input")->required();

    this->app->add_option("-p,--password", this->user_password, "password database")->required();

    this->app->add_option("-t,--migtype", this->migration_type, "type of migrations")->required();

    this->app->add_option("-m,--migrations", this->migration_names, "names of migrations")->required();
}

std::string UtArg::get_database_name() {
    return this->database_name;
}

std::string UtArg::get_user_name() {
    return this->user_name;
}

std::string UtArg::get_user_password() {
    return this->user_password;
}

std::shared_ptr<CLI::App> UtArg::get_app() {
    return this->app;
}

void UtArg::parse() {
    try {
        this->app->parse(this->argc, this->argv);

    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}
