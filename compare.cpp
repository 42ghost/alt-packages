#include "compare.hpp"

Comparator::Comparator(string br1, string br2)
{
    branch1 = br1;
    branch2 = br2;
    branch1_packs = {};
    branch2_packs = {};
}

Comparator::~Comparator()
{
}

void Comparator::readJSON(string br, map<pair<string, string>, packege_json> br_packs){
    std::ifstream ifs(br + ".json");
    rapidjson::IStreamWrapper isw(ifs);
    
    rapidjson::Document document;
    document.ParseStream(isw);

    const int length = document["length"].GetInt();
    const auto& packages = document["packages"];

    for (rapidjson::SizeType i = 0; i < length; i++) {
        const auto& pkg = packages[i];
        const string arch = pkg["arch"].GetString();
        const int buildtime = pkg["buildtime"].GetInt();
        const string disttag = pkg["disttag"].GetString();
        const int epoch = pkg["epoch"].GetInt();
        const string name = pkg["name"].GetString();
        const string release = pkg["release"].GetString();
        const string source = pkg["source"].GetString();
        const string version = pkg["version"].GetString();
        
        br_packs[{arch, name}] = {
            arch,
            buildtime,
            disttag,
            epoch,
            name,
            release,
            source,
            version
        };
    }
    int j = 0;
    for (auto& f: br_packs) {     
        cout << br_packs[f.first].name << endl;
        if (j == 5){
            cout << endl;
            break;
        }
        j++;
    }

    return ;
}