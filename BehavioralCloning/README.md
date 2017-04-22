#**Behavioral Cloning** 

##Writeup Template

###I am 5 days overdue becasue of changing jobs and moving house wherein my iMac is packed away.
So please excuse the delay and hopefully this run is acceptable. I think the Current run goes periliously close to the edge ( in fact past the lane markings) but doesnt go "off-roading" so I am hoping this is acceptable.
Best I could do with the time I had

---

**Behavioral Cloning Project**

The goals / steps of this project are the following:
* Use the simulator to collect data of good driving behavior
* Build, a convolution neural network in Keras that predicts steering angles from images
* Train and validate the model with a training and validation set
* Test that the model successfully drives around track one without leaving the road
* Summarize the results with a written report


[//]: # (Image References)

[image1]: ./examples/placeholder.png "Model Visualization"
[image2]: ./examples/placeholder.png "Grayscaling"
[image3]: ./examples/placeholder_small.png "Recovery Image"
[image4]: ./examples/placeholder_small.png "Recovery Image"
[image5]: ./examples/placeholder_small.png "Recovery Image"
[image6]: ./examples/placeholder_small.png "Normal Image"
[image7]: ./examples/placeholder_small.png "Flipped Image"

## Rubric Points
###Here I will consider the [rubric points](https://review.udacity.com/#!/rubrics/432/view) individually and describe how I addressed each point in my implementation.  

---
###Files Submitted & Code Quality

####1. Submission includes all required files and can be used to run the simulator in autonomous mode

My project includes the following files:
* model.py containing the script to create and train the model
* drive.py for driving the car in autonomous mode
* model.h5 containing a trained convolution neural network 
* writeup_report.md or writeup_report.pdf summarizing the results

####2. Submission includes functional code
Using the Udacity provided simulator and my drive.py file, the car can be driven autonomously around the track by executing 
```sh
python drive.py model.h5
```

####3. Submission code is usable and readable

The model.py file contains the code for training and saving the convolution neural network. The file shows the pipeline I used for training and validating the model, and it contains comments to explain how the code works.
Things I tried
1. Added code to flip the image around. That has helped a lot
2. Tried adding code to handle left and right camera and just like instructors run...it got worse...so I put it on the back burner...if needed will resurrect that thread 
3. Used the same data provided and I myself created data2,3,4,5 runs. My lack of video game practice means only data2 was truly an example of good driving. I also recorded data-r ( the recovery run where I stop recording when I go off the trail and restart recording when I correct it) did not actually need to use it. YET.
4. I had a lot of trouble with the unmarked dirt road...as did a lot of folks...I tried to record a drive lap wherein I hugged the other side lane marking....seems to have helped...a bit.
5. Cropping out even a few slices speeded up the training significantly. I read some people talk about it degrading learning...not so sure if thats a good thing because I would not want to drive around looking at the sceenery...prefer to look at the road for clues to stay on the road. More robust I think.
6. Not yet tried running it against 2nd track which I can't manually run at all without falling off. SO I could not get much data from that at all. Not enough video games...maybe when my kid's exams are over he can generate a nice run.

###Model Architecture and Training Strategy

####1. I basically evolved my Arch with the Prof's...eventually ending with the NVIDIA arch.
The only addition I did was to add a bit of drop out to prevent over fitting.
It uses 5 Convs with Strides and then flattens and 
My model consists of a convolution neural network with 3x3 filter sizes and depths between 32 and 128 (model.py lines 18-24) 
The honest truth is I mostly followed the Professors video.
Used the Mean Square Error and Adams Optimizer for learning rate control.

The model includes RELU layers to introduce nonlinearity and the data is normalized using a Keras lambda layer Just like the lessons tell us to. 

####2. Attempts to reduce overfitting in the model

The model contains dropout layers in order to reduce overfitting (model.py lines 21). 

The model was trained and validated on different data sets to ensure that the model was not overfitting (code line 10-16). The model was tested by running it through the simulator and ensuring that the vehicle could stay on the track.

####3. Model parameter tuning

The model used an adam optimizer, so the learning rate was not tuned manually.

####4. Appropriate training data

Refer "Things I tried " above ( points 3 thru 6 ). I shuffled and split data 80/20 for train/validate...again as the lessons suggested

###Model Architecture and Training Strategy

####1. Solution Design Approach
I basically followed each lesson and tried running the model and see how gradually there were improvements as more and more powerful models were used.

At the end of the process, the vehicle is able to drive autonomously around the track without leaving the road.

####2. Final Model Architecture

The final model architecture (model.py lines 67-81) consisted of a convolution neural network with the dropout and fully connected layers


####3. Creation of the Training Set & Training Process

Refer "Things I tried " above ( points 3 thru 6 ). I shuffled and split data 80/20 for train/validate...again as the lessons suggested
