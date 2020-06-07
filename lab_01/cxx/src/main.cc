#include <iostream>

#include "Picard.h"
#include "MTPicard.h"
#include "FFTPicard.h"

int main()
{
    //mmlabs::APicard *picard = new mmlabs::FFTPicard;
    //mmlabs::APicard *picard = new mmlabs::MTPicard;
    mmlabs::APicard *picard = new mmlabs::Picard;

    picard->computePol(12);
    std::cout << (*picard)(2) << '\n';
    //std::cout << (*picard)(2, 10) << '\n';
    //std::cout << (*picard)(2, 7) << '\n';
    std::cout << (*picard)(2, 4) << '\n';
    std::cout << (*picard)(2, 3) << '\n';
    std::cout << (*picard)(2, 2) << '\n';
    std::cout << (*picard)(2, 1) << '\n';

    delete picard;

    return 0;
}

