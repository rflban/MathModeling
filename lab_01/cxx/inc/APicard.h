#ifndef MMLAB01_APICARD_H_
#define MMLAB01_APICARD_H_

namespace mmlabs {

class APicard
{
public:
    virtual ~APicard() = default;

    virtual void computePol(int approx) = 0;
    virtual double operator()(double x) = 0;
};

}

#endif // MMLAB01_APICARD_H_

