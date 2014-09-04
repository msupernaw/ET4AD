/* 
 * File:   Exp.hpp
 * Author: matthewsupernaw
 *
 * Created on June 18, 2014, 12:37 PM
 */

#ifndef ET4AD_EXP_HPP
#define	ET4AD_EXP_HPP

#include <cmath>
#include "Expression.hpp"

namespace et4ad {
    /**
     * Expression template to compute e raised to a expression template.
     * @param expr
     */
    template <class REAL_T, class EXPR>
    class Exp;
}

namespace std {

    /**
     * Override for the exp function in namespace std.
     * 
     * @param expr
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const et4ad::Exp<REAL_T, EXPR> exp(const et4ad::ExpressionBase<REAL_T, EXPR>& expr);
}
namespace et4ad {

    /**
     * Expression template to compute e raised to a expression template.
     * @param expr
     */
    template <class REAL_T, class EXPR>
    class Exp : public ExpressionBase<REAL_T, Exp<REAL_T, EXPR> > {
    public:
        const EXPR& expr_m;
        const REAL_T value_m;

        Exp(const ExpressionBase<REAL_T, EXPR>& expr)
        : expr_m(expr.Cast()), value_m(Compute(expr.GetValue())) {
        }

        inline const REAL_T GetValue() const {
            return value_m;
        }

        operator REAL_T() {
            return this->GetValue();
        }

        operator REAL_T()const {
            return this->GetValue();
        }

        inline const REAL_T Compute(const REAL_T &x) {
            return std::exp(x);
        }

        inline const REAL_T Derivative(const uint32_t id, bool &found) const {

            return expr_m.Derivative(id, found) * (value_m);

            //            bool f = false;
            //            REAL_T dx = expr_m.Derivative(id, f);
            //            if (f) {
            //                found = true;
            //                return dx * (value_m);
            //            } else {
            //                return 0.0;
            //            }
        }

        inline const REAL_T Derivative(const uint32_t &id) const {
            return expr_m.Derivative(id) * this->GetValue();

            //            bool f = false;
            //            REAL_T dx = expr_m.Derivative(id, f);
            //            if (f) {
            //                found = true;
            //                return dx * (value_m);
            //            } else {
            //                return 0.0;
            //            }
        }

        inline const std::complex<REAL_T> ComplexStepValue(const uint32_t & id, REAL_T h = REAL_T(0.00000000000001)) const {
            return std::exp(expr_m.ComplexStepValue(id, h));
        }

        inline void PushIds(et4ad::IDSet &ids) const {
            expr_m.PushIds(ids);
        }

        inline void PushStatements(std::vector<Statement<REAL_T> > &storage) const {
            expr_m.PushStatements(storage);
            storage.push_back(Statement<REAL_T > (EXP));
        }


    private:

    };

}

namespace std {

    /**
     * Override for the exp function in namespace std.
     * 
     * @param expr
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const et4ad::Exp<REAL_T, EXPR> exp(const et4ad::ExpressionBase<REAL_T, EXPR>& expr) {

        return et4ad::Exp<REAL_T, EXPR > (expr.Cast());
    }
}

#endif	/* EXP_HPP */

