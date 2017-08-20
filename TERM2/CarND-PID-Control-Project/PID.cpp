#include "PID.h"

using namespace std;

/*
* TODO: Complete the PID class.
*/
double prev_error = 0;
double sum_error = 0;
PID::PID() {}

PID::~PID() {}

void PID::Init(double aKp, double aKi, double aKd) {
   Kp = aKp;
   Kd = aKi;
   Ki = aKd;
   p_error = 0;
   d_error = 0;
   i_error = 0;
   printf("USING Kp=%f, Ki=%f, Kd=%f\n", Kp, Ki, Kd);
}

void PID::UpdateError(double cte) {
   p_error = Kp * cte;
   d_error = Kd * (cte - prev_error);
   prev_error = cte;
   sum_error += cte;
   i_error = Ki * sum_error;
}

double PID::TotalError() {
    return p_error + d_error + i_error;
}

