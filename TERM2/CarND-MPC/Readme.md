<h1>Using model Predictive Controller to drive the car around the track.</h1>

<h2>State and Model</h2>
<ol>
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


