<h1>Extended Kalman Filter</h1>

0. Using series of (Predict Step)guesses of location(using physical model)  
and (Update step) measurements(potentially noisy)....  
develop a better estimate of actual location of a moving object ( such as a vehicle )  
than can be developed by purely the physical model or purely measurement. 

1. Had to update CMakelists.txt to use libuv 1.12 but did not use any functions specific to 1.12
link_directories(/usr/local/Cellar/libuv/1.12.0/lib)

2. Initially I was only getting [0.14, 0.6668, 0.6051, 1.6270]
even though I was normalizing atan2

3. The trick was to normalize after the step 
y = z - hOfx_;
y(1) contains the angle and it needed to stay within -pi and +pi
Thats all!

4. Blue & Red dots are Radar/LIDAR measurements and the green dots are the predictions. My final ROOT MEAN SQUARE ERROR ( RMSE) values were within project rubric...<img src="https://github.com/paragrt/SelfDrivingCARND/blob/master/TERM2/EKF/FInalRMSE.png" />
