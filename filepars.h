#pragma once

#include <iostream>
#include <filesystem>
#include <fstream>
#include <map>

using namespace std;

namespace global {
    extern string _error_code;
    extern bool _was_error;
}

bool check_map_string_correct (string, int);
bool check_first_map_string_correct (string str);
string search_file();
vector<string> map_parsing ();