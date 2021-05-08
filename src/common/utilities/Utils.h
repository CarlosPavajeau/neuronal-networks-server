//
// Created by cantte on 3/05/21.
//

#ifndef NEURONAL_NETWORKS_SERVER_UTILS_H
#define NEURONAL_NETWORKS_SERVER_UTILS_H

#include "Define.hpp"

#include <cmath>
#include <string>
#include <functional>
#include <unordered_map>
#include <algorithm>
#include <Eigen/Dense>

inline double sigmoid(double x)
{
    return 1 / (1 + std::exp(-x));
}

inline double d_sigmoid(double x)
{
    return sigmoid(x) * (1 - sigmoid(x));
}

inline double hyperbolic_tan(double x)
{
    return std::tanh(x);
}

inline double d_hyperbolic_tan(double x)
{
    return 1 - std::pow(hyperbolic_tan(x), 2);
}

inline double linear(double x)
{
    return x;
}

inline double d_linear(double x)
{
    return 1;
}

inline double thin_plate_spline(double x)
{
    return std::pow(x, 2) * std::log(x);
}

inline double d_thin_plate_spline(double x)
{
    return 2 * x * std::log(x) + x;
}

struct Point
{
public:
    Point(uint64 x, uint64 y) : X(x), Y(y) {}

    Point() : X(0), Y(0) {}

    uint64 X, Y;
};

class ActivationFunctionsMgr
{
public:
    static ActivationFunctionsMgr& instance()
    {
        static ActivationFunctionsMgr instance;
        return instance;
    }

    bool GetActivationFunctionPair(const std::string& activation_name,
                                   std::pair<std::function<double(double)>, std::function<double(double)>>** pair)
    {
        auto iter = activation_functions_map.find(activation_name);
        if (iter == activation_functions_map.end())
        {
            return false;
        }

        *pair = &(iter->second);
        return false;
    }

private:
    ActivationFunctionsMgr()
    {
        AddNewPair("sigmoid", sigmoid, d_sigmoid);
        AddNewPair("tanh", hyperbolic_tan, d_hyperbolic_tan);
        AddNewPair("linear", linear, d_linear);
        AddNewPair("thin_plate_spline", thin_plate_spline, d_thin_plate_spline);
    }

    void AddNewPair(std::string function_name, std::function<double(double)> function,
                    std::function<double(double)> d_function)
    {
        activation_functions_map.insert(std::make_pair(function_name, std::make_pair(function, d_function)));
    }

    std::unordered_map<std::string, std::pair<std::function<double(double)>, std::function<double(
            double)>>> activation_functions_map;
};

#define sActivationFunctionMgr ActivationFunctionsMgr::instance()

inline void Softmax(std::vector<double>* output)
{
    size_t num_elements = output->size();
    std::vector<double> exp_output(num_elements);
    double exp_total = 0.0;

    for (size_t i = 0; i < num_elements; ++i)
    {
        exp_output[i] = exp((*output)[i]);
        exp_total += exp_output[i];
    }

    for (size_t i = 0; i < num_elements; ++i)
    {
        (*output)[i] = exp_output[i] / exp_total;
    }
}

inline void GetIdMaxElement(const std::vector<double>& output, size_t* class_id)
{
    *class_id = std::distance(output.begin(), std::max_element(output.begin(), output.end()));
}

inline void matrix_vector_to_eigen_matrix(const std::vector<std::vector<double>>& x, Eigen::MatrixXd* matrix)
{
    matrix->resize(x.size(), x[0].size());

    for (int i = 0; i < x.size(); ++i)
    {
        for (int j = 0; j < x[i].size(); ++j)
        {
            (*matrix)(i, j) = x[i][j];
        }
    }
}

inline void vector_to_eigen_vector(const std::vector<double>& x, Eigen::VectorXd* vector)
{
    vector->resize(x.size());
    for (int i = 0; i < x.size(); ++i)
    {
        (*vector)[i] = x[i];
    }
}

#endif //NEURONAL_NETWORKS_SERVER_UTILS_H
