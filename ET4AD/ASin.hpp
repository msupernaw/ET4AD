/* 
 * File:   ASin.hpp
 * Author: matthewsupernaw
 *
 * Created on June 18, 2014, 12:23 PM
 */

#ifndef ET4AD_ASIN_HPP
#define	ET4AD_ASIN_HPP

#include <cmath>
#include "Expression.hpp"

namespace et4ad {

    /**
     * Expression template for computing the inverse Sine of an expression 
     * template.
     * 
     * @param expr
     */
    template <class REAL_T, class EXPR>
    class ASin : public ExpressionBase<REAL_T, ASin<REAL_T, EXPR> > {
    public:

        inline explicit ASin(const ExpressionBase<REAL_T, EXPR>& expr)
        : expr_m(expr.Cast()) {
        }

        inline const REAL_T GetValue() const {
            return std::asin(expr_m.GetValue());
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
                return (dx * static_cast<REAL_T> (1.0) / std::pow((static_cast<REAL_T> (1.0) - std::pow(v, static_cast<REAL_T> (2.0))), static_cast<REAL_T> (0.5)));
            } else {
                return 0.0;
            }
        }

        inline void Derivative(const uint32_t& id, REAL_T& dx) const {
            expr_m.Derivative(id, dx);
            dx *= (static_cast<REAL_T> (1.0) / std::pow((static_cast<REAL_T> (1.0) - std::pow(expr_m.GetValue(), static_cast<REAL_T> (2.0))), static_cast<REAL_T> (0.5)));

        }

        inline const REAL_T Derivative(const uint32_t &id) const {
            return (expr_m.Derivative(id) * static_cast<REAL_T> (1.0) / std::pow((static_cast<REAL_T> (1.0) - std::pow(expr_m.GetValue(), static_cast<REAL_T> (2.0))), static_cast<REAL_T> (0.5)));
        }

        inline void Derivative(std::vector<REAL_T>& gradient) const {
            expr_m.Derivative(gradient);
            for (int i = 0; i < gradient.size(); i++) {
                gradient[i] *= (static_cast<REAL_T> (1.0) / std::pow((static_cast<REAL_T> (1.0) - std::pow(expr_m.GetValue(), static_cast<REAL_T> (2.0))), static_cast<REAL_T> (0.5)));
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
            storage.push_back(Statement<REAL_T > (ASIN));
        }

        inline void PushAll(et4ad::VariableStorage<REAL_T> &storage) const {
            expr_m.PushAll(storage);
        }


    private:
        const EXPR& expr_m;
    };


}

#endif	/* ASIN_HPP */

