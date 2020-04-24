#include "Polynomial.h"

#include "Monomial.h"

Polynomial::Polynomial()
{
}

Polynomial::Polynomial(std::initializer_list<Monomial *> monomials)
{
    for (Monomial *monomial: monomials)
    {
        this->monomials.push_back(monomial);
    }
}

Polynomial::Polynomial(const Polynomial &other)
{
    for (Monomial *monomial: other.monomials)
    {
        this->monomials.push_back(new Monomial(*monomial));
    }
}

Polynomial::~Polynomial()
{
    for (Monomial *item: monomials)
        delete item;

    monomials.clear();
}

double Polynomial::operator()(double x)
{
    double sum = 0;

    for (Monomial *monomial: monomials)
        sum += (*monomial)(x);

    return sum;
}

Polynomial &Polynomial::operator+=(Monomial *monomial)
{
    monomials.push_back(monomial);

    return *this;
}

Polynomial &Polynomial::operator*=(double num)
{
    for (Monomial *monomial: monomials)
        *monomial *= num;

    return *this;
}

Polynomial &Polynomial::operator*=(const Monomial &m)
{
    for (Monomial *monomial: monomials)
        *monomial *= m;

    return *this;
}

Polynomial &Polynomial::operator*=(const Polynomial &other)
{
    const Polynomial *p;

    if (&other == this)
        p = new Polynomial(*this);
    else
        p = &other;

    std::vector<Monomial *> new_monomials;

    for (Monomial *monomial: monomials)
        for (const Monomial *m: p->monomials)
        {
            //Monomial *new_monomial = new Monomial(*monomial);
            //new_monomials.push_back(new_monomial);
            new_monomials.push_back(new Monomial(*monomial));
            **new_monomials.rbegin() *= *m;
        }

    for (Monomial *item: monomials)
        delete item;
    monomials.swap(new_monomials);
    //monomials.clear();

    //for (Monomial *item: new_monomials)
        //monomials.push_back(item);

    if (&other == this)
        delete p;

    return *this;
}

Polynomial &Polynomial::power(unsigned int power)
{
    Polynomial temp = *this;

    while (--power)
        *this *= temp;

    return *this;
}

Polynomial::operator std::string() const
{
    std::string base = "";

    for (size_t idx = 0; idx < monomials.size() - 1; idx++)
        base += (std::string)*monomials[idx] + " + ";

    if (!monomials.empty())
        base += (std::string)*monomials[monomials.size() - 1];

    return base;
}

