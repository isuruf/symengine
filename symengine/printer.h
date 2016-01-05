#ifndef SYMENGINE_PRINTER_H
#define SYMENGINE_PRINTER_H

#include <symengine/visitor.h>

namespace SymEngine {

enum class PrecedenceEnum {
    Add, Mul, Pow, Atom
};

class Precedence : public BaseVisitor<Precedence> {
public:
    PrecedenceEnum precedence;

    void bvisit(const Add &x) {
        precedence = PrecedenceEnum::Add;
    }

    void bvisit(const Mul &x) {
        precedence = PrecedenceEnum::Mul;
    }

    void bvisit(const Pow &x) {
        precedence = PrecedenceEnum::Pow;
    }

    void bvisit(const UnivariatePolynomial &x) {
        if (x.dict_.size() == 1) {
            auto it = x.dict_.begin();
            if (it->second == 0) {
                precedence = PrecedenceEnum::Atom;
            } else if (it->second == 1) {
                if (it->first == 0 or it->first == 1) {
                    precedence = PrecedenceEnum::Atom;
                } else {
                    precedence = PrecedenceEnum::Pow;
                }
            } else {
                if (it->first == 0 and it->second >= 0) {
                    precedence = PrecedenceEnum::Atom;
                } else {
                    precedence = PrecedenceEnum::Mul;
                }
            }
        } else {
            precedence = PrecedenceEnum::Add;
        }
    }

    void bvisit(const Rational &x) {
        precedence = PrecedenceEnum::Add;
    }

    void bvisit(const Complex &x) {
        if(x.is_re_zero()) {
            if(x.imaginary_ == 1) {
                precedence = PrecedenceEnum::Atom;
            } else {
                precedence = PrecedenceEnum::Mul;
            }
        } else {
            precedence = PrecedenceEnum::Add;
        }
    }

    void bvisit(const Integer &x) {
        if (x.is_negative()) {
            precedence = PrecedenceEnum::Mul;
        } else {
            precedence = PrecedenceEnum::Atom;
        }
    }

    void bvisit(const RealDouble &x) {
        if (x.is_negative()) {
            precedence = PrecedenceEnum::Mul;
        } else {
            precedence = PrecedenceEnum::Atom;
        }
    }

    void bvisit(const ComplexDouble &x) {
        precedence = PrecedenceEnum::Add;
    }
#ifdef HAVE_SYMENGINE_MPFR
    void bvisit(const RealMPFR &x) {
        if (x.is_negative()) {
            precedence = PrecedenceEnum::Mul;
        } else {
            precedence = PrecedenceEnum::Atom;
        }
    }
#endif
#ifdef HAVE_SYMENGINE_MPC
    void bvisit(const ComplexMPC &x) {
        precedence = PrecedenceEnum::Add;
    }
#endif

    void bvisit(const Basic &x) {
        precedence = PrecedenceEnum::Atom;
    }

    PrecedenceEnum getPrecedence(const RCP<const Basic> &x) {
        (*x).accept(*this);
        return precedence;
    }
};

template <typename T>
class StrPrinter : public BaseVisitor<T, Visitor> {
protected:
    std::string str_;
public:
    static const std::vector<std::string> names_;
    inline void bvisit(const Basic &x);
    inline void bvisit(const Symbol &x);
    inline void bvisit(const Integer &x);
    inline void bvisit(const Rational &x);
    inline void bvisit(const Complex &x);
    inline void bvisit(const Add &x);
    inline void bvisit(const Mul &x);
    inline void bvisit(const Pow &x);
    inline void bvisit(const UnivariatePolynomial &x);
    inline void bvisit(const Log &x);
    inline void bvisit(const Constant &x);
    inline void bvisit(const Function &x);
    inline void bvisit(const FunctionSymbol &x);
    inline void bvisit(const Derivative &x);
    inline void bvisit(const Subs &x);
    inline void bvisit(const RealDouble &x);
    inline void bvisit(const ComplexDouble &x);
#ifdef HAVE_SYMENGINE_MPFR
    inline void bvisit(const RealMPFR &x);
#endif
#ifdef HAVE_SYMENGINE_MPC
    inline void bvisit(const ComplexMPC &x);
#endif
    inline void bvisit(const NumberWrapper &x);

    inline std::string parenthesizeLT(const RCP<const Basic> &x, PrecedenceEnum precedenceEnum);
    inline std::string parenthesizeLE(const RCP<const Basic> &x, PrecedenceEnum precedenceEnum);

    inline std::string apply(const RCP<const Basic> &b);
    inline std::string apply(const vec_basic &v);
    inline std::string apply(const Basic &b);
};

std::vector<std::string> init_str_printer_names();

class StrPrinterFinal : public StrPrinter<StrPrinterFinal> {

};

template <typename T>
void StrPrinter<T>::bvisit(const Basic &x) {
    std::ostringstream s;
    s << "<" << typeName<Basic>(x) << " instance at " << (const void*)this << ">";
    str_ = s.str();
}

template <typename T>
void StrPrinter<T>::bvisit(const Symbol &x) {
    str_ = x.get_name();
}

template <typename T>
void StrPrinter<T>::bvisit(const Integer &x) {
    std::ostringstream s;
    s << x.i;
    str_ = s.str();
}

template <typename T>
void StrPrinter<T>::bvisit(const Rational &x) {
    std::ostringstream s;
    s << x.i;
    str_ = s.str();
}

template <typename T>
void StrPrinter<T>::bvisit(const Complex &x) {
    std::ostringstream s;
    if (x.real_ != 0) {
        s << x.real_;
        // Since Complex is in canonical form, imaginary_ is not 0.
        if (sgn(x.imaginary_) == 1) {
            s << " + ";
        } else {
            s << " - ";
        }
        // If imaginary_ is not 1 or -1, print the absolute value
        if (x.imaginary_ != sgn(x.imaginary_)) {
            s << abs(x.imaginary_);
            s << "*I";
        } else {
            s << "I";
        }
    } else {
        if (x.imaginary_ != sgn(x.imaginary_)) {
            s << x.imaginary_;
            s << "*I";
        } else {
            if (sgn(x.imaginary_) == 1) {
                s << "I";
            } else {
                s << "-I";
            }
        }
    }
    str_ = s.str();
}

template <typename T>
void StrPrinter<T>::bvisit(const RealDouble &x) {
    std::ostringstream s;
    s.precision(std::numeric_limits< double >::digits10);
    s << x.i;
    str_ = s.str();
    if (str_.find(".") == std::string::npos) {
        s << ".0";
        str_ =  s.str();
    }
}

template <typename T>
void StrPrinter<T>::bvisit(const ComplexDouble &x) {
    std::ostringstream s;
    s.precision(std::numeric_limits< double >::digits10);
    s << x.i.real();
    if (s.str().find(".") == std::string::npos) {
        s << ".0";
    }
    if (x.i.imag() < 0) {
        s << " - " << -x.i.imag();
    } else {
        s << " + " << x.i.imag();
    }
    str_ = s.str();
    if (str_.find(".") == str_.find_last_of(".")) {
        str_ += ".0*I";
    } else {
        str_ += "*I";
    }
}

#ifdef HAVE_SYMENGINE_MPFR
template <typename T>
void StrPrinter<T>::bvisit(const RealMPFR &x) {
    mpfr_exp_t ex;
    char* c = mpfr_get_str(nullptr, &ex, 10, 0, x.i.get_mpfr_t(), MPFR_RNDN);
    std::ostringstream s;
    str_ = std::string(c);
    if (str_.at(0)== '-') {
        s << '-';
        str_ = str_.substr(1, str_.length() - 1);
    }
    if (ex > 6) {
        s << str_.at(0) << '.' << str_.substr(1, str_.length() - 1) << 'e' << (ex - 1);
    } else if (ex > 0) {
        s << str_.substr(0, (unsigned long)ex) << ".";
        s << str_.substr((unsigned long)ex, str_.length() - ex);
    } else if (ex > -5) {
        s << "0.";
        for (int i = 0; i < -ex; ++i) {
            s << '0';
        }
        s << str_;
    } else {
        s << str_.at(0) << '.' << str_.substr(1, str_.length() - 1) << 'e' << (ex - 1);
    }
    mpfr_free_str(c);
    str_ = s.str();
}
#endif
#ifdef HAVE_SYMENGINE_MPC
template <typename T>
void StrPrinter<T>::bvisit(const ComplexMPC &x) {
    RCP<const Number> imag = x.imaginary_part();
    if (imag->is_negative()) {
        std::string str = this->apply(imag);
        str = str.substr(1, str.length() - 1);
        str_ = this->apply(x.real_part()) + " - " + str + "*I";
    } else {
        str_ = this->apply(x.real_part()) + " + " + this->apply(imag) + "*I";
    }
}
#endif
template <typename T>
void StrPrinter<T>::bvisit(const Add &x) {
    std::ostringstream o;
    bool first = true;
    std::map<RCP<const Basic>, RCP<const Number>,
            RCPBasicKeyLessCmp> dict(x.dict_.begin(), x.dict_.end());

    if (neq(*(x.coef_), *zero)) {
        o << this->apply(x.coef_);
        first = false;
    }
    for (const auto &p: dict) {
        std::string t;
        if (eq(*(p.second), *one)) {
            t = this->apply(p.first);
        } else if(eq(*(p.second), *minus_one)) {
            t = "-" + parenthesizeLT(p.first, PrecedenceEnum::Mul);
        } else {
            t = parenthesizeLT(p.second, PrecedenceEnum::Mul) + "*" + parenthesizeLT(p.first, PrecedenceEnum::Mul);
        }

        if (not first) {
            if (t[0] == '-') {
                o << " - " << t.substr(1);
            } else {
                o << " + " << t;
            }
        } else {
            o << t;
            first = false;
        }
    }
    str_ = o.str();
}

template <typename T>
void StrPrinter<T>::bvisit(const Mul &x) {
    std::ostringstream o, o2;
    bool num = false;
    unsigned den = 0;
    std::map<RCP<const Basic>, RCP<const Basic>,
            RCPBasicKeyLessCmp> dict(x.dict_.begin(), x.dict_.end());

    if (eq(*(x.coef_), *minus_one)) {
        o << "-";
    } else if (neq(*(x.coef_), *one)) {
        o << parenthesizeLT(x.coef_, PrecedenceEnum::Mul) << "*";
        num = true;
    }

    for (const auto &p: dict) {
        if ((is_a<Integer>(*p.second) and
             rcp_static_cast<const Integer>(p.second)->is_negative()) ||
            (is_a<Rational>(*p.second) and
             rcp_static_cast<const Rational>(p.second)->is_negative())) {
            if(eq(*(p.second), *minus_one)) {
                o2 << parenthesizeLT(p.first, PrecedenceEnum::Mul);
            } else {
                o2 << parenthesizeLE(p.first, PrecedenceEnum::Pow);
                o2 << "**";
                o2 << parenthesizeLE(neg(p.second), PrecedenceEnum::Pow);
            }
            o2 << "*";
            den++;
        } else {
            if(eq(*(p.second), *one)) {
                o << parenthesizeLT(p.first, PrecedenceEnum::Mul);
            } else {
                o << parenthesizeLE(p.first, PrecedenceEnum::Pow);
                o << "**";
                o << parenthesizeLE(p.second, PrecedenceEnum::Pow);
            }
            o << "*";
            num = true;
        }
    }

    if (not num) {
        o << "1*";
    }

    std::string s = o.str();
    s = s.substr(0, s.size() - 1);

    if (den != 0) {
        std::string s2 = o2.str();
        s2 = s2.substr(0, s2.size() - 1);
        if (den > 1) {
            str_ = s + "/(" + s2 + ")";
        } else {
            str_ = s + "/" + s2;
        }
    } else {
        str_ = s;
    }
}

template <typename T>
void StrPrinter<T>::bvisit(const Pow &x) {
    std::ostringstream o;
    o << parenthesizeLE(x.get_base(), PrecedenceEnum::Pow);
    o << "**";
    o << parenthesizeLE(x.get_exp(), PrecedenceEnum::Pow);
    str_ = o.str();
}

//UnivariatePolynomial printing, tests taken from SymPy and printing ensures that there is compatibility
template <typename T>
void StrPrinter<T>::bvisit(const UnivariatePolynomial &x) {
    std::ostringstream s;
    //bool variable needed to take care of cases like -5, -x, -3*x etc.
    bool first = true;
    //we iterate over the map in reverse order so that highest degree gets printed first
    for (auto it = x.dict_.rbegin(); it != x.dict_.rend();) {
        //given a term in univariate polynomial, if coefficient is zero, print nothing
        if (it->second == 0) {
            //except when it is the only term, say "0"
            if (it->first == 0)
                s << "0";
            ++it;
        }
        //if the coefficient of a term is +1 or -1
        else if (abs(it->second) == 1) {
            //if exponent is 0, then print only coefficient
            //in cases of -7, it is the only term, hence we print -7
            //in cases of x - 7, the '-' is considered earlier, hence print only 7
            if (it->first == 0) {
                if (first)
                    s << it->second;
                else
                    s << abs(it->second);
            }
            //if exponent is 1, print x instead of x**1
            else if (it->first == 1) {
                //in cases of -x, print -x
                //in cases of x**2 - x, print x, the '-' is considered earlier
                if (first and it->second == -1) {
                    s << "-" << x.var_->get_name();
                } else {
                    s << x.var_->get_name();
                }
            } else {
                if (first and it->second == -1) {
                    s << "-" << x.var_->get_name() << "**"  << it->first;
                } else {
                    s << x.var_->get_name() << "**"  << it->first;
                }
            }
            //if next term is going to be 0, don't print +, so that x**3 + 0 + x becomes x**3 + x
            //also consider that sign of term here itself to avoid prints like x + -1
            if ((++it != x.dict_.rend()) and (it->second != 0)) {
                if (it->second < 0) {
                    s << " - ";
                } else {
                    s << " + ";
                }
            }
        }
        //same logic is followed as above
        else {
            if (it->first == 0) {
                if (first)
                    s << it->second;
                else
                    s << abs(it->second);
            } else if (it->first == 1) {
                if (first and it->second < 0) {
                    s << it->second << "*" << x.var_->get_name();
                } else {
                    s << abs(it->second) << "*" << x.var_->get_name();
                }
            } else {
                s << abs(it->second) << "*" << x.var_->get_name() << "**"  << it->first;
            }
            if ((++it != x.dict_.rend()) and (it->second != 0)) {
                if (it->second < 0) {
                    s << " - ";
                } else {
                    s << " + ";
                }
            }
        }
        //corner cases of only first term handled successfully, switch the bool
        first = false;
    }
    str_ = s.str();
}

template <typename T>
void StrPrinter<T>::bvisit(const Log &x) {
    str_ = "log(" + this->apply(x.get_arg()) + ")";
}

template <typename T>
void StrPrinter<T>::bvisit(const Constant &x) {
    str_ = x.get_name();
}

template <typename T>
std::string StrPrinter<T>::apply(const vec_basic &d)
{
    std::ostringstream o;
    for (auto p = d.begin(); p != d.end(); p++) {
        if (p != d.begin()) {
            o << ", ";
        }
        o << this->apply(*p);
    }
    return o.str();
}
template <typename T>
void StrPrinter<T>::bvisit(const Function &x) {
    std::ostringstream o;
    o << names_[x.get_type_code()];
    o << "(";
    vec_basic vec = x.get_args();
    o << this->apply(vec) << ")";
    str_ = o.str();
}

template <typename T>
void StrPrinter<T>::bvisit(const FunctionSymbol &x) {
    std::ostringstream o;
    o << x.get_name();
    o << "(";
    vec_basic vec = x.get_args();
    o << this->apply(vec) << ")";
    str_ = o.str();
}

template <typename T>
void StrPrinter<T>::bvisit(const Derivative &x) {
    std::ostringstream o;
    o << "Derivative(";
    vec_basic vec = x.get_args();
    o << this->apply(vec) << ")";
    str_ = o.str();
}

template <typename T>
void StrPrinter<T>::bvisit(const Subs &x) {
    std::ostringstream o, vars, point;
    for (auto p = x.dict_.begin(); p != x.dict_.end(); p++) {
        if (p != x.dict_.begin()) {
            vars << ", ";
            point << ", ";
        }
        vars << *(p->first);
        point << *(p->second);
    }
    o << "Subs(" << this->apply(x.arg_) << ", (" << vars.str() << "), (" << point.str() << "))";
    str_ = o.str();
}

template <typename T>
void StrPrinter<T>::bvisit(const NumberWrapper &x) {
    str_ = x.__str__();
}

template <typename T>
std::string StrPrinter<T>::parenthesizeLT(const RCP<const Basic> &x, PrecedenceEnum precedenceEnum) {
    Precedence prec;
    if(prec.getPrecedence(x) < precedenceEnum) {
        return "(" + this->apply(x) + ")";
    } else {
        return this->apply(x);
    }
}

template <typename T>
std::string StrPrinter<T>::parenthesizeLE(const RCP<const Basic> &x, PrecedenceEnum precedenceEnum) {
    Precedence prec;
    if(prec.getPrecedence(x) <= precedenceEnum) {
        return "(" + this->apply(x) + ")";
    } else {
        return this->apply(x);
    }
}

template <typename T>
std::string StrPrinter<T>::apply(const RCP<const Basic> &b) {
    b->accept(*static_cast<T*>(this));
    return str_;
}

template <typename T>
std::string StrPrinter<T>::apply(const Basic &b) {
    b.accept(*static_cast<T*>(this));
    return str_;
}

template <typename T>
const std::vector<std::string> StrPrinter<T>::names_ = init_str_printer_names();




}



#endif //SYMENGINE_PRINTER_H
