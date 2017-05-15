

<h1>Vehicle Detection Project</h1>

The goals / steps of this project are the following:

* Perform a Histogram of Oriented Gradients (HOG) feature extraction on a labeled training set of images and train a classifier Linear SVM classifier
* Optionally, you can also apply a color transform and append binned color features, as well as histograms of color, to your HOG feature vector. 
* Note: for those first two steps don't forget to normalize your features and randomize a selection for training and testing.
* Implement a sliding-window technique and use your trained classifier to search for vehicles in images.
* Run your pipeline on a video stream (start with the test_video.mp4 and later implement on full project_video.mp4) and create a heat map of recurring detections frame by frame to reject outliers and follow detected vehicles.
* Estimate a bounding box for vehicles detected.

<a href="./P_Vehicle-Detection.ipynb">P_Vehicle-Detection.ipynb</a><br/>
<a href="./pv3_test_video.mp4">TEST Video</a><br/>
<a href="./pv3_project_video.mp4">Project Video</a><br/>

## [Rubric](https://review.udacity.com/#!/rubrics/513/view) Points
###Here I will consider the rubric points individually and describe how I addressed each point in my implementation.  

---
###Writeup / README

####1. Provide a Writeup / README that includes all the rubric points and how you addressed each one.  
You're reading it!

###Histogram of Oriented Gradients (HOG)

####1. Explain how (and identify where in your code) you extracted HOG features from the training images.

Cell1, Line 60 is the extract features which is from the lessons. Its focus is on HOG, SPatial Bins and Color.
Cell3, line 3 to 6, I globbed and shuffled the 'vehicle' and 'non-vehicle' images because I created some copies of (vehicles)white car at the required perspective and also of (non-vehicles)left railing and shadows.   
I extracted the features at line 27 cell 3. and then went on to training linear SVM

I then explored different color spaces and different `skimage.hog()` parameters 
and settled on 
<pre>
color_space = 'RGB' # Can be RGB, HSV, LUV, HLS, YUV, YCrCb
orient = 9  # HOG orientations
pix_per_cell = 8 # HOG pixels per cell
cell_per_block = 2 # HOG cells per block
hog_channel = 'ALL' # Can be 0, 1, 2, or "ALL"
spatial_size = (32, 32) # Spatial binning dimensions
hist_bins = 32    # Number of histogram bins
</pre>

####2. Explain how you settled on your final choice of HOG parameters.
I tried HOG channel of 0 and ALL and ALL worked better. RGB worked better. The rest of the defaults were acceptable. I tried Hist and spatial bins of 16 but 32 is a nice middle ground between speed and effectiveness.

####3. Describe how (and identify where in your code) you trained a classifier using your selected HOG features (and color features if you used them).

Cell 3, line 27/33 after extracting features HOG, color and spatial bin features, I trained a linear SVM using(59 and 62)
Used the train/test/split of 80/20
I shuffled the training set filenames before extraction so that files(esp of the white car of which I made copies) will be spread out thru the train/test.

###Sliding Window Search

####1. Describe how (and identify where in your code) you implemented a sliding window search.  How did you decide what scales to search and how much to overlap windows?

The window size less than x=128 was missing the white car as it enters the frame but a y of greater than 110 was mixing up the black casr and shadows. SO 128,110 with more overlap in the wider (horizontal) direction is what I settled after much tweaking

I focused on catching all the cars...even at the cost of a few extra false positives...which we eliminate by heat map thresholding across multiple frames.

I setup a 10 frame memory and as long as the box shows up in each frame...it was considered a "hot enough" box to be kept. I tried hard to eliminate some hot windows on the left...thinking they were false positives...but later realized that we were picking up some oncoming traffic cars...so its not all false positives...many are valid boxes....admitedly some false positives remain.

####2. Show some examples of test images to demonstrate how your pipeline is working.  What did you do to optimize the performance of your classifier?
I tried using smaller windows but it started taking too long and not much improvement in quality...so gave up on that angle.

To compare if my classifier was as good as the one that the lessons were unpickling, I copied over test_image7 from Search and Classify lesson. I suspect that the teschers classifier is better trained over a larger (more varied) data set such as the 1GB and 3GB additional data. However, the incremental improvement was not enough to warrant taking up that much resources(space/time) for this project. <b>I am sure if I had to sit in that car however, I would do the extra bit:) coz its a once and done thing....so well worth it in a production scenario.</b>

The detected window markings(left column) and then the heat-map resultant windows to keep (right column) are shown inline in the notebook
I made sure I did a decent job on that and the test video before moving to the pipeline.

I had to make sure that when I am doing static images, I cant really leverage my 10 frame averaging...so there is a biot of conditional logic if running a stream of frames from video or if running static disjointed images.

### Video Implementation

####1. Provide a link to your final video output.  Your pipeline should perform reasonably well on the entire project video (somewhat wobbly or unstable bounding boxes are ok as long as you are identifying the vehicles most of the time with minimal false positives.)
Here's a [link to my video result](./pv3_project_video.mp4)


####2. Describe how (and identify where in your code) you implemented some kind of filter for false positives and some method for combining overlapping bounding boxes.

 I used the lessons heatmap/labels and tweaked the thresholding as described above.( cell 4 line 47,50,53 )

Here's an example result showing the heatmap from a series of frames of video, the result of `scipy.ndimage.measurements.label()` and the bounding boxes then overlaid on the last frame of video:

The bounding boxes after heatmap application are on the right.


---

###Discussion

####1. Briefly discuss any problems / issues you faced in your implementation of this project.  Where will your pipeline likely fail?  What could you do to make it more robust?
I am sure that it will have some issues when it turns left. And when we encountrer cars which do not contrast well against the asphalt(silver color) or on sharp curves where the car presents a perspective which we have not trained for,

More examples of different angles and maybe missing guardrails...will definitely improve the chances of mitigating this...so its a scalable(learning) behavior. This is what led me to create some examples.
Another idea might be to use perspective transforms on existing images to see how cars might look from say 10 to 20 degrees rotated from either side. More examples of "occluded" partial cars will also help...Right now I miss out on catching partial cars coz its probably not in the training set
