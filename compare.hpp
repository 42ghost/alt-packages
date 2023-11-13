#ifndef COMPARE_HPP
#define COMPARE_HPP

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <utility>
#include <fstream>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/ostreamwrapper.h>

using std::string, std::map, std::vector, std::pair;
using std::cout, std::cin, std::endl;

struct packege_json
{
    string arch;
    int buildtime;
    string disttag;
    int epoch;
    string name;
    string release;
    string source;
    string version;
};

class BranchData
{
private:
    string branch;
public:
    map<pair<string, string>, packege_json> branch_packs;
    BranchData(string);
    ~BranchData();
    void readJSON();
};

class Comparator
{
private:
    vector<packege_json> br1_except_br2;
    vector<packege_json> br2_except_br1;
public:
    Comparator();
    ~Comparator();
    void search(BranchData b1, BranchData b2);
    void makeJSON(map<pair<string, string>, packege_json>&);
};


#endif