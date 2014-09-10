/* 
 * File:   Sin.hpp
 * Author: matthewsupernaw
 *
 * Created on June 18, 2014, 12:04 PM
 */

#ifndef ET4AD_SIN_HPP
#define	ET4AD_SIN_HPP

#include <cmath>

#include "Expression.hpp"
#include "Cos.hpp"


namespace et4ad {

    /**
     * Expression template for taking the sine of an expression.
     * 
     * @param a
     */
    template <class REAL_T, class EXPR>
    class Sin;
}

namespace std {

    /**
     * Override for the sin function in namespace std.
     * 
     * @param expr
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const et4ad::Sin<REAL_T, EXPR> sin(const et4ad::ExpressionBase<REAL_T, EXPR>& expr);

}

namespace et4ad {

    /**
     * Expression template for taking the sine of an expression.
     * 
     * @param a
     */
    template <class REAL_T, class EXPR>
    class Sin : public ExpressionBase<REAL_T, Sin<REAL_T, EXPR> > {
    public:

        Sin(const ExpressionBase<REAL_T, EXPR>& a)
        : expr_m(a.Cast()) {
        }

        inline const REAL_T GetValue() const {
            return std::sin(expr_m.GetValue());
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
                return dx * std::cos(expr_m.GetValue());
            } else {
                return 0.0;
            }
        }

        inline const REAL_T Derivative(const uint32_t &id) const {
            return expr_m.Derivative(id) * std::cos(expr_m.GetValue());
        }

        inline const std::complex<REAL_T> ComplexStepValue(const uint32_t & id, REAL_T h = REAL_T(0.00000000000001)) const {
            return std::sin(expr_m.ComplexStep(id, h));
        }

        inline void PushIds(et4ad::IDSet &ids) const {
            expr_m.PushIds(ids);
        }

        inline void PushStatements(std::vector<Statement<REAL_T> > &storage) const {
            expr_m.PushStatements(storage);
            storage.push_back(Statement<REAL_T > (SIN, GetValue()));
        }



    private:
        const EXPR& expr_m;
    };



}

namespace std {

    /**
     * Override for the sin function in namespace std.
     * 
     * @param expr
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const et4ad::Sin<REAL_T, EXPR> sin(const et4ad::ExpressionBase<REAL_T, EXPR>& expr) {
        return et4ad::Sin<REAL_T, EXPR > (expr.Cast());
    }
}

#endif	/* SIN_HPP */

