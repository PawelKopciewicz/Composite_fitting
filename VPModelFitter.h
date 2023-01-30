#pragma once
#include <vector>
#include <string>
/** @class ModelFitter VPModelFitter.h VPEmu/VPModelFitter.h
 *  
 *  @author Pawel Kopciewicz
 *  @date   2020-02-28
 */

namespace VPEmu {

  class VPModelFitter {
  public: 
  
    VPModelFitter(int thr_min, int thr_max, int thr_step, int number_of_TP);    
    std::vector<double> Fit(std::vector<double> in_data_X, std::vector<double> in_data_Y, std::string polarity);
    std::vector<double> Calculate_cost( std::vector<double> agents);
    double Apply_boundary(double min, double max, double value);
    double Sigmoid(double x, double B, double c, double r); // x - argument; A, sigma, u - Gauss parameters; B, c, r - Sigmoid parameters B/(1 + exp(-c(x-r))
    
  private:
  
    std::string Polarity;
    int threshold_min;
    int threshold_max;
    int threshold_step;
    int number_of_steps;
    int number_of_TP;
    int N; //number of agents
    
    std::vector<double> data_Y;
    std::vector<double> data_X; 
    
  };
}

