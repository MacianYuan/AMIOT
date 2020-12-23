
#ifndef INIPARSE_H
#define INIPARSE_H

#include <string>
#include <boost/noncopyable.hpp>
#include <boost/property_tree/ptree.hpp>  
#include <boost/property_tree/ini_parser.hpp> 

class IniParse : public boost::noncopyable
{
public:
	IniParse(std::string fileName);
	~IniParse();
	bool Get(std::string path, bool& value);
	bool Get(std::string path, long& value);
	bool Get(std::string path, std::string& value);

	bool GetBool(std::string path, bool def = false);
	long GetLong(std::string path, long def = 0);
	std::string GetString(std::string path, std::string def = "");

	bool Set(std::string path, bool value);
	bool Set(std::string path, long value);
	bool Set(std::string path, int value);
	bool Set(std::string path, const std::string& value);
	bool SaveFile();

	bool Del(std::string path);

private:
	std::string m_FileName;
    bool m_InitSuccess;
	boost::property_tree::ptree m_PT;

};

#endif //INIPARSE_H
