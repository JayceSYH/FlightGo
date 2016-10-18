#pragma once
#include <exception>
#include <stdio.h>
#include "TemplateTools.h"
using namespace std;

static char buff[1024];

#define _formatString(format, ...) ((sprintf(buff, format, __VA_ARGS__) && 0) ? (char*)0 : buff) 
#define formatString _formatString

class RegionTypeNotSupportedException : public exception {
public:
	RegionTypeNotSupportedException(char *typeName, char *funcName): 
		exception(formatString("Region type '%s' is not supported by function '%s'", typeName, funcName)) {
	}
};

class IndexOutOfRangeException : public exception {
public:
	IndexOutOfRangeException(int index) :
		exception(formatString("Index out of range(%d)", index)) {
	}
};

class NoRegisteredViewFoundException : public exception {
public:
	NoRegisteredViewFoundException() :
		exception("No registered view found") {
	}
};

class TimerNotExistException : public exception {
public:
	TimerNotExistException(int timerId):
		exception(formatString("Timer '%d' dose not exist (may be destroyed or not created)", timerId)) {
	}
};