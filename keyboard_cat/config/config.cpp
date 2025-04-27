#include "keyboard_cat/config/config.h"
#include <fstream>
#include <iostream>
#include <toml++/toml.hpp>

ApplicationParameters Config::Load(const std::filesystem::path& path) const
{
    ApplicationParameters parameters;

    try
    {
        if (!std::filesystem::exists(path))
        {
            std::cerr << "Config file not found at '" << path
                      << "'. Creating default config.\n";
            if (!Save(path, parameters))
            {
                std::cerr << "Warning: Failed to create default config file\n";
            }
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
        std::cerr << "Config parse error in '" << path << "'\n"
                  << e.description() << "\n"
                  << "at line " << e.source().begin.line << ", col "
                  << e.source().begin.column << "\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error loading config: " << e.what() << "\n";
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
            std::cerr << "Save failed: Path is a directory\n";
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
            std::cerr << "Failed to open file for writing: " << path << "\n";
            return false;
        }
        file << config;
        if (file.fail())
        {
            std::cerr << "Failed to write config data to: " << path << "\n";
            return false;
        }

        return true;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error saving config: " << e.what() << "\n";
        return false;
    }
}
