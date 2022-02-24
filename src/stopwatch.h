/* Author: Ahmed Soliman
   Date: July 25, 2021
   File: stopwatch.h
   Description: header file for the stopwatch class.*/

#ifndef __STOPWATCH_H__
#define __STOPWATCH_H__

#include <chrono>
#include <string>

enum TIME_UNIT { TIME_UNIT_MS, TIME_UNIT_SEC };

class stopwatch
  {
    private:

      //time_point marking last time start() was called.
      static std::chrono::time_point
                <std::chrono::high_resolution_clock> time_start;
      
      //sum of all durations since the stop watch has been reset.
      static std::chrono::nanoseconds total_time_taken_ns; 

    //member variables
    public: 
      
      /* constructor */
      stopwatch();

      /* reset the stop watch */
      void reset();

      /* start the stop watch */
      void start();
      
      /*  Get the time duration from the last time start() has been called.
          Add the time duration to the total time taken.
          Print time taken along with a custom informative message.
          parameters:
            start : high resolution time point returned by calling 
                  the function high_resolution_clock::now();
                  This should be called right before executing the 
                  set of instructions to be timed.
            time_unit : [s|ms] "s" for seconds and "ms" for milliseconds.
            msg : a custom message to be printed out along the output
          return:
            time duration in seconds*/
      double lap(TIME_UNIT time_unit, std::string msg= "Time taken : ");

      double lap(std::string msg= " Total time : ");
      
      /* Print total time taken in detailed units. 
         Also return time taken in seconds*/
      double total();

      /* Get total time in seconds */
      double gettotalseconds();
  };

#endif