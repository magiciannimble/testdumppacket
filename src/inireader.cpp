// inireader.cpp
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include "ini.h"
#include "inireader.h"


namespace core
{

IniReader::IniReader(const std::string& filename)
{
    error_ = ini_parse(filename.c_str(), valueHandler, this);
}

int IniReader::parseError() const
{
    return error_;
}

std::string IniReader::get(const std::string& section, const std::string& name, const std::string& default_value) const
{
    std::string key = makeKey(section, name);
    return values_.count(key) ? values_.at(key) : default_value;
}

long IniReader::getInteger(const std::string& section, const std::string& name, long default_value) const
{
    std::string valstr = get(section, name, "");
    const char* value = valstr.c_str();
    char* end;
    // This parses "1234" (decimal) and also "0x4D2" (hex)
    long n = strtol(value, &end, 0);
    return end > value ? n : default_value;
}

double IniReader::getReal(const std::string& section, const std::string& name, double default_value) const
{
    std::string valstr = get(section, name, "");
    const char* value = valstr.c_str();
    char* end;
    double n = strtod(value, &end);
    return end > value ? n : default_value;
}

bool IniReader::getBoolean(const std::string& section, const std::string& name, bool default_value) const
{
    bool retValue = default_value;
    std::string valstr = get(section, name, "");
    // Convert to lower case to make string comparisons case-insensitive
    std::transform(valstr.begin(), valstr.end(), valstr.begin(), ::tolower);
    if (valstr == "true" || valstr == "yes" || valstr == "on" || valstr == "1")
    {
        retValue = true;
    }
    else if (valstr == "false" || valstr == "no" || valstr == "off" || valstr == "0")
    {
        retValue = false;
    }

    return retValue;
}

std::string IniReader::makeKey(const std::string& section, const std::string& name)
{
    std::string key = section + "=" + name;
    // Convert to lower case to make section/name lookups case-insensitive
    std::transform(key.begin(), key.end(), key.begin(), ::tolower);
    return key;
}

int IniReader::valueHandler(void* user, const char* section, const char* name,
                            const char* value)
{
    IniReader* reader = (IniReader*)user;
    std::string key = makeKey(section, name);
    if (reader->values_[key].size() > 0)
    {
        reader->values_[key] += "\n";
    }
    reader->values_[key] += value;

    return 1;
}

} // namespace
