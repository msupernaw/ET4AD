/* 
 * File:   Pow.hpp
 * Author: matthewsupernaw
 *
 * Created on June 18, 2014, 12:32 PM
 */

#ifndef ET4AD_POW_HPP
#define	ET4AD_POW_HPP

#include <cmath>
#include "Expression.hpp"

namespace et4ad {
    template <class REAL_T, class EXPR1, class EXPR2>
    struct Pow;

    template <class REAL_T, class EXPR1>
    struct PowConstant;

    template <class REAL_T, class EXPR2>
    struct ConstantPow;
    ;
}


namespace std {

    /**
     * Override for the pow function in namespace std.
     * 
     * @param expr1
     * @param expr2
     * @return 
     */
    template <class REAL_T, class EXPR1, class EXPR2>
    inline
    et4ad::Pow<REAL_T, EXPR1, EXPR2> pow(const et4ad::ExpressionBase<REAL_T, EXPR1>& expr1,
            const et4ad::ExpressionBase<REAL_T, EXPR2>& expr2);

    /**
     * Override for the pow function in namespace std.
     * 
     * @param expr1
     * @param val
     * @return 
     */
    template <class REAL_T, class EXPR>
    inline
    et4ad::PowConstant< REAL_T, EXPR > pow(const et4ad::ExpressionBase<REAL_T, EXPR>& expr,
            const REAL_T& val);

    /**
     * Override for the pow function in namespace std.
     * 
     * @param val
     * @param expr2
     * @return 
     */
    template <class REAL_T, class EXPR>
    inline
    et4ad::ConstantPow<REAL_T, EXPR> pow(const REAL_T& val,
            const et4ad::ExpressionBase<REAL_T, EXPR>& expr);

}

namespace et4ad {

    /**
     *Expression template for computing the power of a expression template,
     * where both arguments are expression templates. 
     */
    template <class REAL_T, class EXPR1, class EXPR2>
    struct Pow : public ExpressionBase<REAL_T, Pow<REAL_T, EXPR1, EXPR2> > {

        Pow(const ExpressionBase<REAL_T, EXPR1>& expr1, const ExpressionBase<REAL_T, EXPR2>& expr2)
        : expr1_m(expr1.Cast()), expr2_m(expr2.Cast()) {
        }

        inline const REAL_T GetValue() const {
            return std::pow(expr1_m.GetValue(), expr2_m.GetValue());
        }

        operator REAL_T() {
            return this->GetValue();
        }

        operator REAL_T()const {
            return this->GetValue();
        }

        inline const REAL_T Derivative(const uint32_t id, bool &found) const {
            REAL_T dx = expr1_m.Derivative(id, found);
            REAL_T dx2 = expr2_m.Derivative(id, found);
            if (found) {
                REAL_T v = expr1_m.GetValue();
                REAL_T v2 = expr2_m.GetValue();

                return (dx * v2) *std::pow(v, (v2 - static_cast<REAL_T> (1.0)));
            } else {
                return 0.0;
            }
        }

        inline const REAL_T Derivative(const uint32_t & id) const {
            return (expr1_m.Derivative(id) * expr2_m.GetValue()) *std::pow(expr1_m.GetValue(), (expr2_m.GetValue() - static_cast<REAL_T> (1.0)));
        }

        inline const std::complex<REAL_T> ComplexStepValue(const uint32_t & id, REAL_T h = REAL_T(0.00000000000001)) const {
            return std::pow(expr1_m.ComplexStepValue(id, h), expr2_m.ComplexStepValue(id, h));
        }

        inline void PushIds(et4ad::IDSet & ids) const {
            expr1_m.PushIds(ids);
            expr2_m.PushIds(ids);
        }

        inline void PushStatements(std::vector<Statement<REAL_T> > &storage) const {
            expr1_m.PushStatements(storage);
            expr2_m.PushStatements(storage);
            storage.push_back(Statement<REAL_T > (POW));
        }





        const EXPR1& expr1_m;
        const EXPR2& expr2_m;
    };

    /**
     *Expression template for computing the power of a expression template,
     * where first argument is an expression templates, the second a constant. 
     */
    template <class REAL_T, class EXPR1>
    struct PowConstant : public ExpressionBase<REAL_T, PowConstant<REAL_T, EXPR1> > {

        PowConstant(const ExpressionBase<REAL_T, EXPR1>& expr1, const REAL_T & expr2)
        : expr1_m(expr1.Cast()), expr2_m(expr2) {
        }

        inline const REAL_T GetValue() const {
            return std::pow(expr1_m.GetValue(), expr2_m);
        }

        inline const REAL_T Derivative(const uint32_t id, bool &found) const {
            REAL_T dx = expr1_m.Derivative(id, found);
            REAL_T dx2 = 0;
            if (found) {
                REAL_T v = expr1_m.GetValue();
                REAL_T v2 = expr2_m;

                return (dx * v2) *std::pow(v, (v2 - static_cast<REAL_T> (1.0)));
            } else {
                return 0.0;
            }
        }

        inline const REAL_T Derivative(const uint32_t & id) const {
            return (expr1_m.Derivative(id) * expr2_m) *std::pow(expr1_m.GetValue(), (expr2_m - static_cast<REAL_T> (1.0)));
        }

        inline const std::complex<REAL_T> ComplexStepValue(const uint32_t & id, REAL_T h = REAL_T(0.00000000000001)) const {
            return std::pow(expr1_m.ComplexStepValue(id, h), std::complex < REAL_T>(expr2_m));
        }

        inline void PushIds(et4ad::IDSet & ids) const {
            expr1_m.PushIds(ids);
        }

        inline void PushStatements(std::vector<Statement<REAL_T> >&storage) const {
            expr1_m.PushStatements(storage);
            storage.push_back(Statement<REAL_T > (CONSTANT, expr2_m));
            storage.push_back(Statement<REAL_T > (POW));
        }




        const EXPR1& expr1_m;
        const REAL_T& expr2_m;
    };

    /**
     *Expression template for computing the power of a expression template,
     * where both the first argument is constant, the second is an expression template. 
     */
    template <class REAL_T, class EXPR2>
    struct ConstantPow : public ExpressionBase<REAL_T, ConstantPow<REAL_T, EXPR2> > {

        ConstantPow(const REAL_T& expr1, const ExpressionBase<REAL_T, EXPR2>& expr2)
        : expr1_m(expr1), expr2_m(expr2.Cast()) {
        }

        inline const REAL_T GetValue() const {
            return std::pow(expr1_m, expr2_m.GetValue());
        }

        inline const REAL_T Derivative(const uint32_t id, bool &found) const {
            return 0;
            //            REAL_T dx = 0.0;
            //            REAL_T dx2 = expr2_m.Derivative(id, found);
            //            if (found) {
            //                REAL_T v = expr1_m;
            //                REAL_T v2 = expr2_m.GetValue();
            //
            //                return (dx * v2) *std::pow(v, (v2 - static_cast<REAL_T> (1.0)));
            //            } else {
            //                return 0.0;
            //            }
        }

        inline const REAL_T Derivative(const uint32_t & id) const {
            return 0;
        }
        
        inline const std::complex<REAL_T> ComplexStepValue(const uint32_t & id, REAL_T h = REAL_T(0.00000000000001)) const {
            return std::pow(std::complex<REAL_T>(expr1_m), expr2_m.ComplexStepValue(id, h));
        }


        inline void PushIds(et4ad::IDSet & ids) const {
            expr2_m.PushIds(ids);
        }

        inline void PushStatements(std::vector<Statement<REAL_T> > &storage) const {
            storage.push_back(Statement<REAL_T > (CONSTANT, expr1_m));
            expr2_m.PushStatements(storage);
            storage.push_back(Statement<REAL_T > (POW));
        }






        const REAL_T& expr1_m;
        const EXPR2& expr2_m;
    };

}

namespace std {

    /**
     * Override for the pow function in namespace std.
     * 
     * @param expr1
     * @param expr2
     * @return 
     */
    template <class REAL_T, class EXPR1, class EXPR2>
    inline
    et4ad::Pow<REAL_T, EXPR1, EXPR2> pow(const et4ad::ExpressionBase<REAL_T, EXPR1>& expr1,
            const et4ad::ExpressionBase<REAL_T, EXPR2>& expr2) {
        return et4ad::Pow<REAL_T, EXPR1, EXPR2 > (expr1.Cast(), expr2.Cast());
    }

    /**
     * Override for the pow function in namespace std.
     * 
     * @param expr1
     * @param val
     * @return 
     */
    template <class REAL_T, class EXPR>
    inline
    et4ad::PowConstant< REAL_T, EXPR > pow(const et4ad::ExpressionBase<REAL_T, EXPR>& expr,
            const REAL_T& val) {
        return et4ad::PowConstant< REAL_T, EXPR > (expr.Cast(), val);
    }

    /**
     * Override for the pow function in namespace std.
     * 
     * @param val
     * @param expr2
     * @return 
     */
    template <class REAL_T, class EXPR>
    inline
    et4ad::ConstantPow<REAL_T, EXPR> pow(const REAL_T& val,
            const et4ad::ExpressionBase<REAL_T, EXPR>& expr) {

        return et4ad::ConstantPow<REAL_T, EXPR > (val, expr.Cast());
    }

}

#endif	/* POW_HPP */

