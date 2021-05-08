//
// Created by cantte on 7/05/21.
//

#include "RadialBasisFunction.h"
#include "Utils.h"

#include <Eigen/Dense>

RadialBasisFunction::RadialBasisFunction(const std::vector<uint64>& layers_nodes, Session* session) :
        _output_node((int) layers_nodes[1])
{
    Init(layers_nodes);
}

void RadialBasisFunction::Init(const std::vector<uint64>& layers_nodes)
{
    _num_inputs = layers_nodes[0];

    for (int i = 0; i < layers_nodes[1]; ++i)
    {
        _radial_centers.emplace_back(_num_inputs);
    }
}

void RadialBasisFunction::GetOutput(const std::vector<double>& input, double* output) const
{
    std::vector<double> euclidean_distances;
    for (const auto& _radial_center : _radial_centers)
    {
        double distance = 0.0;
        _radial_center.GetEuclideanDistance(input, &distance);

        euclidean_distances.push_back(distance);
    }

    double temp_out;

    _output_node.GetOutputAfterActivationFunction(euclidean_distances, thin_plate_spline, &temp_out);

    *output = temp_out;
}

bool RadialBasisFunction::Train(const RadialBasisFunctionTrainingInfo& trainingInfo)
{
    double current_iteration_cost_function;
    for (int i = 0; i < trainingInfo.MaxSteps; ++i)
    {
        current_iteration_cost_function = 0.0;
        SetUpRadialCenters(trainingInfo.Inputs);

        std::vector<std::vector<double>> distances;
        for (const auto& input : trainingInfo.Inputs)
        {
            std::vector<double> euclidean_distances;
            euclidean_distances.push_back(1);
            for (const auto& _radial_center : _radial_centers)
            {
                double distance = 0.0;
                _radial_center.GetEuclideanDistance(input, &distance);
                euclidean_distances.push_back(distance);
            }
            distances.push_back(euclidean_distances);
        }

        Eigen::MatrixXd m_distances;
        Eigen::VectorXd v_output;

        matrix_vector_to_eigen_matrix(distances, &m_distances);
        vector_to_eigen_vector(trainingInfo.Outputs, &v_output);

        auto v_result = m_distances.bdcSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(v_output).eval();
        std::vector<double> new_weights;
        for (int j = 1; j < v_result.size(); ++j)
        {
            new_weights.push_back(v_result[j]);
        }

        _output_node.SetBias(v_result[0]);
        _output_node.SetWeights(new_weights);

        std::vector<double> predicted_output;
        for (const auto& input : trainingInfo.Inputs)
        {
            double output = 0.0;
            GetOutput(input, &output);
            predicted_output.push_back(output);
        }

        std::vector<double> corrected_output = trainingInfo.Outputs;;
        for (int j = 0; j < predicted_output.size(); ++j)
        {
            current_iteration_cost_function += std::pow(corrected_output[j] - predicted_output[j], 2);
        }

        current_iteration_cost_function /= (double) trainingInfo.Inputs.size();
        if (current_iteration_cost_function < trainingInfo.ErrorTolerance)
        {
            return true;
        }
    }
    return false;
}

void RadialBasisFunction::SetUpRadialCenters(const std::vector<std::vector<double>>& inputs)
{
    Eigen::MatrixXd m_inputs;
    matrix_vector_to_eigen_matrix(inputs, &m_inputs);

    double max_value = m_inputs.maxCoeff(), min_value = m_inputs.minCoeff();
    for (auto& _radial_center : _radial_centers)
    {
        _radial_center.CentroidsInitialization(min_value, max_value);
    }
}


