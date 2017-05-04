## Writeup Template

### Using the Template file.

---

**Advanced Lane Finding Project**

The goals / steps of this project are the following:

* Compute the camera calibration matrix and distortion coefficients given a set of chessboard images.
* Apply a distortion correction to raw images.
* Use color transforms, sobel gradients to create a thresholded binary image.
* Apply a perspective transform to rectify binary image ("birds-eye view").
* Detect lane pixels and fit to find the lane boundary.
* Determine the curvature of the lane and vehicle position with respect to center.
* Warp the detected lane boundaries back onto the original image.
* Output visual display of the lane boundaries and numerical estimation of lane curvature and vehicle position.

[//]: # (Image References)

[image1]: ./examples/undistort_output.png "Undistorted"
[image2]: ./test_images/test1.jpg "Road Transformed"
[image3]: ./examples/binary_combo_example.jpg "Binary Example"
[image4]: ./examples/warped_straight_lines.jpg "Warp Example"
[image5]: ./examples/color_fit_lines.jpg "Fit Visual"
[image6]: ./examples/example_output.jpg "Output"
[video1]: ./project_video.mp4 "Video"

## [Rubric](https://review.udacity.com/#!/rubrics/571/view) Points

### Here I will consider the rubric points individually and describe how I addressed each point in my implementation.  

---

### Writeup / README

#### 1. Provide a Writeup. This is it.

You're reading it!

### Camera Calibration

#### 1. Briefly state how you computed the camera matrix and distortion coefficients. Provide an example of a distortion corrected calibration image.

### A. CheesBoard images
1. I used the provided chessboard with 9x6 to calibrate the camera by finding the corners(findChessBoardCorners@cell:3,line23), marking them and undistorting them.
2. Of the 20, a few did not work(unable to find cheesboard corners...I guess thats the real world!)...handled it with if condition(cell:3, line:25)
3. The rest of chessboard images(17 of them) gave me a good set of camera calibration matrix and distortion coefficients (cell:3, lines:35). The corner_unwrap (cell:3 line 44) is more for practice.
4. I averaged over the <b>cam matrix and distortion coefficients</b> to get a good approximartion of the properties of camera
5. I am going to next use it in the test_images of the car video itself.
6. Getting the perspective back is easy here because we know what a chessboard square looks like...but this is not likely to be the exact same perspective that the car camera will have...this was more for practice...more on that next

### B. test_images directory images
1. Once I got the camera properties, ( camera callibration matrix and distortion coefficients ) There are a number images in the test_images folder. Off these, the 2 named straight_lines I used to get a good set of <b>source and destination points</b> such that when I applied the perspective transform to a Birds Eye View, the lanes look parallel(which is a physical reality!Fortunately!!). When I used the same set of source dest points, the curved parallel lines also looked good parallel or as mathemeticians call them...pair of offset curves
2. Using this source and destination points, I applied the transform to rest of the images and also to the rest of the video frames. BIG Assumption is that the camera postion(perspective) doesnt change relative to ground+car 
3. Cell 4 contains some Project 1 helpers which I used( grayscale and region of interest)...others I could have deleted.
4. Cell 5 contains the various sobel(x,y,magnitude and direction) gradients function which I thought I might need..but in the end the color & x gradient sufficied cell:5,line:60 { combinedSobel(img, s_thresh=(170, 200), sx_thresh=(20, 100)) ) }

5. Cell 6 contains the sliding window lane finder almost as-is from the lesson, except for this important insight. Since Lanes are parallel, I basically looked at each of the 2 histogram peaks and saw which one was higher and anchored the lane to that side. So I did use the polyfit regression on the "SURE" side (left or right, whichever had a higer histogram peak) and then used the assumption that lanes are 525 pixels wide(heuristic) to plot the other side. Now, if we want to get more cute, for finding the "other side", we could do a weighted average between confidence levels on the other side and adjust the lane positions between calculated using regression and calculated using the above assumption. I kept it simple though. <b>In the physical world, this is akin to hugging the lane you can see best because the other side is either covered in snow or onc oming headlights make it hard to see...</b>
6. I did add a little bit of "memory" to remember the previous frames peak and confidence levels just in case you come to a frame where histogram peaks are not prominent...i.e. neither side is really a standout lane marking...this is akin to keeping the steering unchanged when you are dumped on by a shower of water or snow from a passing car...completely blocking your view for a few frames(hopefully not too much more!). In this case I just take the average of previous and current and stay with it. 
7. After this the lanes are marked in the Birds Eye View perspective. And they MUST be parallel.
8. Now its a matter of using the reverse camera projection to go back to the original perspective from within the car and overlaying the original frame with thsi marked lane. I am trying to find a way to make it(lane marking) "transparent"....will do later
### Pipeline (single images)

#### 1. Provide an example of a distortion-corrected image.

I have provided the 6 phases of the transformations for each of the test images...as much for explanation as for debugging.
Hopefully the titles help tell the same story as above.

#### 2. Describe how (and identify where in your code) you used color transforms, gradients or other methods to create a thresholded binary image.  Provide an example of a binary image result.
cell:5,line:60 { combinedSobel(img, s_thresh=(170, 200), sx_thresh=(20, 100)) ) }...description above

#### 3. Describe how (and identify where in your code) you performed a perspective transform and provide an example of a transformed image.
Original to Birds EyeView --> Cell:7, Line 50 : function corners_warp2

#### 4. Describe how (and identify where in your code) you identified lane-line pixels and fit their positions with a polynomial?
Used the histogram to decide which side of the lane I knew best...Used the sliding window function from the lessons to pick out both sides and calculate the polynomial fit. Picked the side which I know more definitively and then relied on the lane width to find the "parallel or Offset Curve points"

#### 5. Describe how (and identify where in your code) you calculated the radius of curvature of the lane and the position of the vehicle with respect to center.
Used the calculations in the lesson to find the radius. cell:8 function writeCurvatureNOffCenter(image,leftx,rightx)


#### 6. Provide an example image of your result plotted back down onto the road such that the lane area is identified clearly.
Birds Eye View back to Original--> Cell:7, Line 83 : markedOrigPersp = cv2.warpPerspective(outimg, inversePerspMatrix, img_size)
I struggled with the aplha channel addition to make the lane marking transparent...cell:7 lines 95 to 97

---

### Pipeline (video)

#### 1. Provide a link to your final video output.  Your pipeline should perform reasonably well on the entire project video (wobbly lines are ok but no catastrophic failures that would cause the car to drive off the road!).
When it got to the actual video, simply copied lesson 1 skeleton to drive the find lanes and mostly copied the pipeline I used for test_images into the findLanes function
Here's a [link to my video result](https://github.com/paragrt/SelfDrivingCARND/tree/master/CarND-Advanced-Lane-Lines/videos/pv1.mp4)

---

### Discussion

#### 1. Briefly discuss any problems / issues you faced in your implementation of this project.  Where will your pipeline likely fail?  What could you do to make it more robust?

I think the salient features of my solution is that while I used the polyfit regression to find the laneside which I am "SURE"(based on higher histogram peak) about I guessed at the location of the other edge. I could soften this guess if the "SUREity" of the 1st lane side is suspect....I.e Average it out between calculation and prediction based on known lane width. 
I could play around to improve poor light detection a bit and more averaging/memory from previous frames to smooth out the wobbles. On more curved roads, I could shorten my lane prediction...coz in real life when you have more sudden changes thats what u do...you slow down and you only predict shorter distances of how the road might pan out.
All my video and images are in the Python notebook
Thanks!!
