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
#include <rapidjson/prettywriter.h>
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
    string get_name();
};

class Comparator
{
private:
    vector<packege_json> br1_except_br2;
    vector<packege_json> br2_except_br1;

    vector<packege_json> newer_packs;

    vector<string> split(string, char);
    int compare_version(string, string);
    int compare_release(string, string, char);
public:
    Comparator();
    ~Comparator();
    void diff(BranchData, BranchData);
    void find_np(BranchData, BranchData);
    void make_JSON(string, string);
};


#endif