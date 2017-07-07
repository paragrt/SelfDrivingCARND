#include <iostream>
#include "kalman_filter.h"
#include "tools.h"

using Eigen::MatrixXd;
using Eigen::VectorXd;

KalmanFilter::KalmanFilter() {}

KalmanFilter::~KalmanFilter() {}

void KalmanFilter::Init(VectorXd &x_in, MatrixXd &P_in, MatrixXd &F_in,
                        MatrixXd &H_in, MatrixXd &R_in, MatrixXd &Q_in) {
  x_ = x_in;
  P_ = P_in;
  F_ = F_in;
  H_ = H_in;
  R_ = R_in;
  Q_ = Q_in;
}

void KalmanFilter::Predict() {
  /**
  TODO:
    * predict the state
  */
    x_ = F_ * x_;
    MatrixXd Ft = F_.transpose();
    P_ = F_ * P_ * Ft + Q_;
}

void KalmanFilter::Update(const VectorXd &z) {
  /**
  TODO:
    * update the state by using Kalman Filter equations
  */
    VectorXd z_pred = H_ * x_;
    VectorXd y = z - z_pred;
    MatrixXd Ht = H_.transpose();
    MatrixXd S = H_ * P_ * Ht + R_;
    MatrixXd Si = S.inverse();
    MatrixXd PHt = P_ * Ht;
    MatrixXd K = PHt * Si;

    //new estimate
    x_ = x_ + (K * y);
    long x_size = x_.size();
    MatrixXd I = MatrixXd::Identity(x_size, x_size);
    P_ = (I - K * H_) * P_;
}

     double normalizeAngle(double theta) {
        while ( theta > M_PI )
        {
            theta = theta - 2*M_PI;
        }
        while ( theta < (-1.0*M_PI) )
        {
            theta = theta + 2*M_PI;
        }
        return theta;
     }
void KalmanFilter::UpdateEKF(const VectorXd &z) {
  /**
  TODO:
    * update the state by using Extended Kalman Filter equations
  */
     VectorXd hOfx_= VectorXd(3);
     double px = x_(0);
     double py = x_(1);
     double vx = x_(2);
     double vy = x_(3);
     double rho = sqrt(px*px + py*py);
     if((fabs(rho) < 0.0001)
      || (fabs(px) < 0.0001) ){
        cout << "Skipping update atan2 is undefined\n" << hOfx_ << endl;
        return;
     }
     double theta = atan2(py,px);
     theta = normalizeAngle(theta);
     hOfx_(0) = rho ;
     hOfx_(1) = theta ;
     hOfx_(2) = (  px*vx + py*vy ) /rho ;

     VectorXd y = z - hOfx_;
     y(1) = normalizeAngle(y(1));
     MatrixXd Ht = H_.transpose();
     MatrixXd S = H_ * P_ * Ht + R_;
     MatrixXd Si = S.inverse();
     MatrixXd PHt = P_ * Ht;
     MatrixXd K = PHt * Si;

     //new estimate
     x_ = x_ + (K * y);
     long x_size = x_.size();
     MatrixXd I = MatrixXd::Identity(x_size, x_size);
     P_ = (I - K * H_) * P_;
}
