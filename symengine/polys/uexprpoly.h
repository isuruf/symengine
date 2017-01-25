/**
 *  \file polynomial.h
 *  Class for sparse Polynomial: UExprPoly
 **/
#ifndef SYMENGINE_UEXPRPOLY_H
#define SYMENGINE_UEXPRPOLY_H

#include <symengine/expression.h>
#include <symengine/monomials.h>
#include <symengine/polys/usymenginepoly.h>

namespace SymEngine
{

class UExprDict : public ODictWrapper<int, Expression, UExprDict>
{

public:
    UExprDict() SYMENGINE_NOEXCEPT
    {
    }
    ~UExprDict() SYMENGINE_NOEXCEPT
    {
    }
    UExprDict(UExprDict &&other) SYMENGINE_NOEXCEPT
        : ODictWrapper(std::move(other))
    {
    }
    UExprDict(const int &i) : ODictWrapper(i)
    {
    }
    UExprDict(const map_int_Expr &p) : ODictWrapper(p)
    {
    }
    UExprDict(const Expression &expr) : ODictWrapper(expr)
    {
    }

    UExprDict(const std::string &s) : ODictWrapper(s)
    {
    }

    UExprDict(const UExprDict &) = default;
    UExprDict &operator=(const UExprDict &) = default;

    friend std::ostream &operator<<(std::ostream &os, const UExprDict &expr)
    {
        os << expr.dict_;
        return os;
    }

    friend UExprDict operator/(const UExprDict &a, const Expression &b)
    {
        return a * (1 / b);
    }

    UExprDict &operator/=(const Expression &other)
    {
        *this *= (1 / other);
        return *this;
    }

    std::string __str__(const std::string name) const;

    // const umap_int_basic get_basic() const
    RCP<const Basic> get_basic(std::string var) const;

    int compare(const UExprDict &other) const
    {
        return unified_compare(dict_, other.dict_);
    }

    Expression find_cf(int deg) const
    {
        if (dict_.find(deg) != dict_.end())
            return dict_.at(deg);
        else
            return Expression(0);
    }
}; // UExprDict

class UExprPoly : public USymEnginePoly<UExprDict, UExprPolyBase, UExprPoly>
{
public:
    IMPLEMENT_TYPEID(UEXPRPOLY)
    //! Constructor of UExprPoly class
    UExprPoly(const RCP<const Basic> &var, UExprDict &&dict);

    hash_t __hash__() const;

    typedef Expression coef_type;

    Expression max_coef() const;
    //! Evaluates the UExprPoly at value x
    Expression eval(const Expression &x) const;

    //! \return `true` if `0`
    bool is_zero() const;
    //! \return `true` if `1`
    bool is_one() const;
    //! \return `true` if `-1`
    bool is_minus_one() const;
    //! \return `true` if integer
    bool is_integer() const;
    //! \return `true` if symbol
    bool is_symbol() const;
    //! \return `true` if mul
    bool is_mul() const;
    //! \return `true` if pow
    bool is_pow() const;

}; // UExprPoly

inline RCP<const UExprPoly> uexpr_poly(RCP<const Basic> i, UExprDict &&dict)
{
    return UExprPoly::from_container(i, std::move(dict));
}

inline RCP<const UExprPoly> uexpr_poly(RCP<const Basic> i, map_int_Expr &&dict)
{
    return UExprPoly::from_dict(i, std::move(dict));
}

} // SymEngine

#endif
