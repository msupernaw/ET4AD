/* 
 * File:   Ceil.hpp
 * Author: matthewsupernaw
 *
 * Created on June 18, 2014, 12:56 PM
 */

#ifndef ET4AD_CEIL_HPP
#define	ET4AD_CEIL_HPP

#include <cmath>
#include "Expression.hpp"


namespace et4ad {

    /**
     * Expression template for handling the ceiling of an expression template.
     * @param expr
     */
    template <class REAL_T, class EXPR>
    class Ceil : public ExpressionBase<REAL_T, Ceil<REAL_T, EXPR> > {
    public:

        inline explicit Ceil(const ExpressionBase<REAL_T, EXPR>& expr)
        : expr_m(expr.Cast()) {
        }

        inline const REAL_T GetValue() const {
            return std::ceil(expr_m.GetValue());
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

        inline void Derivative(const uint32_t& id, REAL_T& dx) const {
            dx = 0.0;
        }

        inline const REAL_T Derivative(const uint32_t &id) const {
            return 0.0;
        }

        inline void Derivative(std::vector<REAL_T>& gradient) const {
            gradient.resize(0);
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
            storage.push_back(Statement<REAL_T > (CEIL));
        }

        inline void PushAll(et4ad::VariableStorage<REAL_T> &storage) const {
            expr_m.PushAll(storage);
        }


    private:
        const EXPR& expr_m;
    };



}


#endif	/* CEIL_HPP */

