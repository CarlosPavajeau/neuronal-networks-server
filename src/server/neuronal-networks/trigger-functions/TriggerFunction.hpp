//
// Created by cantte on 28/04/21.
//

#ifndef NEURONAL_NETWORKS_SERVER_TRIGGERFUNCTION_HPP
#define NEURONAL_NETWORKS_SERVER_TRIGGERFUNCTION_HPP

#include <cmath>

enum TriggerFunctions
{
    Linear,
    Sigmoidal,
    HyperbolicTangent
};

class TriggerFunction
{
public:
    virtual double Output(double input) = 0;
};

class LinearFunction final : public TriggerFunction
{
public:
    double Output(double input) override
    {
        return input;
    }
};

class SigmoidalFunction final : public TriggerFunction
{
public:
    double Output(double input) override
    {
        return 1 / (1 + std::exp(-2 * input));
    }
};

class HyperbolicTangentFunction final : public TriggerFunction
{
public:
    double Output(double input) override
    {
        return std::tanh(input);
    }
};

class TriggerFunctionFactory
{
public:
    static TriggerFunctionFactory* instance()
    {
        static TriggerFunctionFactory factory;
        return &factory;
    }

    TriggerFunction* MakeTriggerFunction(TriggerFunctions triggerFunctions)
    {
        switch (triggerFunctions)
        {
            case TriggerFunctions::Linear:
                return new LinearFunction();
            case TriggerFunctions::Sigmoidal:
                return new SigmoidalFunction();
            case TriggerFunctions::HyperbolicTangent:
                return new HyperbolicTangentFunction();
            default:
                return nullptr;
        }
    }

protected:
    TriggerFunctionFactory() = default;
};

#define sTriggerFunctionFactory TriggerFunctionFactory::instance()

#endif //NEURONAL_NETWORKS_SERVER_TRIGGERFUNCTION_HPP
