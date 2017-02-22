# SelfDrivingCARND
Udacity Self Driving car nano degree Term 1
#**Finding Lane Lines on the Road** 
---

**Finding Lane Lines on the Road**

The goals / steps of this project are the following:
* Make a pipeline that finds lane lines on the road
* Reflect on your work in a written report
---
### Reflection

###1. Describe your pipeline. As part of the description, explain how you modified various functions
1. Convert the picture to grayscale using the supplied helper function as is
2. Apply Gaussian Blur to smooth out noise. I experimented with 3,5,7 and settled on 5
3. To detect edges, used the provided helper function 'canny'. Tried range of values from lowthreshold=40 to 60 and highthreshold=120 to 180...Settled on low=55 & high=170. If I lowered the high threshold I was getting noise on left side of picture. If I raised low threshold I was missing the dotted lane lines
4. For region of interest, On the vertices...I took the **trapezoid viewport** bottom left corner, (380,340) and (580,340) and then bottom right corner. Trick seems to be to not look too far ahead else the road curves enough (and is blurry enough) to throw you off.
5. Next applied the hough lines transform. For hough lines rho of 2 and theta of 1...Did not try changing these too much. Settled on nmbr of votes=25...too high and I miss the fainter dashed lane lines...too low and too much noise comes in. Some of the frames where the dashed lane lines are close to camera required me to keep allowed gap to almost 100. min length I found 35 to work
6. In the hough lines, there is a call to draw lines wherein separated out the lines into 2 separate groups
7. I arbitrarily decided that if the slope difference is more than 0.2, then they must be different lines. Its a heuristic....seems to faithfully group separately...much like the finding a cut-set problem
8. Once I got 2 groups of lines ...I applied linear regression ( np.linalg.lstsq ) to find the best fitting line for each group. This gave me a slope and intercept of best fitting line
9. This line is what I am drawing for lanes with thickness of almost 10.
10. How did I extrapolate top and bottom points to draw the 2 lines ? Assuming that my top y coordinate is just past the top of my **trapezoid viewport** around y1=340 and my bottom y is the bottom of the image y2=540 I calculated x1 and x2 using the lineregres returned slope and intercept.
11. Thats it...used the weighted_img to blend my annotations on the actual image.
12. For development...I wrote a for loop to go thru each image and display it one by one in line. For documentation, I saved them in test_images/my_{original name}. Will be Uploaded as part of the package to my github

###2. Identify potential shortcomings with your current pipeline
1. I am not happy with the extrapolation. 
2. My lane line (right one) is not very stable and tends to bleed into the road...not sure why. 
3. I am also not happy with the manual fiddling of various parameters. Wish there was a way to automate that and then find a way to call out when we have the real best fit...something similar to learning I guess...But well beyond what I know at this stage.
4. I am new to python and I may have done things less efficiently. But this is my problem.
###3. Suggest possible improvements to your pipeline
1. I guess some numpy/statistics function might help with better extrapolation?
2. In some cases like the challenge.mp4, the curve is sharp and I think its unreasonable to assume linear regression straight line fit will work so perhaps a polyfit which follows the curving lane would make sense? Just guessing.
