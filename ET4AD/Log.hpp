/* 
 * File:   Log.hpp
 * Author: matthewsupernaw
 *
 * Created on June 18, 2014, 12:34 PM
 */

#ifndef ET4AD_LOG_HPP
#define	ET4AD_LOG_HPP

#include <cmath>
#include "Expression.hpp"
namespace et4ad {

    /**
     * Expression template to compute the log of an expression template.
     * @param expr
     */
    template <class REAL_T, class EXPR>
    class Log;

}

namespace std {

    /**
     * Override for the log function in namespace std.
     * 
     * @param expr
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const et4ad::Log<REAL_T, EXPR> log(const et4ad::ExpressionBase<REAL_T, EXPR>& expr);
}

namespace et4ad {

    /**
     * Expression template to compute the log of an expression template.
     * @param expr
     */
    template <class REAL_T, class EXPR>
    class Log : public ExpressionBase<REAL_T, Log<REAL_T, EXPR> > {
    public:

        Log(const ExpressionBase<REAL_T, EXPR>& expr)
        : expr_m(expr.Cast()), value_m(expr_m.GetValue()), value2_m(REAL_T(1.0) / value_m) {
        }

        inline const REAL_T GetValue() const {
            return std::log(value_m);
        }

        operator REAL_T() {
            return this->GetValue();
        }

        operator REAL_T()const {
            return this->GetValue();
        }

        inline const REAL_T Derivative(const uint32_t id, bool &found) const {
            return expr_m.Derivative(id, found) / value_m;
            //            REAL_T dx = expr_m.Derivative(id, found);
            //
            //            if (found) {
            //                return (dx) / value_m;
            //            } else {
            //                return 0.0;
            //            }
        }

        inline const REAL_T Derivative(const uint32_t &id) const {
            return expr_m.Derivative(id) * value2_m;
            //            REAL_T dx = expr_m.Derivative(id, found);
            //
            //            if (found) {
            //                return (dx) / value_m;
            //            } else {
            //                return 0.0;
            //            }
        }

        inline void PushIds(et4ad::IDSet &ids) const {
            expr_m.PushIds(ids);
        }

        inline void PushStatements(std::vector<Statement<REAL_T> > &storage) const {
            expr_m.PushStatements(storage);
            storage.push_back(Statement<REAL_T > (LOG));
        }

        inline void PushAll(et4ad::VariableStorage<REAL_T> &storage) const {
            expr_m.PushAll(storage);
        }


    private:
        const EXPR& expr_m;
        const REAL_T value_m;
        const REAL_T value2_m;
    };

}

namespace std {

    /**
     * Override for the log function in namespace std.
     * 
     * @param expr
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const et4ad::Log<REAL_T, EXPR> log(const et4ad::ExpressionBase<REAL_T, EXPR>& expr) {

        return et4ad::Log<REAL_T, EXPR > (expr.Cast());
    }

}

#endif	/* LOG_HPP */

