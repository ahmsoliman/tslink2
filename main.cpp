#include "utils.h"

#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

int main(int argc, char** argv){

    string filename;

    cout << "TSLink2 (c) 2021 Ahmed Soliman"<<endl;
    
    // PAT Matrix
    vector<int> Patient_ID;
    vector<float> Patient_HR;
    vector<int> Patient_Epoch;

    // ERGO Matrix
    vector<int> Ergo_ID;
    vector<float> Ergo_HR;
    vector<int> Ergo_Epoch;

    if(argc<3){
        cout<<"Please input filenames. For example, ./tslink2 ../pat.csv ../ergo.csv"<<endl;
        exit(1);
    }
    
   // for(unsigned long long int i=0;i<Patient_ID.size();i++){
    //     cout<< Patient_ID.at(i) << ",   " << Patient_HR.at(i) << " ,  "
    //     << Patient_Epoch.at(i)<< endl;
    // }

    auto start=high_resolution_clock::now();
    filename=argv[1];
    cout << "Reading patient records from file: " << filename << endl;
    read_csv(filename,Patient_ID,Patient_HR,Patient_Epoch);
    cout << "PAT constitutes " << Patient_ID.size() << " rows" << endl;

    filename=argv[2];
    cout << "Reading ERGO records from file: " << filename << endl;
    read_csv(filename,Ergo_ID,Ergo_HR,Ergo_Epoch);
    cout << "ERGO constitutes " << Ergo_ID.size() << " rows" << endl;
    auto stop=high_resolution_clock::now();
    auto duration=duration_cast<seconds>(stop-start);
    cout<<"Time taken = " << duration.count() << " seconds" << endl;
    milliseconds duration_ms=duration_cast<milliseconds>(stop-start);
    cout<<"Time taken = " << duration_ms.count() << " milliseconds" << endl;
    
    return 0;
}
