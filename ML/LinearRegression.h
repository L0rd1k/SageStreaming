#pragma once

#include <vector>
#include <algorithm>
#include <iostream>
class LinearRegression {
public:
    LinearRegression();
    virtual ~LinearRegression();
private:
    std::vector<double> x_train = { 1, 2, 3, 4, 5};
    std::vector<double> y_train = { 1, 3, 3, 2, 5};
    std::vector<double> error_vec;
    double error = 0.0;
    double bias = 0.0;
    double weight = 0.0;
    double learning_rate = 0.01;
private:
    void run_GradientDescent(int values_size, int epochs_num);
    void run_testSample();
};