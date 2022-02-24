#ifndef __UTILS_H__
#define __UTILS_H__

#include <string>
#include <vector>

using namespace std;

size_t read_csv(string filename, vector<int> &ID, vector<float> &HR, vector<int> &Epoch);

#endif