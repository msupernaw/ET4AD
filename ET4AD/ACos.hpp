/* 
 * File:   ACos.hpp
 * Author: matthewsupernaw
 *
 * Created on June 18, 2014, 12:26 PM
 */

#ifndef ET4AD_ACOS_HPP
#define	ET4AD_ACOS_HPP

#include <cmath>
#include "Expression.hpp"
namespace et4ad {

    /**
     * Expression template for computing the inverse cosine of an expression
     * template.
     * 
     * @param a
     */
    template <class REAL_T, class EXPR>
    class ACos;
}


namespace std {

    /**
     * Override for the asin function in namespace std.
     * 
     * @param expr
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const et4ad::ACos<REAL_T, EXPR> acos(const et4ad::ExpressionBase<REAL_T, EXPR>& expr);
}

namespace et4ad {

    /**
     * Expression template for computing the inverse cosine of an expression
     * template.
     * 
     * @param a
     */
    template <class REAL_T, class EXPR>
    class ACos : public ExpressionBase<REAL_T, ACos<REAL_T, EXPR> > {
    public:

        ACos(const ExpressionBase<REAL_T, EXPR>& a)
        : expr_m(a.Cast()) {
        }

        inline const REAL_T GetValue() const {
            return std::acos(expr_m.GetValue());
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
                return (dx * static_cast<REAL_T> (-1.0) / std::pow((static_cast<REAL_T> (1.0) - std::pow(v, static_cast<REAL_T> (2.0))), static_cast<REAL_T> (0.5)));
            } else {
                return 0.0;
            }
        }

        inline const REAL_T Derivative(const uint32_t id) const {
            return (expr_m.Derivative(id) * static_cast<REAL_T> (-1.0) / std::pow((static_cast<REAL_T> (1.0) - std::pow(expr_m.GetValue(), static_cast<REAL_T> (2.0))), static_cast<REAL_T> (0.5)));
        }

        inline const std::complex<REAL_T> ComplexStepValue(const uint32_t & id, REAL_T h = REAL_T(0.00000000000001))  const {
            return std::acos(expr_m.ComplexStepValue(id, h));
        }

        inline void PushIds(et4ad::IDSet &ids) const {
            expr_m.PushIds(ids);
        }

        inline void PushStatements(std::vector<Statement<REAL_T> > &storage) const {
            expr_m.PushStatements(storage);
            storage.push_back(Statement<REAL_T > (ACOS));
        }



    private:
        const EXPR& expr_m;
    };


}

namespace std {

    /**
     * Override for the asin function in namespace std.
     * 
     * @param expr
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const et4ad::ACos<REAL_T, EXPR> acos(const et4ad::ExpressionBase<REAL_T, EXPR>& expr) {
        return et4ad::ACos<REAL_T, EXPR > (expr.Cast());
    }

}
#endif	/* ACOS_HPP */

