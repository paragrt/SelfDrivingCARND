#include "ukf.h"
#include "Eigen/Dense"
#include <iostream>

using namespace std;
using Eigen::MatrixXd;
using Eigen::VectorXd;
using std::vector;

/**
 * Initializes Unscented Kalman filter
 */
UKF::UKF() {
  // if this is false, laser measurements will be ignored (except during init)
  use_laser_ = true;

  // if this is false, radar measurements will be ignored (except during init)
  use_radar_ = true;

  // initial state vector
  x_ = VectorXd(5);

  x_ << 0,0,0,0,0;
  // initial covariance matrix
  P_ = MatrixXd(5, 5);
  P_ << 0.4, 0, 0, 0,0,
          0, 0.4, 0, 0,0, 
          0, 0, 0.4, 0,0,
          0, 0, 0, 0.4,0,
          0, 0, 0, 0,0.4;


  // Process noise standard deviation longitudinal acceleration in m/s^2
  std_a_ = 0.8; //30;

  // Process noise standard deviation yaw acceleration in rad/s^2
  std_yawdd_ = .4; //30;

  // Laser measurement noise standard deviation position1 in m
  std_laspx_ = 0.15;

  // Laser measurement noise standard deviation position2 in m
  std_laspy_ = 0.15;

  // Radar measurement noise standard deviation radius in m
  std_radr_ = 0.3;

  // Radar measurement noise standard deviation angle in rad
  std_radphi_ = 0.03;

  // Radar measurement noise standard deviation radius change in m/s
  std_radrd_ = 0.3;

  /**
  TODO:

  Complete the initialization. See ukf.h for other member properties.

  Hint: one or more values initialized above might be wildly off...
  */
  n_x_ = 5;
  n_aug_ = 7;
}

UKF::~UKF() {}


void UpdateState(VectorXd* x_out, MatrixXd* P_out, VectorXd& z1, MatrixXd& s1, MatrixXd& Zsig, UKF& ukf, MeasurementPackage ms) {
  
  //set state dimension
  int n_x = ukf.n_x_;

  //set augmented dimension
  int n_aug = ukf.n_aug_;

  //set measurement dimension, (when typ==0 px,py & when typ==1 ==> radar can measure r, phi, and r_dot)
  int n_z = ms.sensor_type_==0?2:3;

  //define spreading parameter
  double lambda = 3 - n_aug;

  //set vector for weights
  VectorXd weights = VectorXd(2*n_aug+1);
   double weight_0 = lambda/(lambda+n_aug);
  weights(0) = weight_0;
  for (int i=1; i<2*n_aug+1; i++) {  //2n+1 weights
    double weight = 0.5/(n_aug+lambda);
    weights(i) = weight;
  }

  //create example matrix with predicted sigma points
  MatrixXd Xsig_pred = ukf.Xsig_pred_;

  //create example vector for predicted state mean
  VectorXd x = ukf.x_; 

  //create example matrix for predicted state covariance
  MatrixXd P = ukf.P_; 

  //create example matrix with sigma points in measurement space

  //create example vector for mean predicted measurement
  VectorXd z_pred = z1; 

  //create example matrix for predicted measurement covariance
  MatrixXd S = s1;

  //create example vector for incoming lidar/radar measurement
  VectorXd z = VectorXd(n_z);
  if ( ms.sensor_type_ == 0 ) {
     z << ms.raw_measurements_[0],
          ms.raw_measurements_[1];
  } else {
     z << ms.raw_measurements_[0],
          ms.raw_measurements_[1],
          ms.raw_measurements_[2];
  }
  //create matrix for cross correlation Tc
  MatrixXd Tc = MatrixXd(n_x, n_z);

/*******************************************************************************
 * Student part begin
 ******************************************************************************/

  //calculate cross correlation matrix
  Tc.fill(0.0);
  for (int i = 0; i < 2 * n_aug + 1; i++) {  //2n+1 simga points

    //residual
    VectorXd z_diff = Zsig.col(i) - z_pred;
    //angle normalization
    while (z_diff(1)> M_PI) z_diff(1)-=2.*M_PI;
    while (z_diff(1)<-M_PI) z_diff(1)+=2.*M_PI;

    // state difference
    VectorXd x_diff = Xsig_pred.col(i) - x;
    //angle normalization
    while (x_diff(3)> M_PI) x_diff(3)-=2.*M_PI;
    while (x_diff(3)<-M_PI) x_diff(3)+=2.*M_PI;

    Tc = Tc + weights(i) * x_diff * z_diff.transpose();
  }

  //Kalman gain K;
  MatrixXd K = Tc * S.inverse();

  //residual
  VectorXd z_diff = z - z_pred;

  //angle normalization
  while (z_diff(1)> M_PI) z_diff(1)-=2.*M_PI;
  while (z_diff(1)<-M_PI) z_diff(1)+=2.*M_PI;

  //update state mean and covariance matrix
  x = x + K * z_diff;
  P = P - K*S*K.transpose();

/*******************************************************************************
 * Student part end
 ******************************************************************************/

  //print result
  std::cout << "Updated state x: " << std::endl << x << std::endl;
  std::cout << "Updated state covariance P: " << std::endl << P << std::endl;

  //write result
  *x_out = x;
  *P_out = P;

}

//PREDICT STEP 4.
void PredictMeasurement(VectorXd* z_out, MatrixXd* S_out, MatrixXd& Zsig, UKF& ukf, int msrmnt_type) {

  std::cout << "PredictMeasurement: " << "sensortyp = " << msrmnt_type << std::endl ; 
  //set state dimension
  int n_x = ukf.n_x_;

  //set augmented dimension
  int n_aug = ukf.n_aug_;

  //set measurement dimension, lidar can measure x, y
  //set measurement dimension, radar can measure r, phi, and r_dot

  int n_z = (msrmnt_type == 0)?2:3;

  //define spreading parameter
  double lambda = 3 - n_aug;
  
  MatrixXd& Xsig_pred = ukf.Xsig_pred_;
  
  //set vector for weights
  VectorXd weights = VectorXd(2*n_aug+1);
   double weight_0 = lambda/(lambda+n_aug);
  weights(0) = weight_0;
  for (int i=1; i<2*n_aug+1; i++) {  
    double weight = 0.5/(n_aug+lambda);
    weights(i) = weight;
  }

  //radar measurement noise standard deviation radius in m
  double std_radr = ukf.std_radr_;

  //radar measurement noise standard deviation angle in rad
  double std_radphi = ukf.std_radphi_;

  //radar measurement noise standard deviation radius change in m/s
  double std_radrd = ukf.std_radrd_;


/*******************************************************************************
 * Student part begin
 ******************************************************************************/

  //transform sigma points into measurement space
  for (int i = 0; i < 2 * n_aug + 1; i++) {  //2n+1 simga points

    // extract values for better readibility
    double p_x = Xsig_pred(0,i);
    double p_y = Xsig_pred(1,i);

    // measurement model
    if (msrmnt_type == 0) {  
       Zsig(0,i) = p_x;                        //px
       Zsig(1,i) = p_y;                        //py
    } else {
       double v  = Xsig_pred(2,i);
       double yaw = Xsig_pred(3,i);
       double v1 = cos(yaw)*v;
       double v2 = sin(yaw)*v;

       Zsig(0,i) = sqrt(p_x*p_x + p_y*p_y);                        //r
       Zsig(1,i) = atan2(p_y,p_x);                                 //phi
       Zsig(2,i) = (p_x*v1 + p_y*v2 ) / sqrt(p_x*p_x + p_y*p_y);   //r_dot
    }
  }

  //mean predicted measurement
  VectorXd z_pred = VectorXd(n_z);
  z_pred.fill(0.0);
  for (int i=0; i < 2*n_aug+1; i++) {
      z_pred = z_pred + weights(i) * Zsig.col(i);
  }

  std::cout << "PredictMeasurement3: " << "sensortyp = " << msrmnt_type << std::endl ; 
  //measurement covariance matrix S
  MatrixXd S = MatrixXd(n_z,n_z);
  S.fill(0.0);
  for (int i = 0; i < 2 * n_aug + 1; i++) {  //2n+1 simga points
    //residual
    VectorXd z_diff = Zsig.col(i) - z_pred;

    //angle normalization
    while (z_diff(1)> M_PI) z_diff(1)-=2.*M_PI;
    while (z_diff(1)<-M_PI) z_diff(1)+=2.*M_PI;

    S = S + weights(i) * z_diff * z_diff.transpose();
  }

  std::cout << "PredictMeasurement4: " << "sensortyp = " << msrmnt_type << std::endl ; 
  //add measurement noise covariance matrix
  MatrixXd R = MatrixXd(n_z,n_z);
  if (msrmnt_type == 0) {  
     R <<    ukf.std_laspx_*ukf.std_laspx_,  0,
          0, ukf.std_laspy_*ukf.std_laspy_;
  }else{   
     R <<    std_radr*std_radr, 0, 0,
          0, std_radphi*std_radphi, 0,
          0, 0,std_radrd*std_radrd;
  }
  S = S + R;

  
/*******************************************************************************
 * Student part end
 ******************************************************************************/

  //print result
  std::cout << "z_pred: " << std::endl << z_pred << std::endl;
  std::cout << "S: " << std::endl << S << std::endl;

  //write result
  *z_out = z_pred;
  *S_out = S;

}


//PREDICT STEP 3.
void PredictMeanAndCovariance(VectorXd* x_out, MatrixXd* P_out, MatrixXd& Xsig_pred,  UKF& ukf) {

  //set state dimension
  int n_x = ukf.n_x_;

  //set augmented dimension
  int n_aug = ukf.n_aug_;

  //define spreading parameter
  double lambda = 3 - n_aug;

  //create vector for weights
  VectorXd weights = VectorXd(2*n_aug+1);
  
  //create vector for predicted state
  VectorXd x = VectorXd(n_x);

  //create covariance matrix for prediction
  MatrixXd P = MatrixXd(n_x, n_x);
  

  // set weights
  double weight_0 = lambda/(lambda+n_aug);
  weights(0) = weight_0;
  for (int i=1; i<2*n_aug+1; i++) {  //2n+1 weights
    double weight = 0.5/(n_aug+lambda);
    weights(i) = weight;
  }

  //predicted state mean
  x.fill(0.0);
  for (int i = 0; i < 2 * n_aug + 1; i++) {  //iterate over sigma points
    x = x+ weights(i) * Xsig_pred.col(i);
  }

  //predicted state covariance matrix
  P.fill(0.0);
  for (int i = 0; i < 2 * n_aug + 1; i++) {  //iterate over sigma points

    // state difference
    VectorXd x_diff = Xsig_pred.col(i) - x;
    //angle normalization
    while (x_diff(3)> M_PI) x_diff(3)-=2.*M_PI;
    while (x_diff(3)<-M_PI) x_diff(3)+=2.*M_PI;

    P = P + weights(i) * x_diff * x_diff.transpose() ;
  }

  //print result
  std::cout << "Predicted state" << std::endl;
  std::cout << x << std::endl;
  std::cout << "Predicted covariance matrix" << std::endl;
  std::cout << P << std::endl;

  //write result
  *x_out = x;
  *P_out = P;
}

//PREDICT STEP 2.
void SigmaPointPrediction(MatrixXd* Xsig_out, MatrixXd& Xsig_aug, UKF& ukf, double delta_t) {

  //set state dimension
  int n_x = ukf.n_x_;

  //set augmented dimension
  int n_aug = ukf.n_aug_;

  //create matrix with predicted sigma points as columns
  MatrixXd Xsig_pred = MatrixXd(n_x, 2 * n_aug + 1);

/*******************************************************************************
 * Student part begin
 ******************************************************************************/

  //predict sigma points
  for (int i = 0; i< 2*n_aug+1; i++)
  {
    //extract values for better readability
    double p_x = Xsig_aug(0,i);
    double p_y = Xsig_aug(1,i);
    double v = Xsig_aug(2,i);
    double yaw = Xsig_aug(3,i);
    double yawd = Xsig_aug(4,i);
    double nu_a = Xsig_aug(5,i);
    double nu_yawdd = Xsig_aug(6,i);

    //predicted state values
    double px_p, py_p;

    //avoid division by zero
    if (fabs(yawd) > 0.001) {
        px_p = p_x + v/yawd * ( sin (yaw + yawd*delta_t) - sin(yaw));
        py_p = p_y + v/yawd * ( cos(yaw) - cos(yaw+yawd*delta_t) );
    }
    else {
        px_p = p_x + v*delta_t*cos(yaw);
        py_p = p_y + v*delta_t*sin(yaw);
    }

    double v_p = v;
    double yaw_p = yaw + yawd*delta_t;
    double yawd_p = yawd;

    //add noise
    px_p = px_p + 0.5*nu_a*delta_t*delta_t * cos(yaw);
    py_p = py_p + 0.5*nu_a*delta_t*delta_t * sin(yaw);
    v_p = v_p + nu_a*delta_t;

    yaw_p = yaw_p + 0.5*nu_yawdd*delta_t*delta_t;
    yawd_p = yawd_p + nu_yawdd*delta_t;

    //write predicted sigma point into right column
    Xsig_pred(0,i) = px_p;
    Xsig_pred(1,i) = py_p;
    Xsig_pred(2,i) = v_p;
    Xsig_pred(3,i) = yaw_p;
    Xsig_pred(4,i) = yawd_p;
  }

  //print result
  std::cout << "Xsig_pred = " << std::endl << Xsig_pred << std::endl;

  //write result
  *Xsig_out = Xsig_pred;

}

//PREDICT STEP 1.
void AugmentedSigmaPoints(MatrixXd* Xsig_out, UKF& ukf ) {

  //set state dimension
  int n_x = ukf.n_x_;

  //set augmented dimension
  int n_aug = ukf.n_aug_;

  //Process noise standard deviation longitudinal acceleration in m/s^2
  double std_a = ukf.std_a_;

  //Process noise standard deviation yaw acceleration in rad/s^2
  double std_yawdd = ukf.std_yawdd_;

  //define spreading parameter
  double lambda = 3 - n_aug;

  cout << "set state vector n_x=" << n_x << "\n";
  //set example state
  VectorXd x = VectorXd(n_x);
  x << ukf.x_;   


  //create example covariance matrix
  MatrixXd P = MatrixXd(n_x, n_x);
  P << ukf.P_;  

  cout << "set covariance matrix\n";
  

  //create augmented mean vector
  VectorXd x_aug = VectorXd(7);

  //create augmented state covariance
  MatrixXd P_aug = MatrixXd(7, 7);

  //create sigma point matrix
  MatrixXd Xsig_aug = MatrixXd(n_aug, 2 * n_aug + 1);

  //create augmented mean state
  x_aug.head(5) = x;
  x_aug(5) = 0;
  x_aug(6) = 0;

  //create augmented covariance matrix
  P_aug.fill(0.0);
  P_aug.topLeftCorner(5,5) = P;
  P_aug(5,5) = std_a*std_a;
  P_aug(6,6) = std_yawdd*std_yawdd;

  //create square root matrix
  MatrixXd L = P_aug.llt().matrixL();

  //create augmented sigma points
  Xsig_aug.col(0)  = x_aug;
  for (int i = 0; i< n_aug; i++)
  {
    Xsig_aug.col(i+1)       = x_aug + sqrt(lambda+n_aug) * L.col(i);
    Xsig_aug.col(i+1+n_aug) = x_aug - sqrt(lambda+n_aug) * L.col(i);
  }
  
  //print result
  std::cout << "STEP 1. Xsig_aug = " << std::endl << Xsig_aug << std::endl;

  //write result
  *Xsig_out = Xsig_aug;
}


/**
 * @param {MeasurementPackage} meas_package The latest measurement data of
 * either radar or laser.
 */
void UKF::ProcessMeasurement(MeasurementPackage meas_package) {
  /**
  TODO:

  Complete this function! Make sure you switch between lidar and radar
  measurements.
  */
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
    cout << "UKF: " << endl;
    cout << " Measurement Package sensor type: " << meas_package.sensor_type_ << endl; 
    cout << " Measurement Package raw msrmnts: " << meas_package.raw_measurements_ << endl; 
    if (meas_package.sensor_type_ == MeasurementPackage::RADAR) {

      float px = meas_package.raw_measurements_[0] * cos((float)meas_package.raw_measurements_[1]);// rho * cos phi
      float py = meas_package.raw_measurements_[0] * sin((float)meas_package.raw_measurements_[1]);// rho * sin phi
            x_ << px,
                  py,
                  meas_package.raw_measurements_[2],
                  0,0;
    }
    else if (meas_package.sensor_type_ == MeasurementPackage::LASER) {
      //measurement covariance LASER
      x_ << meas_package.raw_measurements_[0],
            meas_package.raw_measurements_[1],
            0,0,0;
    }
    cout << "Sensor Type=" << meas_package.sensor_type_ <<endl;
    time_us_ = meas_package.timestamp_;
    // done initializing, no need to predict or update
    is_initialized_ = true;
    return;
  }

  //find delta_t
  double delta_t = ( meas_package.timestamp_ - time_us_ )/1000000.0 ;
  //update timestamp for next iteration
  time_us_ = meas_package.timestamp_;
 
  //predict
  Prediction(delta_t); 

  // if radar vs lidar call appropriate updates
  if ( meas_package.sensor_type_ == MeasurementPackage::LASER ) {
	UpdateLidar(meas_package);
  } else if ( meas_package.sensor_type_ == MeasurementPackage::RADAR ) {
	UpdateRadar(meas_package);
  } else {
	cerr << "Data from unknown sensor\n";
  }
}

/**
 * Predicts sigma points, the state, and the state covariance matrix.
 * @param {double} delta_t the change in time (in seconds) between the last
 * measurement and this one.
 */
void UKF::Prediction(double delta_t) {
  /**
  TODO:

  Complete this function! Estimate the object's location. Modify the state
  vector, x_. Predict sigma points, the state, and the state covariance matrix.
  */

  //PREDICT STEP 1.
  //Copied from Augumentation Assignment
  //if we had been allowed to edit ukf.h, I would have added AugumentedSignmaPoints as a private method
  //and avoided passing *this by reference.
  MatrixXd Xsig_aug = MatrixXd(15, 7);
  AugmentedSigmaPoints(&Xsig_aug, *this);
  cout << "STEP 1. Prediction done\n";

  //PREDICT STEP 2.
  //copied from Sigma Point Prediction Assignment
  MatrixXd Xsig_pred = MatrixXd(15, 5);
  SigmaPointPrediction(&Xsig_pred, Xsig_aug, *this, delta_t); 
  cout << "STEP 2. Prediction done\n";

  //PREDICT STEP 3.
  //Copied from Predicted Mean and Covariance Assignment
  VectorXd x_pred = VectorXd(5);
  MatrixXd P_pred = MatrixXd(5, 5);
  PredictMeanAndCovariance(&x_pred, &P_pred, Xsig_pred, *this);
  cout << "STEP 3. Prediction done\n";

  //STORE my objects state variables
  x_ =  x_pred;
  P_ =  P_pred;
  Xsig_pred_ =  Xsig_pred;
  cout << "STEP 1,2,3. Prediction done\n";

}

/**
 * Updates the state and the state covariance matrix using a laser measurement.
 * @param {MeasurementPackage} meas_package
 */
void UKF::UpdateLidar(MeasurementPackage meas_package) {
  /**
  TODO:

  Complete this function! Use lidar data to update the belief about the object's
  position. Modify the state vector, x_, and covariance, P_.

  You'll also need to calculate the lidar NIS.
  */
  //PREDICT STEP 4. Measurement Prediction
  VectorXd z_out = VectorXd(2);
  MatrixXd S_out = MatrixXd(2, 2);
  //create matrix for sigma points in measurement space
  MatrixXd zSig = MatrixXd(2, 2 * n_aug_ + 1);
  PredictMeasurement(&z_out, &S_out, zSig, *this, 0); 

  VectorXd x_out = VectorXd(5);
  MatrixXd P_out = MatrixXd(5, 5);
  UpdateState(&x_out, &P_out, z_out, S_out, zSig, *this, meas_package);
}

/**
 * Updates the state and the state covariance matrix using a radar measurement.
 * @param {MeasurementPackage} meas_package
 */
void UKF::UpdateRadar(MeasurementPackage meas_package) {
  /**
  TODO:

  Complete this function! Use radar data to update the belief about the object's
  position. Modify the state vector, x_, and covariance, P_.

  You'll also need to calculate the radar NIS.
  */
  //PREDICT STEP 4. Measurement Prediction
  VectorXd z_out = VectorXd(3);
  MatrixXd S_out = MatrixXd(3, 3);
  //create matrix for sigma points in measurement space
  MatrixXd zSig = MatrixXd(3, 2 * n_aug_ + 1);
  PredictMeasurement(&z_out, &S_out, zSig, *this, 1); 
  
  VectorXd x_out = VectorXd(5);
  MatrixXd P_out = MatrixXd(5, 5);
  UpdateState(&x_out, &P_out, z_out, S_out, zSig, *this, meas_package);

  x_ << x_out;
  P_ << P_out;
}
