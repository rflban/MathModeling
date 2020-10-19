#include "Monomial.h"

#include <cmath>

#include "Node.hxx"

Monomial::Monomial(int power) :
    power(power),
    sign(1),
    ratiosAboveOne(nullptr),
    ratiosBelowOne(nullptr)
{
}

Monomial::Monomial(const Monomial &other) :
    power(other.power),
    sign(other.sign),
    ratiosAboveOne(nullptr),
    ratiosBelowOne(nullptr)
{
    Node<double> **cursor;
    Node<double> *current;

    for (cursor = &ratiosAboveOne, current = other.ratiosAboveOne;
         current != nullptr;
         cursor = &(*cursor)->next, current = current->next)
    {
        *cursor = new Node<double> { nullptr, current->data };
    }

    for (cursor = &ratiosBelowOne, current = other.ratiosBelowOne;
         current != nullptr;
         cursor = &(*cursor)->next, current = current->next)
    {
        *cursor = new Node<double> { nullptr, current->data };
    }
}

Monomial::~Monomial()
{
    Node<double> *toDelete;

    while (ratiosAboveOne)
    {
        toDelete = ratiosAboveOne;
        ratiosAboveOne = ratiosAboveOne->next;

        delete toDelete;
    }

    while (ratiosBelowOne)
    {
        toDelete = ratiosBelowOne;
        ratiosBelowOne = ratiosBelowOne->next;

        delete toDelete;
    }
}

Monomial &Monomial::operator*=(double r)
{
    Node<double> **cursor;

    if (r < 0)
    {
        sign *= -1;
    }

    r = fabs(r);

    if (r == 1)
    {
        return *this;
    }
    else if (r > 1)
    {
        cursor = &ratiosAboveOne;

        while ((*cursor) && (*cursor)->data < r)
            cursor = &((*cursor)->next);
    }
    else
    {
        cursor = &ratiosBelowOne;

        while ((*cursor) && (*cursor)->data > r)
            cursor = &((*cursor)->next);
    }

    //Node<double> *next = (*cursor);
    //(*cursor) = new Node<double>;
    //(*cursor)->data = r;
    //(*cursor)->next = next;

    *cursor = new Node<double> { *cursor, r };

    return *this;
}

Monomial &Monomial::operator*=(const Monomial &other)
{
    const Monomial *m;

    if (&other == this)
        m = new Monomial(*this);
    else
        m = &other;

    Node<double> **t_cursor;
    Node<double> const *m_cursor;

    t_cursor = &ratiosAboveOne;
    m_cursor = m->ratiosAboveOne;

    while (m_cursor)
    {
        if (!(*t_cursor))
        {
            *t_cursor = new Node<double> { nullptr, m_cursor->data };

            m_cursor = m_cursor->next;
        }
        else
        {
            if ((*t_cursor)->data > m_cursor->data)
            {
                *t_cursor = new Node<double> { *t_cursor, m_cursor->data };

                m_cursor = m_cursor->next;
            }

            t_cursor = &(*t_cursor)->next;
        }
    }

    t_cursor = &ratiosBelowOne;
    m_cursor = m->ratiosBelowOne;

    while (m_cursor)
    {
        if (!(*t_cursor))
        {
            *t_cursor = new Node<double> { nullptr, m_cursor->data };

            m_cursor = m_cursor->next;
        }
        else
        {
            if ((*t_cursor)->data < m_cursor->data)
            {
                *t_cursor = new Node<double> { *t_cursor, m_cursor->data };

                m_cursor = m_cursor->next;
            }

            t_cursor = &(*t_cursor)->next;
        }
    }

    sign *= m->sign;
    power += m->power;

    if (&other == this)
        delete m;

    return *this;
}

Monomial &Monomial::integrate()
{
    power += 1;
    *this *= 1.0 / power;

    return *this;
}

double Monomial::operator()(double x)
{
    Node<double> **cursor;

    if (power > 0 && x != 1)
    {
        Node<double> *powered = nullptr;
        cursor = &powered;

        while (power)
        {
            *cursor = new Node<double> { nullptr, x };

            cursor = &(*cursor)->next;
            power--;
        }

        int cmp_ratio;
        Node<double> **tail = cursor;

        if (x > 1)
        {
            cursor = &ratiosAboveOne;
            cmp_ratio = +1;
        }
        else
        {
            cursor = &ratiosBelowOne;
            cmp_ratio = -1;
        }

        while (*cursor && cmp_ratio * ((*cursor)->data - x) < 0)
            cursor = &(*cursor)->next;

        *tail = *cursor;
        *cursor = powered;
    }

    Node<double> *next;
    Node<double> *big_nums;
    Node<double> *low_nums;
    Node<double> **ra_cursor;
    Node<double> **rb_cursor;

    while (ratiosAboveOne && ratiosBelowOne)
    {
        ra_cursor = &ratiosAboveOne;
        rb_cursor = &ratiosBelowOne;
        big_nums = ratiosAboveOne;
        low_nums = ratiosBelowOne;
        ratiosAboveOne = nullptr;
        ratiosBelowOne = nullptr;

        while (big_nums && low_nums)
        {
            big_nums->data *= low_nums->data;

            next = low_nums->next;
            delete low_nums;
            low_nums = next;

            next = big_nums->next;
            big_nums->next = nullptr;

            if (big_nums->data == 1)
            {
                delete big_nums;
            }
            else if (big_nums->data > 0)
            {
                *ra_cursor = big_nums;
                ra_cursor = &(*ra_cursor)->next;
            }
            else
            {
                *rb_cursor = big_nums;
                rb_cursor = &(*rb_cursor)->next;
            }

            big_nums = next;
        }

        *ra_cursor = big_nums;
        *rb_cursor = low_nums;
    }

    double substituted = sign;

    if (ratiosAboveOne)
        cursor = &ratiosAboveOne;
    else
        cursor = &ratiosBelowOne;

    while (*cursor)
    {
        substituted *= (*cursor)->data;

        cursor = &(*cursor)->next;
    }

    return substituted;
}

Monomial::operator std::string() const
{
    std::string ratios = "";
    std::string power = std::to_string(this->power);

    Node<double> *current;

    current = ratiosAboveOne;
    while (current)
    {
        ratios += std::to_string(current->data) + "*";
        current = current->next;
    }

    current = ratiosBelowOne;
    while (current)
    {
        ratios += std::to_string(current->data) + "*";
        current = current->next;
    }

    if (sign < 0)
    {
        ratios = "-" + ratios;
    }

    return ratios + "x**" + power;
}

