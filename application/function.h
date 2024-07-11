#ifndef FUNCTION_H
#define FUNCTION_H
#include <iostream>
#include <cmath>
#include <vector>
#include <string>

void find_freq(std::vector<int>& sample, int& adc_freq, std::string& freq);
void find_min_max_volt(std::vector<int>& sample, std::string& amplitude, std::string& low_volt);
void find_deviation(std::vector<int>& sample, std::string& deviation);
int min_val(std::vector<int>& sample);
int max_val(std::vector<int>& sample);

#endif // FUNCTION_H
