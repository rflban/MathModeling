#include <iostream>

#include "Picard.h"
#include "MTPicard.h"

int main()
{
    mmlabs::APicard *picard = new mmlabs::MTPicard;
    //mmlabs::APicard *picard = new mmlabs::Picard;

    picard->computePol(15);
    std::cout << (*picard)(2) << '\n';

	delete picard;

    return 0;
}

