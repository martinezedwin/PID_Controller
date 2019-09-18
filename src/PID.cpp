#include "PID.h"
#include <iostream>
#include <vector>

using namespace std;

/**
 * TODO: Complete the PID class. You may add any additional desired functions.
 */

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp_, double Ki_, double Kd_) {
  /**
   * TODO: Initialize PID coefficients (and errors, if needed)
   */
  PID::Kp = Kp_; //Tau_p
  PID::Ki = Ki_; //Tau_i
  PID::Kd = Kd_; //Tau_d
 
  //cte = 0.0;
  p_error = 0.0;
  d_error = 0.0;
  i_error = 0.0;

  //std::cout<<"Done Initializing"<<std::endl;

}

void PID::UpdateError(double cte) {
  /**
   * TODO: Update PID errors based on cte.
   */
  std::cout<<"Starting error update"<<std::endl;
  p_error = cte;             //p error
  d_error = cte - p_error;   //d error
  i_error += cte;            //i error 


}

double PID::TotalError() {
  /**
   * TODO: Calculate and return the total error
   */


  return (-(Kp*p_error)) - (Kd*d_error) - (Ki*i_error);  // TODO: Add your total error calc here!
}

double PID::Twiddle(PID &pid, double cte){
  double result;//vector<double> result;
  std::cout<<"Initiated Twiddle"<<std::endl;

  vector<double> dp = {0.0, 0.0, 0.0};
  vector<double> p = {Kp, Ki, Kd};

  pid.UpdateError(cte);
  result = pid.TotalError();

  std::cout<<"Returning Total Error!"<<std::endl;
  return result;
}