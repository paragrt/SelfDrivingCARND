
I wrote the main.cpp so that I passed in parameters Kp, Ki, Kd as command line parameters and manually tuned the params such that the CrossTrackError remained below 1.0 ( hueristically determined)  
If error exceeded 1.0 substantially, I would abandon that set of params and try another.

Choosing the PID coefficients: ( Kp=75, Ki=150, Kd=0.00003 )

#Choosing Kp
This is the most intuitive. With the other 2 set to zero, just picked a KP starting with small values to large such that it roughly 
follows the desired path  

What it could not handle:
When we came upon a sharp bend, P alone could not correct quickly. 
Neither could it correct overall drift away from the road. 
So with P alone, the car would often run along side (parallel to) the road but not on it.

#Choosing Ki
To bring it back on the road, adding the Ki term, (which I had read can be roughly 2 times Kp as a starting point) helped it get back on the road.
However, it still would veer off the road as we approached sharp turns.

#Choosing Kd
For handling sharp and quick turns, this was needed but its value is very small. And small changes in it caused substantial fluctuations
The combination of Kp and Kd alone was very jittery. If u sat in such a car it would make you seasick :)
To smooth out the changes and generally drift towards the centre, Ki was added.

Also noticed that higher values of Kp tended to make the steering oscillate and thereby lower the speed. While running at slower speed seems to be
easier to handle, I ran everything with one step below fastest speed and tuned with that.
Here is a short video.
<video width="320" height="240" controls>
  <source src="https://youtu.be/nIfawpB4X50" type="video/mp4"/>
</video>

#Note
BTW, Capturing the Video by screen capture seemed to interfere with Unity's simulator execution so I just got a video of my computer using my phone.
After a point I started reporting the CTE (if over 1.0) to std error and collecting the CTE in out.txt  
The last one is amongst the best runs and the CTE did go marginally(1.5) over 1.0 but stayed low overall.

#Improvements:
I think slowing down the throttle in inverse proportion to the change in the wheel angle may help because in real life, thats what we do...slow down at a curve  
  <pre>
  history | grep pid
  525  ./pid 100 .000035 20
  526  ./pid 100 .000015 20
  527  ./pid 100 .000015 50
  528  ./pid 50 .000015 50
  529  ./pid 50 .000020 50
  530  ./pid 50 .000020 100
  531  ./pid 60 .000025 150
  532  ./pid 70 .000020 200
  533  ./pid 900 .000020 200
  534  ./pid 90 .000020 200
  535  ./pid 100 .000020 100
  536  ./pid 50 .000030 50
  537  ./pid 30 .000030 30
  538  ./pid 30 .000030 60
  539  ./pid 30 .000025 100
  540  ./pid 40 .000025 80
  541  ./pid 60 .000025 80
  542  ./pid 80 .000025 160
  543  ./pid 10 .000025 20
  544  ./pid 10 .000025 50
  545  ./pid 10 .00003 50
  546  ./pid 20 .00005 50
  547  ./pid 30 .00004 60
  548  ./pid 35 .000035 70
  549  ./pid 30 .000035 70
  550  ./pid 35 .000035 90
  551  ./pid 50 .00004 100
  552  ./pid 50 .00004 100
  553  ./pid 50 .00004 100
  554  ./pid 50 .000025 100
  555  ./pid 20 .00005 100
  556  ./pid 20 .00005 200
  557  ./pid 20 .00005 200
  558  ./pid 50 .00005 200
  559  ./pid 50 .00005 100
  564  ./pid 50 .00005 100 2>out.txt
  565  ./pid 50 .00005 100 
  568  ./pid 50 .00005 100 2>out.txt
  569  ./pid 100 .00005 100 2>out.txt
  570  ./pid 10 .00007 100 2>out.txt
  571  ./pid 10 .00009 200 2>out.txt
  572  ./pid 30 .00009 200 2>out.txt
  573  ./pid 50 .00009 200 2>out.txt
  574  ./pid 50 .00005 100 2>out.txt
  575  ./pid 60 .00006 120 2>out.txt
  577  ./pid 60 .00006 120 2>out.txt
  580  ./pid 60 .00006 120 2>out.txt
  582  ./pid 60 .00006 120 2>out.txt
  583  ./pid 40 .00005 120 2>out.txt
  588  ./pid 40 .00005 120 2>out.txt
  589  ./pid 50 .00005 100 2>out.txt
  591  ./pid 50 .00005 100 2>out.txt
  592  ./pid 70 .00003 140 2>out.txt
  593  ./pid 10 .00003 100 2>out.txt
  594  ./pid 20 .00005 100 2>out.txt
  595  ./pid 40 .00005 100 2>out.txt
  614  ./pid 100 .00005 200 2>out.txt
  615  ./pid 100 .00005 100 2>out.txt
  616  ./pid 100 .00003 100 2>out.txt
  617  ./pid 75 .00003 150 2>out.txt
</pre>
