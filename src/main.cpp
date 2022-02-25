#include "utils.h"
#include "stopwatch.h"

#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
#include <bits/stdc++.h>
#include <map>
#include <iterator>
#include <boost/filesystem.hpp>

//#define DEBUG

//definition for any row in PAT or ERGO matrices 
//defined as a tuple
// d: date, v: value, i: id, and 
// t: type (0 for patient or 1 for ergo test)
typedef std::tuple<int, float, int, bool> dvit; 

//macros for accessing tuples
#define GET_DATE    get<0>
#define GET_VALUE   get<1>
#define GET_ID      get<2>
#define GET_TYPE    get<3>

//Types of tuple definitions
#define TYPE_PATIENT 0
#define TYPE_ERGO    1

//Performance evaluation mappings
//patient-to-count mappings
typedef std::map<std::string,unsigned short int> pat2cnt;
//Associations, ergo-to-pat2cnt mappings
typedef std::map<std::string, pat2cnt> ergo2pats;
//Linkages, ergo-to-pat mappings
typedef std::map<std::string, std::string> ergo2pat;

using namespace std;
using namespace std::chrono;
namespace fs = boost::filesystem;

bool sort_by_date(const dvit& a, const dvit& b){
    return (GET_DATE(a) < GET_DATE(b));
}

bool sort_by_value(const dvit& a, const dvit& b){
    return (GET_VALUE(a) < GET_VALUE(b));
}

bool sort_by_date_then_value(const dvit& a, const dvit& b){
    
    bool sorted = false;
    if(GET_DATE(a) < GET_DATE(b))
      {
        sorted = true;
      }
    else 
      {
        if( (GET_DATE(a) == GET_DATE(b)) && 
             (GET_VALUE(a) < GET_VALUE(b)) )
          {
            sorted = true;
          }
      }
    return sorted;
}

bool sort_by_date_then_value_then_type(const dvit& a, 
                                        const dvit& b){
    
    bool sorted = false;
    if(GET_DATE(a) < GET_DATE(b))
      {
        sorted = true;
      }
    else 
      {
        if( (GET_DATE(a) == GET_DATE(b)) && 
             (GET_VALUE(a) < GET_VALUE(b)) )
          {
            sorted = true;
          }
        else
          {
            if( (GET_DATE(a) == GET_DATE(b)) && 
                 (GET_VALUE(a) == GET_VALUE(b)) )
              {
                sorted = (GET_TYPE(a) < GET_TYPE(b));
              }
          }
      }
    return sorted;
}

// print the passed in tuple for either a patient
// or ergometric test
// %os could be std::cout to print to stdout or
// an %ofstream to print to a previously openned file
void print_dvit(dvit row, ostream& os){

    string type="";

    if(GET_TYPE(row)==TYPE_PATIENT) 
      {
        type="P"; //"Patient"
      } 
    else 
      {
        type="E"; //"Ergo   "
      }

    os  << type //<< " record: " 
        << GET_DATE(row) << "\t" 
        << setw(4) << GET_VALUE(row) << "\t"
        << GET_ID(row) << endl;
}

bool cmp_by_value(pair<string,unsigned short int>& a,
                  pair<string,unsigned short int>& b)
  {
    return a.second > b.second;
  }

string get_most_frequent_pat(pat2cnt p2c)
  {
    string pat_id;
    vector<pair<string,unsigned short int> > V;
    for(auto& it : p2c) {
      V.push_back(it);
    }
    sort(V.begin(), V.end(), cmp_by_value);
    pat_id = V.begin()->first;
    return pat_id;
  }

void evaluate_performance(ergo2pat e2plinkage, int& tp, int& fp)
  {
    string ergo_id;
    string pat_id;

    tp=0;
    fp=0;

    for(auto it=e2plinkage.begin();it!=e2plinkage.end();++it)
      {
          ergo_id = it->first;
          pat_id  = it->second;

          if ( abs(stoi(ergo_id)-stoi(pat_id))%10000 == 0 )
            {  
              ++tp;  
            } 
          else
            {  
              ++fp; 
#ifdef DEBUG              
              cout<<"E"<<ergo_id<<" != "
                <<"P"<<pat_id<<endl; 
#endif
            }
      }
  }

int main(int argc, char** argv){

    string filename;
    stringstream results;

    /* Results row constitutes the following:
    algo,n_patients,n_ergos,t,t1,t2,t3,t4,t5,TP,FP
    where Algorithm is "TSLink2_C++"
    n_patients = number of unique patients
    n_ergos = number of unique ergo tests
    t = total time in seconds
    t1 = date-to-epoch conversion time (N/A) = -
    t2 = time to form sequence S1
    t3 = time to form sequence S2
    t4 = time to sort S1,S2 and merge them into S and sort the merged sequence S.
    t5 = time to scan sequence S and perform pat-to-ergo associations then linkage
    TP = number of true positive links
    FP = number of false positive links*/

    results << "TSLink2_C++,";

    double t[6];

    cout << "TSLink2 (c) 2021 Ahmed Soliman"<<endl;
    
    // PAT Matrix
    vector<int> Patient_ID;
    vector<float> Patient_HR;
    vector<int> Patient_Epoch;

    // ERGO Matrix
    vector<int> Ergo_ID;
    vector<float> Ergo_HR;
    vector<int> Ergo_Epoch;

    stopwatch sw; //instantiating a stopwatch object

    if(argc<4){
        cout<<"Please input filenames. "
         <<"For example, ./tslink2 ../pat.csv ../ergo.csv ../link_experiments.csv"
         <<endl;
        exit(1);
    }

    // Reading patient records    
    sw.start();
    filename = argv[1];
    cout << "Reading patient records from file: " 
      << filename << endl;
    size_t pat_records = read_csv(filename,
                               Patient_ID,
                               Patient_HR,
                               Patient_Epoch);
    cout << " " << pat_records << " PAT records read in ";
    sw.lap("");

    // Get number of patients
    set<int> patids;
    for(int i=0; i<pat_records; ++i)
      {
        patids.insert(Patient_ID[i]);
      }
    cout << " " << patids.size() << " Unique patients" << endl;
    results<<to_string(patids.size())<<",";

    // Reading ergo records
    sw.start();
    filename=argv[2];
    cout << "Reading ERGO records from file: " << filename << endl;
    size_t ergo_records = read_csv(filename,
                                Ergo_ID,
                                Ergo_HR,
                                Ergo_Epoch);
    cout << " " << ergo_records << " ERGO records read in ";
    sw.lap("");

    // Get number of ergo tests
    {
      set<int> ergoids;
      for(int i=0; i<ergo_records; ++i)
        {
          ergoids.insert(Ergo_ID[i]);
        }
      cout << " " << ergoids.size() << " Unique patients" << endl;
      results<<to_string(ergoids.size())<<",";
      
    }
    
    // Form the sequence S1 from the pat matrix.
    // Each row constitutes: (Patient_ID, Patient_Epoch, Patient_HR)
    // Each tuple in S1 constitutes (date, value, pat_id, 0)
    sw.start();
    vector<dvit> S1;
    S1.resize(Patient_ID.size());
    for(unsigned long int i=0; i<Patient_ID.size(); i++)
        {
            S1[i]=make_tuple(Patient_Epoch[i], Patient_HR[i], 
                         Patient_ID[i], TYPE_PATIENT);
        }
    t[2]=sw.lap("Forming sequence S1 took : ");

    // Form the sequence S2 from the ergo matrix.
    // Each row constitutes: (Ergo_ID, Ergo_Epoch, Ergo_HR)
    // Each tuple in S2 constitutes (date, value, ergo_id, 1)
    // start=high_resolution_clock::now();
    sw.start();
    vector<dvit> S2;
    S2.resize(Ergo_ID.size());
    for(unsigned long int i=0; i<Ergo_ID.size(); i++)
        {
            S2[i]=make_tuple(Ergo_Epoch[i], Ergo_HR[i],
                         Ergo_ID[i], TYPE_ERGO);
        }
    t[3]=sw.lap("Forming sequence S2 took : ");

    // sort S1 and S2
    sw.start();
    sort(S1.begin(), S1.end(), sort_by_date_then_value);
    t[4]=sw.lap("Sorting S1 took : ");
    
    sw.start();
    sort(S2.begin(), S2.end(), sort_by_date_then_value);
    t[4]+=sw.lap("Sorting S2 took : ");
    
    //Merge S1 and S2 into S
    sw.start();
    vector<dvit> S;
    S.reserve(S1.size()+S2.size());
    merge(S1.begin(), S1.end(), S2.begin(), S2.end(), 
      back_inserter(S), sort_by_date_then_value_then_type);
    t[4]+=sw.lap("Merging S1 and S2 into S took : ");

    //For debugging only: print sorted sequence to a file
    // {
    //   ofstream of;
    //   of.open("out.txt");
    //   streambuf *buf;
    //   buf = of.rdbuf();
    //   ostream out(buf);
    //   int i=0;
    //   //for(auto it=S.begin();i<100000;++it,++i)
    //   for(auto it=S.begin(); it!=S.end();++it)
    //     {
    //       print_dvit(*it, of);          
    //     }
    //   of.close();
    // }

    /* Scan S to associate patients with ergo tests
       then link patients to ergo tests. */
    sw.start();
    ergo2pats e2p; //ergo-patients associations
    ergo2pat e2plinkage; //ergo-patient linkages
    vector<dvit> pv,ev; //patient and ergo vectors
    dvit p,e;   //patient and ergo items
    string ergo_id;
    string pat_id;

    // Associate potential patients with ergo tests
    for(auto it=S.begin();it!=S.end();)
      {
        ev.clear();
        pv.clear();
        //skip ergo tuples
        while ( it!=S.end() && GET_TYPE(*it)==TYPE_ERGO) 
            ++it;
        if (it!=S.end() && GET_TYPE(*it)==TYPE_PATIENT)
          {
            p=*it; 
            pv.push_back(p); //add first patient record
            ++it;
          }
        //get potential patient records
        while ( it!=S.end() && GET_TYPE(*it)==TYPE_PATIENT
                && GET_DATE(*it)==GET_DATE(p) && 
                GET_VALUE(*it)==GET_VALUE(p)) 
          { 
            pv.push_back(*it); //add other patients
            ++it;
          }
        //get ergo records
        while ( it!=S.end() && GET_TYPE(*it)==TYPE_ERGO
                && GET_DATE(*it)==GET_DATE(p) && 
                GET_VALUE(*it)==GET_VALUE(p))
          {
            ev.push_back(*it);
            ++it;
          }

        //associate all patients in pv with all ergo tests in ev
        for(auto itp=pv.begin();itp!=pv.end();++itp)
          {
            p=*itp;
            for(auto ite=ev.begin();ite!=ev.end();++ite)
              {
                e=*ite;
                //associate e with p
                ergo_id = to_string(GET_ID(e));
                pat_id  = to_string(GET_ID(p));
                e2p[ergo_id][pat_id]++;
              }
          }
      }

    /* Link the proper (most frequently associated) patient 
       to each ergo test */
    for(auto it=e2p.begin();it!=e2p.end();++it)
      {
          ergo_id = it->first;
          pat2cnt p2c = it->second;
          if(p2c.size()==1) 
            { pat_id=p2c.begin()->first; }
          else
            {
              pat_id=get_most_frequent_pat(p2c);
            }
          //link patient to ergo
          e2plinkage[ergo_id]=pat_id;
      }

    t[5]=sw.lap("Scanning sequence S took : ");

    // Evaluating linkage performance

    int tp=0; //number of true positives
    int fp=0; //number of false positives

    evaluate_performance(e2plinkage, tp, fp);

    cout << "True Positives  = " << tp << endl;
    cout << "False Positives = " << fp << endl;

    sw.total();

    //For debugging only
    //save ergo-to-pat linkages
    {
      ofstream of;
      of.open("e2p.out");
      streambuf *buf;
      buf = of.rdbuf();
      ostream out(buf);
      //int i = 0;
      of << "EID      PID   CNT\n";
      for(auto it = e2p.begin(); it != e2p.end(); ++it) 
        {
          //if (i==10) break;
          of << "E" << setw(6) << left << it->first;
          auto p2c = it->second;
          for(auto it2 = p2c.begin(); it2!= p2c.end(); ++it2)
            {
              of << "  P" << setw(4) << it2->first 
               << "  " << setw(2) << it2->second;
            }
            of << endl;
          //++i;
        }
      of.close();
    }

    results<<sw.gettotalseconds()
        <<",-"
        <<","<<t[2]<<","<<t[3]
        <<","<<t[4]<<","<<t[5]
        <<","<<tp<<","<<fp<<endl;

    string logfilename = argv[3];
    fs::path dir(logfilename);
    fs::create_directories(dir.parent_path());
    ofstream log;
    if(!fs::exists(logfilename))
      {
        ofstream log(logfilename, std::ios_base::app | std::ios_base::out);
        log << "algo,n_patients,n_ergos,t,t1,t2,t3,t4,t5,TP,FP" << endl;
        log << results.str();
      }
    else
      {
        ofstream log(logfilename, std::ios_base::app | std::ios_base::out);
        log << results.str();
      }

    cout<<"Results: "<<results.str();

    return 0;
}
