# SelfDrivingCARND
Udacity Self Driving car nano degree Term 1

<font size="3" color="red">RESUBMIT: IMPROVED lane stability by frame averaging and more lines picked up due to change in thresholds and blur as per 2nd review...Did not add before after pics...tired of uploading everything all over again. This time just the Readme File, PI.ipynb and the 2 videos</font>
<font size="3" color="red">RESUBMIT: IMPROVED alignment on 2nd last image(See before after pic)</font>


#**Finding Lane Lines on the Road** 
---

**Finding Lane Lines on the Road**

1. [Annotated white.mp4 video](/videos/white.mp4) (Use save as to download and then play it)
2. [Annotated yellow.mp4 video](/videos/yellow.mp4) (Use save as to download and then play it)
3. I could not get the challenge.mp4 marked very nicely. Not worth showing.

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
12. For development...I wrote a for loop to go thru each image and display it one by one in line. For documentation, I saved them in test_images/my_{original name}. Uploaded as part of the package to my github
<table>
<tr><td>1st Submission</td><td>2nd Submission</td></tr>
<tr><td><img src="/test_images/my_solidWhiteCurve.jpg" style="width:100px;"/></td><td><img src="/test_images/my2_solidWhiteCurve.jpg" style="width: 200px;"/></td></tr>
<tr><td><img src="/test_images/my_solidWhiteRight.jpg" style="width:100px;"/></td><td><img src="/test_images/my2_solidWhiteRight.jpg" style="width: 200px;"/></td></tr>
<tr><td><img src="/test_images/my_solidYellowCurve.jpg" style="width:100px;"/></td><td><img src="/test_images/my2_solidYellowCurve.jpg" style="width: 200px;"/></td></tr>
<tr><td><img src="/test_images/my_solidYellowCurve2.jpg" style="width:100px;"/></td><td><img src="/test_images/my2_solidYellowCurve2.jpg" style="width: 200px;"/></td></tr>
<tr><td><img src="/test_images/my_solidYellowLeft.jpg" style="width:100px;"/></td><td><img src="/test_images/my2_solidYellowLeft.jpg" style="width: 200px;"/></td></tr>
<tr><td><img src="/test_images/my_whiteCarLaneSwitch.jpg" style="width:100px;"/></td><td><img src="/test_images/my2_whiteCarLaneSwitch.jpg" style="width:100px;"/></td></tr>
</table>

###2. Identify potential shortcomings with your current pipeline
1. I am not happy with the extrapolation. 
2. My lane line (right one) is not very stable and tends to bleed into the road...not sure why. 
3. I am also not happy with the manual fiddling of various parameters. Wish there was a way to automate that and then find a way to call out when we have the real best fit...something similar to learning I guess...But well beyond what I know at this stage.
4. I am new to ComputerVisaion and I may have done things less efficiently. But this is my problem.
###3. Suggest possible improvements to your pipeline
1. I guess some numpy/statistics function might help with better extrapolation?
2. In some cases like the challenge.mp4, the curve is sharp and I think its unreasonable to assume linear regression straight line fit will work so perhaps a polyfit which follows the curving lane would make sense? Just guessing.
