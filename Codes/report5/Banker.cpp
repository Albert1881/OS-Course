//
// Created by Albert on 2021/4/12.
//

#include "Banker.h"
#include <deque>
#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <map>

using std::cin;
using std::cout;
using std::deque;
using std::endl;
using std::string;
using std::map;
using std::unordered_map;
using std::vector;
using command_tuple = std::tuple<int32_t, string, vector<int32_t>>;

static int r;
static vector<int32_t> resource_Max;
static vector<int32_t> resource_Available;


struct Process {
    int32_t process_id;
    vector<int32_t> resource_max;
    vector<int32_t> resource_allocation;
    vector<int32_t> resource_need;
};

static map<int32_t, Process> process_map;

void set_resource(vector<int32_t> &);

bool banker_algorithm();

void set_resource(vector<int32_t> &resource) {
    for (auto &i : resource) {
        std::cin >> i;
    }

}

bool createProcess(int32_t process_id, vector<int32_t> command_resource) {
    map<int32_t, Process>::iterator iter = process_map.find(process_id);

    if (iter == process_map.end()) {
        bool check = true;
        for (int i = 0; i < r; ++i) {
            if (command_resource[i] > resource_Max[i]) {
                check = false;
                break;
            }
        }
        if (check) {
            Process process;
            process.process_id = process_id;
            process.resource_max = vector<int32_t>(command_resource);
            process.resource_allocation = vector<int32_t>(r, 0);
            process.resource_need = vector<int32_t>(command_resource);
            process_map[process_id] = process;
            return true;
        }
    }
    return false;
}

bool terminate(int32_t process_id) {
    bool result = false;
    map<int32_t, Process>::iterator iter = process_map.find(process_id);
    if (iter != process_map.end()) {
        Process process = iter->second;
        for (int i = 0; i < r; ++i) {
            resource_Available[i] += process.resource_allocation[i];
        }
        result = true;
    }
    return result;
}

bool request(int32_t process_id, vector<int32_t> command_resource) {
    map<int32_t, Process>::iterator iter = process_map.find(process_id);
    Process process = iter->second;
    for (int i = 0; i < r; ++i) {
        if (resource_Available[i] < command_resource[i] || process.resource_need[i] < command_resource[i]) {
            return false;
        }
    }
    for (int i = 0; i < r; ++i) {
        resource_Available[i] -= command_resource[i];
        process.resource_allocation[i] += command_resource[i];
        process.resource_need[i] -= command_resource[i];
    }
    process_map[process_id] = process;
//    for (auto &i : process_map.find(process_id)->second.resource_need) {
//        std::cout << i << " ";
//    }
//    std::cout << endl;
    bool do_request = banker_algorithm();

    if (!do_request) {
        for (int i = 0; i < r; ++i) {
            resource_Available[i] += command_resource[i];
            process.resource_allocation[i] -= command_resource[i];
            process.resource_need[i] += command_resource[i];
        }
    }
    return do_request;
}

bool banker_algorithm() {
    map<int32_t, Process> process_map_copy = map<int32_t, Process>(process_map);
    vector<int32_t> resource_Available_copy = vector<int32_t>(resource_Available);
    while (!process_map_copy.empty()) {
        bool found = false;
        for (map<int32_t, Process>::iterator i = process_map_copy.begin(); i != process_map_copy.end();) {
            Process process = i->second;
            bool check = true;
            for (int j = 0; j < r; ++j) {
                if (process.resource_need[j] > resource_Available_copy[j]) {
                    check = false;
                    break;
                }
            }
//            std::cout << "resource_Available_copy" << process.process_id << endl;
//            for (auto &i : resource_Available_copy) {
//                std::cout << i << " ";
//            }
//            std::cout << endl;
//            for (auto &i : process.resource_need) {
//                std::cout << i << " ";
//            }
//            std::cout << endl;
            if (check) {
                for (int j = 0; j < r; ++j) {
                    resource_Available_copy[j] += process.resource_allocation[j];
                }
                process_map_copy.erase(i++);
                found = true;
            } else {
                i++;
            }
        }
        if (!found) {
            return false;
        }
    }
    return true;
}


void process() {
    process_map.clear();
    std::cin >> r;
    resource_Max = vector<int32_t>(r, 0);
    set_resource(resource_Max);
    resource_Available = vector<int32_t>(resource_Max);
    int32_t process_id{0};
    string command;
    vector<int32_t> command_resource(r, 0);
    while (std::cin >> process_id) {
        std::cin >> command;
        bool output = false;
        if (command == "new") {
            set_resource(command_resource);
            output = createProcess(process_id, command_resource);
        } else if (command == "request") {
            set_resource(command_resource);
            output = request(process_id, command_resource);
        } else if (command == "terminate") {
            output = terminate(process_id);
        }
        if (output) {
            std::cout << "OK" << endl;
        } else {
            std::cout << "NOT OK" << endl;
        }
    }
}

void check(string testfile, string outfile) {
    string t, ans, ans2;
    int i;
    freopen(testfile.c_str(), "r", stdin);
    char c;
    while (scanf("%c", &c) != EOF) ans += c;
    fclose(stdin);
    freopen(outfile.c_str(), "r", stdin);
    while (scanf("%c", &c) != EOF) ans2 += c;;
    fclose(stdin);
    if (ans.size() != ans2.size()) {
        std::cout << "NO\n";
        return;
    }
    for (i = 0; i < ans.size(); i++)
        if (ans[i] != ans2[i]) {
            std::cout << "NO\n";
        }
    cout << "YES\n";
}

int main() {
//5
    int32_t i{5};
    string infile;
    string outfile;
    std::stringstream in;
    std::stringstream out;
    std::stringstream test;
    in << "data/0" << i << ".data.in";
    in >> infile;
    string testfile;
    out << "data/0" << i << ".out";
    out >> outfile;
    test << "data/0" << i << ".test.out";
    test >> testfile;
    std::cout << testfile;
    freopen(infile.c_str(), "r", stdin);
    freopen(outfile.c_str(), "w", stdout);
    process();
    freopen("CON", "w", stdout);
    freopen("CON", "r", stdin);
    check(testfile, outfile);


    return 0;
}
