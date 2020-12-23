#include <unistd.h>
#include <iostream>
#include <boost/filesystem.hpp>

#include "public/IniParse.h"

IniParse::IniParse(std::string fileName)
{
	try {
		if (0 != access(fileName.c_str(), F_OK)) {
			fclose(fopen(fileName.c_str(), "w"));
		}
		boost::property_tree::ini_parser::read_ini(fileName, m_PT);
		m_InitSuccess = true;
		m_FileName = fileName;
	} catch (std::exception& e) {
		std::cout << "init ini file failed : " << e.what() << std::endl;
	}
}

IniParse::~IniParse()
{
}

template<typename T>
static bool IniGet(std::string path, T& value, boost::property_tree::ptree& pt)
{
	bool status = false;
	try {
		value = pt.get<T>(path);
		status = true;
	} catch (std::exception& e) {
		std::cout << "Get Path[" << path << "] failed: " << std::endl;
	}
	return status;
}

bool IniParse::Get(std::string path, bool & value)
{
	if (false == m_InitSuccess) {
		return false;
	}

	return IniGet<bool>(path, value, m_PT);
}

bool IniParse::Get(std::string path, long & value)
{
	if (false == m_InitSuccess) {
		return false;
	}

	return IniGet<long>(path, value, m_PT);
}

bool IniParse::Get(std::string path, std::string & value)
{
	if (false == m_InitSuccess) {
		return false;
	}

	return IniGet<std::string>(path, value, m_PT);
}

bool IniParse::GetBool(std::string path, bool def)
{
	bool v = def;
	Get(path, v);
	return v;
}

long IniParse::GetLong(std::string path, long def)
{
	long v = def;
	Get(path, v);
	return v;
}

std::string IniParse::GetString(std::string path, std::string def)
{
	std::string v = def;
	Get(path, v);
	return v;
}

template<typename T>
static bool IniSet(std::string path, T& value, boost::property_tree::ptree& pt)
{
	bool status = false;
	try {
		pt.put<T>(path, value);
		status = true;
	} catch (std::exception& e) {
		std::cout << "Set Path[" << path << "] failed: " << std::endl;
	}
	return status;
}

bool IniParse::Set(std::string path, bool value)
{
	if (false == m_InitSuccess) {
		return false;
	}

	return IniSet<bool>(path, value, m_PT);
}

bool IniParse::Set(std::string path, long value)
{
	if (false == m_InitSuccess) {
		return false;
	}

	return IniSet<long>(path, value, m_PT);
}

bool IniParse::Set(std::string path, int value)
{
	if (false == m_InitSuccess) {
		return false;
	}

	return IniSet<int>(path, value, m_PT);
}

bool IniParse::Set(std::string path, const std::string & value)
{
	if (false == m_InitSuccess) {
		return false;
	}

	return IniSet<const std::string>(path, value, m_PT);
}

bool IniParse::SaveFile()
{
	boost::property_tree::ini_parser::write_ini(m_FileName, m_PT);
	return true;
}

bool IniParse::Del(std::string path)
{
	m_PT.erase(path);
	return true;
}
