#include "OptParser.h"
#include <iostream>
#include <sstream>

OptParser::OptParser()
{

}

//----------------------------------
// Public Methods
//----------------------------------

void OptParser::addOption(const std::string& short_flag, 
               const std::string& long_flag, 
               const std::string& description,
               const std::string& value_string)
{
    flagKey key(short_flag, long_flag);

    m_shortMap[short_flag] = long_flag;
    m_longMap[long_flag] = short_flag;

    m_descriptionMap[key] = description;
    m_valueStringMap[key] = value_string;
}

bool OptParser::parse(int argc, char *argv[], int start)
{
    flagKey key;

    for(int i = start; i < argc; i++)
    {
        std::string flag = argv[i];

        key = getKey(flag);
        if(keyIsEmpty(key))
        {
            m_errorString = "Unknown option '" + flag + "'";
            return false;
        }

        if(m_valueStringMap[key] != "")
        {
            i += 1;
            if(i >= argc)
            {
                m_errorString = "Value missing for argument '" + flag + "'. Expected " + m_valueStringMap[key];
                return false;
            }
            else 
            {
                m_valueMap[key] = argv[i];
            }
        }
        else
        {
            m_valueMap[key] = "";
        } 
    }

    return true;
}

bool OptParser::hasOption(const std::string& flag)
{
    flagKey key = getKey(flag);
    return m_valueMap.count(key) > 0;
}

std::string OptParser::getValue(const std::string& flag)
{
    flagKey key = getKey(flag);

    if(keyIsEmpty(key))
    {
        m_errorString = "Unknown option '" + flag + "'";
        return "";
    }

    if(!m_valueMap.count(key))
    {
        m_errorString = "No value for option '" + flag + "'";
        return "";
    }

    return m_valueMap[key];
}

std::string OptParser::getDescription(const std::string& flag)
{
    flagKey key = getKey(flag);

    if(keyIsEmpty(key))
    {
        m_errorString = "Unknown option '" + flag + "'";
        return "";
    }

    return m_descriptionMap[key];
}

std::string OptParser::errorString()
{
    return m_errorString;
}

std::string OptParser::helpString()
{
    std::ostringstream ss;
    std::map<flagKey, std::string>::const_iterator it;

    for(it = m_descriptionMap.begin(); it != m_descriptionMap.end(); it++)
    {
        flagKey key = (*it).first;
        ss << "  " << key.first << " " << key.second 
           << " " << m_valueStringMap[key] << "\t\t\t" << m_descriptionMap[key] << std::endl;
    }

    return ss.str();
}

//-------------------------------
// Private Methods
//-------------------------------

flagKey OptParser::getKey(const std::string& flag)
{
    flagKey key;
    if(m_shortMap.count(flag))
    {
        key.first = flag;
        key.second = m_shortMap[flag];
    }
    else if(m_longMap.count(flag))
    {
        key.first = m_longMap[flag];
        key.second = flag;
    }

    return key;
}

bool OptParser::keyIsEmpty(const flagKey& key)
{
    return (key.first == "") && (key.second == "");
}

















