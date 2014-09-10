/* 
 * File:   Tan.hpp
 * Author: matthewsupernaw
 *
 * Created on June 18, 2014, 12:17 PM
 */

#ifndef ET4AD_TAN_HPP
#define	ET4AD_TAN_HPP

#include <cmath>
#include "Expression.hpp"
#include "Cos.hpp"

namespace et4ad {

    /**
     * Expression template for computing the tangent of a expression template.
     * 
     * @param expr
     */
    template <class REAL_T, class EXPR>
    class Tan;
}


namespace std {

    /**
     * Override for the tan function in namespace std.
     * 
     * @param expr
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const et4ad::Tan<REAL_T, EXPR> tan(const et4ad::ExpressionBase<REAL_T, EXPR>& expr);

}



namespace et4ad {

    /**
     * Expression template for computing the tangent of a expression template.
     * 
     * @param expr
     */
    template <class REAL_T, class EXPR>
    class Tan : public ExpressionBase<REAL_T, Tan<REAL_T, EXPR> > {
    public:

        Tan(const ExpressionBase<REAL_T, EXPR>& expr)
        : expr_m(expr.Cast()) {
        }

        inline const REAL_T GetValue() const {
            return std::tan(expr_m.GetValue());
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
                REAL_T v = expr_m.GetValue();
                return dx * ((1.0 / std::cos(v))*(1.0 / std::cos(v)));
            } else {
                return 0.0;
            }
        }

        inline const REAL_T Derivative(const uint32_t &id) const {
            return expr_m.Derivative(id) * ((1.0 / std::cos(expr_m.GetValue()))*(1.0 / std::cos(expr_m.GetValue())));
        }

         inline const std::complex<REAL_T> ComplexStepValue(const uint32_t & id, REAL_T h = REAL_T(0.00000000000001)) const {
            return std::tan(expr_m.ComplexStepValue(id, h));
        }
     
        inline void PushIds(et4ad::IDSet & ids) const {
            expr_m.PushIds(ids);
        }

        inline void PushStatements(std::vector<Statement<REAL_T> > &storage) const {
            expr_m.PushStatements(storage);
            storage.push_back(Statement<REAL_T > (TAN, GetValue()));
        }




    private:
        const EXPR& expr_m;
    };



}

namespace std {

    /**
     * Override for the tan function in namespace std.
     * 
     * @param expr
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const et4ad::Tan<REAL_T, EXPR> tan(const et4ad::ExpressionBase<REAL_T, EXPR>& expr) {
        return et4ad::Tan<REAL_T, EXPR > (expr.Cast());
    }

}
#endif	/* TAN_HPP */

