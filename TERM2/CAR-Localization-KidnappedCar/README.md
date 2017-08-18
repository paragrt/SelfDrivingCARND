A kidnapped car needs to figure out where it is!! A localization problem.


1. In the Init step, create 8 to 20 particles....whatever works for you.
2. In the predict step, I found an issue in my code when yaw_rate went to zero at time_step 905  
Had to handle it by this if statement.
<pre>
      if ( yaw_rate == 0 ) {
         p.x = p.x + velocity*sin(p.theta)*delta_t;
         p.y = p.y + velocity*cos(p.theta)*delta_t;
       } else { //when yaw rate is zero
         p.x = p.x + (velocity/yaw_rate)*(sin(p.theta+yaw_rate*delta_t) - sin(p.theta));
         p.y = p.y + (velocity/yaw_rate)*(cos(p.theta) - cos(p.theta+yaw_rate*delta_t));
       }
       p.theta = p.theta + yaw_rate*delta_t;
</pre>

3. I skipped dataAssociation so as to avoid vector/object create and tear down and just looped over the logic in the updateWeights  
I checked for handling only within range 
And mostly just followed the lessons and instructions to calculate the weights and the normalizartion of probablities.  

4. The resampling explanation Sebastian gave was a bit confusing, but during his lesson, in the 1st challenge, I had come up with this simple algorithm for resampling.  

Resampling algo:
Imagine you have 4 particles with following probabilities:
p1 = 0.1, p2=0.3, p3=0.2, p4=0.5
To ensure that they are sampled in proportion to their probablities, we can simple create a new vector wherein there are  
5 copies of P4  
2 copies of P3  
3 copies of p2  
1 copy of   p1

And then sample it 4 times with replacement ensuring that the chances of P4 getting picked is 5 times that of P1 and so on
