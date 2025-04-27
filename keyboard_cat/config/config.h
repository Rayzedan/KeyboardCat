#ifndef CONFIG_H
#define CONFIG_H

#include "keyboard_cat/domain/domain.h"
#include <filesystem>

class Config
{
public:
    ApplicationParameters Load(const std::filesystem::path& path) const;
    bool Save(const std::filesystem::path& path, const ApplicationParameters& parameters) const;
};

#endif // !CONFIG_H
