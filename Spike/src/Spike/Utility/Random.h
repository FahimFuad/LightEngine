//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#pragma once
#include <random>

namespace Spike
{
    class Random
    {
    public:
        template<typename T>
        static T SPKRandom(T min = 0, T max = 100)
        {
            if constexpr (std::is_same_v<T, int>)
            {
                if (min > max)
                {
                    std::swap(min, max);
                    SPK_CORE_LOG_WARN("Minimum is greater than maximum in %s", __FUNCTION__);
                }

                std::uniform_int_distribution<> distribution(min, max);
                auto& mt = Engine();
                return distribution(mt);
            }

            if constexpr (std::is_same_v<T, float>)
            {
                if (min > max)
                {
                    std::swap(min, max);
                    SPK_CORE_LOG_WARN("Minimum is greater than maximum in %s", __FUNCTION__);
                }

                std::uniform_real_distribution<T> distribution(min, max);
                auto& mt = Engine();
                return distribution(mt);
            }

            if constexpr (std::is_same_v<T, double>)
            {
                if (min > max)
                {
                    std::swap(min, max);
                    SPK_CORE_LOG_WARN("Minimum is greater than maximum in %s", __FUNCTION__);
                }

                std::uniform_real_distribution<T> distribution(min, max);
                auto& mt = Engine();
                return distribution(mt);
            }

            if constexpr (std::is_same_v<T, bool>)
            {
                std::bernoulli_distribution distribution(0.5);
                auto& mt = Engine();
                return distribution(mt);
            }

            if constexpr (!std::is_same_v<T, bool> || !std::is_same_v<T, int> || !std::is_same_v<T, float> || !std::is_same_v<T, double>)
            {
                SPK_INTERNAL_ASSERT("Given data type in SPKRandom is not supported!");
                return -1;
            }
        }
    private:
        static std::mt19937_64& Random::Engine()
        {
            static std::random_device rd;
            static std::mt19937_64 engine(rd());
            return engine;
        }
    };

}
