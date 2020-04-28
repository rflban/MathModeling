#ifndef MMLAB01_MTPICARD_H_
#define MMLAB01_MTPICARD_H_

#include "Picard.h"

namespace mmlabs {

class MTPicard : public Picard
{
public:
    void computePol(int approx) override;
};

}

#endif // MMLAB01_MTPICARD_H_

