#include "Utils.h"
#include <map>

std::map<std::pair<f32, f32>, std::pair< std::uniform_real_distribution<f32>, std::mt19937>> s_gens;

f32 random_float(f32 min, f32 max)
{
    return min + (max - min) * random_float();


    if (s_gens.find({ min,max }) == s_gens.cend())
    {
        auto& gens = s_gens[{min, max}];
        gens.first = std::uniform_real_distribution<f32>(min, max);
        gens.second = std::mt19937();
    }

    auto& gens = s_gens[{min, max}];
    return gens.first(gens.second);

    /*
    std::uniform_real_distribution<f32> distribution(min, max);
    std::mt19937 generator;
    return distribution(generator);
    */
}