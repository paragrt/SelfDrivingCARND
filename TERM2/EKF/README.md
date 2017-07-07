<h1>Extended Kalman Filter</h1>

1. Had to update CMakelists.txt to use libuv 1.12 but did not use any functions specific to 1.12
link_directories(/usr/local/Cellar/libuv/1.12.0/lib)

2. Initially I was only getting [0.14, 0.6668, 0.6051, 1.6270]
even though I was normalizing atan2

3. The trick was to normalize after the step 
y = z - hOfx_;
y(1) contains the angle and it needed to stay within -pi and +pi
Thats all!
