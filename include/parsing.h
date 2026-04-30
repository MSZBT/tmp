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
    bool file_conf = false;
    bool status_flag = false;

    UtArg(int argc, char** argv);
    ~UtArg() = default;

    void configurate();

    std::string get_database_name();
    std::string get_user_name();
    std::string get_user_password();

    void set_database_name(const std::string &database_name);
    void set_user_name(const std::string &user_name);
    void set_user_password(const std::string &user_password);

    std::shared_ptr<CLI::App> get_app();

    void parse();
    bool check_database_options();
};

#endif //MIGR_PARSER_H