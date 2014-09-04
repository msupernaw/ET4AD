/* 
 * File:   ATan2.hpp
 * Author: matthewsupernaw
 *
 * Created on June 18, 2014, 12:29 PM
 */

#ifndef ET4AD_ATAN2_HPP
#define	ET4AD_ATAN2_HPP

#include <cmath>
#include "Expression.hpp"

namespace et4ad {

    /**
     * Expression template for computing the two argument inverse tangent,
     * where both arguments are expression templates. 
     * 
     */
    template <class REAL_T, class EXPR1, class EXPR2>
    struct ATan2;
    
      /**
     * Expression template for computing the two argument inverse tangent,
     * where both the first argument is an expression template, the second
     * is constant. 
     * 
     */
    template <class REAL_T, class EXPR1>
    struct ATan2Constant;
    
     /**
     * Expression template for computing the two argument inverse tangent,
     * where both arguments are expression templates. 
     * 
     */
    template <class REAL_T, class EXPR2>
    struct ConstantATan2;
    
}
    


namespace std {

    /**
     * Override for the atan2 function in namespace std.
     * 
     * @param expr1
     * @param expr2
     * @return 
     */
    template <class REAL_T, class EXPR1, class EXPR2>
    inline
    et4ad::ATan2<REAL_T, EXPR1, EXPR2> atan2(const et4ad::ExpressionBase<REAL_T, EXPR1>& expr1,
            const et4ad::ExpressionBase<REAL_T, EXPR2>& expr2);

    /**
     * Override for the atan2 function in namespace std.
     * 
     * @param expr1
     * @param val
     * @return 
     */
    template <class REAL_T, class EXPR>
    inline
    et4ad::ATan2Constant<REAL_T, EXPR > atan2(const et4ad::ExpressionBase<REAL_T, EXPR>& expr,
            const REAL_T& val);

    /**
     * Override for the atan2 function in namespace std.
     * 
     * @param expr1
     * @param expr2
     * @return 
     */
    template <class REAL_T, class EXPR>
    inline
    et4ad::ConstantATan2<REAL_T, EXPR> atan2(const REAL_T& val,
            const et4ad::ExpressionBase<REAL_T, EXPR>& expr);



}

namespace et4ad {

    /**
     * Expression template for computing the two argument inverse tangent,
     * where both arguments are expression templates. 
     * 
     */
    template <class REAL_T, class EXPR1, class EXPR2>
    struct ATan2 : public ExpressionBase<REAL_T, ATan2<REAL_T, EXPR1, EXPR2> > {

        ATan2(const ExpressionBase<REAL_T, EXPR1>& expr1, const ExpressionBase<REAL_T, EXPR2>& expr2)
        : expr1_m(expr1.Cast()), expr2_m(expr2.Cast()), value1_m(expr1.GetValue()), value2_m(expr2.GetValue()) {
        }

        inline const REAL_T GetValue() const {
            return std::atan2(value1_m, value2_m);
        }

        operator REAL_T() {
            return this->GetValue();
        }

        operator REAL_T()const {
            return this->GetValue();
        }

        inline const REAL_T Derivative(const uint32_t id, bool &found) const {
            REAL_T dx = expr1_m.Derivative(id, found);

            if (found) {
                REAL_T v = expr1_m.GetValue();
                REAL_T v2 = expr2_m.GetValue();

                return ((value2_m * dx) / (value1_m * value1_m + (value2_m * value2_m)));
            } else {
                return 0.0;
            }
        }

        inline const REAL_T Derivative(const uint32_t id) const {
            return ((value2_m * expr1_m.Derivative(id)) / (value1_m * value1_m + (value2_m * value2_m)));
        }

        inline void PushIds(et4ad::VariableStorage<REAL_T> &storage) const {
            expr1_m.PushIds(storage);
            expr2_m.PushIds(storage);
        }

        inline void PushIds(et4ad::IDSet & ids) const {
            expr1_m.PushIds(ids);
            expr2_m.PushIds(ids);
        }

        inline void PushStatements(std::vector<Statement<REAL_T> > &storage) const {
            expr1_m.PushStatements(storage);
            expr2_m.PushStatements(storage);
            storage.push_back(Statement<REAL_T > (ATAN2));
        }

        inline void PushStatements(et4ad::VariableStorage<REAL_T> &storage) const {
            expr1_m.PushStatements(storage);
            expr2_m.PushStatements(storage);
            storage.GetStatements().push_back(Statement<REAL_T > (ATAN2));
        }

        inline void PushAll(et4ad::VariableStorage<REAL_T> &storage) const {
            expr1_m.PushAll(storage);
            expr2_m.PushIds(storage);
        }


        const EXPR1& expr1_m;
        const EXPR2& expr2_m;
        REAL_T value1_m;
        REAL_T value2_m;
    };

    /**
     * Expression template for computing the two argument inverse tangent,
     * where both the first argument is an expression template, the second
     * is constant. 
     * 
     */
    template <class REAL_T, class EXPR1>
    struct ATan2Constant : public ExpressionBase<REAL_T, ATan2Constant<REAL_T, EXPR1> > {

        ATan2Constant(const ExpressionBase<REAL_T, EXPR1>& expr1, const REAL_T & expr2)
        : expr1_m(expr1.Cast()), expr2_m(expr2), value1_m(expr1_m.GetValue()) {
        }

        inline const REAL_T GetValue() const {
            return std::atan2(value1_m, expr2_m);
        }

        operator REAL_T() {
            return this->GetValue();
        }

        operator REAL_T()const {
            return this->GetValue();
        }

        inline const REAL_T Derivative(const uint32_t id, bool &found) const {
            REAL_T dx = expr1_m.Derivative(id, found);

            if (found) {
                REAL_T v = expr1_m.GetValue();
                REAL_T v2 = expr2_m;

                return ((expr2_m * dx) / (value1_m * value1_m + (expr2_m * expr2_m)));
            } else {
                return 0.0;
            }
        }

        inline const REAL_T Derivative(const uint32_t & id) const {
            return ((expr2_m * expr1_m.Derivative(id)) / (value1_m * value1_m + (expr2_m * expr2_m)));

        }

        void Derivative(REAL_T &d, const uint32_t & id) const {
            d = ((expr2_m * expr1_m.Derivative(d, id)) / (value1_m * value1_m + (expr2_m * expr2_m)));
        }

        inline void PushIds(et4ad::VariableStorage<REAL_T> &storage) const {
            expr1_m.PushIds(storage);
        }

        inline void PushIds(et4ad::IDSet & ids) const {
            expr1_m.PushIds(ids);
        }

        inline void PushStatements(et4ad::VariableStorage<REAL_T> &storage) const {
            expr1_m.PushStatements(storage);
            storage.GetStatements().push_back(Statement<REAL_T > (CONSTANT, expr2_m));
            storage.GetStatements().push_back(Statement<REAL_T > (ATAN2));
        }

        inline void PushAll(et4ad::VariableStorage<REAL_T> &storage) const {
            expr1_m.PushAll(storage);
        }


        const EXPR1& expr1_m;
        const REAL_T& expr2_m;
        REAL_T value1_m;
    };

    /**
     * Expression template for computing the two argument inverse tangent,
     * where both arguments are expression templates. 
     * 
     */
    template <class REAL_T, class EXPR2>
    struct ConstantATan2 : public ExpressionBase<REAL_T, ConstantATan2<REAL_T, EXPR2> > {

        ConstantATan2(const REAL_T& expr1, const ExpressionBase<REAL_T, EXPR2>& expr2)
        : expr1_m(expr1), expr2_m(expr2.Cast()) {
        }

        inline const REAL_T GetValue() const {
            return std::atan2(expr1_m, expr2_m.GetValue());
        }

        operator REAL_T() {
            return this->GetValue();
        }

        operator REAL_T()const {
            return this->GetValue();
        }

        inline const REAL_T Derivative(const uint32_t &id, bool &found) const {
            return 0.0;
            //            REAL_T dx = 0.0;
            //
            //            if (found) {
            //                REAL_T v = expr1_m;
            //                REAL_T v2 = expr2_m.GetValue();
            //
            //                return ((v2 * dx) / (v * v + (v2 * v2)));
            //            } else {
            //                return 0.0;
            //            }
        }

        inline const REAL_T Derivative(const uint32_t & id) const {
            return 0.0;
        }

        void Derivative(REAL_T &d, const uint32_t & id) const {
            d = 0.0;
        }

        inline void PushIds(et4ad::VariableStorage<REAL_T> &storage) const {
            expr2_m.PushIds(storage);
        }

        inline void PushIds(et4ad::IDSet & ids) const {
            expr2_m.PushIds(ids);
        }

        inline void PushStatements(std::vector<Statement<REAL_T> >&storage) const {
            storage.push_back(Statement<REAL_T > (CONSTANT, expr1_m));
            expr2_m.PushStatements(storage);
            storage.push_back(Statement<REAL_T > (ATAN2));
        }

        inline void PushAll(et4ad::VariableStorage<REAL_T> &storage) const {
            expr2_m.PushIds(storage);
        }


        const REAL_T& expr1_m;
        const EXPR2& expr2_m;
    };

}

namespace std {

    /**
     * Override for the atan2 function in namespace std.
     * 
     * @param expr1
     * @param expr2
     * @return 
     */
    template <class REAL_T, class EXPR1, class EXPR2>
    inline
    et4ad::ATan2<REAL_T, EXPR1, EXPR2> atan2(const et4ad::ExpressionBase<REAL_T, EXPR1>& expr1,
            const et4ad::ExpressionBase<REAL_T, EXPR2>& expr2) {
        return et4ad::ATan2<REAL_T, EXPR1, EXPR2 > (expr1.Cast(), expr2.Cast());
    }

    /**
     * Override for the atan2 function in namespace std.
     * 
     * @param expr1
     * @param val
     * @return 
     */
    template <class REAL_T, class EXPR>
    inline
    et4ad::ATan2Constant<REAL_T, EXPR > atan2(const et4ad::ExpressionBase<REAL_T, EXPR>& expr,
            const REAL_T& val) {
        return et4ad::ATan2Constant<REAL_T, EXPR> (expr.Cast(), val);
    }

    /**
     * Override for the atan2 function in namespace std.
     * 
     * @param expr1
     * @param expr2
     * @return 
     */
    template <class REAL_T, class EXPR>
    inline
    et4ad::ConstantATan2<REAL_T, EXPR> atan2(const REAL_T& val,
            const et4ad::ExpressionBase<REAL_T, EXPR>& expr) {
        return et4ad::ConstantATan2<REAL_T, EXPR > (val, expr.Cast());
    }

}
#endif	/* ATAN2_HPP */

