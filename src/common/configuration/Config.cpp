//
// Created by cantte on 14/05/21.
//

#include "Config.h"
#include "Utils.h"

#include <boost/property_tree/ini_parser.hpp>
#include <algorithm>
#include <memory>
#include <mutex>

namespace bpt = boost::property_tree;

namespace
{
    std::string _filename;
    std::vector<std::string> _args;
    bpt::ptree _config;
    std::mutex _configLock;

    bool LoadFile(std::string const& file, bpt::ptree& fullTree, std::string& error)
    {
        try
        {
            bpt::ini_parser::read_ini(file, fullTree);

            if (fullTree.empty())
            {
                error = "empty file (" + file + ")";
                return false;
            }
        } catch (bpt::ini_parser::ini_parser_error const& e)
        {
            if (e.line() == 0)
                error = e.message() + " (" + e.filename() + ")";
            else
                error = e.message() + " (" + e.filename() + ":" + std::to_string(e.line()) + ")";
            return false;
        }

        return true;
    }
}

bool ConfigMgr::LoadInitial(const std::string& file, std::vector<std::string> args, std::string& error)
{
    std::lock_guard<std::mutex> lock(_configLock);

    _filename = std::move(file);
    _args = std::move(args);

    bpt::ptree fullTree;
    if (!LoadFile(_filename, fullTree, error))
    {
        return false;
    }

    _config = fullTree.begin()->second;

    return true;
}

ConfigMgr* ConfigMgr::instance()
{
    static ConfigMgr instance;
    return &instance;
}

template<class T>
T ConfigMgr::GetValueDefault(const std::string& name, T def, bool quiet) const
{
    try
    {
        return _config.get<T>(bpt::ptree::path_type(name, '/'));
    } catch (bpt::ptree_bad_path const&)
    {

    }

    return def;
}

template<>
std::string ConfigMgr::GetValueDefault<std::string>(std::string const& name, std::string def, bool quiet) const
{
    try
    {
        return _config.get<std::string>(bpt::ptree::path_type(name, '/'));
    } catch (bpt::ptree_bad_path const&)
    {

    }

    return def;
}

std::string ConfigMgr::GetStringDefault(const std::string& name, const std::string& def, bool quiet) const
{
    std::string val = GetValueDefault(name, def, quiet);
    val.erase(std::remove(val.begin(), val.end(), '"'), val.end());
    return val;
}

bool ConfigMgr::GetBoolDefault(const std::string& name, bool def, bool quiet) const
{
    std::string val = GetValueDefault(name, std::string(def ? "1" : "0"), quiet);
    val.erase(std::remove(val.begin(), val.end(), '"'), val.end());


    return StringToBool(val);
}

int ConfigMgr::GetIntDefault(const std::string& name, int def, bool quiet) const
{
    return GetValueDefault(name, def, quiet);
}

float ConfigMgr::GetFloatDefault(const std::string& name, float def, bool quiet) const
{
    return GetValueDefault(name, def, quiet);
}

std::string const& ConfigMgr::GetFilename()
{
    std::lock_guard<std::mutex> lock(_configLock);
    return _filename;
}

std::vector<std::string> const& ConfigMgr::GetArguments() const
{
    return _args;
}

std::vector<std::string> ConfigMgr::GetKeysByString(const std::string& name)
{
    std::lock_guard<std::mutex> lock(_configLock);

    std::vector<std::string> keys;

    for (bpt::ptree::value_type const& child : _config)
        if (child.first.compare(0, name.length(), name) == 0)
            keys.push_back(child.first);

    return keys;
}
