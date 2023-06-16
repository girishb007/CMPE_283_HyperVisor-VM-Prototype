// commands to compile and run the code 
// g++ myvmm.cpp 
// ./a.out -v assembly_file_vm1
// ./a.out -v assembly_file_vm1 -v assembly_file_vm2
// ./a.out -v assembly_file_vm1 -s snapshot_vm1
// ./a.out -v assembly_file_vm1 -s snapshot_vm1 -v assembly_file_vm2 -s snapshot_vm2

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <string.h>
#include <vector>
#include <map>
#include <unistd.h>
#include <unordered_map>

using namespace std;

// int register1[32] = { 0 };
// int register2[32] = { 0 };
// vector<int> registers(32, 0);
vector<int> register1(32, 0);
vector<int> register2(32, 0);

vector<string> customSplit(string str, char separator) {
    vector < string > strings;
    int startIndex = 0, endIndex = 0;
    for (int i = 0; i <= str.size(); i++) {
        
        // If we reached the end of the word or the end of the input.
        if (str[i] == separator || i == str.size()) {
            endIndex = i;
            string temp;
            temp.append(str, startIndex, endIndex - startIndex);
            strings.push_back(temp);
            startIndex = endIndex + 1;
        }
    }
    return strings;
}

void create_snapshot(const vector<int>& v, const string& filename) {
    // function to create a snapshot of the registers and save it in a file 
    ofstream out(filename);
    if (out.is_open()) {
        for (vector<int>::const_iterator it = v.begin(); it != v.end(); ++it) {
            out << *it << "\n";
        }
        out.close();
    }
    cout << filename << " snapshot created" <<endl;
}

vector<int> retrieve_snapshot(const string& filename) {
    vector<int> v;
    ifstream in(filename);
    cout << "Retrieving snapshot: " << filename <<endl;
    try {
       if (in.is_open()) {
            string line;
            while (getline(in, line)) {
                v.push_back(stoi(line));
            }
            in.close();
        }
    }
    catch (const std::exception& ex) {
        cerr << "Error retrieving snapshot" << endl;
        cerr << "Error: " << ex.what() << std::endl;
    }
    return v; 
}


void execute_instructions(string instruction, vector<int>& registers) {
    cout << "Executing instruction:" << instruction << endl;
    string arg, op, snapshot_file;
    vector < string > ret;
    istringstream iss(instruction);
    iss >> op >> arg;

    if (op == "add") {
        ret = customSplit(arg, ',');
        registers[stoi(ret[0].substr(1))] = registers[stoi(ret[1].substr(1))] + registers[stoi(ret[2].substr(1))];
    }
    else if (op == "addi") {
        ret = customSplit(arg, ',');
        registers[stoi(ret[0].substr(1))] = registers[stoi(ret[1].substr(1))] + stoi(ret[2]);
    }
    else if (op == "sub") {
        ret = customSplit(arg, ',');
        registers[stoi(ret[0].substr(1))] = registers[stoi(ret[1].substr(1))] - registers[stoi(ret[2].substr(1))];
    }
    else if (op == "mul") {
        ret = customSplit(arg, ',');
        registers[stoi(ret[0].substr(1))] = registers[stoi(ret[1].substr(1))] * registers[stoi(ret[2].substr(1))];
    }
    else if (op == "and") {
        ret = customSplit(arg, ',');
        registers[stoi(ret[0].substr(1))] = registers[stoi(ret[1].substr(1))] & registers[stoi(ret[2].substr(1))];
    }
    else if (op == "or") {
        ret = customSplit(arg, ',');
        if ('$' == ret[2][0]){
            registers[stoi(ret[0].substr(1))] = registers[stoi(ret[1].substr(1))] | registers[stoi(ret[2].substr(1))];
        }
        else
        {
            registers[stoi(ret[0].substr(1))] = registers[stoi(ret[1].substr(1))] | stoi(ret[2]);
        }
    }
    else if (op == "xor") {
        ret = customSplit(arg, ',');
        registers[stoi(ret[0].substr(1))] = registers[stoi(ret[1].substr(1))] ^ registers[stoi(ret[2].substr(1))];
    }
    else if (op == "sll") {
        ret = customSplit(arg, ',');
        registers[stoi(ret[0].substr(1))] = registers[stoi(ret[1].substr(1))] << stoi(ret[2]);
    }
    else if (op == "srl") {
        ret = customSplit(arg, ',');
        registers[stoi(ret[0].substr(1))] = registers[stoi(ret[1].substr(1))] >> stoi(ret[2]);
    }          
    else if (op == "li") {
        ret = customSplit(arg, ',');
        registers[stoi(ret[0].substr(1))] = stoi(ret[1]);
    }
    else if (op == "DUMP_PROCESSOR_STATE") {
        cout << "Registers:" << endl;
        for (int i = 0; i < 32; i++) {
        cout << "$" << i << ": " << registers[i] << endl;
        }
    }
    else if (op == "SNAPSHOT") {
        snapshot_file = arg;
        create_snapshot(registers, snapshot_file);
    }
    else if (op == "#")
        return;
    else {
        cerr << "Error: Unknown instruction " << op << endl;
    return;
    }

}

void handle_instructions(string file_name, int inst_limit) {
    // int registers[32] = { 0 };
    ifstream file(file_name);

    // validating the file
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << file_name << endl;
        return;
    }

    vector < string > ret;

    // Read the file line by line
    string line;

    int inst_count = 0;
    int temp = 0;

    while (getline(file, line)) {
        temp += 1;
        if (temp == inst_limit){
            cout << "Context switch after executing " << inst_limit << " instructions" <<endl;
            temp = 0;
        }
        inst_count += 1;
        // Parse the instruction
        string arg, op;
        istringstream iss(line);
        iss >> op >> arg;
        if (string(file_name).find("vm1")) {
            execute_instructions(line, register1);
        }
        else {
            execute_instructions(line, register2);
        }
    }
    cout << "Number of instructions executed: " << inst_count << endl;

// Close the file
file.close();

}

pair<string, string> read_config_file(string config_file_name){
    map<string, string> config;
    pair<string, string> ret_val;
    ifstream config_file(config_file_name);

    if (!config_file.is_open()) {
        cerr << "Error opening configuration file: " << config_file_name << endl;
        ret_val.first =  "Invalid";
        ret_val.second =  "1";
        return ret_val;
    }

    string line;
    while (getline(config_file, line)) {
        size_t pos = line.find('=');
        if (pos != string::npos) {
            string key = line.substr(0, pos);
            string value = line.substr(pos + 1);

            if (key != "vm_exec_slice_in_instructions" && key != "vm_binary") {
                cerr << "Invalid contents of configuration file !!" << key << endl;
                ret_val.first =  "Invalid";
                ret_val.second =  "1";
                return ret_val;
            }
            config[key] = value;
        }
    }

    config_file.close();
    ret_val.first =  config["vm_binary"];
    ret_val.second = config["vm_exec_slice_in_instructions"];
    return ret_val;
}


// main program
int main(int argc, char* argv[]) {
    try{
        int option;
        // if no arguments passed, then program waits for the context switch
        if (argc < 2) {
            cerr << "Error: No configuration file is passed!" << endl;
            cout << "Execute in this format:" << endl;
            cout << "./a.out -v assembly_file_vm2 -v assembly_file_vm1" << endl;
            return 1;
        }

        // Handling the instructions with one parameter
        else if (argc <= 3) {
            string file_name, inst_count;
            pair<string, string> ret_val;

            option = getopt(argc, argv, "v:s:");
            switch (option) {
            case 'v':
                ret_val = read_config_file(optarg);
                file_name = ret_val.first;
                inst_count = ret_val.second;
                if (file_name == "Invalid") {
                    cerr << "Error: Invalid file name" << endl;
                    return 0;
                }
                else{
                    cout << "------------------------------------------------" << endl;
                    cout << "Handling the instructions of " << file_name << endl;
                    handle_instructions(file_name, stoi(inst_count));
                    cout << "Execution success !!!" << endl << endl;
                }
                break;
            default:
                cerr << "Error: Usage" << endl;
                cout << "Execute in this format:" << endl;
                cout << "./a.out -v assembly_file_vm2 -v assembly_file_vm1" << endl;
                return 1;
            }
            return 0;
        }

        // Handling multiple parameters
        vector<string> vm1_instructions;
        vector<string> vm2_instructions;
        pair<string, string> ret_val;
        string vm_inst_file_name, line_trace, vm1_inst_limit, vm2_inst_limit;

        while ((option = getopt(argc, argv, "v:s:")) != -1) {
            ifstream vm_file;
            string snapshot_name;
            switch (option) {
                case 'v':
                    ret_val = read_config_file(optarg);
                    vm_inst_file_name = ret_val.first;
                    if (vm_inst_file_name == "Invalid") {
                        cerr << "Error: Invalid file name" << endl;
                        return 1;
                    }

                    vm_file.open(vm_inst_file_name);
                    if (string(optarg).find("vm1") != std::string::npos) {
                        vm1_inst_limit = ret_val.second;
                        while(getline(vm_file, line_trace)){
                            stringstream s_s(line_trace);
                            vm1_instructions.push_back(s_s.str());
                        }
                    }
                    else if (string(optarg).find("vm2") != std::string::npos) {
                        vm2_inst_limit = ret_val.second;
                        cout << vm2_inst_limit << endl;
                        while(getline(vm_file, line_trace)){
                            stringstream s_s(line_trace);
                            vm2_instructions.push_back(s_s.str());
                        }
                    }
                    break;
                case 's':
                    snapshot_name = optarg;
                    if (!vm2_instructions.empty()) {
                        register2 = retrieve_snapshot(snapshot_name);
                    }
                    else {
                        register1 = retrieve_snapshot(snapshot_name);
                    }
                    break;
                default:
                    cerr << "Error: Usage" << endl;
                    cout << "Execute in this format:" << endl;
                    cout << "./a.out -v assembly_file_vm2 -v assembly_file_vm1" << endl;
                    return 1;
            }
        }

        // to keep track of context switch 
        int temp = 0;
        int vm1_inst_counter = 0;
        int vm2_inst_counter = 0;

        if (!vm1_instructions.empty() && !vm2_instructions.empty()) {
            int total_inst = vm1_instructions.size() + vm2_instructions.size();
            for(int k=0; k < total_inst; k++) {
                if (temp == 0) {
                    cout << "Context switch to VM1 ....." << endl;
                    int i = 0;
                    while (i<stoi(vm1_inst_limit) && vm1_inst_counter<vm1_instructions.size()) {
                        execute_instructions(vm1_instructions[vm1_inst_counter], register1);
                        vm1_inst_counter++;
                        i++;
                    }
                    temp = 1;
                }
                else if (temp == 1) {
                    cout << "Context switch to VM2 ....." << endl;
                    int j = 0;
                    while (j<stoi(vm2_inst_limit) && vm2_inst_counter<vm2_instructions.size()) {
                        execute_instructions(vm2_instructions[vm2_inst_counter], register2);
                        vm2_inst_counter++;
                        j++;
                        k++;
                    }
                    temp = 0;
                }
            }
        }
        else {
            if (vm2_instructions.empty()) {
                // Handle only VM1 instructions 
                cout << "Handling the instructions of " << vm_inst_file_name << endl;
                handle_instructions(vm_inst_file_name, stoi(vm1_inst_limit));
                cout << "Execution success !!!" << endl << endl;
            }
            else {
                // Handle only VM2 instructions 
                cout << "Handling the instructions of " << vm_inst_file_name << endl;
                handle_instructions(vm_inst_file_name, stoi(vm2_inst_limit));
                cout << "Execution success !!!" << endl << endl;
            }
        }
        
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
