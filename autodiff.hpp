#ifndef AUTODIFF_HPP
#define AUTODIFF_HPP

#include <set>
#include <vector>
#include <functional>
#include <ostream>
#include <math.h>

#include <iostream>

namespace autodiff{

class scalar;
typedef scalar* node;
std::vector<node> nodes;

template<typename foo>
std::function<void(foo)> DO_NOTHING = [](foo bar){ return; };

class scalar{
public:
    // Members
    double val;
    double grad = 0;
    std::pair<node, node> parents;
    std::function<void(node)> compute_grad = DO_NOTHING<node>;
    // Constructors
    scalar(double a);
    scalar(scalar& b);
    scalar(double a, node first, node second);
    scalar(scalar& a, node first, node second); 
    // Backwards
    void backward();
    // Arithmetic Operators
    scalar operator + (scalar& b);
    scalar operator + (scalar&& b);
    scalar operator + (double b);
    scalar operator * (scalar& b);
    scalar operator * (scalar&& b);
    scalar operator * (double b);
};

std::ostream& operator<<(std::ostream& os, const scalar& a);
scalar pow(scalar& a, const double&& p);
scalar pow(scalar&& a, const double&& p);

} // autodiff [namespace#end]

// Definitions
#include "autodiff.cpp"

#endif