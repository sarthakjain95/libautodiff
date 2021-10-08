
#include <iostream>
#include "autodiff.hpp"

int main(){

    using scalar = autodiff::scalar;
    using std::cout; using std::endl;

    scalar x{-4};
    scalar y{2};

    auto z = x * y;

    // auto z{(x * y + autodiff::pow(x, 2) * y) * y};
    // auto z{(x * y + (x * x) * y)};
    // auto z{(x * y + x * (x * y))};

    // z.backward();

    cout << x << endl;
    cout << y << endl;
    cout << z << endl;

    return 0;
}
