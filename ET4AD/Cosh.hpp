/* 
 * File:   Cosh.hpp
 * Author: matthewsupernaw
 *
 * Created on June 18, 2014, 12:50 PM
 */

#ifndef ET4AD_COSH_HPP
#define	ET4AD_COSH_HPP


#include <cmath>
#include "Expression.hpp"


namespace et4ad {

    /**
     * Expression template for computing the hyperbolic cosine of an 
     * expression template.
     */
    template <class REAL_T, class EXPR>
    class Cosh;

}

namespace std {
    /**
     * Override for the cosh function in namespace std.
     * 
     * @param expr
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const et4ad::Cosh<REAL_T, EXPR> cosh(const et4ad::ExpressionBase<REAL_T, EXPR>& expr);
}

namespace et4ad {

    /**
     * Expression template for computing the hyperbolic cosine of an 
     * expression template.
     */
    template <class REAL_T, class EXPR>
    class Cosh : public ExpressionBase<REAL_T, Cosh<REAL_T, EXPR> > {
    public:

        Cosh(const ExpressionBase<REAL_T, EXPR>& expr)
        : expr_m(expr.Cast()), value_m(expr.GetValue()) {
        }

        inline const REAL_T GetValue() const {
            return std::cosh(value_m);
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
                return dx * std::sinh(value_m);
            } else {
                return 0.0;
            }
        }

        inline const REAL_T Derivative(const uint32_t &id) const {
            return expr_m.Derivative(id) * std::sinh(value_m);
        }

        inline void PushIds(et4ad::VariableStorage<REAL_T> &storage) const {
            expr_m.PushIds(storage);
        }

        inline void PushIds(et4ad::IDSet &ids) const {
            expr_m.PushIds(ids);
        }

        inline void PushStatements(std::vector<Statement<REAL_T> > &storage) const {
            expr_m.PushStatements(storage);
            storage.push_back(Statement<REAL_T > (COSH));
        }

        inline void PushAll(et4ad::VariableStorage<REAL_T> &storage) const {
            expr_m.PushAll(storage);
        }

    private:
        const EXPR& expr_m;
        REAL_T value_m;
    };



}

namespace std {

    /**
     * Override for the cosh function in namespace std.
     * 
     * @param expr
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const et4ad::Cosh<REAL_T, EXPR> cosh(const et4ad::ExpressionBase<REAL_T, EXPR>& expr) {

        return et4ad::Cosh<REAL_T, EXPR > (expr.Cast());
    }
}



#endif	/* COSH_HPP */

