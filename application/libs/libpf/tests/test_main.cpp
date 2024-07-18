#include <gtest/gtest.h>
#include "function.h"

// Тестовый случай для функции find_freq
TEST(FindFreqTest, BasicTest) {
    std::vector<int> sample = {1, 2, 3, 4, 5};
    int adc_freq = 0;
    std::string freq;
    int warning_flag = 0;

    // Вызов функции find_freq
    find_freq(sample, adc_freq, freq, warning_flag);

    // Проверка результатов
    EXPECT_EQ(adc_freq, 100); // Пример ожидаемого значения
    EXPECT_STREQ(freq.c_str(), "100Hz"); // Пример ожидаемого значения
    EXPECT_EQ(warning_flag, 0); // Пример ожидаемого значения
}

// Дополнительные тестовые случаи могут быть добавлены аналогичным образом
TEST(FindFreqTest, EmptySample) {
    std::vector<int> sample = {};
    int adc_freq = 0;
    std::string freq;
    int warning_flag = 0;

    // Вызов функции find_freq
    find_freq(sample, adc_freq, freq, warning_flag);

    // Проверка результатов
    EXPECT_EQ(adc_freq, 0); // Пример ожидаемого значения
    EXPECT_STREQ(freq.c_str(), ""); // Пример ожидаемого значения
    EXPECT_EQ(warning_flag, 1); // Пример ожидаемого значения
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
