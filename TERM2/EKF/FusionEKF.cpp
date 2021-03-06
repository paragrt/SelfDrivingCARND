#include "FusionEKF.h"
#include "tools.h"
#include "Eigen/Dense"
#include <iostream>

using namespace std;
using Eigen::MatrixXd;
using Eigen::VectorXd;
using std::vector;

/*
 * Constructor.
 */
FusionEKF::FusionEKF() {
  is_initialized_ = false;

  previous_timestamp_ = 0;

  // initializing matrices
  R_laser_ = MatrixXd(2, 2);
  R_radar_ = MatrixXd(3, 3);
  H_laser_ = MatrixXd(2, 4);
  Hj_ = MatrixXd(3, 4);

  //measurement covariance matrix - laser
  R_laser_ << 0.0225, 0,
        0, 0.0225;

  //measurement covariance matrix - radar
  R_radar_ << 0.09, 0, 0,
        0, 0.0009, 0,
        0, 0, 0.09;

  /**
  TODO:
    * Finish initializing the FusionEKF.
    * Set the process and measurement noises
 */
  //state covariance matrix P
    ekf_.P_ = MatrixXd(4, 4);
    ekf_.P_ << 1, 0, 0, 0,
          0, 1, 0, 0,
          0, 0, 1000, 0,
          0, 0, 0, 1000;

    //measurement matrix
    H_laser_ = MatrixXd(2, 4);
    H_laser_ << 1, 0, 0, 0,
          0, 1, 0, 0;

    //the initial transition matrix F_
    ekf_.F_ = MatrixXd(4, 4);
    ekf_.F_ << 1, 0, 1, 0,
          0, 1, 0, 1,
          0, 0, 1, 0,
          0, 0, 0, 1;

}

/**
* Destructor.
*/
FusionEKF::~FusionEKF() {}

void FusionEKF::ProcessMeasurement(const MeasurementPackage &measurement_pack) {


  /*****************************************************************************
   *  Initialization
   ****************************************************************************/
  if (!is_initialized_) {
    /**
    TODO:
      * Initialize the state ekf_.x_ with the first measurement.
      * Create the covariance matrix.
      * Remember: you'll need to convert radar from polar to cartesian coordinates.
    */
    // first measurement
    cout << "EKF: " << endl;

    //convert to cartesian
    ekf_.x_ = VectorXd(4);
    if (measurement_pack.sensor_type_ == MeasurementPackage::RADAR) {

      /**
            Convert radar from polar to cartesian coordinates and initialize state.
            Initialize state.
            FROM the Tips and Tricks....this is a bit incomplete / confusing
            Initializing the State Vector
            You'll need to initialize the state vector  with the first sensor measurement.
            Although radar gives velocity data in the form of the range rate
            ​ρ, a radar measurement does not contain enough information to determine the state variable velocities v​x
            ​​  and v​y. You can, however, use the radar measurements ρ and ϕ to initialize the state variable locations px & py
            So ignore the vx and vy and just set px py ?

            */

      //measurement covariance RADAR
      ekf_.R_ = R_radar_;

      double px = measurement_pack.raw_measurements_[0] * cos((double)measurement_pack.raw_measurements_[1]);// rho * cos phi
      double py = measurement_pack.raw_measurements_[0] * sin((double)measurement_pack.raw_measurements_[1]);// rho * sin phi
            ekf_.x_ << px,
                    py,
                    0,0;


    }
    else if (measurement_pack.sensor_type_ == MeasurementPackage::LASER) {
      //measurement covariance LASER
      ekf_.R_ = R_laser_;
      ekf_.x_ << measurement_pack.raw_measurements_[0],
              measurement_pack.raw_measurements_[1],0,0;
    }
    cout << "Sensor Type=" << measurement_pack.sensor_type_ <<endl;
    previous_timestamp_ = measurement_pack.timestamp_;
    // done initializing, no need to predict or update
    is_initialized_ = true;
    return;
  }

  //cout << "is initialized = true" << endl;
  /*****************************************************************************
   *  Prediction
   ****************************************************************************/

  /**
   TODO:
     * Update the state transition matrix F according to the new elapsed time.
      - Time is measured in seconds.
     * Update the process noise covariance matrix.
     * Use noise_ax = 9 and noise_ay = 9 for your Q matrix.
   */
  //compute the time elapsed between the current and previous measurements
  double noise_ax = 9.0;
  double noise_ay = 9.0;

    double dt = (measurement_pack.timestamp_ - previous_timestamp_) / 1000000.0; //dt - expressed in seconds
    previous_timestamp_ = measurement_pack.timestamp_;

    double dt_2 = dt * dt;
    double dt_3 = dt_2 * dt;
    double dt_4 = dt_3 * dt;

    //Modify the F matrix so that the time is integrated
    ekf_.F_(0, 2) = dt;
    ekf_.F_(1, 3) = dt;

    //set the process covariance matrix Q
    ekf_.Q_ = MatrixXd(4, 4);
    ekf_.Q_ <<  dt_4/4*noise_ax, 0, dt_3/2*noise_ax, 0,
           0, dt_4/4*noise_ay, 0, dt_3/2*noise_ay,
           dt_3/2*noise_ax, 0, dt_2*noise_ax, 0,
           0, dt_3/2*noise_ay, 0, dt_2*noise_ay;

    //cout << "About to predict\n";
  ekf_.Predict();

  /*****************************************************************************
   *  Update
   ****************************************************************************/

  /**
   TODO:
     * Use the sensor type to perform the update step.
     * Update the state and covariance matrices.
   */

  if (measurement_pack.sensor_type_ == MeasurementPackage::RADAR) {
    // Radar update

    //cout << "ABOUT TO CALL jacobian " << endl;
    Hj_ = tools.CalculateJacobian(ekf_.x_);
    if ( Hj_.isZero(0.0001)  ) {
       cout << "Divide by zero...skipping update";
       return;
    }
    ekf_.H_ = Hj_;
    //cout << "DONE Calling jacobian Hj_=" << Hj_ << endl;
    //cout << "About to UpdateEKF RADAR\n";
    ekf_.R_ = R_radar_;
    ekf_.UpdateEKF(measurement_pack.raw_measurements_);
  } else {
    // Laser updates
    //cout << "About to Update LASER\n";
    ekf_.R_ = R_laser_;
    ekf_.H_ = H_laser_;
    ekf_.Update(measurement_pack.raw_measurements_);
  }

  // print the output
  //cout << "x_ = " << ekf_.x_ << endl;
  //cout << "P_ = " << ekf_.P_ << endl;
}
