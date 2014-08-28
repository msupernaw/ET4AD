/* 
 * File:   Fabs.hpp
 * Author: matthewsupernaw
 *
 * Created on June 18, 2014, 12:53 PM
 */

#ifndef ET4AD_FABS_HPP
#define	ET4AD_FABS_HPP

#include <cmath>
#include "Expression.hpp"

namespace et4ad {

    /**
     * Expression template for handling the absolute value of a expression 
     * template.
     * @param expr
     */
    template <class REAL_T, class EXPR>
    class Fabs : public ExpressionBase<REAL_T, Fabs<REAL_T, EXPR> > {
    public:

        inline explicit Fabs(const ExpressionBase<REAL_T, EXPR>& expr)
        : expr_m(expr.Cast()) {
        }

        inline const REAL_T GetValue() const {
            return std::fabs(expr_m.GetValue());
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
                return (dx * v) / std::fabs(v);
            } else {
                return 0.0;
            }
        }

        inline const REAL_T Derivative(const uint32_t &id) const {
            return (expr_m.Derivative(id) * expr_m.GetValue()) / std::fabs(expr_m.GetValue());
        }

        inline void Derivative(const uint32_t& id, REAL_T& dx) const {
            expr_m.Derivative(id, dx);
            dx *= expr_m.GetValue();
            dx /= std::fabs(expr_m.GetValue());
        }

        inline void Derivative(std::vector<REAL_T>& gradient) const {
            expr_m.Derivative(gradient);
            for (int i = 0; i < gradient.size(); i++) {
                gradient[i] *= expr_m.GetValue();
                gradient[i] /= std::fabs(expr_m.GetValue());
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
            storage.push_back(Statement<REAL_T > (ABS));
        }

        inline void PushAll(et4ad::VariableStorage<REAL_T> &storage) const {
            expr_m.PushAll(storage);
        }

    private:
        const EXPR& expr_m;
    };


}

#endif	/* FABS_HPP */

