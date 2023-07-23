#include "mysearch.h"

#include <atomic>
#include <iostream>
#include <filesystem>
#include <chrono>
#include <thread>

using namespace std;
namespace fs = filesystem;

static string file_name;
static atomic_bool is_file_found;
static atomic_int thread_counter;
static atomic_bool is_started;

void Sub_dir_search(string dir_name){
    atomic_fetch_add (&thread_counter, 1);
    is_started.store(true);
    //cout << "starting thead #" << thread_counter.load() << endl;
    for (auto const& dir_entry : fs::recursive_directory_iterator(dir_name, fs::directory_options::skip_permission_denied)){
        if (is_file_found.load()){
            break;
        }
        try {
            if (fs::is_regular_file (dir_entry) && (dir_entry.path().filename() == file_name)){
                cout << dir_entry << '\n';
                is_file_found.store(true);
            }
        } catch(fs::filesystem_error& e){
            //std::cout << "Unable to access file or path " << dir_entry <<": " << e.what() << "\n";
        }
    }
    atomic_fetch_sub (&thread_counter, 1);
}

bool Main_dir_search(string name){
    thread_counter.store(0);
    is_file_found.store(false);
    is_started.store(false);
    fs::path root = "/";
    file_name = name;

    for (auto const& dir_entry : fs::directory_iterator(root, fs::directory_options::skip_permission_denied)){
        if (is_file_found.load()){
            break;
        }
        try {
            if (fs::is_regular_file (dir_entry) && (dir_entry.path().filename() == file_name)){
                cout << dir_entry << '\n';
                is_file_found.store(true);
            } else if (fs::is_directory(dir_entry)){
                // wait until thread_count is less 8
                while(thread_counter.load() >= 8){
                    this_thread::sleep_for(10ns);
                }
                thread th(Sub_dir_search, dir_entry.path());
                // wait until sub thread is started
                while(!is_started.load());
                is_started.store(false);
                th.detach();
            }
        } catch(fs::filesystem_error& e){
            //std::cout << "Unable to access file or path " << dir_entry <<": " << e.what() << "\n";
        }
    }
    while(thread_counter.load() > 0){
        this_thread::sleep_for(1ms);
    }
    return is_file_found.load();
}
