/* 
 * File:   Log10.hpp
 * Author: matthewsupernaw
 *
 * Created on June 18, 2014, 12:35 PM
 */

#ifndef ET4AD_LOG10_HPP
#define	ET4AD_LOG10_HPP

#include <cmath>
#include "Expression.hpp"

namespace et4ad {

    /**
     * Expression template to compute the log base 10 of an expression 
     * template.
     * 
     * @param expr
     */
    template <class REAL_T, class EXPR>
    class Log10;
}

namespace std {

    /**
     * Override for the log function in namespace std.
     * 
     * @param expr
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const et4ad::Log10<REAL_T, EXPR> log10(const et4ad::ExpressionBase<REAL_T, EXPR>& expr);
}


namespace et4ad {

    /**
     * Expression template to compute the log base 10 of an expression 
     * template.
     * 
     * @param expr
     */
    template <class REAL_T, class EXPR>
    class Log10 : public ExpressionBase<REAL_T, Log10<REAL_T, EXPR> > {
    public:

        Log10(const ExpressionBase<REAL_T, EXPR>& expr)
        : expr_m(expr.Cast()) {
        }

        inline const REAL_T GetValue() const {
            return std::log10(expr_m.GetValue());
        }

        operator REAL_T() {
            return this->GetValue();
        }

        operator REAL_T()const {
            return this->GetValue();
        }

        inline const REAL_T Derivative(const uint32_t id, bool &found) const {

            REAL_T dx = expr_m.Derivative(id, found);
            if (found) {
                return (dx * 1.0) / (expr_m.GetValue() * std::log(10.0));
            } else {
                return 0.0;
            }
        }

        inline const REAL_T Derivative(const uint32_t id) const {
            return (expr_m.Derivative(id) * 1.0) / (expr_m.GetValue() * std::log(10.0));
        }

        inline const std::complex<REAL_T> ComplexStepValue(const uint32_t & id, REAL_T h = REAL_T(0.00000000000001)) const {
            return std::log10(expr_m.ComplexStepValue(id, h));
        }

        inline void PushIds(et4ad::IDSet &ids) const {
            expr_m.PushIds(ids);
        }

        inline void PushStatements(std::vector<Statement<REAL_T> > &storage) const {
            expr_m.PushStatements(storage);
            storage.push_back(Statement<REAL_T > (LOG10, GetValue()));
        }


    private:
        const EXPR& expr_m;
    };


}

namespace std {

    /**
     * Override for the log function in namespace std.
     * 
     * @param expr
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const et4ad::Log10<REAL_T, EXPR> log10(const et4ad::ExpressionBase<REAL_T, EXPR>& expr) {

        return et4ad::Log10<REAL_T, EXPR > (expr.Cast());
    }
}

#endif	/* LOG10_HPP */

