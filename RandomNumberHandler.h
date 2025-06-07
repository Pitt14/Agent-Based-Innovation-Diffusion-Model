#pragma once

#include "ModelParameter.h"

#include <random>
#include <vector>
#include <algorithm>

class RandomNumberHandler
{
public:
    RandomNumberHandler()
    {
        _gen = std::mt19937(_rd());
        _bernoulli_distribution = std::bernoulli_distribution(ModelParameter::c0);
        _uniform_int_distribution = std::uniform_int_distribution <int> (0, ModelParameter::N - 1);
        _uniform_real_distribution = std::uniform_real_distribution <float> (0.0f, 1.0f);
    }

    bool getBernoulliDistribution()
    {
        return _bernoulli_distribution(_gen);
    }

    int getUniformIntDistribution()
    {
        return _uniform_int_distribution(_gen);
    }

    float getUniformRealDistribution()
    {
        return _uniform_real_distribution(_gen);
    }

    std::vector <std::size_t> getRandomSampleWithException(std::size_t number_of_neighbors, uint16_t q, int forbidden_value)
    {
        std::vector <std::size_t> result;
        std::array <bool, ModelParameter::N> allowed_values;

        allowed_values.fill(true);
        allowed_values[forbidden_value] = false;

        for(int i = 0; i < q; i++)
        {
            int proposed_value;

            while(true)
            {
                proposed_value = getUniformIntDistribution();
                if(allowed_values[proposed_value] == true)
                {
                    allowed_values[proposed_value] = false;
                    break;
                }
            }
            
            result.push_back(getUniformIntDistribution());
        }

        return result;
    }

    #if NETWORK_MODEL == PREDEFINED_TOPOLOGY || NETWORK_MODEL == THEORETICAL_GRAPH
    std::vector <size_t> getRandomSample(std::size_t number_of_neighbors, uint16_t q)
    {
        std::size_t return_value_size = std::min(number_of_neighbors, static_cast<std::size_t>(q));

        std::vector <std::size_t> return_values;
        if(number_of_neighbors == NetworkTopology::MAX_NEIGHBORS - 1) {
            std::sample(ModelParameter::CONSECUTIVE_NUMBERS.begin(), ModelParameter::CONSECUTIVE_NUMBERS.end(), std::back_inserter(return_values), return_value_size, _gen);
        }
        else {
            std::sample(ModelParameter::CONSECUTIVE_NUMBERS.begin(), ModelParameter::CONSECUTIVE_NUMBERS.begin() + number_of_neighbors, std::back_inserter(return_values), return_value_size, _gen);
        }
        return return_values;
    }
    #endif

private:
    std::random_device _rd;
    std::mt19937 _gen;
    std::bernoulli_distribution _bernoulli_distribution;
    std::uniform_int_distribution <int> _uniform_int_distribution;
    std::uniform_real_distribution <float> _uniform_real_distribution;
};