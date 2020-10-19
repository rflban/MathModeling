#include <iostream>
#include <vector>

#include "Monomial.h"
#include "PicardMethod.h"

int main()
{
    std::cout << PicardMethod::calc(Monomial(2), 1, 4) << '\n';

    return 0;
}

//int main()
//{
    //Monomial m(5);
    //Monomial m1(2);

    //m *= 4;
    //m *= 0.3;
    //m *= -1;
    //m *= 5;
    //m *= 0.25;
    //m *= 0.7;
    //m *= 1;
    //m *= -1;

    //m1 *= 3;
    //m1 *= 4.5;
    //m1 *= 4.7;
    //m1 *= 4.4;
    //m1 *= 10.4;
    //m1 *= 0.29;

    //std::cout << (std::string)m << '\n';
    //std::cout << m(0.5) << '\n';
    //std::cout << (std::string)m << '\n';

    //return 0;
//}

