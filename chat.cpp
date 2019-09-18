#include "zk_cpp.h"
#include "zk_chat.h"

#include <stdio.h>
#include <iostream>
#include <string>
#include <unistd.h>
using namespace std;



int main(int argc, char* argv[])
{
    cout << "zk_chat test" << endl;
    string urls = "127.0.0.1:2181";
    zkChat* chat = zkChat::GetInstance();
    if (!chat->connect(urls)) {
        return 0;
    }

   // int svid = atoi(argv[1]);
    string node = "/chat";

    if (!chat->exists_path(node)) {
        chat->create_path(node, "1", 1);
    } 
    chat->watch_data(node);
    string msg;
    while(cin >> msg) {
        chat->set_path_value(node, msg);
    }
    return 0;
}