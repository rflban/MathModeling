#ifndef MMLAB01_PICARDMETHOD_H_
#define MMLAB01_PICARDMETHOD_H_

class Monomial;

class PicardMethod
{
public:
    static double calc(const Monomial &m, double x, unsigned int n);
};

#endif // MMLAB01_PICARDMETHOD_H_

