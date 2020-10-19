#include "EulerMethod.h"

#include <cmath>

namespace mmlabs {
namespace EulerMethod {

double explicitMethod(double x, double h)
{
    double res = 0;
    double x0 = h;

    while (x0 < x + h)
    {
        res += h * (x0 * x0 + res * res);
        x0 += h;
    }

    return res;
}

double implicitMethod(double x, double h)
{
    double D;
    //double root1;
    //double root2;
    double res = 0;
    double x0 = h;

    while (x0 < x + h)
    {
        D = 1 - 4 * h * (h * x0 * x0 + res);

        //root1 = (1 + ::sqrt(D)) / 2 / h;
        //root2 = (1 - ::sqrt(D)) / 2 / h;

        if (D >= 0)
            //res = fabs(root1) > fabs(root2) ? root2 : root1;
            res = (1 - ::sqrt(D)) / 2 / h;

        x0 += h;
    }

    return res;
}

}
}

