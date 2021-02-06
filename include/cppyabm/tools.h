
#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <random>
#include <cmath>
#include <algorithm>
//! Tools commonly used in CppyABM
namespace tools{
    //! Generate random numbers
    auto randomly_seeded_MT () ;
}

inline auto tools::randomly_seeded_MT () {
    std::vector<unsigned int> random_data(624);
    std::random_device source;
    std::generate(begin(random_data), end(random_data), [&](){return source();});
    std::seed_seq seeds(begin(random_data), end(random_data));
    std::mt19937 seededEngine (seeds);
    return seededEngine;
}