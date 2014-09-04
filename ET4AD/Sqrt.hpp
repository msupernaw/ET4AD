/* 
 * File:   Sqrt.hpp
 * Author: matthewsupernaw
 *
 * Created on June 18, 2014, 12:31 PM
 */

#ifndef ET4AD_SQRT_HPP
#define	ET4AD_SQRT_HPP

#include <cmath>
#include "Expression.hpp"

namespace et4ad {

    /**
     * Expression template for computing the square root of an expression
     * template.
     * @param a
     */
    template <class REAL_T, class EXPR>
    class Sqrt;

}

namespace std {

    /**
     * Override for the sqrt function in namespace std.
     * 
     * @param expr
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const et4ad::Sqrt<REAL_T, EXPR> sqrt(const et4ad::ExpressionBase<REAL_T, EXPR>& expr);

}

namespace et4ad {

    /**
     * Expression template for computing the square root of an expression
     * template.
     * @param a
     */
    template <class REAL_T, class EXPR>
    class Sqrt : public ExpressionBase<REAL_T, Sqrt<REAL_T, EXPR> > {
    public:

        Sqrt(const ExpressionBase<REAL_T, EXPR>& a)
        : expr_m(a.Cast()) {
        }

        inline const REAL_T GetValue() const {
            return std::sqrt(expr_m.GetValue());
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
                return dx * .5 / std::sqrt(expr_m.GetValue());
            } else {
                return 0.0;
            }
        }

        inline const REAL_T Derivative(const uint32_t &id) const {
            return expr_m.Derivative(id) * .5 / std::sqrt(expr_m.GetValue());

        }

        inline void PushIds(et4ad::VariableStorage<REAL_T> &storage) const {
            expr_m.PushIds(storage);
        }

        inline void PushIds(et4ad::IDSet &ids) const {
            expr_m.PushIds(ids);
        }

        inline void PushStatements(std::vector<Statement<REAL_T> > &storage) const {
            expr_m.PushStatements(storage);
            storage.push_back(Statement<REAL_T > (SQRT));
        }

        inline void PushAll(et4ad::VariableStorage<REAL_T> &storage) const {
            expr_m.PushAll(storage);
        }

    private:
        const EXPR& expr_m;
    };
}

namespace std {

    /**
     * Override for the sqrt function in namespace std.
     * 
     * @param expr
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const et4ad::Sqrt<REAL_T, EXPR> sqrt(const et4ad::ExpressionBase<REAL_T, EXPR>& expr) {
        return et4ad::Sqrt<REAL_T, EXPR > (expr.Cast());
    }
}

#endif	/* SQRT_HPP */

