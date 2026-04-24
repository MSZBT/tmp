#ifndef MIGR_PARSER_H
#define MIGR_PARSER_H

#include <CLI/CLI.hpp>
#include <vector>
#include <string>

class UtArg {
private:
    int argc;
    char** argv;

    std::shared_ptr<CLI::App> app;
    std::string database_name;
    std::string user_name;
    std::string user_password;

public:

    std::vector<std::string> migration_names;
    std::string migration_type;

    bool help_flag = false;

    UtArg(int argc, char** argv);
    ~UtArg() = default;

    void configurate();

    std::string get_database_name();
    std::string get_user_name();
    std::string get_user_password();

    std::shared_ptr<CLI::App> get_app();

    void parse();
};

#endif //MIGR_PARSER_H