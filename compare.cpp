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

    const unsigned int length = document["length"].GetInt();
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

string BranchData::get_name(){
    return branch;
}

Comparator::Comparator(){}

Comparator::~Comparator(){}

void Comparator::diff(BranchData b1, BranchData b2){
    for (auto& it: b1.branch_packs){
        if (b2.branch_packs.find(it.first) == b2.branch_packs.end()){
            br1_except_br2.push_back(it.second);
        }
    }

    for (auto& it: b2.branch_packs){
        if (b1.branch_packs.find(it.first) == b1.branch_packs.end()){
            br2_except_br1.push_back(it.second);
        }
    }
}

void Comparator::find_np(BranchData b1, BranchData b2){
    for (auto& it: b1.branch_packs){
        if (b2.branch_packs.find(it.first) != b2.branch_packs.end()){
            switch (strcmp((it.second.version).c_str(), (b2.branch_packs[it.first].version).c_str()))
            {
            case 1:
                newer_packs.push_back(it.second);
                break;
            case 0:
                if (strcmp((it.second.release).c_str(), (b2.branch_packs[it.first].release).c_str()) == 1){
                    newer_packs.push_back(it.second);
                }
                break;
            default:
                break;
            }
        }
    }
}

void Comparator::make_JSON(string b1_name, string b2_name){
    rapidjson::Document document;
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

    rapidjson::Value br1_ex_br2(rapidjson::kArrayType);
    for(const auto& data : br1_except_br2) {
        rapidjson::Value item(rapidjson::kObjectType);
        item.AddMember("arch", rapidjson::Value(data.arch.c_str(), allocator).Move(), allocator);
        item.AddMember("buildtime", data.buildtime, allocator);
        item.AddMember("disttag", rapidjson::Value(data.disttag.c_str(), allocator).Move(), allocator);
        item.AddMember("epoch", data.epoch, allocator);
        item.AddMember("name", rapidjson::Value(data.name.c_str(), allocator).Move(), allocator);
        item.AddMember("release", rapidjson::Value(data.release.c_str(), allocator).Move(), allocator);
        item.AddMember("source", rapidjson::Value(data.source.c_str(), allocator).Move(), allocator);
        item.AddMember("version", rapidjson::Value(data.version.c_str(), allocator).Move(), allocator);
        br1_ex_br2.PushBack(item, allocator);
    }

    rapidjson::Value br2_ex_br1(rapidjson::kArrayType);
    for(const auto& data : br2_except_br1) {
        rapidjson::Value item(rapidjson::kObjectType);
        item.AddMember("arch", rapidjson::Value(data.arch.c_str(), allocator).Move(), allocator);
        item.AddMember("buildtime", data.buildtime, allocator);
        item.AddMember("disttag", rapidjson::Value(data.disttag.c_str(), allocator).Move(), allocator);
        item.AddMember("epoch", data.epoch, allocator);
        item.AddMember("name", rapidjson::Value(data.name.c_str(), allocator).Move(), allocator);
        item.AddMember("release", rapidjson::Value(data.release.c_str(), allocator).Move(), allocator);
        item.AddMember("source", rapidjson::Value(data.source.c_str(), allocator).Move(), allocator);
        item.AddMember("version", rapidjson::Value(data.version.c_str(), allocator).Move(), allocator);
        br2_ex_br1.PushBack(item, allocator);
    }

    rapidjson::Value n_packs(rapidjson::kArrayType);
    for(const auto& data : newer_packs) {
        rapidjson::Value item(rapidjson::kObjectType);
        item.AddMember("arch", rapidjson::Value(data.arch.c_str(), allocator).Move(), allocator);
        item.AddMember("buildtime", data.buildtime, allocator);
        item.AddMember("disttag", rapidjson::Value(data.disttag.c_str(), allocator).Move(), allocator);
        item.AddMember("epoch", data.epoch, allocator);
        item.AddMember("name", rapidjson::Value(data.name.c_str(), allocator).Move(), allocator);
        item.AddMember("release", rapidjson::Value(data.release.c_str(), allocator).Move(), allocator);
        item.AddMember("source", rapidjson::Value(data.source.c_str(), allocator).Move(), allocator);
        item.AddMember("version", rapidjson::Value(data.version.c_str(), allocator).Move(), allocator);
        n_packs.PushBack(item, allocator);
    }

    document.SetObject();
    document.AddMember("branch1_except_branch2", br1_ex_br2, allocator);
    document.AddMember("branch2_except_branch1", br2_ex_br1, allocator);
    document.AddMember("branch1_newer_branch2", n_packs, allocator);

    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);

    std::ofstream ofs(b1_name + "_" + b2_name + ".json");
    ofs << buffer.GetString();
    ofs.close();
}
