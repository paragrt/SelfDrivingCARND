<h1>Using model Predictive Controller to drive the car around the track.</h1>
<h2>Resubmit ( added MPC.h due to addition of prev_delta_ and prev_accel_ ) </h2>
Added latency handling by predicting car state 100ms in future.
Changed the N to 10 and dt to .12 seconds rather than 0.1 or less. This has the benefit of driving with looking further out into the road/future(as evidenced by the green line which extends further and also the earlier application of brakes when approaching a sharp turn). This allowed me to set ref velocity to 85mph(up from 50) and the ride is much smoother. When dt was smaller there was a tendency to look just in front of the car and take "short-sighted corrections" giving much more zigzagging ride and overcorrecting(a typical new driver mistake)...thus not being able to go fast.
<pre>
These are the Kinematic model equations used to predict the car position 100ms in the future.
Constituent state is x,y(position), orientation(psi), velocity(v), cte=cross track error(deviation from waypoints given) and orientation error epsi and are calculated thus.
Only 2 input actuators ( steering angle delta and throttle ) are used. Prev_delta and Prev_a represent the steering angle and accel for the previous set of way points.
Since we are only using acceleration as an actuator which is the 2nd derivative of position vs time, the 3rd order polynomial is sufficient. Also real roads tend to have smooth curves for practical reasons and allow being predicted with a 3rd order polynomial. I imagine if we have a road with unrealistically sharp curves, going to a 4th order might help. Not sure...will try.
  x = x + v * cos(psi) * dt; 
  y = y + v * sin(psi) * dt; 
  psi = psi + v * prev_delta_/Lf * dt; 
  v = v + prev_a_ * dt;
  cte = cte + (v * sin(epsi) * dt);
  epsi = epsi + v * prev_delta_ / Lf * dt;
</pre>
<h2>State and Model</h2>
<ol>
<li> while this is a predominantly kinematic model ( displ = velocity * time, velocity = accel * time ); i.e, we do not account for dynamic forces that result and physical limitations of acceleration/jerk that cars can really handle.
However we do allow for delayed actuation by adding a 100ms latency to achieving the Steer and acceleration affects
</li>     
<li>In main.cpp, the current state is received from simulator ( lines 90 to 97 ).  </li>
<li>It is converted to vehicle co-ordinates with origin at the car.  </li>
<li>Using the waypoints, the desired polynomial fit(3rd order) is calculated.</li>  
<li>Using the coefficients from above polynomial, The Cross Track error and Orientation error is calculated at lines 117, 120.  </li>
<li>The MPC solver is fed the data of waypoints in car co-ordinates and returns the steer and throttle and predicted trajectory in reply.  </li>
<li>These points are plotted ( mpc & next ) to compare the closeness of fit.</li>
<li>The timestep and sampling speed dt and the latency delay and speed all play a part.
  If there is no delay, the car runs very smooth almost at 70 to 100 mph and does not go off the road. But once we introduce the latency delay, its like having a driver whose reflexes are compromised(such as by age or sleepiness of DUI) and you see the car swerving heavly. The logical thing to do is reduce speed. Or do a bit of predictive driving by using the next position instead of the 1st. 
  </li>
  <li>Similarly If the product of N and dt is beyond 1 second, then correspondingly we need to slow down. Essentially how far off track can you tolerate the car going in the 1 second that you may not be able to look? In such case, increasing the N and lowering dt would help but practical computational limits prevent us from choosing a very large N and a very small dt. the </li>
 <li> I tried larger Ns without much benefit and had to keep the N * dt product around 1 second for the 50mph speed. If I increased the speed to 70, with delay of 100ms, it went off track even though without delay it stayed on and ran pretty smooth.
   I played around with these numbers but eventually settled on the ones shown in the video. and simply dropped the speed to 50mph to drive around successfully.
  </li>
</ol>


