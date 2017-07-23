<h1>The Unscented Kalman Filter</h1>

<h2>Updated to include LIDAR update at July 23 11:19 AM CDT</h2>
I had forgotten to update the x_ and P_ variables after doing all the LIDAR calcs.
Adding it brought the RMSE values well within the  RUBRIC
<img src="./ukf-result-20170724_FINAL.png"/>

<h3>Tunable Parameters:</h3>
<pre>
P_ << 0.4, 0, 0, 0,0,
          0, 0.4, 0, 0,0,
          0, 0, 0.4, 0,0,
          0, 0, 0, 0.4,0,
          0, 0, 0, 0,0.4;


  // Process noise standard deviation longitudinal acceleration in m/s^2
  std_a_ = 0.8; 

  // Process noise standard deviation yaw acceleration in rad/s^2
  std_yawdd_ = .4; 
  </code>
  
  <h3>Results:</h3>
  <img src="./ukf-result-20170724.png"/>
