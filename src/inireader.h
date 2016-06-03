// inireader.h

#ifndef __INIREADER_H__
#define __INIREADER_H__

#include <map>
#include <string>

namespace core
{

class IniReader
{
public:
    // Construct IniReader and parse given filename. See ini.h for more info
    // about the parsing.
    IniReader(const std::string& filename);

    // Return the result of ini_parse(), i.e., 0 on success, line number of
    // first error on parse error, or -1 on file open error.
    int parseError() const;

    // Get a string value from INI file, returning default_value if not found.
    std::string get(const std::string& section, const std::string& name,
                    const std::string& default_value) const;

    // Get an integer (long) value from INI file, returning default_value if
    // not found or not a valid integer (decimal "1234", "-1234", or hex "0x4d2").
    long getInteger(const std::string& section, const std::string& name, long default_value) const;

    // Get a real (floating point double) value from INI file, returning
    // default_value if not found or not a valid floating point value
    // according to strtod().
    double getReal(const std::string& section, const std::string& name, double default_value) const;

    // Get a boolean value from INI file, returning default_value if not found or if
    // not a valid true/false value. Valid true values are "true", "yes", "on", "1",
    // and valid false values are "false", "no", "off", "0" (not case sensitive).
    bool getBoolean(const std::string& section, const std::string& name, bool default_value) const;

private:

    static std::string makeKey(const std::string& section, const std::string& name);
    static int valueHandler(void* user, const char* section, const char* name,
                            const char* value);
private:

    int error_;
    std::map<std::string, std::string> values_;

};

} // namespace

#endif  // __INIREADER_H__

