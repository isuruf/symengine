#include <symengine/integer.h>
#include <symengine/rational.h>

namespace SymEngine {

std::size_t Integer::__hash__() const
{
    std::hash<long long int> hash_fn;
    // only the least significant bits that fit into "signed long int" are
    // hashed:
    return hash_fn(((long long int)get_ui(this->i)) * sign(this->i));
}

bool Integer::__eq__(const Basic &o) const
{
    if (is_a<Integer>(o)) {
        const Integer &s = static_cast<const Integer &>(o);
        return this->i == s.i;
    }
    return false;
}

int Integer::compare(const Basic &o) const
{
    SYMENGINE_ASSERT(is_a<Integer>(o))
    const Integer &s = static_cast<const Integer &>(o);
    if (i == s.i) return 0;
    return i < s.i ? -1 : 1;
}

signed long int Integer::as_int() const
{
    // get_si() returns "signed long int", so that's what we return from
    // "as_int()" and we leave it to the user to do any possible further integer
    // conversions.
    if (not (fits_slong_p(this->i))) {
        throw std::runtime_error("as_int: Integer larger than int");
    }
    return get_si(this->i);
}

RCP<const Number> Integer::divint(const Integer &other) const {
    if (other.i == 0)
        throw std::runtime_error("Rational: Division by zero.");
    rational_class q(this->i, other.i);

    // This is potentially slow, but has to be done, since q might not
    // be in canonical form.
    //q.canonicalize();

    return Rational::from_mpq(std::move(q));
}

RCP<const Number> Integer::rdiv(const Number &other) const
{
    if (is_a<Integer>(other)) {
        if (this->i == 0) {
            throw std::runtime_error("Rational: Division by zero.");
        }
        rational_class q((static_cast<const Integer&>(other)).i, this->i);

        // This is potentially slow, but has to be done, since q might not
        // be in canonical form.
        canonicalize(q);

        return Rational::from_mpq(std::move(q));
    } else {
        throw std::runtime_error("Not implemented.");
    }
};

RCP<const Number> Integer::pow_negint(const Integer &other) const {
    RCP<const Number> tmp = powint(*other.neg());
    if (is_a<Integer>(*tmp)) {
        rational_class q(sign(static_cast<const Integer &>(*tmp).i), abs(static_cast<const Integer &>(*tmp).i));
        return make_rcp<const Rational>(std::move(q));
    } else {
        throw std::runtime_error("powint returned non-integer");
    }
}

RCP<const Integer> isqrt(const Integer &n)
{
    integer_class m;
    mpz_sqrt(get_mpz_t(m), get_mpz_t(n.as_mpz()));
    return integer(std::move(m));
}

RCP<const Integer> iabs(const Integer &n)
{
    integer_class m;
    mpz_abs(get_mpz_t(m), get_mpz_t(n.as_mpz()));
    return integer(std::move(m));
}

int i_nth_root(const Ptr<RCP<const Integer>> &r, const Integer &a,
        unsigned long int n)
{
    if (n == 0)
        throw std::runtime_error("i_nth_root: Can not find Zeroth root");

    int ret_val;
    integer_class t;

    ret_val = mpz_root(get_mpz_t(t), get_mpz_t(a.as_mpz()), n);
    *r = integer(std::move(t));

    return ret_val;
}

int perfect_square(const Integer &n)
{
    return mpz_perfect_square_p(get_mpz_t(n.as_mpz()));
}

int perfect_power(const Integer &n)
{
    return mpz_perfect_power_p(get_mpz_t(n.as_mpz()));
}

} // SymEngine
