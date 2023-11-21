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
            string version1 = it.second.version;
            string version2 = b2.branch_packs[it.first].version;
            int cv = compare_version(version1, version2);

            if (cv == 1){
                newer_packs.push_back(it.second);
            } else if (cv == 0) {
                string rel1 = it.second.release;
                string rel2 = b2.branch_packs[it.first].release;
                int cr = compare_release(rel1, rel2, '_');
                if (cr == 1){
                    newer_packs.push_back(it.second);
                }
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

vector<string> Comparator::split(string s, char delimiter) {
    vector<string> tokens;
    string token;
    
    size_t pos = 0;
    while ((pos = s.find(delimiter)) != string::npos) {
        token = s.substr(0, pos);
        tokens.push_back(token);
        s.erase(0, pos + 1);
    }
    tokens.push_back(s);
    return tokens;
}

int Comparator::compare_version(string version1, string version2){
    vector<string> v1 = split(version1, '.');
    vector<string> v2 = split(version2, '.');

    for (size_t i = 0; i < std::max(v1.size(), v2.size()); i++) {
        try
        {
            int num1 = (i < v1.size()) ? stoi(v1[i]) : 0;
            int num2 = (i < v2.size()) ? stoi(v2[i]) : 0;
            if (num1 > num2) {
                return 1;
            } else if (num1 < num2) {
                return -1;
            }
        }
        catch(const std::exception& e)
        {
            if (isdigit(v1[i][0]))
                return 1;
            else if (isdigit(v2[i][0]))
                return -1;
            else
                return 0;
        }
    }

    if (version1.length() > version2.length()){
        return 1;
    } else if (version1.length() < version2.length()){
        return -1;
    } else if (version1[version1.length() - 1] > version2[version2.length() - 1]){
        return 1;
    } else if (version1[version1.length() - 1] < version2[version2.length() - 1]){
        return -1;
    } else {
        return 0;
    }
}

int Comparator::compare_release(string s1, string s2, char delimeter){
    s1.erase(0, 3);
    s2.erase(0, 3);

    vector<string> r1 = split(split(s1, '.')[0], delimeter);
    vector<string> r2 = split(split(s2, '.')[0], delimeter);
    

    if (stoi(r1[0]) > stoi(r2[0])){
        return 1;
    } else if (stoi(r1[0]) < stoi(r2[0])) {
        return -1;
    }

    if (r1.size() > r2.size()){
        return 1;
    } else if (r1.size() < r2.size()) {
        return -1;
    }

    for (size_t i = 1; i < r1.size(); i++){
        try
        {
            int num1 = stoi(r1[i]);
            int num2 = stoi(r2[i]);

            if (num1 > num2) {
                return 1;
            } else if (num1 < num2) {
                return -1;
            }
        }
        catch (const std::exception& e)
        {
            return 0;
        }
    }

    return 0;
}