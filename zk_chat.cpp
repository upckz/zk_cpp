#include "zk_chat.h"


zkChat* zkChat::_instance = nullptr;

zkChat* zkChat::GetInstance() {
    if (_instance == nullptr) {
       _instance = new zkChat;
    }
    return _instance;

}


bool zkChat::connect(string urls) {

    utility::zoo_rc ret = zk.connect(urls);
    if (ret != utility::z_ok) {
        printf("try connect zk server failed, code[%d][%s]\n", ret, utility::zk_cpp::error_string(ret));
        return false;
    }
    _isConnect = true;
    return true;
}

bool zkChat::exists_path(string path) {
    if (!_isConnect) 
    {
        printf("zkChat not connect\n");
        return false;
    }

    utility::zoo_rc ret = zk.exists_node(path.c_str(), nullptr, true);
    if (ret == utility::z_ok) {
        return true;
    } else {
        return false;
    }
}

bool zkChat::create_path(string path, string value, int32_t flag) {
    if (!_isConnect) 
    {
        printf("zkChat not connect\n");
        return false;
    }

    string rpath;
    utility::zoo_rc ret = utility::z_ok;
    vector<utility::zoo_acl_t>acl;
    acl.push_back(utility::zk_cpp::create_world_acl(utility::zoo_perm_all));
    switch(flag) {
        case 0:
            ret = zk.create_persistent_node(path.c_str(), value, acl);
            break;
        case 1:
            ret = zk.create_ephemeral_node(path.c_str(), value, acl);
            break;
        case 2:
            ret = zk.create_sequence_node(path.c_str(), value, acl, rpath);
            break;
        case 3:
            ret = zk.create_sequance_ephemeral_node(path.c_str(), value, acl, rpath);
            break;
        default:
            printf("invalid create path flag[%d]\n", flag);
            break;
    }
    printf("create path[%s] flag[%d] ret[%d][%s], rpath[%s]\n", path.c_str(), flag, ret, utility::zk_cpp::error_string(ret), rpath.c_str());
    if (ret == utility::z_ok) {
        return true;
    }
    return false;
}


string zkChat::get_path_value(string path) {
    if (!_isConnect) 
    {
        printf("zkChat not connect\n");
        return "";
    }
    string value = "";
    utility::zoo_rc ret = zk.get_node(path.c_str(), value, nullptr, true);
    printf("try get path[%s] value, value[%s] ret[%d][%s]\n", path.c_str(), value.c_str(), ret, utility::zk_cpp::error_string(ret));
    return value;

}

bool zkChat::set_path_value(string path, string value) {
    if (!_isConnect) 
    {
        printf("zkChat not connect\n");
        return false;
    }
    utility::zoo_rc ret =  zk.set_node(path.c_str(), value, -1);
    printf("try set path[%s] value[%s] ret[%d][%s]\n", path.c_str(),value.c_str(), ret, utility::zk_cpp::error_string(ret));
    if (ret == utility::z_ok) {
        return true;
    }
    return false;
}

vector<string> zkChat::get_children_path(string path) {
    vector<string>children;
    if (!_isConnect) 
    {
        printf("zkChat not connect\n");
        return children;
    }
  
    utility::zoo_rc ret = zk.get_children(path.c_str(), children, true);
    printf("try get children path[%s], children size[%d] ret[%d][%s]\n", path.c_str(), (int)children.size(), ret, utility::zk_cpp::error_string(ret));
    return children;

}


void zkChat::delete_path(string path) {
    if (!_isConnect) 
    {
        printf("zkChat not connect\n");
        return ;
    }
    utility::zoo_rc ret = zk.delete_node(path.c_str(), -1);
    printf("try delete path[%s] ret[%d][%s]\n", path.c_str(), ret, utility::zk_cpp::error_string(ret));
}


void zkChat::watch_data(string path) {
    if (!_isConnect) 
    {
        printf("zkChat not connect\n");
        return ;
    }
    string value = "";
    utility::zoo_rc ret = zk.watch_data_change(path.c_str(), data_change_event, &value);
    printf("try watch_data change of path[%s] value[%s] ret[%d][%s]\n", path.c_str(), value.c_str(), ret, utility::zk_cpp::error_string(ret));
}

void zkChat::watch_child(string path) {
    std::vector<std::string> children;
    if (!_isConnect) 
    {
        printf("zkChat not connect\n");
        return ;
    }
    utility::zoo_rc ret = zk.watch_children_event(path.c_str(), child_change_events, &children);
    printf("try watch_child change of path[%s], child_count[%d] ret[%d][%s]\n",path.c_str(), (int32_t)children.size(), ret, utility::zk_cpp::error_string(ret));
}


int zkChat::ProcessPacket(const string &path, string msg) {
    printf("recv msg[%s]\n",  msg.c_str());
   
    return 0;
}


void data_change_event(const std::string& path, const std::string& new_value) {
     printf("data_change_event, path[%s] new_data[%s]\n", path.c_str(), new_value.c_str());
     zkChat::GetInstance()->ProcessPacket(path, new_value);
    
}

void child_change_events(const std::string& path, const std::vector<std::string>& children) {
    printf("child_change_events, path[%s] new_child_count[%d]\n", path.c_str(), (int32_t)children.size());

    for (int32_t i = 0; i < (int32_t)children.size(); ++i) {
        printf("%d, %s\n", i, children[i].c_str());
    }
}
