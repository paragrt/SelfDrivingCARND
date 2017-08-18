/*
 * particle_filter.cpp
 *
 *  Created on: Dec 12, 2016
 *      Author: Tiffany Huang
 */

#include <random>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <math.h> 
#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include <iterator>

#include "particle_filter.h"

using namespace std;
int executeCount = 0;
LandmarkObs lm;
void ParticleFilter::init(double x, double y, double theta, double std[]) {
	// TODO: Set the number of particles. Initialize all particles to first position (based on estimates of 
	//   x, y, theta and their uncertainties from GPS) and all weights to 1. 
	// Add random Gaussian noise to each particle.
	// NOTE: Consult particle_filter.h for more information about this method (and others in this file).

    cout << " Entered init...." << endl;
    normal_distribution<double> dist_x(x, std[0]);
    normal_distribution<double> dist_y(y, std[1]);
    normal_distribution<double> dist_psi(theta, std[2]);
   
 
    default_random_engine gen;
    num_particles = 8;
    for (int i = 0; i < num_particles; ++i) {
        Particle p;
        p.weight = 1;
        p.id = i;
        p.x = dist_x(gen);
        p.y = dist_y(gen);
        p.theta = dist_psi(gen);
        particles.push_back(p);
        weights.push_back(1);
    }
    is_initialized = true;    
}

void ParticleFilter::prediction(double delta_t, double std_pos[], double velocity, double yaw_rate) {
	// TODO: Add measurements to each particle and add random Gaussian noise.
	// NOTE: When adding noise you may find std::normal_distribution and std::default_random_engine useful.
	//  http://en.cppreference.com/w/cpp/numeric/random/normal_distribution
	//  http://www.cplusplus.com/reference/random/default_random_engine/

   default_random_engine gen;
    for( int i = 0; i < num_particles; i++)
    {
       Particle &p = particles[i];

       if ( yaw_rate == 0 ) {
         p.x = p.x + velocity*sin(p.theta)*delta_t;
         p.y = p.y + velocity*cos(p.theta)*delta_t;
       } else { //when yaw rate is zero
         p.x = p.x + (velocity/yaw_rate)*(sin(p.theta+yaw_rate*delta_t) - sin(p.theta));
         p.y = p.y + (velocity/yaw_rate)*(cos(p.theta) - cos(p.theta+yaw_rate*delta_t));
       }
       p.theta = p.theta + yaw_rate*delta_t;

       normal_distribution<double> dist_x(p.x, std_pos[0]);
       normal_distribution<double> dist_y(p.y, std_pos[1]);
       normal_distribution<double> dist_psi(p.theta, std_pos[2]);
       p.x = dist_x(gen);
       p.y = dist_y(gen);
       p.theta = dist_psi(gen);
    }
   cout << " LEFT prediction...." << endl;
}

void ParticleFilter::dataAssociation(std::vector<LandmarkObs> predicted, std::vector<LandmarkObs>& observations) {
	// TODO: Find the predicted measurement that is closest to each observed measurement and assign the 
	//   observed measurement to this particular landmark.
	// NOTE: this method will NOT be called by the grading code. But you will probably find it useful to 
	//   implement this method and use it as a helper during the updateWeights phase.
}
double calculateWeight( double sig_x, double sig_y,
                        double x_obs, double y_obs,
                        double mu_x , double mu_y ) {
   double gauss_norm = (1.0/(2.0 * M_PI * sig_x * sig_y));
   double delta_x_sq = (x_obs - mu_x)*(x_obs - mu_x);
   double delta_y_sq = (y_obs - mu_y)*(y_obs - mu_y);
   double exponent = delta_x_sq/(2.0 * sig_x * sig_x ) + delta_y_sq/(2.0 * sig_y * sig_y);
   double r =  gauss_norm / exp(exponent);
   if ( executeCount > 5000 ) {
   cout << "sig_x=" << sig_x << " sig_y=" << sig_y <<  " x_obs=" << x_obs << " y_obs=" << y_obs << " mu_x=" << mu_x << " mu_y=" << mu_y << endl;
   cout << "calcWeight=" << r << endl;
   }
   return r;
}
void ParticleFilter::updateWeights(double sensor_range, double std_landmark[], 
		std::vector<LandmarkObs> observations, Map map_landmarks) {
	// TODO: Update the weights of each particle using a mult-variate Gaussian distribution. You can read
	//   more about this distribution here: https://en.wikipedia.org/wiki/Multivariate_normal_distribution
	// NOTE: The observations are given in the VEHICLE'S coordinate system. Your particles are located
	//   according to the MAP'S coordinate system. You will need to transform between the two systems.
	//   Keep in mind that this transformation requires both rotation AND translation (but no scaling).
	//   The following is a good resource for the theory:
	//   https://www.willamette.edu/~gorr/classes/GeneralGraphics/Transforms/transforms2d.htm
	//   and the following is a good resource for the actual equation to implement (look at equation 
	//   3.33
	//   http://planning.cs.uiuc.edu/node99.html

    //From the lesson, to transform observations from vehicle to map co-ordinates
    //x_map = x_particle + ( cos(theta) * x_observation) + sin(theta) * y_observation
    //y_map = y_particle - ( sin(theta) * x_observation) + cos(theta) * y_observation

    cout << " Entered updateweights...." << executeCount++ << endl;
    double prob_sum = 0.0; //for use in normalization

    if ( executeCount > 5000) cout << "Nbr of particles" << particles.size() << endl; 
    //in the context of each particle
    for (int i = 0; i < particles.size(); i++) {
       Particle &p = particles[i];
       //start clean
       
       //Will contain "tobs" ==> transformed observations ( as in transformed from Vehicle to Map coordinates)

       //convert each observation to map co-ordinates
       for (int j = 0; j < observations.size(); j++) {
         if ( isnan(observations[j].x) || isnan(observations[j].y) 
            || observations[j].x > sensor_range 
            || observations[j].y > sensor_range ) continue; 
         lm.x = p.x + ( cos(p.theta) * observations[j].x) - sin(p.theta) * observations[j].y;
         lm.y = p.y + ( sin(p.theta) * observations[j].x) + cos(p.theta) * observations[j].y;

         //which observation best fits which Landmark as viewed/measured from particle? The one that is closest.

         //for current transformed observation(in context of current particle),
         //find the closest landmark from the landmark's map list
         double closest = std::numeric_limits<double>::max();
         int closestIndex = -1;
         for (int j = 0; j < map_landmarks.landmark_list.size(); j++) {
           double curr_dist = dist(lm.x,lm.y,
                                map_landmarks.landmark_list[j].x_f,
                                map_landmarks.landmark_list[j].y_f); 
           if ( curr_dist < closest ) {
             closest = curr_dist;
             closestIndex = j; //The jth guy is the closest.
           }
         }
         //lm contains the id of the closest landmark by now

         //for each transformed observation, what is the Multivariate Gaussian
         //Probability?
         
         //calculate the Weights of all the associations of this Particle
         double x_tobs = lm.x;
         double y_tobs = lm.y; 
         double mu_x = map_landmarks.landmark_list[closestIndex].x_f;
         double mu_y = map_landmarks.landmark_list[closestIndex].y_f;
         double currentObsWeight = calculateWeight(std_landmark[0], std_landmark[1],x_tobs, y_tobs, mu_x, mu_y);
         //if we get a really low weight and the weight goes to zero it implies
         //that this particle is unlikely to survive resampling
         p.weight = p.weight * currentObsWeight; 
       }
       prob_sum += p.weight;
    }
//NORMALIZING the probabilities
cout << " ====>LEAVING prob_sum = " << prob_sum << endl;
    for (int i = 0; prob_sum > 0 && i < particles.size(); i++) {
       Particle &p = particles[i];
       p.weight = p.weight/prob_sum;
    }
}
//Because I did not really understand Sebastian's resampling algo, here is my
//my own algorithm...take the normalized probabilities and multiple by 1000
//then take each particle and add as many copies of it as dictated by its
//probability....all to a new vector.
//then pick num_particles from this new vector...
//say u have 4 particles...
//probabilities of 1/10, 5/10, 2/10, 2/10
//then multiplying by 1000 gives you the new vector which 
//will have 100 copies of 1st particle approx.
//will have 500 copies of 2nd particle approx.
//will have 200 copies of 3rd particle approx.
//will have 200 copies of 4th particle approx.
//and then, picking 4 random indexes from 0 to 999 will give us a resampled
//vector which slants the result towards 2nd particle and then 3rd and 4th and
//lastly 1st particle
//I think Sebastian's wheel does the same but this is much easier for me to
//visualize

void ParticleFilter::resample() {
   cout << " Entered Resample...." << endl;
   std::vector<Particle> temp_particles;
   for (int i = 0; i < particles.size(); i++) {
      Particle &p = particles[i];
      int count = p.weight * 1000;
      for(int k = 0; k < count; k++) {
         temp_particles.push_back(p);//add count copies of current particle.
      }
   }
   // temp_particles now has as many copies of each particle as dictated by
   // their weights.
   int temp_count = temp_particles.size();
   if ( temp_count == 0 ) {
        cout << " Got zero size temp_particles vector\n";
        return;
   }
   // now sample num_particle times to recreate a resampled particle vector.
   std::vector<Particle> new_particles;
   for(int i = 0; i < num_particles; i++) {
     int v1 = rand() % temp_count;
     Particle p;
     p.id = i;
     p.x = temp_particles[v1].x;
     p.y = temp_particles[v1].y;
     p.theta = temp_particles[v1].theta;
     p.weight = temp_particles[v1].weight;
     new_particles.push_back(p);
   }
   particles.clear();
   temp_particles.clear();
   particles = new_particles;
   cout << " LEFT Resample...." << endl;
}

Particle ParticleFilter::SetAssociations(Particle particle, std::vector<int> associations, std::vector<double> sense_x, std::vector<double> sense_y)
{
	//particle: the particle to assign each listed association, and association's (x,y) world coordinates mapping to
	// associations: The landmark id that goes along with each listed association
	// sense_x: the associations x mapping already converted to world coordinates
	// sense_y: the associations y mapping already converted to world coordinates

	//Clear the previous associations
	particle.associations.clear();
	particle.sense_x.clear();
	particle.sense_y.clear();

	particle.associations= associations;
 	particle.sense_x = sense_x;
 	particle.sense_y = sense_y;

 	return particle;
}

string ParticleFilter::getAssociations(Particle best)
{
	vector<int> v = best.associations;
	stringstream ss;
    copy( v.begin(), v.end(), ostream_iterator<int>(ss, " "));
    string s = ss.str();
    s = s.substr(0, s.length()-1);  // get rid of the trailing space
    return s;
}
string ParticleFilter::getSenseX(Particle best)
{
	vector<double> v = best.sense_x;
	stringstream ss;
    copy( v.begin(), v.end(), ostream_iterator<float>(ss, " "));
    string s = ss.str();
    s = s.substr(0, s.length()-1);  // get rid of the trailing space
    return s;
}
string ParticleFilter::getSenseY(Particle best)
{
	vector<double> v = best.sense_y;
	stringstream ss;
    copy( v.begin(), v.end(), ostream_iterator<float>(ss, " "));
    string s = ss.str();
    s = s.substr(0, s.length()-1);  // get rid of the trailing space
    return s;
}
