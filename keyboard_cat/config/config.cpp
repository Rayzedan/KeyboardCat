#include "keyboard_cat/config/config.h"
#include "keyboard_cat/logger/logger.h"
#include <fstream>
#include <toml++/toml.hpp>

ApplicationParameters Config::Load(const std::filesystem::path& path) const
{
    ApplicationParameters parameters;

    try
    {
        if (!std::filesystem::exists(path))
        {
            Save(path, parameters);
            return parameters;
        }

        toml::table config = toml::parse_file(path.string());
        auto& window = parameters.windowSize;

        if (auto tbl = config["window"].as_table())
        {
            window.offsetX = tbl->get("offset_x")->value_or(window.offsetX);
            window.offsetY = tbl->get("offset_y")->value_or(window.offsetY);
        }
    }
    catch (const toml::parse_error& e)
    {
    }
    catch (const std::exception& e)
    {
    }

    return parameters;
}

bool Config::Save(const std::filesystem::path& path,
    const ApplicationParameters& parameters) const
{
    try
    {
        if (std::filesystem::exists(path) && std::filesystem::is_directory(path))
        {
            return false;
        }

        const auto parent_dir = path.parent_path();
        if (!parent_dir.empty())
        {
            std::filesystem::create_directories(parent_dir);
        }

        const auto& window = parameters.windowSize;
        toml::table config{{"window", toml::table{
                                          {"offset_x", window.offsetX},
                                          {"offset_y", window.offsetY},
                                      }}};

        std::ofstream file(path, std::ios::trunc);
        if (!file.is_open())
        {
            return false;
        }
        file << config;
        if (file.fail())
        {
            return false;
        }

        return true;
    }
    catch (const std::exception& e)
    {
        return false;
    }
}
