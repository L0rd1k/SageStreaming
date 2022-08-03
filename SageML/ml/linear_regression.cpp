#include "linear_regression.h"

LinearRegression::LinearRegression() {
    run_GradientDescent(x_train.size(), 100);
    // run_testSample(weight, bias);
}

LinearRegression::~LinearRegression() {
}

bool linear_sorting(double val_1, double val_2) {
    double v1 = std::abs(val_1 - 0);
    double v2 = std::abs(val_2 - 0);
    return v1 < v2;
}

double LinearRegression::getSlope(std::vector<double> &train_X, std::vector<double> &train_Y) {

    // sloep =  ( n * sum(x*y) - sum(x) * sum(y)) / (n * sum(x*x) - sum(x) * sum(x))


    double sum_X = std::accumulate(train_X.begin(), train_X.end(), 0);
    double sum_Y = std::accumulate(train_Y.begin(), train_Y.end(), 0);

    double sxx = std::inner_product(train_X.begin(), train_X.end(), train_X.begin(),0);
    double sxy = std::inner_product(train_X.begin(), train_X.end(), train_Y.begin(),0);

    int n = static_cast<int>(train_X.size());
    // ( n * sxy - sum_X * sum_Y) / (n * sxx - sum_X * sum_Y)
    double nor = n * sxy - sum_X * sum_Y;
    double denor = n * sxx - sum_X * sum_X;

    if(denor != 0) 
        return nor/denor;

    return std::numeric_limits<double>::max(); 
}

double LinearRegression::getIntercept(std::vector<double> &train_X, std::vector<double> &train_Y, double slope) {
    double sum_X = std::accumulate(train_X.begin(), train_X.end(), 0);
    double sum_Y = std::accumulate(train_Y.begin(), train_Y.end(), 0);

    int n = static_cast<int>(train_X.size());

    return (sum_Y - slope * sum_X) / n;
}


double LinearRegression::getCost(std::vector<double> &train_X, std::vector<double> &train_Y, 
double slope, double intercept, double &deriv_slope, double &deriv_intercept) {
    int n = static_cast<int>(train_X.size());
    double sum_X = std::accumulate(train_X.begin(), train_X.end(), 0);
    double sum_Y = std::accumulate(train_Y.begin(), train_Y.end(), 0);

    double sxx = std::inner_product(train_X.begin(), train_X.end(), train_X.begin(), 0);    
    double sxy = std::inner_product(train_X.begin(), train_X.end(), train_Y.begin(), 0);    
    double syy = std::inner_product(train_Y.begin(), train_Y.end(), train_Y.begin(), 0);    

    double cost = syy - (2 * slope * sxy) - 
                (2 * intercept * sum_Y) +
                (std::pow(slope,2) * sxx) + 
                (2 * slope * intercept * sum_X) + 
                (n * std::pow(intercept,2));
    cost /= n;

    deriv_slope = 2 * (-sxy + slope*sxx + intercept*sum_X) / n;
    deriv_intercept = 2 * (-sum_Y + slope*sum_X + intercept*n) / n;
    return cost;
}

void LinearRegression::run_linearRegression(std::vector<double> &train_X, std::vector<double> &train_Y, double slope, double intercept) {
    double learning_rate = 0.0002;
    double threshold = 0.0001;
    int iter = 0;

    while(true) {
        double derivative_slope = 0;
        double derivative_intercept = 0;
        double cost = getCost(train_X, train_Y, slope, intercept, derivative_slope, derivative_intercept);
        if(iter % 1000 == 0) {
            std::cout << "Iter: " << iter << 
            " cost = " << cost << " derivative slope = " << derivative_slope <<
            " derivative intercept = " << derivative_intercept << std::endl;
        }
        iter++;
        if(std::abs(derivative_slope) < threshold && std::abs(derivative_intercept) < threshold) {
            std::cout << "Final Slope/Intercept: y = " << slope << " * x + " << intercept << std::endl;
            break;
        }
        slope = slope - learning_rate * derivative_slope;
        intercept = intercept - learning_rate * derivative_intercept;
    }
}

void LinearRegression::run_GradientDescent(size_t values_size, int epochs_num) {
#ifdef METHOD_1
    for (int i = 0; i < values_size * epochs_num; i++) {
        int cur_index = i % values_size;
        double predicted = weight * x_train[cur_index] + bias;
        
        
        error = predicted - y_train[cur_index]; // Loss function in our case
                
        
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
#endif

#ifndef METHOD_2
    std::vector<double> xSub = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<double> ySub = {1, 3, 2, 5, 7, 8, 8, 9, 10, 12};

    double slopeSub =getSlope(xSub,ySub);
    double interceptSub = getIntercept(xSub, ySub, slopeSub);
    std::cout << "Calculated Slope/Intercept: y = " << slopeSub <<" * x + " << interceptSub << std::endl;
    run_linearRegression(x_train, y_train, slopeSub, interceptSub);
    std::cout << std::string(100, '-') << std::endl;
    run_testSample(slopeSub, interceptSub);

    std::cout << "Compare with ground truth" << std::endl;
    double slope = getSlope(x_train, y_train);
    double intercept = getIntercept(x_train, y_train, slope);
    std::cout<< "y = " << slope << " * x + " << intercept << std::endl;  
    run_testSample(slope, intercept);
#endif

}

void LinearRegression::run_testSample(double slope, double intercept) {
    std::cout<<"Enter a test x value: ";
    double test;
    std::cin >> test;
    double prediction = intercept + slope * test;
    std::cout << std::endl;
    std::cout << "The value predicted by the model= " << prediction << std::endl;
}