/* 
 * File:   Floor.hpp
 * Author: matthewsupernaw
 *
 * Created on June 18, 2014, 12:54 PM
 */

#ifndef ET4AD_FLOOR_HPP
#define	ET4AD_FLOOR_HPP

#include <cmath>
#include "Expression.hpp"

namespace et4ad {

    /**
     * Expression template for handling the floor of an expression template.
     * 
     * @param a
     */
    template <class REAL_T, class EXPR>
    class Floor;
    
}

namespace std {

    /**
     * Override for the floor function in namespace std.
     * 
     * @param expr
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const et4ad::Floor<REAL_T, EXPR> floor(const et4ad::ExpressionBase<REAL_T, EXPR>& expr);
}

namespace et4ad {

    /**
     * Expression template for handling the floor of an expression template.
     * 
     * @param a
     */
    template <class REAL_T, class EXPR>
    class Floor : public ExpressionBase<REAL_T, Floor<REAL_T, EXPR> > {
    public:

        Floor(const ExpressionBase<REAL_T, EXPR>& a)
        : expr_m(a.Cast()) {
        }

        inline const REAL_T GetValue() const {
            return std::floor(expr_m.GetValue());
        }

        operator REAL_T() {
            return this->GetValue();
        }

        operator REAL_T()const {
            return this->GetValue();
        }

        inline const REAL_T Derivative(const uint32_t id, bool &found) const {
            return 0.0;
        }

        inline const REAL_T Derivative(const uint32_t &id) const {
            return 0.0;
        }

         inline const std::complex<REAL_T> ComplexStepValue(const uint32_t & id, REAL_T h = REAL_T(0.00000000000001)) const {
            return std::floor(expr_m.ComplexStepValue(id, h));
        }
      

        inline void PushIds(et4ad::IDSet &ids) const {
            expr_m.PushIds(ids);
        }

        inline void PushStatements(std::vector<Statement<REAL_T> >&storage) const {
            expr_m.PushStatements(storage);
            storage.push_back(Statement<REAL_T > (FLOOR));
        }

    private:
        const EXPR& expr_m;
    };

}

namespace std {

    /**
     * Override for the floor function in namespace std.
     * 
     * @param expr
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const et4ad::Floor<REAL_T, EXPR> floor(const et4ad::ExpressionBase<REAL_T, EXPR>& expr) {

        return et4ad::Floor<REAL_T, EXPR > (expr.Cast());
    }
}

#endif	/* FLOOR_HPP */

