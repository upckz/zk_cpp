#include "zk_cpp.h"

#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;

void data_change_event(const std::string& path, const std::string& new_value);
void child_change_events(const std::string& path, const std::vector<std::string>& children);

class zkChat {
public:
  
    ~zkChat(){};

    static zkChat* GetInstance();

    int ProcessPacket(const string& path, string msg);

    bool connect(string urls);


    bool exists_path(string path);
    bool create_path(string path, string value, int32_t flag);

    string get_path_value(string path);
    bool set_path_value(string path, string value);

    vector<string> get_children_path(string path);
    void watch_data(string path);
    void watch_child(string path);


    void delete_path(string path);
   
private:
    zkChat(){};
    static zkChat *_instance;


    utility::zk_cpp zk;
    bool _isConnect = false;
    string urls = "";
};

