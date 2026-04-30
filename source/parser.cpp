#include "logger.h"
#include "parsing.h"

#include <format>
#include <fstream>
#include <nlohmann/json.hpp>

UtArg::UtArg(int argc, char **argv)
{

    Logger::getInstance().info("Creating parsing app");

    this->app = std::make_shared<CLI::App>();
    this->configurate();

    Logger::getInstance().info("Parsing app was configurated");

    this->argc = argc;
    this->argv = argv;

    this->parse();
}

void UtArg::configurate()
{

    Logger::getInstance().info("Starting app configurate");

    this->app->add_flag("-l,--list", this->help_flag, "command list");

    this->app->add_flag("-s,--status", this->status_flag, "migrations status");

    this->app->add_flag("-f,--fileconf", this->file_conf, "configuration from file '.migr_conf.json' (database, username, password)");

    this->app->add_option("-d,--database", this->database_name, "database name input")->delimiter('=');

    this->app->add_option("-u,--username", this->user_name, "username input")->delimiter('=');

    this->app->add_option("-p,--password", this->user_password, "password database")->delimiter('=');

    this->app->add_option("-a,--action", this->migration_type, "type of migrations")->delimiter('=');

    this->app->add_option("-m,--migrations", this->migration_names, "names of migrations");

    Logger::getInstance().info("End app configurate");
}

std::string UtArg::get_database_name()
{
    return this->database_name;
}

std::string UtArg::get_user_name()
{
    return this->user_name;
}

std::string UtArg::get_user_password()
{
    return this->user_password;
}

std::shared_ptr<CLI::App> UtArg::get_app()
{
    return this->app;
}

void UtArg::set_database_name(const std::string &d_name)
{
    this->database_name = d_name;
}

void UtArg::set_user_name(const std::string &u_name)
{
    this->user_name = u_name;
}

void UtArg::set_user_password(const std::string &u_password)
{
    this->user_password = u_password;
}

bool UtArg::check_database_options()
{

    Logger::getInstance().info("Checking database options\n");

    if (this->database_name.empty() || this->user_name.empty() || this->user_password.empty())
    {
        return false;
    }

    if ((this->migration_type.empty() || this->migration_names.empty()) && !this->help_flag && !this->status_flag)
    {
        return false;
    }

    return true;
}

void UtArg::parse()
{
    Logger::getInstance().info("Start to getting parsing options");

    try
    {
        this->app->parse(this->argc, this->argv);

        if (this->file_conf)
        {

            Logger::getInstance().info("Finding conf file");

            auto config = std::ifstream(std::format("{}/.migr_conf.json", PROJECT_DIR));

            if (config)
            {
                Logger::getInstance().info("Parsing conf file");

                auto result = nlohmann::json::parse(config);

                if (!result.is_null())
                {
                    if (result.contains("database") && !result["database"].is_null())
                    {
                        this->set_database_name(result["database"].get<std::string>());
                    }

                    if (result.contains("username") && !result["username"].is_null())
                    {
                        this->set_user_name(result["username"].get<std::string>());
                    }

                    if (result.contains("password") && !result["password"].is_null())
                    {
                        this->set_user_password(result["password"].get<std::string>());
                    }
                }
                else
                {
                    Logger::getInstance().error("No config file found");
                }
            }
        }

        if (!this->check_database_options() && !this->help_flag && !this->status_flag)
        {
            Logger::getInstance().error("Specify all parameters to use postgres or "
                                        "use \"migr_conf\" for configuration");
        }
    }
    catch (std::exception &e)
    {
        Logger::getInstance().info("Error parsing command line arguments");
        std::cerr << e.what() << std::endl;
    }
}