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
    
    Comparator Cmp;
    Cmp.diff(Branch1, Branch2);
    Cmp.find_np(Branch1, Branch2);

    Cmp.make_JSON(Branch1.get_name(), Branch2.get_name());
    return 0;
}
