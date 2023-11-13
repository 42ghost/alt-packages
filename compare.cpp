#include "compare.hpp"

BranchData::BranchData(string br1)
{
    branch = br1;
    branch_packs = {};
}

BranchData::~BranchData(){}

void BranchData::readJSON(){
    std::ifstream ifs(branch + ".json");
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
        
        branch_packs[{arch, name}] = {
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
}

Comparator::Comparator(){}

Comparator::~Comparator(){}

void Comparator::search(BranchData b1, BranchData b2){
    
}