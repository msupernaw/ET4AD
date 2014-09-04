/* 
 * File:   Cos.hpp
 * Author: matthewsupernaw
 *
 * Created on June 18, 2014, 12:15 PM
 */

#ifndef ET4AD_COS_HPP
#define	ET4AD_COS_HPP

#include <cmath>
#include "Expression.hpp"
#include "Sin.hpp"
#include "Multiply.hpp"

namespace et4ad {

    /**
     * Expression template for taking the cosine of an expression.
     * 
     * @param expr
     */
    template <class REAL_T, class EXPR>
    class Cos;

}


namespace std {

    /**
     * Override for the cos function in namespace std.
     * 
     * @param expr
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const et4ad::Cos<REAL_T, EXPR> cos(const et4ad::ExpressionBase<REAL_T, EXPR>& expr);

}


namespace et4ad {

    /**
     * Expression template for taking the cosine of an expression.
     * 
     * @param expr
     */
    template <class REAL_T, class EXPR>
    class Cos : public ExpressionBase<REAL_T, Cos<REAL_T, EXPR> > {
    public:

        Cos(const ExpressionBase<REAL_T, EXPR>& expr)
        : expr_m(expr.Cast()), value_m(expr.GetValue()) {

        }

        inline const REAL_T GetValue() const {
            return std::cos(value_m);
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
                return dx*-1.0 * std::sin(value_m);
            } else {
                return 0.0;
            }
        }

        inline const REAL_T Derivative(const uint32_t &id) const {
            return expr_m.Derivative(id)*-1.0 * std::sin(value_m);
        }

         inline const std::complex<REAL_T> ComplexStepValue(const uint32_t & id, REAL_T h = REAL_T(0.00000000000001)) const {
            return std::cos(expr_m.ComplexStepValue(id, h));
        }
       
        inline void PushIds(et4ad::IDSet &ids) const {
            expr_m.PushIds(ids);
        }

        inline void PushStatements(std::vector<Statement<REAL_T> > &storage) const {
            expr_m.PushStatements(storage);
            storage.push_back(Statement<REAL_T > (COS));
        }


    private:
        const EXPR& expr_m;
        REAL_T value_m;
    };


}

namespace std {

    /**
     * Override for the cos function in namespace std.
     * 
     * @param expr
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const et4ad::Cos<REAL_T, EXPR> cos(const et4ad::ExpressionBase<REAL_T, EXPR>& expr) {
        return et4ad::Cos<REAL_T, EXPR > (expr.Cast());
    }
}

#endif	/* COS_HPP */

