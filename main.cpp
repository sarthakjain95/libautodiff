
#include <iostream>
#include "autodiff.hpp"

int main(){

    using scalar = autodiff::scalar;
    using std::cout; using std::endl;

    scalar x{-4};
    scalar y{2};

    // auto z{y + (autodiff::pow(x, 2) * y)};
    auto z{(x * y * autodiff::pow(x, 2) * y) * y};
    // auto z{(x * y + x * (x * y))};

    z.backward(true);

    cout << endl;
    for(const autodiff::node& n : autodiff::nodes){
        cout << ((uint64_t)n) << ' ' << *n << endl;
    }   cout << endl;

    cout << ((uint64_t)&x) << " " << x << endl;
    cout << ((uint64_t)&y) << " " << y << endl;
    cout << ((uint64_t)&z) << " " << z << endl;

    return 0;
}
