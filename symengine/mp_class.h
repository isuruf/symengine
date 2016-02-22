#ifndef SYMENGINE_INTEGER_CLASS_H
#define SYMENGINE_INTEGER_CLASS_H

#include <symengine/symengine_rcp.h>

#if SYMENGINE_INTEGER_CLASS == SYMENGINE_PIRANHA
#include <piranha/mp_integer.hpp>
#include <piranha/mp_rational.hpp>

#elif SYMENGINE_INTEGER_CLASS == SYMENGINE_FLINT
#include <flint/fmpzxx.h>
#include <flint/fmpqxx.h>

#else
#define __GMPXX_USE_CXX11 1
#include <gmpxx.h>
#endif

#include <gmp.h>

namespace SymEngine {


#if SYMENGINE_INTEGER_CLASS == SYMENGINE_PIRANHA
typedef piranha::integer integer_class;
typedef piranha::rational rational_class;
#elif SYMENGINE_INTEGER_CLASS == SYMENGINE_FLINT
class fmpz_wrapper;
class fmpq_wrapper;
typedef fmpz_wrapper integer_class;
typedef fmpq_wrapper rational_class;
}
std::ostream &operator<<(std::ostream &os, const SymEngine::fmpq_wrapper &f);
std::ostream &operator<<(std::ostream &os, const SymEngine::fmpz_wrapper &f);
namespace SymEngine {
#else
typedef mpz_class integer_class;
typedef mpq_class rational_class;
#endif

#if SYMENGINE_INTEGER_CLASS == SYMENGINE_GMPXX
// Helper functions for mpz_class
inline double get_d(const mpz_class &i) {
    return i.get_d();
}

inline bool fits_ulong_p(const mpz_class &i) {
    return i.fits_ulong_p();
}

inline bool fits_slong_p(const mpz_class &i) {
    return i.fits_slong_p();
}

inline unsigned long get_ui(const mpz_class &i) {
    return i.get_ui();
}

inline long get_si(const mpz_class &i) {
    return i.get_si();
}

inline int sign(const mpz_class &i) {
    return sgn(i);
}

inline mpz_srcptr get_mpz_t(const mpz_class &i) {
    return i.get_mpz_t();
}

inline mpz_ptr get_mpz_t(mpz_class &i) {
    return i.get_mpz_t();
}

// Helper functions for mpq_class
inline const mpz_class& get_den(const mpq_class &i) {
    return i.get_den();
}

inline const mpz_class& get_num(const mpq_class &i) {
    return i.get_num();
}

inline mpz_class& get_den(mpq_class &i) {
    return i.get_den();
}

inline mpz_class& get_num(mpq_class &i) {
    return i.get_num();
}

inline mpq_srcptr get_mpq_t(const mpq_class &i) {
    return i.get_mpq_t();
}

inline void canonicalize(mpq_class &i) {
    i.canonicalize();
}

inline double get_d(const mpq_class &i) {
    return i.get_d();
}

inline int sign(const mpq_class &i) {
    return sgn(i);
}

#elif SYMENGINE_INTEGER_CLASS == SYMENGINE_PIRANHA
// Helper functions for piranha::integer
inline piranha::integer abs(const piranha::integer &i) {
    return i.abs();
}

inline piranha::integer sqrt(const piranha::integer &i) {
    return i.sqrt();
}

inline mpz_ptr get_mpz_t(piranha::integer &i) {
    return i._get_mpz_ptr();
}

inline auto get_mpz_t(const piranha::integer &i) -> decltype(i.get_mpz_view())  {
    return i.get_mpz_view();
}

inline int sign(const piranha::integer &i) {
    return i.sign();
}

inline long get_si(const piranha::integer &i) {
    return mpz_get_si(i.get_mpz_view());
}

inline unsigned long get_ui(const piranha::integer &i) {
    return mpz_get_ui(i.get_mpz_view());
}

inline double get_d(const piranha::integer &i) {
    return mpz_get_d(i.get_mpz_view());
}

inline bool fits_ulong_p(const piranha::integer &i) {
    return mpz_fits_ulong_p(i.get_mpz_view()) != 0;
}

inline bool fits_slong_p(const piranha::integer &i) {
    return mpz_fits_slong_p(i.get_mpz_view()) != 0;
}

// Helper functions for piranha::rational

inline piranha::rational abs(const piranha::rational &i) {
    return i.abs();
}

inline const piranha::integer& get_den(const piranha::rational &i) {
    return i.den();
}

inline const piranha::integer& get_num(const piranha::rational &i) {
    return i.num();
}

inline piranha::integer& get_den(piranha::rational &i) {
    return i._den();
}

inline piranha::integer& get_num(piranha::rational &i) {
    return i._num();
}


inline void canonicalize(piranha::rational &i) {
    i.canonicalise();
}

inline double get_d(const piranha::rational &i) {
    return mpq_get_d(i.get_mpq_view().get());
}

inline auto get_mpq_t(const piranha::rational &i) -> decltype(i.get_mpq_view()) {
    return i.get_mpq_view();
}

inline int sign(const piranha::rational &i) {
    return i.num().sign();
}
#elif SYMENGINE_INTEGER_CLASS == SYMENGINE_FLINT


class fmpz_wrapper {
private:
    fmpz_t mp;
public:
    fmpz_wrapper(const unsigned long i) {
        fmpz_init(mp);
        fmpz_set_ui(mp, i);
    }
    fmpz_wrapper(const long i) {
        fmpz_init(mp);
        fmpz_set_si(mp, i);
    }
    fmpz_wrapper(const unsigned i) {
        fmpz_init(mp);
        fmpz_set_ui(mp, i);
    }
    fmpz_wrapper(const int i) {
        fmpz_init(mp);
        fmpz_set_si(mp, i);
    }
    fmpz_wrapper() {
        fmpz_init(mp);
    }
    fmpz_wrapper(const mpz_t m) {
        fmpz_init(mp);
        fmpz_set_mpz(mp, m);
    }
    fmpz_wrapper(const fmpz_t m) {
        fmpz_init(mp);
        fmpz_set(mp, m);
    }
    fmpz_wrapper(const std::string& s, unsigned base = 10) {
        fmpz_init(mp);
        fmpz_set_str(mp, s.c_str(), base);
    }
    fmpz_wrapper(const fmpz_wrapper& other) {
        fmpz_init(mp);
        fmpz_set(mp, other.get_fmpz_t());
    }
    fmpz_wrapper(fmpz_wrapper&& other) {
        fmpz_init(mp);
        fmpz_swap(mp, other.get_fmpz_t());
    }
    fmpz_wrapper& operator=(const fmpz_wrapper& other) {
        fmpz_set(mp, other.get_fmpz_t());
        return *this;
    }
    fmpz_wrapper& operator=(fmpz_wrapper&& other) {
        fmpz_swap(mp, other.get_fmpz_t());
        return *this;
    }
    ~fmpz_wrapper() {
        fmpz_clear(mp);
    }
    fmpz* get_fmpz_t() { return mp; }
    const fmpz* get_fmpz_t() const { return mp; }
    friend fmpz_wrapper operator+(const fmpz_wrapper &a, const fmpz_wrapper &b)
    {
        fmpz_wrapper res;
        fmpz_add(res.get_fmpz_t(), a.get_fmpz_t(), b.get_fmpz_t());
        return res;
    }
    fmpz_wrapper operator+=(const fmpz_wrapper &a)
    {
        fmpz_add(mp, mp, a.get_fmpz_t());
        return *this;
    }
    friend fmpz_wrapper operator-(const fmpz_wrapper &a, const fmpz_wrapper &b)
    {
        fmpz_wrapper res;
        fmpz_sub(res.get_fmpz_t(), a.get_fmpz_t(), b.get_fmpz_t());
        return res;
    }
    fmpz_wrapper operator-=(const fmpz_wrapper &a)
    {
        fmpz_sub(mp, mp, a.get_fmpz_t());
        return *this;
    }
    fmpz_wrapper operator-() const
    {
        fmpz_wrapper res;
        fmpz_neg(res.get_fmpz_t(), mp);
        return res;
    }
    friend fmpz_wrapper operator*(const fmpz_wrapper &a, const fmpz_wrapper &b)
    {
        fmpz_wrapper res;
        fmpz_mul(res.get_fmpz_t(), a.get_fmpz_t(), b.get_fmpz_t());
        return res;
    }
    fmpz_wrapper operator*=(const fmpz_wrapper &a)
    {
        fmpz_mul(mp, mp, a.get_fmpz_t());
        return *this;
    }
    friend fmpz_wrapper operator/(const fmpz_wrapper &a, const fmpz_wrapper &b)
    {
        fmpz_wrapper res;
        fmpz_tdiv_q(res.get_fmpz_t(), a.get_fmpz_t(), b.get_fmpz_t());
        return res;
    }
    fmpz_wrapper operator/=(const fmpz_wrapper &a)
    {
        fmpz_tdiv_q(mp, mp, a.get_fmpz_t());
        return *this;
    }
    friend fmpz_wrapper operator%(const fmpz_wrapper &a, const fmpz_wrapper &b)
    {
        fmpz_wrapper res, tmp;
        fmpz_tdiv_qr(tmp.get_fmpz_t(), res.get_fmpz_t(), a.get_fmpz_t(), b.get_fmpz_t());
        return res;
    }
    fmpz_wrapper operator%=(const fmpz_wrapper &a)
    {
        fmpz_wrapper tmp;
        fmpz_tdiv_qr(tmp.get_fmpz_t(), mp, mp, a.get_fmpz_t());
        return *this;
    }
    fmpz_wrapper operator++()
    {
        fmpz_add_ui(mp, mp, 1);
        return *this;
    }
    fmpz_wrapper operator++(int)
    {
        fmpz_wrapper orig = *this;
        ++(*this);
        return orig;
    }
    fmpz_wrapper operator--()
    {
        fmpz_sub_ui(mp, mp, 1);
        return *this;
    }
    fmpz_wrapper operator--(int)
    {
        fmpz_wrapper orig = *this;
        --(*this);
        return orig;
    }
    friend bool operator==(const fmpz_wrapper &a, const fmpz_wrapper &b)
    {
        return fmpz_equal(a.get_fmpz_t(), b.get_fmpz_t()) == 1;
    }
    friend bool operator!=(const fmpz_wrapper &a, const fmpz_wrapper &b)
    {
        return fmpz_equal(a.get_fmpz_t(), b.get_fmpz_t()) != 1;
    }
    friend bool operator<(const fmpz_wrapper &a, const fmpz_wrapper &b)
    {
        return fmpz_cmp(a.get_fmpz_t(), b.get_fmpz_t()) < 0;
    }
    friend bool operator<=(const fmpz_wrapper &a, const fmpz_wrapper &b)
    {
        return fmpz_cmp(a.get_fmpz_t(), b.get_fmpz_t()) <= 0;
    }
    friend bool operator>(const fmpz_wrapper &a, const fmpz_wrapper &b)
    {
        return fmpz_cmp(a.get_fmpz_t(), b.get_fmpz_t()) > 0;
    }
    friend bool operator>=(const fmpz_wrapper &a, const fmpz_wrapper &b)
    {
        return fmpz_cmp(a.get_fmpz_t(), b.get_fmpz_t()) >= 0;
    }
    fmpz_wrapper operator<<=(unsigned long u)
    {
        fmpz_mul_2exp(mp, mp, u);
        return *this;
    }
    fmpz_wrapper operator<<(unsigned long u) const
    {
        fmpz_wrapper res;
        fmpz_mul_2exp(res.get_fmpz_t(), mp, u);
        return res;
    }
    fmpz_wrapper operator>>=(unsigned long u)
    {
        fmpz_tdiv_q_2exp(mp, mp, u);
        return *this;
    }
    fmpz_wrapper operator>>(unsigned long u) const
    {
        fmpz_wrapper res;
        fmpz_tdiv_q_2exp(res.get_fmpz_t(), mp, u);
        return res;
    }
};

class mpz_view_flint {

public:
    mpz_view_flint(const fmpz_wrapper &i) {
        if (!COEFF_IS_MPZ(*i.get_fmpz_t()))
        {
            mpz_init_set_si(m, *i.get_fmpz_t());
        }
        else
        {
            ptr = COEFF_TO_PTR(*i.get_fmpz_t());
        }
    }
    operator mpz_srcptr() const
	{
        if (ptr == nullptr) return m;
        return ptr;
	}
    ~mpz_view_flint() {
        if (ptr == nullptr) mpz_clear(m);
    }

private:
    mpz_srcptr ptr = nullptr;
    mpz_t m;
};

inline mpz_view_flint get_mpz_t(const fmpz_wrapper &i) {
    return mpz_view_flint(i);
}

inline mpz_ptr get_mpz_t(fmpz_wrapper &i) {
    return _fmpz_promote_val(i.get_fmpz_t());
}

inline int sign(const fmpz_wrapper &i) {
    return fmpz_sgn(i.get_fmpz_t());
}

inline long get_si(const fmpz_wrapper &i) {
    return fmpz_get_si(i.get_fmpz_t());
}

inline unsigned long get_ui(const fmpz_wrapper &i) {
    return fmpz_get_ui(i.get_fmpz_t());
}


inline bool fits_slong_p(const fmpz_wrapper &i) {
    return fmpz_fits_si(i.get_fmpz_t());
}


inline bool fits_ulong_p(const fmpz_wrapper &i) {
    return fmpz_sgn(i.get_fmpz_t()) >= 0 && fmpz_abs_fits_ui(i.get_fmpz_t());
}


inline double get_d(const fmpz_wrapper &i) {
    return fmpz_get_d(i.get_fmpz_t());
}

inline fmpz_wrapper abs(const fmpz_wrapper &i) {
    fmpz_wrapper res;
    fmpz_abs(res.get_fmpz_t(), i.get_fmpz_t());
    return res;
}

inline fmpz_wrapper sqrt(const fmpz_wrapper &i) {
    fmpz_wrapper res;
    fmpz_sqrt(res.get_fmpz_t(), i.get_fmpz_t());
    return res;
}

class fmpq_wrapper {
private:
    fmpq_t mp;
public:
    fmpq* get_fmpq_t() { return mp; }
    const fmpq* get_fmpq_t() const { return mp; }
    fmpq_wrapper() {
        fmpq_init(mp);
    }
    fmpq_wrapper(const mpz_t m) {
        fmpq_init(mp);
        fmpz_set_mpz(fmpq_numref(mp), m);
    }
    fmpq_wrapper(const fmpz_t m) {
        fmpq_init(mp);
        fmpz_set(fmpq_numref(mp), m);
    }
    fmpq_wrapper(const mpq_t m) {
        fmpq_init(mp);
        fmpq_set_mpq(mp, m);
    }
    fmpq_wrapper(const fmpq_t m) {
        fmpq_init(mp);
        fmpq_set(mp, m);
    }
    fmpq_wrapper(const unsigned long i) {
        fmpq_init(mp);
        fmpz_set_ui(fmpq_numref(mp), i);
    }
    fmpq_wrapper(const long i) {
        fmpq_init(mp);
        fmpz_set_si(fmpq_numref(mp), i);
    }
    fmpq_wrapper(const unsigned i) {
        fmpq_init(mp);
        fmpz_set_ui(fmpq_numref(mp), i);
    }
    fmpq_wrapper(const int i) {
        fmpq_init(mp);
        fmpz_set_si(fmpq_numref(mp), i);
    }
    fmpq_wrapper(const fmpz_wrapper& n, const fmpz_wrapper& d = 1) {
        fmpq_init(mp);
        fmpz_set(fmpq_numref(mp), n.get_fmpz_t());
        fmpz_set(fmpq_denref(mp), d.get_fmpz_t());
    }
    fmpq_wrapper(const fmpq_wrapper& other) {
        fmpq_init(mp);
        fmpq_set(mp, other.get_fmpq_t());
    }
    fmpq_wrapper(fmpq_wrapper&& other) {
        fmpq_init(mp);
        fmpq_swap(mp, other.get_fmpq_t());
    }
    fmpq_wrapper& operator=(const fmpq_wrapper& other) {
        fmpq_set(mp, other.get_fmpq_t());
        return *this;
    }
    fmpq_wrapper& operator=(fmpq_wrapper&& other) {
        fmpq_swap(mp, other.get_fmpq_t());
        return *this;
    }
    ~fmpq_wrapper() {
        fmpq_clear(mp);
    }
    const fmpz_wrapper& get_den() const {
        return reinterpret_cast<const fmpz_wrapper&>(*fmpq_denref(mp));
    }
    const fmpz_wrapper& get_num() const {
        return reinterpret_cast<const fmpz_wrapper&>(*fmpq_numref(mp));
    }
    fmpz_wrapper& get_den() {
        return reinterpret_cast<fmpz_wrapper&>(*fmpq_denref(mp));
    }
    fmpz_wrapper& get_num() {
        return reinterpret_cast<fmpz_wrapper&>(*fmpq_numref(mp));
    }
    friend fmpq_wrapper operator+(const fmpq_wrapper &a, const fmpq_wrapper &b)
    {
        fmpq_wrapper res;
        fmpq_add(res.get_fmpq_t(), a.get_fmpq_t(), b.get_fmpq_t());
        return res;
    }
    fmpq_wrapper operator+=(const fmpq_wrapper &a)
    {
        fmpq_add(mp, mp, a.get_fmpq_t());
        return *this;
    }
    friend fmpq_wrapper operator-(const fmpq_wrapper &a, const fmpq_wrapper &b)
    {
        fmpq_wrapper res;
        fmpq_sub(res.get_fmpq_t(), a.get_fmpq_t(), b.get_fmpq_t());
        return res;
    }
    fmpq_wrapper operator-=(const fmpq_wrapper &a)
    {
        fmpq_sub(mp, mp, a.get_fmpq_t());
        return *this;
    }
    fmpq_wrapper operator-() const
    {
        fmpq_wrapper res;
        fmpq_neg(res.get_fmpq_t(), mp);
        return res;
    }
    friend fmpq_wrapper operator*(const fmpq_wrapper &a, const fmpq_wrapper &b)
    {
        fmpq_wrapper res;
        fmpq_mul(res.get_fmpq_t(), a.get_fmpq_t(), b.get_fmpq_t());
        return res;
    }
    fmpq_wrapper operator*=(const fmpq_wrapper &a)
    {
        fmpq_mul(mp, mp, a.get_fmpq_t());
        return *this;
    }
    friend fmpq_wrapper operator/(const fmpq_wrapper &a, const fmpq_wrapper &b)
    {
        fmpq_wrapper res;
        fmpq_div(res.get_fmpq_t(), a.get_fmpq_t(), b.get_fmpq_t());
        return res;
    }
    fmpq_wrapper operator/=(const fmpq_wrapper &a)
    {
        fmpq_div(mp, mp, a.get_fmpq_t());
        return *this;
    }
    bool operator==(const fmpq_wrapper &other) const
    {
        return fmpq_equal(mp, other.get_fmpq_t());
    }
    bool operator!=(const fmpq_wrapper &other) const
    {
        return not (*this == other);
    }
    bool operator<(const fmpq_wrapper &other) const
    {
        return fmpq_cmp(mp, other.get_fmpq_t()) < 0;
    }
    bool operator<=(const fmpq_wrapper &other) const
    {
        return fmpq_cmp(mp, other.get_fmpq_t()) <= 0;
    }
    bool operator>(const fmpq_wrapper &other) const
    {
        return fmpq_cmp(mp, other.get_fmpq_t()) > 0;
    }
    bool operator>=(const fmpq_wrapper &other) const
    {
        return fmpq_cmp(mp, other.get_fmpq_t()) >= 0;
    }
};

inline const fmpz_wrapper& get_den(const fmpq_wrapper &i) {
    return i.get_den();
}

inline const fmpz_wrapper& get_num(const fmpq_wrapper &i) {
    return i.get_num();
}

inline fmpz_wrapper& get_den(fmpq_wrapper &i) {
    return i.get_den();
}

inline fmpz_wrapper& get_num(fmpq_wrapper &i) {
    return i.get_num();
}

inline mpq_srcptr get_mpq_t(const fmpq_wrapper &i) {
    return nullptr;
}

inline void canonicalize(fmpq_wrapper &i) {
    fmpq_canonicalise(i.get_fmpq_t());
}

inline double get_d(const fmpq_wrapper &i) {
    return get_d(i.get_num())/get_d(i.get_den());
}

inline int sign(const fmpq_wrapper &i) {
    return fmpq_sgn(i.get_fmpq_t());
}

inline fmpq_wrapper abs(const fmpq_wrapper &i) {
    fmpq_wrapper res;
    fmpq_abs(res.get_fmpq_t(), i.get_fmpq_t());
    return res;
}

#endif

} // SymEngine namespace

#endif //SYMENGINE_INTEGER_CLASS_H
