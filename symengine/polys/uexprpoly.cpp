#include <symengine/visitor.h>

namespace SymEngine
{

std::string UExprDict::__str__(const std::string name) const
{
    std::ostringstream o;
    bool first = true;
    for (auto it = dict_.rbegin(); it != dict_.rend(); ++it) {
        std::string t;
        // if exponent is 0, then print only coefficient
        if (it->first == 0) {
            if (first) {
                o << it->second;
            } else {
                t = detail::poly_print(it->second);
                if (t[0] == '-') {
                    o << " - " << t.substr(1);
                } else {
                    o << " + " << t;
                }
            }
            first = false;
            continue;
        }
        // if the coefficient of a term is +1 or -1
        if (it->second == 1 or it->second == -1) {
            // in cases of -x, print -x
            // in cases of x**2 - x, print - x
            if (first) {
                if (it->second == -1)
                    o << "-";
            } else {
                if (down_cast<const Integer &>(*it->second.get_basic())
                        .as_integer_class()
                    < 0) {
                    o << " "
                      << "-"
                      << " ";
                } else {
                    o << " "
                      << "+"
                      << " ";
                }
            }
        }
        // if the coefficient of a term is 0, skip
        else if (it->second == 0)
            continue;
        // same logic is followed as above
        else {
            // in cases of -2*x, print -2*x
            // in cases of x**2 - 2*x, print - 2*x
            if (first) {
                o << detail::poly_print(it->second) << "*";
            } else {
                t = detail::poly_print(it->second);
                if (t[0] == '-') {
                    o << " - " << t.substr(1);
                } else {
                    o << " + " << t;
                }
                o << "*";
            }
        }
        o << name;
        // if exponent is not 1, print the exponent;
        if (it->first > 1) {
            o << "**" << it->first;
        } else if (it->first < 0) {
            o << "**(" << it->first << ")";
        }
        // corner cases of only first term handled successfully, switch the
        // bool
        first = false;
    }
    return o.str();
}

// const umap_int_basic get_basic() const
RCP<const Basic> UExprDict::get_basic(std::string var) const
{
    RCP<const Symbol> x = symbol(var);
    umap_basic_num dict;
    RCP<const Number> coeff = zero;
    for (const auto &it : dict_) {
        if (it.first != 0) {
            auto term = SymEngine::mul(it.second.get_basic(),
                                       SymEngine::pow(x, integer(it.first)));
            Add::coef_dict_add_term(outArg(coeff), dict, one, term);
        } else {
            Add::coef_dict_add_term(outArg(coeff), dict, one,
                                    it.second.get_basic());
        }
    }
    return Add::from_dict(coeff, std::move(dict));
}

UExprPoly::UExprPoly(const RCP<const Basic> &var, UExprDict &&dict)
    : USymEnginePoly(var, std::move(dict))
{
    SYMENGINE_ASSIGN_TYPEID()
    SYMENGINE_ASSERT(is_canonical(get_poly()))
}

hash_t UExprPoly::__hash__() const
{
    hash_t seed = UEXPRPOLY;

    seed += get_var()->hash();
    for (const auto &it : get_poly().dict_) {
        hash_t temp = UEXPRPOLY;
        hash_combine<unsigned int>(temp, it.first);
        hash_combine<Basic>(temp, *(it.second.get_basic()));
        seed += temp;
    }
    return seed;
}

Expression UExprPoly::max_coef() const
{
    Expression curr = get_poly().get_dict().begin()->second;
    for (const auto &it : get_poly().get_dict())
        if (curr.get_basic()->__cmp__(*it.second.get_basic()))
            curr = it.second;
    return curr;
}

Expression UExprPoly::eval(const Expression &x) const
{
    Expression ans = 0;
    for (const auto &p : get_poly().get_dict()) {
        Expression temp;
        temp = pow_ex(x, Expression(p.first));
        ans += p.second * temp;
    }
    return ans;
}

bool UExprPoly::is_zero() const
{
    return get_poly().empty();
}

bool UExprPoly::is_one() const
{
    return get_poly().size() == 1 and get_poly().get_dict().begin()->second == 1
           and get_poly().get_dict().begin()->first == 0;
}

bool UExprPoly::is_minus_one() const
{
    return get_poly().size() == 1
           and get_poly().get_dict().begin()->second == -1
           and get_poly().get_dict().begin()->first == 0;
}

bool UExprPoly::is_integer() const
{
    if (get_poly().empty())
        return true;
    return get_poly().size() == 1 and get_poly().get_dict().begin()->first == 0;
}

bool UExprPoly::is_symbol() const
{
    return get_poly().size() == 1 and get_poly().get_dict().begin()->first == 1
           and get_poly().get_dict().begin()->second == 1;
}

bool UExprPoly::is_mul() const
{
    return get_poly().size() == 1 and get_poly().get_dict().begin()->first != 0
           and get_poly().get_dict().begin()->second != 1
           and get_poly().get_dict().begin()->second != 0;
}

bool UExprPoly::is_pow() const
{
    return get_poly().size() == 1 and get_poly().get_dict().begin()->second == 1
           and get_poly().get_dict().begin()->first != 1
           and get_poly().get_dict().begin()->first != 0;
}

} // SymEngine
