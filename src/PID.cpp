#include "PID.h"
#include <iostream>
#include <vector>
#include <uWS/uWS.h>

using namespace std;

/**
 * TODO: Complete the PID class. You may add any additional desired functions.
 */

PID::PID() {}

PID::~PID() {}

void PID::Init(std::vector<double> &k){//double Kp_, double Ki_, double Kd_) {  
  
  PID::Kp = k[0]; //Tau_p
  PID::Ki = k[1]; //Tau_i
  PID::Kd = k[2]; //Tau_d

  //cte = 0.0;
  p_error = 0.0;
  d_error = 0.0;
  i_error = 0.0;

}

void PID::UpdateError(double cte) {

  p_error = cte;             //p error
  d_error = cte - p_error;   //d error
  i_error += cte;            //i error 


}

double PID::TotalError() {

  return -(Kp * p_error + Ki * i_error + Kd * d_error);
}