
#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <random>
#include <cmath>
#include <algorithm>
namespace random_{
    // double random(double start, double end){ // between 0-1
    //     srand((unsigned) time(NULL));
    //     return (float) rand()/RAND_MAX;
    // }
    // int random_discrete(int start, int end){// int values between start and end
    //     srand((unsigned) time(NULL));
    //     auto randomNumber = (rand() % end) + start;
    //     return randomNumber;
    // }
    auto randomly_seeded_MT () ;
}

inline auto random_::randomly_seeded_MT () {
    // Magic number 624: The number of unsigned ints the MT uses as state
    std::vector<unsigned int> random_data(624);
    std::random_device source;
    std::generate(begin(random_data), end(random_data), [&](){return source();});
    std::seed_seq seeds(begin(random_data), end(random_data));
    std::mt19937 seededEngine (seeds);
    return seededEngine;
}