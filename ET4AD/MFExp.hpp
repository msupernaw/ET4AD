/* 
 * File:   MFExp.hpp
 * Author: matthewsupernaw
 *
 * Created on June 18, 2014, 12:44 PM
 */

#ifndef ET4AD_MFEXP_HPP
#define	ET4AD_MFEXP_HPP

#include <cmath>
#include "Expression.hpp"

#define EXP_OF_B REAL_T(114200738981568423454048256.0)


namespace et4ad {

    /**
     * Expression template used to protect overflow in exp calculations. 
     * 
     * Author: Dave Fournier.
     * Original implementation in ADMB.
     * 
     * Source: http://admb-project.org/documentation/api/mfexp_8cpp_source.html
     * 
     * @param expr
     */
    template <class REAL_T, class EXPR>
    class MFExp; 
}

namespace std {

    template<class REAL_T, class EXPR>
    inline const et4ad::MFExp<REAL_T, EXPR> mfexp(const et4ad::ExpressionBase<REAL_T, EXPR>& expr);
}

namespace et4ad {

    /**
     * Expression template used to protect overflow in exp calculations. 
     * 
     * Author: Dave Fournier.
     * Original implementation in ADMB.
     * 
     * Source: http://admb-project.org/documentation/api/mfexp_8cpp_source.html
     * 
     * @param expr
     */
    template <class REAL_T, class EXPR>
    class MFExp : public ExpressionBase<REAL_T, MFExp<REAL_T, EXPR> > {
    public:

        MFExp(const ExpressionBase<REAL_T, EXPR>& expr)
        : expr_m(expr.Cast()), value_m(Compute(expr.GetValue())) {
        }

        inline const REAL_T GetValue() const {
            return value_m;
            //
            //            return std::exp(expr_m.GetValue());
        }

        operator REAL_T() {
            return this->GetValue();
        }

        operator REAL_T()const {
            return this->GetValue();
        }

        inline const REAL_T Compute(const REAL_T & value) {
            //            REAL_T x = value;
            REAL_T b = REAL_T(60);
            if (value <= b && value >= REAL_T(-1) * b) {
                return std::exp(value);
            } else if (value > b) {
                return /*std::exp(b)*/EXP_OF_B * (REAL_T(1.) + REAL_T(2.) * (value - b)) / (REAL_T(1.) + value - b);
            } else {
                return std::exp(REAL_T(-1) * b)*(REAL_T(1.) - value - b) / (REAL_T(1.) + REAL_T(2.) * (REAL_T(-1) * value - b));
            }
        }

        inline const REAL_T Derivative(const uint32_t id, bool &found) const {
            return expr_m.Derivative(id, found) * this->GetValue();
        }

        inline const REAL_T Derivative(const uint32_t &id) const {
            return expr_m.Derivative(id) * value_m;
        }

        inline void PushIds(et4ad::VariableStorage<REAL_T> &storage) const {
            expr_m.PushIds(storage);
        }

        inline void PushIds(et4ad::IDSet &ids) const {
            expr_m.PushIds(ids);
        }

        inline void PushStatements(std::vector<Statement<REAL_T> > &storage) const {
            expr_m.PushStatements(storage);
            storage.push_back(Statement<REAL_T > (MFEXP));
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

    template<class REAL_T, class EXPR>
    inline const et4ad::MFExp<REAL_T, EXPR> mfexp(const et4ad::ExpressionBase<REAL_T, EXPR>& expr) {

        return et4ad::MFExp<REAL_T, EXPR > (expr.Cast());
    }
}

#endif	/* MFEXP_HPP */

