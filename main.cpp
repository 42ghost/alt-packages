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

    BranchData Branch1(argv[1]), Branch2(argv[2]);
    Branch1.readJSON();
    Branch2.readJSON();

    return 0;
}
