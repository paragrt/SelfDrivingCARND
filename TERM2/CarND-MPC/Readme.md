<h1>Using model Predictive Controller to drive the car around the track.</h1>

<h2>State and Model</h2>
In main.cpp, the current state is received from simulator ( lines 90 to 97 ).  
It is converted to vehicle co-ordinates with origin at the car.  
Using the waypoints, the desired polynomial fit(3rd order) is calculated.  
Using the coefficients from above polynomial, The Cross Track error and Orientation error is calculated at lines 117, 120.  
The MPC solver is fed the data of waypoints in car co-ordinates and returns the steer and throttle and predicted trajectory in reply.  
These points are plotted ( mpc & next ) to compare the closeness of fit.


