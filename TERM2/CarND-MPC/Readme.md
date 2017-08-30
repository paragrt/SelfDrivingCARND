<h1>Using model Predictive Controller to drive the car around the track.</h1>

<h2>State and Model</h2>
<ol>
<li>In main.cpp, the current state is received from simulator ( lines 90 to 97 ).  </li>
<li>It is converted to vehicle co-ordinates with origin at the car.  </li>
<li>Using the waypoints, the desired polynomial fit(3rd order) is calculated.</li>  
<li>Using the coefficients from above polynomial, The Cross Track error and Orientation error is calculated at lines 117, 120.  </li>
<li>The MPC solver is fed the data of waypoints in car co-ordinates and returns the steer and throttle and predicted trajectory in reply.  </li>
<li>These points are plotted ( mpc & next ) to compare the closeness of fit.</li>
</ol>


