#include <math.h>
#include <uWS/uWS.h>
#include <iostream>
#include <string>
#include "json.hpp"
#include "PID.h"

// for convenience
using nlohmann::json;
using std::string;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
string hasData(string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != string::npos) {
    return "";
  }
  else if (b1 != std::string::npos && b2 != std::string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}

int main() {
  uWS::Hub h;

  PID steer_pid; //declare steering pid of type PID
  PID pid_speed;

  /**
   * TODO: Initialize the pid variable.
   */
  double k_p = 0.044;//0.097221000; //0.14;//0.044;//0.038; //0.2; //0.35;  //0.1; 
  double k_i = 0.000005;//0.000018;    //0.0001;//0.0001; //0.000005;   //0.004; //0.01;  //0.0001;
  double k_d = 2.0;//1.992889;    //2.5;//0.0001;//180.0;  //3.0; //0.004; //1.0;

  double s_k_p = 0.109170;  //0.14;//0.044;//0.038; //0.2; //0.35;  //0.1; 
  double s_k_i = 0.000754;  //0.0001;//0.0001; //0.000005;   //0.004; //0.01;  //0.0001;
  double s_k_d = 0.841226;  //2.5;//0.0001;//180.0;  //3.0; //0.004; //1.0;

  std::vector<double> k = {k_p, k_i, k_d};

  steer_pid.Init(k);//k_p, k_i, k_d);           //Initialize the steering pid with k_p, k_i, and k_d constants
  pid_speed.Init(k);//s_k_p, s_k_i, s_k_d);     //Initialize the speed pid with k_p, k_i, and k_d constants

  h.onMessage([&steer_pid, &pid_speed](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, 
                     uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event

    if (length && length > 2 && data[0] == '4' && data[1] == '2') {
      auto s = hasData(string(data).substr(0, length));
      if (s != "") {
        auto j = json::parse(s);
        string event = j[0].get<string>();
        if (event == "telemetry") {
          // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<std::string>());           //Read in cross track error (cte)
          double speed = std::stod(j[1]["speed"].get<string>());            //Read in speed
          double angle = std::stod(j[1]["steering_angle"].get<string>());   //Read in steering angle
          
          double steer_value; //Declare the steering value we will 
          double steer_value_Twiddle;
          double gas_value;
          /**
           * TODO: Calculate steering value here, remember the steering value is
           *   [-1, 1].
           * NOTE: Feel free to play around with the throttle and speed.
           *   Maybe use another PID controller to control the speed!
           */
          
          //steer_pid.UpdateError(cte);            //Calculate the errors for each P, I, and D.
          //steer_value = steer_pid.TotalError();  //Calculate hte total error PID and set the steering value to that.

          steer_value_Twiddle = steer_pid.Twiddle(steer_pid, cte);
          steer_value = steer_value_Twiddle; 

          //pid_speed.UpdateError(speed-20);
          //gas_value = pid_speed.TotalError();
          
          // DEBUG
          std::cout << "CTE: " << cte << " Steering Value: " << steer_value 
                    << std::endl;

          json msgJson;
          msgJson["steering_angle"] = steer_value;               //Feed the steering value to 
          msgJson["throttle"] = 0.2;//gas_value;
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          std::cout << msg << std::endl;
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }  // end "telemetry" if
      } else {
        // Manual driving
        string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }  // end websocket message if
  }); // end h.onMessage

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, 
                         char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port)) {
    std::cout << "Listening to port " << port << std::endl;
  } else {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  
  h.run();
}