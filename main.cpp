#include "utils.h"

#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
#include <bits/stdc++.h>

typedef std::tuple<int, float, int, bool> dvit; //  d: date, v: value, i: id 
                            //  t: type (0 for patient or 1 for ergo test)

using namespace std;
using namespace std::chrono;

bool sortbysecond(const dvit& a, const dvit& b){
    return (get<1>(a) < get<1>(b));
}

bool sortbythird(const dvit& a, const dvit& b){
    return (get<2>(a) < get<2>(b));
}

bool sortbyfst(const dvit& a, const dvit& b){
    bool sorted = false;

    if(get<2>(a) < get<2>(b))
        if(get<1>(a) < get<1>(b))
            if(get<0>(a) < get<0>(b))
                sorted = true;
    return sorted;
}

// print the passed in tuple for either a patient
// or ergometric test
// %os could be std::cout to print to stdout or
// an %ofstream to print to a previously openned file
void print_dvit(dvit row, ostream& os){

    string type="";

    if(get<3>(row)==0) 
      {
        type="Patient";
      } 
    else 
      {
        type="Ergo";
      }

    os  << type << " record: " 
        << " Date: "  << get<0>(row) << " , " 
        << " Value: " << get<1>(row) << " , "
        << " ID: "    << get<2>(row) << endl;

}

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

    // Reading patient records    
    auto start=high_resolution_clock::now();
    filename=argv[1];
    cout << "Reading patient records from file: " << filename << endl;
    size_t pat_records = read_csv(filename,Patient_ID,Patient_HR,Patient_Epoch);
    cout << " PAT records = " << pat_records << endl;
    cout << " PAT constitutes " << Patient_ID.size() << " rows" << endl;
    auto stop=high_resolution_clock::now();
    auto duration=duration_cast<seconds>(stop-start);
    cout<<"Time taken = " << duration.count() << " seconds" << endl;
    milliseconds duration_ms=duration_cast<milliseconds>(stop-start);
    cout<<"Time taken = " << duration_ms.count() << " milliseconds" << endl;

    // Reading ergo records
    start=high_resolution_clock::now();
    filename=argv[2];
    cout << "Reading ERGO records from file: " << filename << endl;
    size_t ergo_records = read_csv(filename,Ergo_ID,Ergo_HR,Ergo_Epoch);
    cout << " ERGO records = " << ergo_records << endl;
    cout << " ERGO constitutes " << Ergo_ID.size() << " rows" << endl;
    stop=high_resolution_clock::now();
    duration=duration_cast<seconds>(stop-start);
    cout<<"Time taken = " << duration.count() << " seconds" << endl;
    duration_ms=duration_cast<milliseconds>(stop-start);
    cout<<"Time taken = " << duration_ms.count() << " milliseconds" << endl;
    
    // Form the sequence S1 from the pat matrix (Patient_ID, Patient_Epoch, Patient_HR)
    // Each tuple (ivdb) in S1 constitutes (date, value, pat_id, 0)
    start=high_resolution_clock::now();
    vector<dvit> S1;
    S1.resize(Patient_ID.size());

    for(unsigned long int i=0; i<Patient_ID.size(); i++)
        {
            S1[i]=make_tuple(Patient_Epoch[i], Patient_HR[i], Patient_ID[i], 0);
        }
    stop=high_resolution_clock::now();
    duration=duration_cast<seconds>(stop-start);
    cout<<"Time taken for forming sequence S1 = " << duration.count() << " seconds" << endl;
    duration_ms=duration_cast<milliseconds>(stop-start);
    cout<<"Time taken for forming sequence S1 = " << duration_ms.count() << " milliseconds" << endl;

    // print_dvit(S1[0], std::cout);

    // Form the sequence S2 from the ergo matrix (Ergo_ID, Ergo_Epoch, Ergo_HR)
    // Each tuple (ivdb) in S2 constitutes (date, value, ergo_id, 1)
    start=high_resolution_clock::now();
    vector<dvit> S2;
    S2.resize(Ergo_ID.size());

    for(unsigned long int i=0; i<Ergo_ID.size(); i++)
        {
            S2[i]=make_tuple(Ergo_Epoch[i], Ergo_HR[i], Ergo_ID[i], 1);
        }
    stop=high_resolution_clock::now();
    duration=duration_cast<seconds>(stop-start);
    cout<<"Time taken for forming sequence S2 = " << duration.count() << " seconds" << endl;
    duration_ms=duration_cast<milliseconds>(stop-start);
    cout<<"Time taken for forming sequence S2 = " << duration_ms.count() << " milliseconds" << endl;

    // print_dvit(S2[0]); 
    
    // sort S1 and S2
    sort(S1.begin(), S1.end(), sortbyfst);
    sort(S2.begin(), S2.end(), sortbyfst);

    //Merge S1 and S2 into S
    vector<dvit> S;
    S.reserve(S1.size()+S2.size());
    merge(S1.begin(), S1.end(), S2.begin(), S2.end(), back_inserter(S));

   // for debugging only: print sorted sequence to a file
    ofstream of;
    of.open("out.txt");
    streambuf *buf;
    buf = of.rdbuf();
    ostream out(buf);
    int i=0;
    for(auto it=S.begin();i<10;++it,++i)
      {
        print_dvit(*it, of);          
      }
    of.close();

    return 0;
}
