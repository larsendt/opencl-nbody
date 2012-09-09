#ifndef _LASP_UTIL_OPTPARSER_H
#define _LASP_UTIL_OPTPARSER_H

#include <string>
#include <map>
#include <utility>

typedef std::pair<std::string, std::string> flagKey;

class OptParser
{
    public:
        OptParser();

        void addOption(const std::string& short_flag, 
                       const std::string& long_flag, 
                       const std::string& description,
                       const std::string& value_string = "");

        bool parse(int argc, char *argv[], int start);
        bool hasOption(const std::string& flag);

        std::string errorString();
        std::string helpString();

        std::string getValue(const std::string& flag);
        std::string getDescription(const std::string& flag);

    private:
        flagKey getKey(const std::string& flag);
        bool keyIsEmpty(const flagKey& key);

        std::map<std::string, std::string> m_shortMap;
        std::map<std::string, std::string> m_longMap;

        // keys should be in the form of short_flag, long_flag
        std::map<flagKey, std::string> m_descriptionMap;
        std::map<flagKey, std::string> m_valueStringMap;
        std::map<flagKey, std::string> m_valueMap;

        std::string m_errorString;
};

#endif
