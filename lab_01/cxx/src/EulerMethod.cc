#include "EulerMethod.h"

#include <cmath>

namespace mmlabs {
namespace EulerMethod {

double explicitMethod(double x, double h)
{
    double res = 0;
    double x0 = h;

    while (x0 < x + h / 2)
    {
        res += h * (x0 * x0 + res * res);
        x0 += h;
    }

    return res;
}

double implicitMethod(double x, double h)
{
    double f1;
    double f2;
    double tmp;
    double res = 0;
    double x0 = h;

    while (x0 < x + h)
    {
        tmp = 1 - 4 * h * (h * x0 * x0 + res);

        if (tmp >= 0)
        {
            f1 = (1 + ::sqrt(tmp)) / 2 / h;
            f2 = (1 - ::sqrt(tmp)) / 2 / h;

            if (f2 < 0)
                res = f1;
            else if (f1 < 0)
                res = f2;
            else
                res = f1 > f2 ? f2 : f1;
        }

        x0 += h;
    }

    return res;
}

}
}

