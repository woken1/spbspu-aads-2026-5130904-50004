#include <iostream>
#include <string>

namespace aushev {

int run(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Error: filename is required" << std::endl;
        return 1;
    }
    std::string filename = argv[1];
    return 0;
}

}

int main(int argc, char* argv[]) {
    return aushev::run(argc, argv);
}
