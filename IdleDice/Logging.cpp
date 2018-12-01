#include "pch.h"
#include "Logging.h"

string Logging::mFileName = "Log.txt";
ofstream Logging::mOs(Logging::mFileName);
