# Package Comparison

The program receives 2 branch names as input, receives a list of their packages and generates a JSON file:
```json
{
    "branch1_except_branch2": [
        {   },
    ],
    "branch2_except_branch1": [
        {   },
    ],
    "branch1_newer_branch2": [
        {   },
    ]
}
```

## Installation
```bash
#1. library compilation
g++ -g -c -fPIC -Wall compare.cpp
g++ -shared -Wall -o libalt.so.1.0.0 compare.o

# 2. Program compilation
g++ -o compare main.o -L. -lalt -Wl,-rpath,.
```
or

```bash
make Makefile
```


## Usage

```bash
./compare branchname1 branchname2
```
1. The program performs 2 a GET request to the https://rdb.altlinux.org/api/export/branch_binary_packages/{branchname_N}.
2. Generates 2 json files {branchname1}.json and {branchname2}.json
3. Reads these files into Branch Data classes
4. Compares them and generates the final file {branchname1}_{branchname2}.json
