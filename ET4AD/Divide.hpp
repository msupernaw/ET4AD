/* 
 * File:   Divide.hpp
 * Author: matthewsupernaw
 *
 * Created on June 18, 2014, 11:54 AM
 */

#ifndef ET4AD_DIVIDE_HPP
#define	ET4AD_DIVIDE_HPP

#include "Expression.hpp"
#include "Constant.hpp"

namespace et4ad {

    template <class REAL_T, class LHS, class RHS>
    class Divide : public ExpressionBase<REAL_T, Divide<REAL_T, LHS, RHS> > {
    public:
        const LHS& lhs_m;
        const RHS& rhs_m;
        const REAL_T lhs_value_m;
        const REAL_T rhs_value_m;
        const REAL_T mult;

        Divide(const ExpressionBase<REAL_T, LHS>& lhs, const ExpressionBase<REAL_T, RHS>& rhs)
        : lhs_m(lhs.Cast()), rhs_m(rhs.Cast()), lhs_value_m(lhs_m.GetValue()), rhs_value_m(rhs_m.GetValue()), mult(1.0 / (rhs_value_m)) {


        }

        inline const REAL_T GetValue() const {
            return lhs_value_m*mult;
        }

        operator REAL_T() {
            return this->GetValue();
        }

        operator REAL_T()const {
            return this->GetValue();
        }

        inline const REAL_T Derivative(const uint32_t id, bool &found) const {

            return (lhs_m.Derivative(id, found) * rhs_value_m -
                    lhs_value_m * rhs_m.Derivative(id, found)) / (rhs_value_m * rhs_value_m);
        }

        inline const REAL_T Derivative(const uint32_t &id) const {
            //             REAL_T d = rhs_value_m*lhs_m.Derivative(id);
            //             d-=rhs_m.Derivative(id)*lhs_value_m;
            //             d*=mult;
            //             return d;
            return ( rhs_value_m * lhs_m.Derivative(id) -
                    rhs_m.Derivative(id) * lhs_value_m) *mult*mult; /// (rhs_value_m * rhs_value_m);
        }

        inline void PushIds(et4ad::VariableStorage<REAL_T> &storage) const {
            lhs_m.PushIds(storage);
            rhs_m.PushIds(storage);
        }

        inline void PushIds(et4ad::IDSet &ids) const {
            lhs_m.PushIds(ids);
            rhs_m.PushIds(ids);
        }

        inline void PushStatements(std::vector<Statement<REAL_T> > &storage) const {
            lhs_m.PushStatements(storage);
            rhs_m.PushStatements(storage);
            storage.push_back(Statement<REAL_T > (DIVIDE));
        }

        inline void PushAll(et4ad::VariableStorage<REAL_T> &storage) const {
            lhs_m.PushAll(storage);
            rhs_m.PushAll(storage);
        }


    private:

    };

    /**
     * Operator for addition of two expression templates.
     * @param a
     * @param b
     * @return 
     */
    template <class REAL_T, class LHS, class RHS>
    inline Divide<REAL_T, LHS, RHS> operator/(const ExpressionBase<REAL_T, LHS>& a,
    const ExpressionBase<REAL_T, RHS>& b) {
        return Divide<REAL_T, LHS, RHS > (a.Cast(), b.Cast());
    }

    template <class REAL_T, class LHS>
    class DivideConstant : public ExpressionBase<REAL_T, DivideConstant<REAL_T, LHS> > {
    public:

        DivideConstant(const ExpressionBase<REAL_T, LHS>& lhs, const REAL_T& rhs)
        : lhs_m(lhs.Cast()), rhs_m(rhs), value_m(lhs_m.GetValue() / rhs_m) {


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

        inline const REAL_T Derivative(const uint32_t id, bool &found) const {
            return (lhs_m.Derivative(id, found) * rhs_m) / (rhs_m * rhs_m);
        }

        inline const REAL_T Derivative(const uint32_t &id) const {
            return (lhs_m.Derivative(id) * rhs_m) / (rhs_m * rhs_m);
        }

        inline void PushIds(et4ad::VariableStorage<REAL_T> &storage) const {
            lhs_m.PushIds(storage);
        }

        inline void PushIds(et4ad::IDSet &ids) const {
            lhs_m.PushIds(ids);
        }

        inline void PushStatements(std::vector<Statement<REAL_T> > &storage) const {
            lhs_m.PushStatements(storage);
            storage.push_back(Statement<REAL_T > (CONSTANT, rhs_m));
            storage.push_back(Statement<REAL_T > (DIVIDE));
        }

        inline void PushAll(et4ad::VariableStorage<REAL_T> &storage) const {
            lhs_m.PushAll(storage);
        }


    private:

        const LHS& lhs_m;
        const REAL_T& rhs_m;
        const REAL_T value_m;
    };


    /**
     * Operator for adding a expression templates to a constant .
     * @param lhs
     * @param rhs
     * @return 
     */

    /**
     * Operator for adding a expression templates to a constant .
     * @param lhs
     * @param rhs
     * @return 
     */
    template <class LHS, class REAL_T>
    inline const DivideConstant<REAL_T, LHS > operator/(const ExpressionBase<REAL_T, LHS>& lhs,
    const REAL_T& rhs) {
        return DivideConstant<REAL_T, LHS > (lhs.Cast(), rhs);
    }

    template <class REAL_T, class RHS>
    class ConstantDivide : public ExpressionBase<REAL_T, ConstantDivide<REAL_T, RHS> > {
    public:

        ConstantDivide(const REAL_T& lhs, const ExpressionBase<REAL_T, RHS>& rhs)
        : lhs_m(lhs), rhs_m(rhs.Cast()), value_m(lhs_m / rhs_m.GetValue()) {


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

        inline const REAL_T Derivative(const uint32_t id, bool &found) const {
            return (0 - lhs_m * rhs_m.Derivative(id, found)) / (rhs_m.GetValue() * rhs_m.GetValue());
        }

        inline const REAL_T Derivative(const uint32_t &id) const {
            return (0 - lhs_m * rhs_m.Derivative(id)) / (rhs_m.GetValue() * rhs_m.GetValue());
        }

        inline void PushIds(et4ad::VariableStorage<REAL_T> &storage) const {
            rhs_m.PushIds(storage);
        }

        inline void PushIds(et4ad::IDSet &ids) const {
            rhs_m.PushIds(ids);
        }

        inline void PushStatements(std::vector<Statement<REAL_T> > &storage) const {
            storage.push_back(Statement<REAL_T > (CONSTANT, lhs_m));
            rhs_m.PushStatements(storage);
            storage.push_back(Statement<REAL_T > (DIVIDE));
        }

        inline void PushAll(et4ad::VariableStorage<REAL_T> &storage) const {
            rhs_m.PushAll(storage);
        }


    private:

        const REAL_T& lhs_m;
        const RHS& rhs_m;
        const REAL_T value_m;
    };

    //    /**
    //     * Operator for adding a constant value to a expression templates.
    //     * @param lhs
    //     * @param rhs
    //     * @return 
    //     */

    template <class REAL_T, class RHS >
    inline const ConstantDivide< REAL_T, RHS > operator/(const REAL_T& lhs,
    const ExpressionBase<REAL_T, RHS>& rhs) {
        return ConstantDivide<REAL_T, RHS > (lhs, rhs.Cast());
    }

}

#endif	/* DIVIDE_HPP */

