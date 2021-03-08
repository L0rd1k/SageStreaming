#pragma once

#include <vector>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <math.h>

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
    void run_testSample(double slope, double intercept);

    void run_linearRegression(std::vector<double> &train_X, std::vector<double> &train_Y, double slope = 1, double intercept = 0);


    double getSlope(std::vector<double> &train_X, std::vector<double> &train_Y);
    double getIntercept(std::vector<double> &train_X, std::vector<double> &train_Y, double slope);
    double getCost(std::vector<double> &train_X, std::vector<double> &train_Y, double slope, double intercept, double &deriv_slope, double &deriv_intercept);
};