#pragma once
#include "pch.h"
using namespace std;

class Logging
{
public:
	static ofstream mOs;
	static string mFileName;
	template<typename _Ty> static void Log(_Ty data) {
		Logging::mOs << data << " " << endl;
	}
	template<typename _Ty> static void Log(_Ty data1, _Ty data2) {
		Logging::mOs << data1 << " " << data2 << endl;
	}
};

