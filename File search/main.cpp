#include <iostream>
#include "mysearch.h"

using namespace std;

int main(int argc, char **args) {
    if (argc != 2) {
        cout << "Use:\n\t" << args[0] << " file_name" << endl;
        return 0;
    }

    if(!Main_dir_search(args[1])){
        cout << "File not found!" << endl;
    }

    return 0;
}