#include <iostream>
#include <string>
#include "compare.hpp"

int main(int argc, char* argv[]) {
    if (argc == 3){
        std::string pyscript = "python3 parse.py ";
        system((pyscript + argv[1]).c_str());
        system((pyscript + argv[2]).c_str());
    } else {
        std::cout << "Not enougth params" << std::endl;
    }

    Comparator C(argv[1], argv[2]);
    C.readJSON(C.branch1, C.branch1_packs);
    C.readJSON(C.branch2, C.branch2_packs);
    return 0;
}
