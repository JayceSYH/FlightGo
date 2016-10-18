#include"stdafx.h"
#include "ConfigLoader.h"
#include<string.h>
#include<iostream>
#include<fstream>
#include<stdio.h>

map<string, string> ConfigLoader::loadConfig(string path, string configHead)
{
	map<string, string> ret;
	ifstream input(path);
	if (!input) return *new map<string, string>();

	char buff[1025];
	while (!input.eof()) {
		input.getline(buff, 1024);
		char *p = strstr(buff, configHead.data());
		if (p != nullptr) break;
	}

	while (!input.eof()) {
		input.getline(buff, 1024);
		if (buff[0] == '#') {
			continue;
		}
		if (!strstr(buff, "[")) {
			char key[200], value[200];
			sscanf_s(buff, "%[^= ] = %[^ ]", key, sizeof(key),value, sizeof(value));
			ret[string(key)] = string(value);
		}
		else {
			break;
		}
	}

	return ret;
}
