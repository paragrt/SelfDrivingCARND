#**Traffic Sign Recognition** 

##Writeup


---

**Build a Traffic Sign Recognition Project**

The goals / steps of this project are the following:
* Load the data set (see below for links to the project data set)
* Explore, summarize and visualize the data set
* Design, train and test a model architecture
* Use the model to make predictions on new images
* Analyze the softmax probabilities of the new images
* Summarize the results with a written report

[image14]: ./class_14.png " Stop Sign"
[image18]: ./class_18.png
[image19]: ./lass_19.png
[image20]: ./class_20.png
[image22]: ./class_22.png
[image23]: ./class_23.png
[image27]: ./class_27.png
[image29]: ./class_29.png
[image3]: ./class_3.png
[image31]: ./class_31.png
[image36]: ./class_36.png
[image37]: ./class_37.png
[image38]: ./class_38.png
[image39]: ./class_39.png

## Rubric Points
**Submission Files include**
1) Notebook file
2) HTML File
3) 8 images I downloaded from the website to test real world data

###DataSet Summary & Exploratory Visualization
1) There are 43 classes ( different signs )
2) The training set is 34799 images each of 32x32x3 . Validation is 4410 and Test is 12630
3) I explored using grayscaling+normalize vs 3 color and it looks like model learns better with color.
4) The picture below on the left, shows how some of the classes have very few examples and some have a lot more.
On the right, after Augumentation, it shows how I have tried to balance out the examples by cloning  existing examples of respective classes.
[ImageBeforeAfterAugumentation]:./BeforeAndAfter.png "Before and After Augumentation to create Balance"
5) I tried adding some noise ( rotation or just changing pixel values Plus or Minus 10 at random to about 10% of the pixels ) only to the images I was adding.
While this may result in more robust learning....due to my modest python skills, the process was too slow. So I abandoned that (for now) and just used np.tile which is fast

###Design and Test

**Preprocessing:**

1) Tried 3 channel vs grayscale and found 3 channel works best. Also tried normalization/equalHist but neither helped me...or I was not paitent enough. I did show some example pictures to ensure that there are various examples of same class.
2) I did Augument the Test set by adding clones of classes with less exmaples and my well balanced Train set was New total =  81417 of 32x32x3 images. I made sure that most classes had a minim of 1500 examples

**Model**

3) Copied the Lenet Code from previous. Did not reinvent the wheel here. Just changed it to handle 3 channels and 43 classes instead of 1 channel and 10 classes
4) I did not initially have Dropout Regularization and I kep reaching but not staying above the 0.93 threshold. Adding DropOut to last Fully connected layer with a Keep Probability of 0.5 did the trick. While a paper I read claims that doing the same to other layer helps as well, I did not see much improvement by adding it to the 1st Conv Layer ( this paper https://arxiv.org/ftp/arxiv/papers/1512/1512.00242.pdf ). used the Adam Optimizer which adjust learnrates as well....again copied from example....

**Model Training**

5) Tried larger Batches, larger number of Epochs and lower training rates. On reviewing the videos I heard that stoping training when your validation accuracy starts to drops so I added a little code that if I have stayed over 0.935 ( desired minimum threshold ) then quit
6) Once I added Dropout, I was able to use a better training rate and My model has 0.947 accuracy.
7) My test Accuracy is 0.932 as well. So I was happy with the solution.

**Test a Model on New Images**

8) Downloaded 13 images from https://en.wikipedia.org/wiki/Road_signs_in_Germany and resized them to 32x32. Some of the triangular ones had a slightly warped aspect ratio but surprisingly the model did not have problems identifying them.
9) Showed each image
10) Ran prediction for each and found that I successfully identify 11/13 i.e. 84.6% Accuracy. Good not great. I cannot explain what is unique about the 2 I missed that my model cannot classify...but hoping the next part will explain. Output Top 5 Softmax Probabilities For Each Image Found on the Web has how my model classified the top5 for each of the 13 images I tested with
11) When I printed softmax using tf.nn.top_k( where k=5) I can see that I narrowly miss categorized class 36 ( go straight or right got 9.9% ) vs class 25 ( RoadWork got 10.5 ) and class 26 ( Traffic signals got 7.5 ) so it is pretty close. I can see some structural similarities between the 3 
12) Class 37 ( Go straight or Left ) I missed it badly...its not even in the top 5 and I seem to be pretty sure it is Class 40 ( Round about mandatory got 46%). Again maybe some structural similarity with left curving arrows but can't explain why I missed it so badly.

**Suggestions to Make Your Project Stand Out!**

13) As I mentioned, I will figure out a way to speed up the noise addition to my fake examples once I am better at python
14) I think in this domain, an important feature we need to add is how to give more weightage to "more important signs". For example...if I mis-classify the wild animal sign as a Roadwork...not so bad...but if I misclassify a stop sign as a 100kph sign...thats bad. We need to build in some intelligence wherein even if a prediction is slightly lower propbablity ( like my bullet #11 ), maybe picking the 2nd is allowed...because that is a critical sign.

**Since the Instructor mentioned some of the aspects of Neural networks (such as Drop Out) feel like black magic, I have tried to create a table of intuitive understanding of each concept.**

|Neural Networking Terms|	Intution/Real Life examples|
|:---------------------:|:---------------------------:|
|Neural Network|	The Brain|
|Neuron	|Computational Construct that Simulates the firing (or not firing) of a neuron based on stimuli|
|Learning |	Doing your homework and checking your answers|
|Learning rate |	While Doing your HomeWork, Did you jump from one concept to another suddenly or did you gradually evolve from one to another. Perhaps explains why smaller learning rates are better/deeper and more stable at Training the Neural Network|
|Balancing the Number of Input Examples of each kind|	What happens if you study 100s of examples on addition and division but only a couple of examples on Subtraction and Multiplication? Or In an exam, when you are faced with a question on a topic which you had decided to "skip it"? Usually do better on stuff you have practised more|
|Augumenting Examples of the Infrequent kind by taking existing examples of the infrequent kind and Balancing out the Number of Examples of each kind|	This is like repeating the same HomeWork problem more times because there are not as many homework problems of that particular style…albeit with slightly different inputs|
|Validation of Neural Network|	Taking an Exam after having doing the HomeWork ( Training the Neural Net )|
|Using it in the Real world	|Using what you studied(the model/understanding) at a Job|
|Overfitting a Network|	Remember the guy who spent a lot of time studying and did well in Tests but somehow when it came to appying things in the real world….unless it was something exactly like the Test or Homework he had done before….he could not come up with the right answer? Someone so buried in the minutiae that he is unable to see the forest for the trees.|
|UnderFitting a Network|	Just did not train hard enough|
|Regularization	|To prevent overfitting…or to put it another way, learn to generalize and spotting the forest for the trees. |
|L2 regularization|Penalize bold heavy weights ( i.e. landmark features which are dead-giveaways for the answer) and make the network work harder to figure out the answer…. Like taking away "Hints"|
|DropOut regularization	| How doctors treat lazy eye by covering up the Good eye and forcing the "lazy" eye to start functioning…or how one learns to do house chores when Mom is away :)|
