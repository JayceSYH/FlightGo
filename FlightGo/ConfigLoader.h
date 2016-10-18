#pragma once
#include<map>
#include<sstream>
using namespace std;

class ConfigLoader 
{
public:
	static map<string, string> loadConfig(string path, string configHead);
	template <class T>
	static void getAttribute(map<string, string> config, string key, T* value);
};

template<class T>
inline void ConfigLoader::getAttribute(map<string, string> config, string key, T * value)
{
	if (config.find(key) != config.end()) {
		stringstream ss;
		ss << config[key];
		ss >> *value;
	}
}
