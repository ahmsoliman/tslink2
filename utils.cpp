#include "utils.h"
#include <fstream>

using namespace std;

void read_csv(string filename, vector<int> &ID, vector<float> &HR, vector<int> &Epoch){
    
    ifstream fs(filename);
    string line,str;
    if(fs.is_open()){
        getline(fs, line); //skip header
    }
    while(!fs.eof()){
        getline(fs,str,',');
        if(str.length()==0) continue;
        ID.push_back(stoi(str));
        getline(fs,str,',');
        HR.push_back(stof(str));
        getline(fs,str,'\n');
        Epoch.push_back(stoi(str));
    }
    fs.close();

}
 