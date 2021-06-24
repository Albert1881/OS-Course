//
// Created by Albert on 2021/4/12.
//

#include <iostream>
#include <string>
#include <vector>
#include <map>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::map;
using std::vector;

static int r;
static vector<int> resource_Max;
static vector<int> resource_Available;


struct Process {
    int process_id;
    vector<int> resource_max;
    vector<int> resource_allocation;
    vector<int> resource_need;
};

static map<int, Process> process_map;

bool createProcess(int process_id, vector<int> command_resource);

bool terminate(int process_id);

void set_resource(vector<int> &);

bool request(int process_id, vector<int> command_resource);

bool banker_algorithm();

void process();

void set_resource(vector<int> &resource) {
    for (auto &i : resource) {
        std::cin >> i;
    }
}

bool createProcess(int process_id, vector<int> command_resource) {
    map<int, Process>::iterator iter = process_map.find(process_id);
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
            process.resource_max = vector<int>(command_resource);
            process.resource_allocation = vector<int>(r, 0);
            process.resource_need = vector<int>(command_resource);
            process_map[process_id] = process;
            return true;
        }
    }
    return false;
}

bool terminate(int process_id) {
    bool result = false;
    map<int, Process>::iterator iter = process_map.find(process_id);
    if (iter != process_map.end()) {
        Process process = iter->second;
        for (int i = 0; i < r; ++i) {
            resource_Available[i] += process.resource_allocation[i];
        }
        result = true;
    }
    return result;
}

bool request(int process_id, vector<int> command_resource) {
    map<int, Process>::iterator iter = process_map.find(process_id);
    if (iter == process_map.end()) {
        return false;
    }
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

    bool do_request = banker_algorithm();
    if (!do_request) {
        for (int i = 0; i < r; ++i) {
            resource_Available[i] += command_resource[i];
            process.resource_allocation[i] -= command_resource[i];
            process.resource_need[i] += command_resource[i];
        }
        process_map[process_id] = process;
    }
    return do_request;
}

bool banker_algorithm() {
    map<int, Process> process_map_copy = map<int, Process>(process_map);
    vector<int> resource_Available_copy = vector<int>(resource_Available);
    while (!process_map_copy.empty()) {
        bool found = false;
        for (map<int, Process>::iterator i = process_map_copy.begin(); i != process_map_copy.end();) {
            Process process = i->second;
            bool check = true;
            for (int j = 0; j < r; ++j) {
                if (process.resource_need[j] > resource_Available_copy[j]) {
                    check = false;
                    break;
                }
            }
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
    resource_Max = vector<int>(r, 0);
    set_resource(resource_Max);
    resource_Available = vector<int>(resource_Max);
    int process_id{0};
    string command;
    vector<int> command_resource(r, 0);
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

int main() {
    process();
    return 0;
}
