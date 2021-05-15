//
// Created by cantte on 14/05/21.
//

#ifndef NEURONAL_NETWORKS_SERVER_CONFIG_H
#define NEURONAL_NETWORKS_SERVER_CONFIG_H

#include "Define.hpp"
#include <string>
#include <vector>

class ConfigMgr
{
    ConfigMgr() = default;

    ~ConfigMgr() = default;

public:
    ConfigMgr(ConfigMgr const&) = delete;

    ConfigMgr& operator=(ConfigMgr const&) = delete;

    bool LoadInitial(std::string const& file, std::vector<std::string> args, std::string& error);

    static ConfigMgr *instance();

    std::string GetStringDefault(std::string const& name, const std::string& def, bool quiet = false) const;

    bool GetBoolDefault(std::string const& name, bool def, bool quiet = false) const;

    int GetIntDefault(std::string const& name, int def, bool quiet = false) const;

    float GetFloatDefault(std::string const& name, float def, bool quiet = false) const;

    std::string const& GetFilename();

    std::vector<std::string> const& GetArguments() const;

    std::vector<std::string> GetKeysByString(std::string const& name);

private:
    template<class T>
    T GetValueDefault(std::string const& name, T def, bool quiet) const;
};

#define sConfigMgr ConfigMgr::instance()

#endif //NEURONAL_NETWORKS_SERVER_CONFIG_H
