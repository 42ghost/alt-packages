#include <iostream>
#include <string>
#include <fstream>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/ostreamwrapper.h>

int main(int argc, char* argv[]) {
    if (argc == 3){
        std::string pyscript = "python3 parse.py ";
        system((pyscript + argv[1]).c_str());
        system((pyscript + argv[2]).c_str());
    } else {
        std::cout << "Not enougth params" << std::endl;
    }
    
    std::ifstream ifs("p10.json");
    rapidjson::IStreamWrapper isw(ifs);
    
    rapidjson::Document document;
    document.ParseStream(isw);

    const int length = document["length"].GetInt();
    const auto& packages = document["packages"];

    for (rapidjson::SizeType i = 0; i < 1; i++) {
        const auto& pkg = packages[i];
        const std::string arch = pkg["arch"].GetString();
        const int buildtime = pkg["buildtime"].GetInt();
        const std::string disttag = pkg["disttag"].GetString();
        const int epoch = pkg["epoch"].GetInt();
        const std::string name = pkg["name"].GetString();
        const std::string release = pkg["release"].GetString();
        const std::string source = pkg["source"].GetString();
        const std::string version = pkg["version"].GetString();

        std::cout << "Package " << i+1 << ":\n";
        std::cout << "Arch: " << arch << "\n";
        std::cout << "Buildtime: " << buildtime << "\n";
        std::cout << "Disttag: " << disttag << "\n";
        std::cout << "Epoch: " << epoch << "\n";
        std::cout << "Name: " << name << "\n";
        std::cout << "Release: " << release << "\n";
        std::cout << "Source: " << source << "\n";
        std::cout << "Version: " << version << "\n\n";
    }

    return 0;
}
