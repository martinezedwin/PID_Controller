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
  /**
   * TODO: Initialize PID coefficients (and errors, if needed)
   */
  //PID::Kp = Kp_; //Tau_p
  //PID::Ki = Ki_; //Tau_i
  //PID::Kd = Kd_; //Tau_d
  
  PID::Kp = k[0]; //Tau_p
  PID::Ki = k[1]; //Tau_i
  PID::Kd = k[2]; //Tau_d

  //cte = 0.0;
  p_error = 0.0;
  d_error = 0.0;
  i_error = 0.0;

  //err = 0.0;


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

  //return (-(Kp*p_error)) - (Kd*d_error) - (Ki*i_error);  // TODO: Add your total error calc here!
  return -(Kp * p_error + Ki * i_error + Kd * d_error);
}

void PID::Restart(uWS::WebSocket<uWS::SERVER> ws) {
  std::string reset_msg = "42[\"reset\",{}]";
  ws.send(reset_msg.data(), reset_msg.length(), uWS::OpCode::TEXT);
}

double PID::Twiddle(PID &pid, double cte){
  double result;//vector<double> result;
  std::cout<<"Initiated Twiddle"<<std::endl;

  vector<double> dp = {1.0, 1.0, 1.0};
  vector<double> p = {Kp, Ki, Kd};

  pid.UpdateError(cte);
  result = pid.TotalError();


  //std::cout<<"Returning Total Error!"<<std::endl;
  return result;
}