#include <symengine/polys/uintpoly.h>

namespace SymEngine
{

UIntPoly::UIntPoly(const RCP<const Basic> &var, UIntDict &&dict)
    : USymEnginePoly(var, std::move(dict))
{
    SYMENGINE_ASSIGN_TYPEID()
    SYMENGINE_ASSERT(is_canonical(get_poly()))
}

hash_t UIntPoly::__hash__() const
{
    hash_t seed = UINTPOLY;

    seed += get_var()->hash();
    for (const auto &it : get_poly().dict_) {
        hash_t temp = UINTPOLY;
        hash_combine<unsigned int>(temp, it.first);
        hash_combine<long long int>(temp, mp_get_si(it.second));
        seed += temp;
    }
    return seed;
}

UIntDict UIntDict::mul(const UIntDict &a, const UIntDict &b)
{
    int mul = 1;

    unsigned int N = bit_length(std::min(a.degree() + 1, b.degree() + 1))
                     + bit_length(a.max_abs_coef())
                     + bit_length(b.max_abs_coef());

    integer_class full = integer_class(1), temp, res;
    full <<= N;
    integer_class thresh = full / 2;
    integer_class mask = full - 1;
    integer_class s_val = a.eval_bit(N) * b.eval_bit(N);
    if (s_val < 0)
        mul = -1;
    s_val = mp_abs(s_val);

    unsigned int deg = 0, carry = 0;
    UIntDict r;

    while (s_val != 0 or carry != 0) {
        mp_and(temp, s_val, mask);
        if (temp < thresh) {
            res = mul * (temp + carry);
            if (res != 0)
                r.dict_[deg] = res;
            carry = 0;
        } else {
            res = mul * (temp - full + carry);
            if (res != 0)
                r.dict_[deg] = res;
            carry = 1;
        }
        s_val >>= N;
        deg++;
    }
    return r;
}

int UIntDict::compare(const UIntDict &other) const
{
    if (dict_.size() != other.dict_.size())
        return (dict_.size() < other.dict_.size()) ? -1 : 1;
    return unified_compare(dict_, other.dict_);
}

integer_class UIntDict::max_abs_coef() const
{
    integer_class curr(mp_abs(dict_.begin()->second));
    for (const auto &it : dict_) {
        if (mp_abs(it.second) > curr)
            curr = mp_abs(it.second);
    }
    return curr;
}

integer_class UIntDict::eval_bit(const unsigned int &x) const
{
    unsigned int last_deg = dict_.rbegin()->first;
    integer_class result(0);

    for (auto it = dict_.rbegin(); it != dict_.rend(); ++it) {
        result <<= x * (last_deg - (*it).first);
        result += (*it).second;
        last_deg = (*it).first;
    }
    result <<= x * last_deg;

    return result;
}

bool divides_upoly(const UIntPoly &a, const UIntPoly &b,
                   const Ptr<RCP<const UIntPoly>> &out)
{
    if (!(a.get_var()->__eq__(*b.get_var())))
        throw SymEngineException("Error: variables must agree.");

    auto a_poly = a.get_poly();
    auto b_poly = b.get_poly();
    if (a_poly.size() == 0)
        return false;

    map_uint_mpz res;
    UIntDict tmp;
    integer_class q, r;
    unsigned int a_deg, b_deg;

    while (b_poly.size() >= a_poly.size()) {
        a_deg = a_poly.degree();
        b_deg = b_poly.degree();

        mp_tdiv_qr(q, r, b_poly.get_lc(), a_poly.get_lc());
        if (r != 0)
            return false;

        res[b_deg - a_deg] = q;
        UIntDict tmp = UIntDict({{b_deg - a_deg, q}});
        b_poly -= (a_poly * tmp);
    }

    if (b_poly.empty()) {
        *out = UIntPoly::from_dict(a.get_var(), std::move(res));
        return true;
    } else {
        return false;
    }
}

} // SymEngine
