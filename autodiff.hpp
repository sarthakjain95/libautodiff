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

std::string default_code{"a"};
int rounds{};

std::function<void(node)> DO_NOTHING = [](node _){ return; };

class scalar{
public:
    /* Members */
    const double val;
    double grad = 0;
    std::pair<node, node> parents;
    std::function<void(node)> compute_grad = DO_NOTHING;
    std::string eq;
    std::string label;
    /* Constructors */
    scalar(const double a);
    // scalar(const scalar& b);
    // scalar(const scalar&& b);
    // scalar(const double a, std::string _name);
    // scalar(const scalar& b, std::string _name);
    // scalar(const scalar&& b, std::string _name);
    scalar(const double a, const node first, const node second, const std::string op);
    // scalar(const scalar& a, const node first, const node second, const std::string op); 
    /* Backwards */
    void backward(bool generate_graph);
    /* Arithmetic Operators */
    scalar& operator + (scalar& b);
    scalar& operator + (scalar&& b);
    // scalar operator + (double b);
    scalar& operator * (scalar& b);
    scalar& operator * (scalar&& b);
    // scalar operator * (double b);
};

std::ostream& operator<<(std::ostream& os, const scalar& a);
scalar& pow(scalar& a, const double&& p);
scalar& pow(scalar&& a, const double&& p);

} // namespace autodiff

#ifdef PLOT
    #include "plot.cpp"
#endif

// Definitions
#include "autodiff.cpp"

#endif