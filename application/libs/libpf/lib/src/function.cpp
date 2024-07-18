#include "function.h"

void find_freq(std::vector<int>& sample, int& adc_freq, std::string& freq, int& warning_flag){
    int mid_val = 0;
    int start_flag = 0;
    int per_flag = 0;
    int half_per_flag = 0;
    double half_per = 1 / (double)adc_freq / 2;
    int l = 0, r = 0;
    int count_period = 0;
    double fr = 0;

    for (int i = 0; i < sample.size(); ++i)
        mid_val += sample[i];
    mid_val /= sample.size();

    for (int i = 0; i < sample.size() - 2 ; ++i){

        if (mid_val < sample[i] && mid_val < sample[i + 1] && mid_val < sample[i + 2]){
            half_per_flag = 1;
        }

        if (half_per_flag == 1){
            if (mid_val > sample[i] && mid_val > sample[i + 1] && mid_val > sample[i + 2]) {
                per_flag = 1;
                half_per_flag = 0;
            }
        }

        if (per_flag == 1){
            if (r != 0){
                fr += 1 / (half_per * (r - l));
                count_period++;
                l = r;
            } //else l = i;
            start_flag = 1;
            per_flag = 0;
        }

        if (start_flag == 1){
            r++;
        }
    }

    if (fr != 0){
        fr /= count_period;
    }
    else{
        if (l == 0 && r == 0){
            warning_flag = 1;
        }

        if (l == 0 && r != 0){
            warning_flag = 2;
        }
    }

    freq = std::to_string(round(fr * 100) / 100);
    freq = freq.substr(0, freq.length() - 4);
}

void find_min_max_volt(std::vector<int>& sample, std::string& amplitude, std::string& low_volt){
    float ampl = sample[0];
    float low_v = sample[0];

    for (int i = 0; i < sample.size() - 1; ++i){

        if (ampl < sample[i])
            ampl = sample[i];

        if (low_v > sample[i])
            low_v = sample[i];
    }

    ampl = ampl / 4095 * 3500; // уточнить опорное напряжение
    low_v = low_v / 4095 * 3500; // уточнить опорное напряжение

    amplitude = std::to_string(round(ampl * 100) / 100);
    amplitude = amplitude.substr(0, amplitude.length() - 4);
    low_volt = std::to_string(round(low_v * 100) / 100);
    low_volt = low_volt.substr(0, low_volt.length() - 4);
}

void find_deviation(std::vector<int>& sample, std::string& deviation){
    int mid_val = 0;
    float dev_sum = 0;

    for (auto num : sample)
        mid_val += num;
    mid_val /= sample.size();

    for (auto num : sample)
        dev_sum += pow(abs(num - mid_val), 2);
    dev_sum /= sample.size();

    dev_sum = pow(dev_sum, 0.5) / 4095 * 3500; // уточнить опорное напряжение

    deviation = std::to_string(round(dev_sum * 100) / 100);
    deviation= deviation.substr(0, deviation.length() - 4);
}

int min_val(std::vector<int>& sample){
    int mini = sample[0];
    for (auto num : sample){
        if (mini > num)
            mini = num;
    }
    return mini;
}

int max_val(std::vector<int>& sample){
    int maxi = sample[0];
    for (auto num : sample){
        if (maxi < num)
            maxi = num;
    }
    return maxi;
}
