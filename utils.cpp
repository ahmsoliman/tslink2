// Author: Ahmed Soliman
// Date: Jul 12, 2021

#include "utils.h"
#include <iostream>
#include <string>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

#define BUFFER_SIZE (16 * 1024)

/*  read csv files with the following three columns:
    1. ID - Identifier
    2. HR - Heart Rate reading
    3. Epoch - time stamp
    Derived in part from coreutils/wc
        https://github.com/coreutils/coreutils/blob/master/src/wc.c
    and the question below:
        https://stackoverflow.com/questions/3482064/counting-the-number-of-lines-in-a-text-file */
size_t 
read_csv(string filename, vector<int> &ID, vector<float> &HR, vector<int> &Epoch)
{
  size_t bytes_read;
  uintmax_t lines=0;
  char buf[BUFFER_SIZE+1];

  int fd = open (filename.c_str(), O_RDONLY);
  if (fd == -1)
    {
      cout << "Can not open file: " << filename << endl;
      exit(1);
    } 
    
  while ((bytes_read = read (fd, buf, BUFFER_SIZE)) > 0)
    {
      char *p = buf;
      char *end = buf + bytes_read;
      while (p != end)
        lines += *p++ == '\n';
    }
  
  lines--; //exclude header row

  ID.resize (lines);
  HR.resize (lines);
  Epoch.resize (lines);

  lseek (fd, 0, SEEK_SET);

  char *p = buf;
  char f[64];
  char j=0; //field character index
  //string f=""; //field string
  int i=0; //column/field index 
  // i = 0 : state of reading and skipping header line
  // i = 1  : state of reading ID field
  // i = 2  : state of reading HR field
  // i = 3  : state of reading Epoch field
  char d='\n'; //column delimiter 
  char *end = buf + bytes_read;
  uintmax_t r=0; //row

  while ( (bytes_read = read (fd, buf, BUFFER_SIZE)) > 0 )
    {
      char *p = buf;
      char *end = buf + bytes_read;
      while (p != end)
        {
          if (*p == d)
            {
              switch (i)
              {
                case 0:
                  // skip header line
                  i++;
                  j=0;
                  f[0]='\0'; //'\0'; //f.clear();
                  d = ',';
                  break;
                case 1:
                  f[j]='\0';
                  ID[r]=stoi(f);
                  j=0;
                  //f[0]='\0';
                  i++;
                  break;
                case 2:
                  f[j]='\0';
                  HR[r]=stof(f);
                  j=0;
                  //f[0]='\0';
                  i++;
                  d = '\n';
                  break;
                case 3:
                  f[j]='\0';
                  Epoch[r++]= (int) (stoi(f) / 86400); //Drop time and keep date
                  j=0;
                  //f[0]='\0';
                  i=1;
                  d = ',';
                  break;
              }
            }
          else
            {
              f[j++] = *p;  
            }
          p++;
        }
    }
  close(fd);
  return(lines);
}
 