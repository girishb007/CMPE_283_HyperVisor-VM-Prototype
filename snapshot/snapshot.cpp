#include <fstream>
#include <vector>

void dump_vector(const std::vector<int>& v, const std::string& filename) {
    std::ofstream out(filename);
    if (out.is_open()) {
        for (auto it = v.begin(); it != v.end(); ++it) {
            out << *it << "\n";
        }
        out.close();
    }
}

std::vector<int> retrieve_vector(const std::string& filename) {
    std::vector<int> v;
    std::ifstream in(filename);
    if (in.is_open()) {
        std::string line;
        while (std::getline(in, line)) {
            v.push_back(std::stoi(line));
        }
        in.close();
    }
    return v;
}