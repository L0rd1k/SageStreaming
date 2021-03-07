#include "ML/LinearRegression.h"

LinearRegression::LinearRegression() {
    run_GradientDescent(x_train.size(), 100);
    run_testSample();
}

LinearRegression::~LinearRegression() {
}

bool linear_sorting(double val_1, double val_2) {
    double v1 = std::abs(val_1 - 0);
    double v2 = std::abs(val_2 - 0);
    return v1 < v2;
}

void LinearRegression::run_GradientDescent(int values_size, int epochs_num) {
    for (int i = 0; i < values_size * epochs_num; i++) {
        int cur_index = i % values_size;
        double predicted = weight * x_train[cur_index] + bias;
        error = predicted - y_train[cur_index];
        bias = bias - learning_rate * error;
        weight = weight - learning_rate * error * x_train[cur_index];
        std::cout << i+1 << ") Bias = " << bias << " "
            << " Weight = " << weight << " "
            << "Error = " << error << std::endl;
        error_vec.push_back(error);
    }

    sort(error_vec.begin(), error_vec.end(), linear_sorting);
    std::cout << "Final values: Bias = " << bias << " "
        << " Weight = " << weight << " "
        << "Error = " << error_vec[0] << std::endl;
}

void LinearRegression::run_testSample() {
    std::cout<<"Enter a test x value: ";
    double test;
    std::cin >> test;
    double prediction = bias + weight * test;
    std::cout << std::endl;
    std::cout << "The value predicted by the model= " << prediction << std::endl;
}