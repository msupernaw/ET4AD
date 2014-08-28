/* 
 * File:   Exp.hpp
 * Author: matthewsupernaw
 *
 * Created on June 18, 2014, 12:37 PM
 */

#ifndef ET4AD_EXP_HPP
#define	ET4AD_EXP_HPP

#include <cmath>
#include "Expression.hpp"

namespace et4ad {

    /**
     * Expression template to compute e raised to a expression template.
     * @param expr
     */
    template <class REAL_T, class EXPR>
    class Exp : public ExpressionBase<REAL_T, Exp<REAL_T, EXPR> > {
    public:
        const EXPR& expr_m;
        const REAL_T value_m;

        inline explicit Exp(const ExpressionBase<REAL_T, EXPR>& expr)
        : expr_m(expr.Cast()), value_m(Compute(expr.GetValue())) {
        }

        inline const REAL_T GetValue() const {
            return value_m;
        }

        operator REAL_T() {
            return this->GetValue();
        }

        operator REAL_T()const {
            return this->GetValue();
        }

        inline const REAL_T Compute(const REAL_T &x) {
            return std::exp(x);
        }

        inline const REAL_T Derivative(const uint32_t id, bool &found) const {

            return expr_m.Derivative(id, found) * (value_m);

            //            bool f = false;
            //            REAL_T dx = expr_m.Derivative(id, f);
            //            if (f) {
            //                found = true;
            //                return dx * (value_m);
            //            } else {
            //                return 0.0;
            //            }
        }

        inline void Derivative(const uint32_t& id, REAL_T& dx) const {
            expr_m.Derivative(id, dx);
            dx *= this->GetValue();
        }

        inline const REAL_T Derivative(const uint32_t &id) const {
            return expr_m.Derivative(id) * this->GetValue();

            //            bool f = false;
            //            REAL_T dx = expr_m.Derivative(id, f);
            //            if (f) {
            //                found = true;
            //                return dx * (value_m);
            //            } else {
            //                return 0.0;
            //            }
        }

        inline void Derivative(std::vector<REAL_T>& gradient) const {
            expr_m.Derivative(gradient);
            for (int i = 0; i < gradient.size(); i++) {
                gradient[i] *= this->GetValue();
            }
        }

        inline size_t Size() const {
            return expr_m.Size();
        }

        inline void PushIds(et4ad::VariableStorage<REAL_T> &storage) const {
            expr_m.PushIds(storage);
        }

        inline void PushIds(et4ad::IDSet &ids) const {
            expr_m.PushIds(ids);
        }

        inline void PushStatements(std::vector<Statement<REAL_T> > &storage) const {
            expr_m.PushStatements(storage);
            storage.push_back(Statement<REAL_T > (EXP));
        }

        inline void PushAll(et4ad::VariableStorage<REAL_T> &storage) const {
            expr_m.PushAll(storage);
        }


    private:

    };

}



#endif	/* EXP_HPP */

