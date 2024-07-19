#include <gtest/gtest.h>
#include "function.h"

// Функция для создания синусоидального напряжения
std::vector<int> generate_sinusoidal_sample(double frequency, double amplitude, int sample_rate, int num_samples) {
    std::vector<int> sample(num_samples);
    double Ts = 1.0 / sample_rate / 2;

    for (int i = 0; i < num_samples; ++i) {
        double t = i * Ts;
        double value = amplitude * sin(2 * M_PI * frequency * t);
        sample[i] = static_cast<int>(value);
    }

    return sample;
}

// Тестовые случаи для функции find_freq
TEST(FindFreqTest, Test250Hz) {
    std::vector<int> sample = generate_sinusoidal_sample(50, 100, 250, 300);
    int adc_freq = 250;
    std::string freq;
    int warning_flag = 0;

    find_freq(sample, adc_freq, freq, warning_flag);

    EXPECT_STREQ(freq.c_str(), "50.00"); 
    EXPECT_EQ(warning_flag, 0); 
}

TEST(FindFreqTest, Test500Hz) {
    std::vector<int> sample = generate_sinusoidal_sample(50, 100, 500, 300);
    int adc_freq = 500;
    std::string freq;
    int warning_flag = 0;

    find_freq(sample, adc_freq, freq, warning_flag);

    EXPECT_STREQ(freq.c_str(), "50.00"); 
    EXPECT_EQ(warning_flag, 0); 
}

TEST(FindFreqTest, Test1000Hz) {
    std::vector<int> sample = generate_sinusoidal_sample(50, 100, 1000, 300);
    int adc_freq = 1000;
    std::string freq;
    int warning_flag = 0;

    find_freq(sample, adc_freq, freq, warning_flag);

    EXPECT_STREQ(freq.c_str(), "50.00"); 
    EXPECT_EQ(warning_flag, 0); 
}

TEST(FindFreqTest, Test2000Hz) {
    std::vector<int> sample = generate_sinusoidal_sample(50, 100, 2000, 300);
    int adc_freq = 2000;
    std::string freq;
    int warning_flag = 0;

    find_freq(sample, adc_freq, freq, warning_flag);

    EXPECT_STREQ(freq.c_str(), "50.00"); 
    EXPECT_EQ(warning_flag, 0); 
}

TEST(FindFreqTest, Test4000Hz) {
    std::vector<int> sample = generate_sinusoidal_sample(50, 100, 4000, 300);
    int adc_freq = 4000;
    std::string freq;
    int warning_flag = 0;

    find_freq(sample, adc_freq, freq, warning_flag);

    EXPECT_STREQ(freq.c_str(), "50.00"); 
    EXPECT_EQ(warning_flag, 0); 
}

TEST(FindFreqTest, WarningTest) {
    std::vector<int> sample = generate_sinusoidal_sample(1, 100, 500, 300);
    int adc_freq = 500;
    std::string freq;
    int warning_flag = 0;

    find_freq(sample, adc_freq, freq, warning_flag);

    EXPECT_STREQ(freq.c_str(), "0.00"); 
    EXPECT_EQ(warning_flag, 1); 

    sample = generate_sinusoidal_sample(20, 100, 4000, 300);
    adc_freq = 4000;

    find_freq(sample, adc_freq, freq, warning_flag);

    EXPECT_STREQ(freq.c_str(), "0.00"); 
    EXPECT_EQ(warning_flag, 2); 
}

TEST(FindFreqTest, EmptySample) {
    std::vector<int> sample = {};
    int adc_freq = 0;
    std::string freq;
    int warning_flag = 0;

    find_freq(sample, adc_freq, freq, warning_flag);

    EXPECT_STREQ(freq.c_str(), ""); 
    EXPECT_EQ(warning_flag, 0);
}

// Тестовые случаи для функции find_min_max_volt
TEST(FindMinMaxVoltTest, BasicTest){
    std::string amplitutude;
    std::string low_volt;
    std::vector<int> sample = {100, 200, 0, 300, 1000, 700};

    find_min_max_volt(sample, amplitutude, low_volt);

    EXPECT_STREQ(amplitutude.c_str(), "854.70"); 
    EXPECT_STREQ(low_volt.c_str(), "0.00"); 
}

// Тестовые случаи для функции find_deviation
TEST(FindDeviationTest, BasicTest){
    std::vector<int> sample = {100, 300, 70, 30, 200};
    std::string deviation;

    find_deviation(sample, deviation);

    EXPECT_STREQ(deviation.c_str(), "83.57"); 
}

TEST(FindDeviationTest, EmptySample){
    std::vector<int> sample = {};
    std::string deviation;

    find_deviation(sample, deviation);

    EXPECT_STREQ(deviation.c_str(), "");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
