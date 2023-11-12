#ifndef COMPARE_HPP
#define COMPARE_HPP

#include <iostream>
#include <string>
#include <map>
#include <utility>
#include <fstream>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/ostreamwrapper.h>

using std::string, std::map, std::pair;
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

class Comparator
{
private:
public:
    string branch1;
    string branch2;
    map<pair<string, string>, packege_json> branch1_packs;
    map<pair<string, string>, packege_json> branch2_packs;

    map<pair<string, string>, packege_json> br1_except_br2;
    map<pair<string, string>, packege_json> br2_except_br1;
    Comparator(string, string);
    ~Comparator();
    void readJSON(string, map<pair<string, string>, packege_json>);
    void search(map<pair<string, string>, packege_json>, map<pair<string, string>, packege_json>);
};


#endif