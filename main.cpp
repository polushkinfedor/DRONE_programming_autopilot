#include "filepars.h"
#include "deikstra_alg.h"

string get_directory () {
    string path = filesystem::current_path().string();
    cout << path;
    int index = 0;
    for (int i = path.length()-1; i>0; i--) {
        if (path[i] == '\\') {
            index = i;
            break;
        }
    }
    path.erase(index);
    path += "\\Command_Set.txt";
    return path;
}

int main() {
    ofstream report;
    string path = get_directory();
    report.open(path, ios::out);
    vector<string> map_array = map_parsing();
    for (string element : map_array) {
        cout << element << endl;
    }


    vector<string> command_set = deikstra_alg(map_array);
    for (string element : command_set) {
        report << element << "\n";
    }
    report.close();
}
