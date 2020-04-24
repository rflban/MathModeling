#ifndef MMLAB01_POLYNOMIAL_H_
#define MMLAB01_POLYNOMIAL_H_

#include <string>
#include <vector>
#include <initializer_list>

class Monomial;

class Polynomial
{
public:
    Polynomial();
    Polynomial(std::initializer_list<Monomial *> monomials);
    Polynomial(const Polynomial &);

    ~Polynomial();

    double operator()(double x);

    Polynomial &operator+=(Monomial *monomial);

    Polynomial &operator*=(double num);
    Polynomial &operator*=(const Monomial &m);
    Polynomial &operator*=(const Polynomial &p);

    Polynomial &power(unsigned int power);
    Polynomial &integrate();

    operator std::string() const;

private:
    std::vector<Monomial *> monomials;
};

#endif // MMLAB01_POLYNOMIAL_H_

