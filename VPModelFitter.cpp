// Include files 
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <math.h>
#include <algorithm>
#include <iostream>
// local
#include "VPEmu/VPModelFitter.h"

//-----------------------------------------------------------------------------
// Implementation file for class : VPModelFitter
//
// 2020-02-28 : Pawel Kopciewicz
//-----------------------------------------------------------------------------

using namespace VPEmu;

VPModelFitter::VPModelFitter(int thr_min, int thr_max, int thr_step, int No_of_TP){
  
  //for ( auto & f : CVS_files)
  //  files_location.push_back(f);
  threshold_min = thr_min;
  threshold_max = thr_max;
  threshold_step = thr_step;
  number_of_TP = No_of_TP;
}

std::vector<double> VPModelFitter::Fit(std::vector<double> in_data_X, std::vector<double> in_data_Y, std::string polarity){

N = 15;
number_of_steps = in_data_Y.size();
Polarity = polarity;
data_X = in_data_X;
data_Y = in_data_Y;
int params = 2; //number of free parameters

std::vector<double> vec(N*params), vec2(N*params);
std::vector<double> cost(N), cost2(N); 

//if (Polarity=="negative"){
//  for(int i=0; i<data.size(); i++){
//    data_X.push_back(threshold_min + i * threshold_step);
//  }
//}
//if (Polarity=="positive"){
//  for(int i=data_Y.size(); i>0; i--){
//    data_X.push_back(threshold_max - i * threshold_step);
//  }
//}

//for(int i=0; i<data_Y.size(); i++) std::cout<<data_X[i]<<" "<<data_Y[i]<<std::endl;
for(int i=0; i<N; i++){        //agents initialization
	vec[i]       = 1 + ((double)rand()/RAND_MAX-0.5);
	vec[i + N]   = 1500 + 200*((double)rand()/RAND_MAX-0.5);
//  vec[i + 2*N] = 1 + ((double)rand()/RAND_MAX-0.5);
//  vec[i + 3*N] = 1500+200*((double)rand()/RAND_MAX-0.5);
}

cost = Calculate_cost(vec);

for(int loop=0; loop<80; loop++){
  for(int j=0; j<N; j++){
 	  double e = 2*(double)rand()/RAND_MAX-1;
    int z1 = round((N-1)*(double)rand()/RAND_MAX); 
    int z2 = round((N-1)*(double)rand()/RAND_MAX);
    vec2[j] = vec[j] + e*(vec[z1]-vec[z2]); 
    vec2[j + N] = vec[j + N] + e*(vec[z1 + N]-vec[z2 + N]); 
    //vec2[j + 2*N] = vec[j + 2*N] + e*(vec[z1 + 2*N]-vec[z2 + 2*N]);
    //vec2[j + 3*N] = vec[j + 3*N] + e*(vec[z1 + 3*N]-vec[z2 + 3*N]);  

    vec2[j] = Apply_boundary(0, 10, vec2[j]);
    vec2[j + N] = Apply_boundary(800, 2500, vec2[j + N]);     
    
    //vec2[j + 2*N] = Apply_boundary(0, 10, vec2[j + 2*N]);
    //vec2[j + 3*N] = Apply_boundary(800, 2500, vec2[j + 3*N]);          
  }
  
  cost2 = Calculate_cost(vec2);
  for(int j=0; j<N; j++){
    if( cost2[j] < cost[j] ){
      vec[j] = vec2[j];
      vec[j + N] = vec2[j + N];     
      //std::cout<< vec[j]<<" "<< vec[j + N]<<std::endl;
      //vec[j + 2*N] = vec2[j + 2*N];          
      //vec[j + 3*N] = vec2[j + 3*N];               
      cost[j] = cost2[j];	
    }
    
	}
}

cost2 = Calculate_cost(vec);
std::vector<double>::iterator minimal_cost = std::min_element(cost2.begin(), cost2.end());

int best_solution = std::distance(cost2.begin(), minimal_cost);



std::vector<double> result = { number_of_TP, vec[best_solution], vec[best_solution + N]};
return result;
}

std::vector<double> VPModelFitter::Calculate_cost( std::vector<double> agents){
  double Calculated_cost;
  std::vector<double> cost;

  for(int i=0 ; i<N ; i++){                
    Calculated_cost = 0;
	  for(int k=0 ; k < data_X.size() ; k++){            
  		Calculated_cost += pow(Sigmoid(data_X[k], number_of_TP, agents[i], agents[i+N]) - data_Y[k], 2);
    }
    //std::cout<<i<<" "<<Calculated_cost<<std::endl;
    cost.push_back(Calculated_cost);
  }
  return cost;
}

double VPModelFitter::Apply_boundary(double min, double max, double value){
  double result;
  if (value < min)
    result = min + 0.01*(max - min)*(double)rand()/RAND_MAX;
  else if (value > max)
    result = max - 0.01*(max - min)*(double)rand()/RAND_MAX;
  //std::cout<<min<<" "<<max<<" "<<value<<" "<<result<<std::endl;
  else 
    result = value;
  return result;
}

double VPModelFitter::Sigmoid(double x, double B, double c, double r){
  double result;
  if (Polarity == "positive")
    result =  B / (1 + exp(c*(x-r)));
  if (Polarity == "negative")
    result =  B / (1 + exp(-c*(x-r)));
  return result;
}
