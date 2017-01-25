/**
 *  \file uintpoly.h
 *  Class for sparse Polynomial: UIntPoly
 **/
#ifndef SYMENGINE_UINTPOLY_H
#define SYMENGINE_UINTPOLY_H

#include <symengine/polys/usymenginepoly.h>

namespace SymEngine
{
// Calculates bit length of number, used in UIntDict*= only
template <typename T>
unsigned int bit_length(T t)
{
    unsigned int count = 0;
    while (t > 0) {
        count++;
        t = t >> 1;
    }
    return count;
}

class UIntDict : public ODictWrapper<unsigned int, integer_class, UIntDict>
{

public:
    UIntDict() SYMENGINE_NOEXCEPT
    {
    }
    ~UIntDict() SYMENGINE_NOEXCEPT
    {
    }
    UIntDict(UIntDict &&other) SYMENGINE_NOEXCEPT
        : ODictWrapper(std::move(other))
    {
    }
    UIntDict(const int &i) : ODictWrapper(i)
    {
    }
    UIntDict(const map_uint_mpz &p) : ODictWrapper(p)
    {
    }
    UIntDict(const integer_class &i) : ODictWrapper(i)
    {
    }

    UIntDict(const UIntDict &) = default;
    UIntDict &operator=(const UIntDict &) = default;

    //! Evaluates the dict_ at value 2**x
    integer_class eval_bit(const unsigned int &x) const;
    static UIntDict mul(const UIntDict &a, const UIntDict &b);
    int compare(const UIntDict &other) const;
    integer_class max_abs_coef() const;

}; // UIntDict

class UIntPoly : public USymEnginePoly<UIntDict, UIntPolyBase, UIntPoly>
{
public:
    IMPLEMENT_TYPEID(UINTPOLY)
    //! Constructor of UIntPoly class
    UIntPoly(const RCP<const Basic> &var, UIntDict &&dict);

    //! \return size of the hash
    hash_t __hash__() const;
}; // UIntPoly

// true & sets `out` to b/a if a exactly divides b, otherwise false & undefined
bool divides_upoly(const UIntPoly &a, const UIntPoly &b,
                   const Ptr<RCP<const UIntPoly>> &res);

} // SymEngine

#endif
