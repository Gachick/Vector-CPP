#include <iostream>
#include <complex>
#include "vector.h"
#include <vector>
#include <random>
#include <string>

void test_leak()
{
    vector<double> test(10000000);
}

vector<double> test_move()
{
    vector<double> test(10000000);
    return test;
}

vector<double> make_vec()
{
    vector<double> move{3, 4, 5};
    return move;
}

vector<vector<double>> check_move_leak()
{
    vector<vector<double>> vofv(5, vector<double>(100000, 1.2));
    return vofv;
}

int main()
{
    test_leak();
    vector<double> move{test_move()};

    vector<double> a;
    std::cout << "Default constructor " << a << "\n";

    vector<int> b{1, 2, 3};
    std::cout << "Initializer list " << b << "\n";

    vector<double> c(5);
    std::cout << "Set size default argument " << c << "\n";

    vector<double> d(3, 1.2);
    std::cout << "Set size set argument " << d << "\n";

    vector<double> copy{1.4, 2.72, 3.14};
    vector<double> e{copy};
    std::cout << "Copy constructor " << e << "\n";
    vector<double> f;
    f = e;
    std::cout << "Copy operator " << f << "\n";

    vector<double> g{make_vec()};
    std::cout << "Move constructor " << g << "\n";
    vector<double> j;
    j = make_vec();
    std::cout << "Move operator " << j << "\n";

    vector<std::complex<double>> k{std::complex<double>{1, 2}, std::complex<double>{3, 4}};
    std::cout << "Complex type " << k << "\n";

    vector<double> l;
    l.push_back(6.7);
    std::cout << "Push back into empty vector " << l << "\n";

    vector<double> p{8.9};
    p.push_back(3.2);
    p.push_back(4.5);
    std::cout << "Push back into non-empty vector " << p << "\n";

    // vector<int> v(-1);
    //  std::vector<int>(-1);
    // while (true)
    // {
    //     vector<double> v(100000);
    //     for (int i = 0; i < 100000; ++i)
    //     {
    //         v[i] = i;
    //     }
    //     std::cout << "Loop";
    // }

    // vector<std::string> type_test;
    //  type_test.push_back(1);

    vector<vector<double>> vofv;

        // while (true)
    // {
    //     vofv = check_move_leak();
    //     std::cout << "Loop";
    // }

    // while (true)
    // {
    //     vector<vector<double>> vofvc(5, vector<double>(100000, 1.2));
    //     vofv = vofvc;
    //     std::cout << "Loop";
    // }

    // vofv[1][2] = 13;
    // std::cout << vofv;
}