
I wrote the main.cpp so as to enable me to pass in parameters Kp, Ki, Kd as command line parameters and manually tuned the params such that the CrossTrackError (CTE)  remained below/close to 1.0 ( heuristically determined)  
If CTE exceeded 1.0 substantially, I would abandon that set of params and try another.
<h3>UPDATE</h3>

I had a bug in the parameters being passed. Corrected that and redid the twiddle algorithm with new Kp, Ki, Kd
Used this from a stackoverflow recommendation
To tune a PID use the following steps:
<ol>
  <li>Set all gains to zero.</li>
<li>Increase the P gain until the response to a disturbance is steady oscillation.</li>
<li>Increase the D gain until the the oscillations go away (i.e. it's critically damped).</li>
<li>Repeat steps 2 and 3 until increasing the D gain does not stop the oscillations.</li>
<li>Set P and D to the last stable values.</li>
<li>Increase the I gain until it brings you to the setpoint with the number of oscillations desired (normally zero but a quicker response can be had if you don't mind a couple oscillations of overshoot)</li>
</ol>
<h2>
Choosing the PID coefficients: ./pid 0.07 0.0001 0.9
USING Kp=0.070000, Ki=0.000100, Kd=0.900000 )
</h2>

## Choosing Kp
This is the most intuitive. With the other 2 set to zero, just picked a KP starting with small values to large such that it roughly 
follows the desired path  

What it could not handle:
When we came upon a sharp bend, P alone could not correct quickly. 
Neither could it correct overall drift away from the road. 
So with P alone, the car would often run along side (parallel to) the road but not on it.

## Choosing Ki
To bring it back on the road, adding the Ki term, (which I had read can be roughly 2 times Kp as a starting point) helped it get back on the road.
However, it still would veer off the road as we approached sharp turns.

## Choosing Kd
For handling sharp and quick turns, this was needed but its value is very small. And small changes in it caused substantial fluctuations
The combination of Kp and Kd alone was very jittery. If u sat in such a car it would make you seasick :)
To smooth out the changes and generally drift towards the centre, Ki was added.

Also noticed that higher values of Kp tended to make the steering oscillate and thereby lower the speed. While running at slower speed seems to be
easier to handle, I ran everything with one step below fastest speed and tuned with that.
<a target="_blank" href="https://youtu.be/nIfawpB4X50">Here is a short video.</a>
<video width="320" height="240" controls>
  <source src="https://youtu.be/nIfawpB4X50" type="video/mp4"/>
</video>

## Note
BTW, Capturing the Video by screen capture seemed to interfere with Unity's simulator execution so I just got a video of my computer using my phone.
After a point I started reporting the CTE (if over 1.0) to std error and collecting the CTE in out.txt  
The last one is amongst the best runs and the CTE did go marginally(1.5) over 1.0 but stayed low overall.

## Improvements:
I think slowing down the throttle in inverse proportion to the change in the steer angle may help because in real life, thats what we do...slow down when we change direction

<p/>
Below is a sample of my runs and my attempts at twiddling the params
  <pre>
  history | grep pid
  660  ./pid 1 0.00003 2
  661  ./pid 1 2 0.00003
  662  ./pid 1 0 0
  663  ./pid 0.1 0 0
  664  ./pid 0.1 1 0
  665  ./pid 0.1 0.2 0
  666  ./pid 0.1 0.2 0
  667  ./pid 0.1 0.001 0
  668  ./pid 0.1 0 0.001
  669  ./pid 0.01 0 0.0001
  670  ./pid 0.03 0 0.0001
  671  ./pid 0.03 0.0001 0.0001
  672  ./pid 0.03 0.00001 0.00001
  674  ./pid 0.03 0.0 0.0
  675  ./pid 0.03 0.001 0.0
  676  ./pid 0.03 0.00001 0.0
  677  ./pid 0.03 0.00001 0.00001
  679  ./pid 0.01 0.00001 0.00001
  680  ./pid 0.02 0.00001 0.00001
  682  ./pid 0.02 0 0
  683  ./pid 0.025 0 0
  684  ./pid 0.05 0 0
  685  ./pid 0.07 0 0
  686  ./pid 0.055 0 0
  688  ./pid 0.055 0 0.01
  689  ./pid 0.055 0 0.1
  690  ./pid 0.055 0 0.2
  691  ./pid 0.06 0 0.3
  692  ./pid 0.06 0 0.6
  693  ./pid 0.06 0.0001 0.6
  694  ./pid 0.07 0.0001 0.9
  695  ./pid 0.07 0.0001 0.9
</pre>
