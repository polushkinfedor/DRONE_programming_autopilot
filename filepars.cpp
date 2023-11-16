#include "filepars.h"

namespace global {
    string _error_code;
    bool _was_error = false;
}

string search_file () {
    string path = filesystem::current_path().string();
    int index = 0;
    for (int i = path.length()-1; i>0; i--) {
        if (path[i] == '\\') {
            index = i;
            break;
        }
    }
    path.erase(index);
    path += "\\MapFolder\\map.txt";
    return path;
}

bool check_first_map_string_correct (string str) {
    int counter = 0;
    for (int i=0; i<str.length(); i++) {
        char letter = str[i];
        if (letter == ':' and counter == 0 and i!= 0 and i != str.length()-1) counter ++;
        else if (letter == ':') return false;
        else if (not(letter <= '9' and '0' <= letter)) return false;
    }
    return true;
}

bool check_map_string_correct (string str, int i) {
    for (int j=0; j<str.length(); j++) {
        char letter = str[j];
        if (letter != '.' and letter != 'X' and letter != 'O' and letter != 'S') return false;
    }
    return true;
}

vector<string> map_parsing () {
    vector<string> map_area_array;
    ifstream map_of_area;

    map_of_area.open(search_file());

    if (not(map_of_area)) return (map_area_array);

    int counter = 0;
    bool tall_size = true;
    int size_curr_string = 0;
    while (map_of_area) {
        string curr_string = "";

        getline(map_of_area, curr_string);
        if (curr_string == "") continue;
        counter ++;
        if (counter == 1 and not(check_first_map_string_correct(curr_string))) {
            global::_was_error = true;
            global::_error_code = "error03_generation_map_scale";
        }
        else if (counter > 1 and not(check_map_string_correct(curr_string, counter))) {
            global::_was_error = true;
            global::_error_code = "error02_generation_map_symbols";
        }
        else if (counter > 1) {
            if (not(size_curr_string)) size_curr_string = curr_string.length();
            else if (size_curr_string != curr_string.length()) {
                global::_was_error = true;
                global::_error_code = "error03_generation_map_size";
            }
        }

        map_area_array.push_back(curr_string);
    }

    if (counter == 0) {
        global::_was_error = true;
        global::_error_code = "error00_empty_map";
    }
    if (counter == 1) {
        global::_was_error = true;
        global::_error_code = "error01_only_scale_map";
    }
    return map_area_array;
}