#ifndef MMLAB01_MONOMIAL_H_
#define MMLAB01_MONOMIAL_H_

#include <string>

template <typename _T>
struct Node;

class Monomial
{
public:
    Monomial(int power=1);
    Monomial(const Monomial &other);

    ~Monomial();

    double operator()(double x);

    Monomial &operator*=(double r);
    Monomial &operator*=(const Monomial &m);

    Monomial &integrate();

    operator std::string() const;

private:
    int power;
    int sign;

    Node<double> *ratiosAboveOne;
    Node<double> *ratiosBelowOne;
};

#endif // MMLAB01_MONOMIAL_H_

