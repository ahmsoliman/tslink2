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

#define DEBUG

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
    where Algorithm is "TSLink1_C++"
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

    results << "TSLink1_C++,";

    double t[6];

    cout << "TSLink1 (c) 2021 Ahmed Soliman"<<endl;
    
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
         <<"For example, ./tslink1 ../pat.csv ../ergo.csv "     
         <<"../link_experiments.csv"
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

    /* Construct PAT matrices.
     PAT matrix is of size m x n where:
     m is the number of unique patients
     n is the maximum number of samples per patient.
     A sample is defined by a date-value pair.
     Assume equal number of samples per patient.
     So, n = total number of samples / number of patients */
    sw.start();
    // Get number of patients
    set<int> patids;
    for(int i=0; i<pat_records; ++i)
      {
        patids.insert(Patient_ID[i]);
      }
    cout << " " << patids.size() 
      << " Unique patients" << endl;
    results<<to_string(patids.size())<<",";
    int m = patids.size();
    int n = pat_records/m;
    int* PAT_I = new int[m]; // ID
    float** PAT_V = new float*[m]; // Value (HR)
    for(int i=0;i<m;i++)
      PAT_V[i]=new float[n];
    int**   PAT_D = new int*[m]; // Date
    for(int i=0;i<m;i++)
      PAT_D[i]=new int[n];
    int k=0;
    for(int i=0;i<m;++i)
      {
        PAT_I[i]=Patient_ID[k];
        for(int j=0;j<n;++j)
          {
            PAT_D[i][j]=Patient_Epoch[k];
            PAT_V[i][j]=Patient_HR[k++];
          }
      }
    t[2]=sw.lap("Constructing PAT matrices took : ");

    // Reading ergo records
    sw.start();
    filename=argv[2];
    cout << "Reading ERGO records from file: " 
      << filename << endl;
    size_t ergo_records = read_csv(filename,
                                Ergo_ID,
                                Ergo_HR,
                                Ergo_Epoch);
    cout << " " << ergo_records << " ERGO records read in ";
    sw.lap("");

    
    /* Construct ERGO matrices.
     ERGO matrix is of size p x q where:
     p is the number of ergo tests
     q is the maximum number of samples per ergo test.
     A sample is defined by a date-value pair.
     Assume equal number of samples per ergo test.
     So, q = total number of samples / number of ergo tests.*/
    sw.start();
    // Get number of ergo tests
    set<int> ergoids;
    for(int i=0; i<ergo_records; ++i)
      {
        ergoids.insert(Ergo_ID[i]);
      }
    cout << " " << ergoids.size() 
      << " Unique ergometric tests" << endl;
    results<<to_string(ergoids.size())<<",";
    int p = ergoids.size();
    int q = ergo_records/p;
    int* ERGO_I = new int[p]; // ID
    float** ERGO_V = new float*[p]; // Value (HR)
    for(int i=0;i<p;++i)
      {
        ERGO_V[i] = new float[q];
      }
    int**   ERGO_D = new int*[p]; // Date
    for(int i=0;i<p;++i)
      {
        ERGO_D[i] = new int[q];
      }
    k=0;
    for(int i=0;i<p;++i)
      {
        ERGO_I[i]=Ergo_ID[k];
        for(int j=0;j<q;++j)
          {
            ERGO_D[i][j]=Ergo_Epoch[k];
            ERGO_V[i][j]=Ergo_HR[k++];
          }
      }
    t[3]=sw.lap("Constructing ERGO matrices took: ");

    /* Linking patients to ergo tests 
      For each date-value pair in PAT, scan the entire
      ERGO for a match. Update matrix D and vector E
      accordingly.*/
    sw.start();
    int** D = new int*[p]; //pxn
    for(int i=0;i<p;++i)
      D[i] = new int[n];
    int* E = new int[p];
    for(int pp=0;pp<p;++pp)
      {
        for(int nn=0;nn<n;++nn)
          {
            D[pp][nn]=0;
          }
      }
    int pat_id;
    int ergo_id;
    ergo2pats e2p;
    ergo2pat e2plinkage;
    for(int i=0;i<m;++i)
      {
        pat_id = PAT_I[i];
        cout<<"patient: "<<pat_id<<endl;
        for(int j=0;j<n;++j)
          {
            int d = PAT_D[i][j];
            float v = PAT_V[i][j];
            for(int k=0;k<p;++k)
              {
                D[k][j]=0; //clear column j
              }
            for(int k=0;k<p;++k)
              {
                for(int l=0;l<q;++l)
                  {
                    if((ERGO_D[k][l]==d) && 
                      (ERGO_V[k][l]==v))
                      {
                        D[k][j]++;
                      }
                  }
              }
          }
        // sum each row of D into E
        for(int k=0;k<p;++k)
          {
            int s=0; //sum
            for(int j=0;j<n;++j)
              {
                s+=D[k][j];
              }
            E[k]=s;
          }
        // get the maximum value in E
        int Emax=0;
        for(int k=0;k<p;++k)
          {
            if(Emax<E[k])
              Emax=E[k];
          }
        if(Emax>0)
          {
            for(int j=0;j<p;++j)
              {
                if(E[j]==Emax)
                  {
                    //Associate row j of ERGO with patient i
                    ergo_id = ERGO_I[j];
                    e2p[to_string(ergo_id)][to_string(pat_id)]++;
                  }
              }
          }
      }

    /* Link the proper (most frequently associated) patient 
       to each ergo test */
    for(auto it=e2p.begin();it!=e2p.end();++it)
      {
          ergo_id = stoi(it->first);
          pat2cnt p2c = it->second;
          if(p2c.size()==1) 
            { pat_id=stoi(p2c.begin()->first); }
          else
            {
              pat_id=stoi(get_most_frequent_pat(p2c));
            }
          //link patient to ergo
          e2plinkage[to_string(ergo_id)]=to_string(pat_id);
      }
    t[5]=sw.lap("TSLink1 Linking took : ");

    // Evaluating linkage performance
    int tp=0; //number of true positives
    int fp=0; //number of false positives

    evaluate_performance(e2plinkage, tp, fp);

    cout << "True Positives  = " << tp << endl;
    cout << "False Positives = " << fp << endl;

    sw.total();

    //For debugging only
    //save ergo-to-pat linkages
    #ifdef DEBUG
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
    #endif

    results<<sw.gettotalseconds()
        <<",-"
        <<","<<t[2]<<","<<t[3]
        <<","<<t[4]<<","<<t[5]
        <<","<<tp<<","<<fp<<endl;

    string logfilename = argv[3];
    ofstream log(logfilename, std::ios_base::app | std::ios_base::out);
    log << results.str();
    
    cout<<"Results: "<<results.str();

    return 0;
}
