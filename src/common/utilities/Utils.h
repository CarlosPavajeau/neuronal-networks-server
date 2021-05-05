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
    }

    void AddNewPair(std::string function_name, std::function<double(double)> function,
                    std::function<double(double)> d_function)
    {
        activation_functions_map.insert(std::make_pair(function_name, std::make_pair(function, d_function)));
    }

    std::unordered_map<std::string, std::pair<std::function<double(double)>, std::function<double(
            double)>>> activation_functions_map;
};

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

#define sActivationFunctionMgr ActivationFunctionsMgr::instance()

#endif //NEURONAL_NETWORKS_SERVER_UTILS_H
