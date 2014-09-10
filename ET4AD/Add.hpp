/* 
 * File:   Addition.hpp
 * Author: matthewsupernaw
 *
 * Created on June 18, 2014, 11:36 AM
 */

#ifndef ET4AD_ADD_HPP
#define	ET4AD_ADD_HPP
#include "Expression.hpp"
#include "Constant.hpp"

namespace et4ad {

    template <class REAL_T, class LHS, class RHS>
    struct Add : public ExpressionBase<REAL_T, Add<REAL_T, LHS, RHS> > {

        Add(const ExpressionBase<REAL_T, LHS>& lhs, const ExpressionBase<REAL_T, RHS>& rhs)
        : /*value_m(lhs.Cast().GetValue()+rhs.Cast().GetValue()),*/ lhs_m(lhs.Cast()), rhs_m(rhs.Cast()) {


        }

        inline const REAL_T GetValue() const {
            return lhs_m.GetValue() + rhs_m.GetValue(); //value_m;
        }

        operator REAL_T() {
            return this->GetValue();
        }

        operator REAL_T()const {
            return this->GetValue();
        }

        inline const REAL_T Derivative(const uint32_t id, bool &found) const {
            return lhs_m.Derivative(id, found) + rhs_m.Derivative(id, found);
        }

        inline const REAL_T Derivative(const uint32_t & id) const {
            return lhs_m.Derivative(id) + rhs_m.Derivative(id);
        }

        inline const std::complex<REAL_T> ComplexStepValue(const uint32_t & id, REAL_T h = REAL_T(0.00000000000001)) const {
            return lhs_m.ComplexStepValue(id, h) + rhs_m.ComplexStepValue(id, h);
        }

        inline void PushIds(et4ad::IDSet & ids) const {
            lhs_m.PushIds(ids);
            rhs_m.PushIds(ids);
        }

        inline void PushStatements(std::vector<Statement<REAL_T> > &storage) const {
            lhs_m.PushStatements(storage);
            rhs_m.PushStatements(storage);
            storage.push_back(Statement<REAL_T > (PLUS, GetValue()));
        }



        const LHS& lhs_m;
        const RHS& rhs_m;
        //        const REAL_T value_m;
        //        const REAL_T value_m;
    };

    /**
     * Operator for addition of two expression templates.
     * @param a
     * @param b
     * @return 
     */
    template <class REAL_T, class LHS, class RHS>
    inline Add<REAL_T, LHS, RHS> operator+(const ExpressionBase<REAL_T, LHS>& a,
            const ExpressionBase<REAL_T, RHS>& b) {
        return Add<REAL_T, LHS, RHS > (a.Cast(), b.Cast());
    }

    template <class REAL_T, class LHS>
    struct AddConstant : public ExpressionBase<REAL_T, AddConstant<REAL_T, LHS> > {

        AddConstant(const ExpressionBase<REAL_T, LHS>& lhs, const REAL_T & rhs)
        : lhs_m(lhs.Cast()), rhs_m(rhs) {


        }

        inline const REAL_T GetValue() const {
            return lhs_m.GetValue() + rhs_m;
        }

        operator REAL_T() {
            return this->GetValue();
        }

        operator REAL_T()const {
            return this->GetValue();
        }

        inline const REAL_T Derivative(const uint32_t id, bool &found) const {
            return lhs_m.Derivative(id, found);
        }

        inline const REAL_T Derivative(const uint32_t & id) const {
            return lhs_m.Derivative(id);
        }

        inline const std::complex<REAL_T> ComplexStepValue(const uint32_t & id, REAL_T h = REAL_T(0.00000000000001)) const {
            return lhs_m.ComplexStepValue(id, h) + std::complex<REAL_T>(rhs_m);
        }

        inline void PushIds(et4ad::IDSet & ids) const {
            lhs_m.PushIds(ids);
        }

        inline void PushStatements(std::vector<Statement<REAL_T> > &storage) const {
            lhs_m.PushStatements(storage);
            storage.push_back(Statement<REAL_T > (CONSTANT, rhs_m));
            storage.push_back(Statement<REAL_T > (PLUS, GetValue()));
        }



        const LHS& lhs_m;
        const REAL_T& rhs_m;
        //        const REAL_T value_m;
    };

    /**
     * Operator for adding a expression templates to a constant .
     * @param lhs
     * @param rhs
     * @return 
     */
    template <class LHS, class REAL_T>
    inline const AddConstant<REAL_T, LHS> operator+(const ExpressionBase<REAL_T, LHS>& lhs,
            const REAL_T& rhs) {
        return AddConstant<REAL_T, LHS > (lhs.Cast(), rhs);
    }

    template <class REAL_T, class RHS>
    struct ConstantAdd : public ExpressionBase<REAL_T, ConstantAdd<REAL_T, RHS> > {

        ConstantAdd(const REAL_T& lhs, const ExpressionBase<REAL_T, RHS>& rhs)
        : lhs_m(lhs), rhs_m(rhs.Cast()), value_m(lhs_m + rhs_m.GetValue()) {


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
            return rhs_m.Derivative(id, found);
        }

        inline const REAL_T Derivative(const uint32_t & id) const {
            return rhs_m.Derivative(id);
        }

        inline const std::complex<REAL_T> ComplexStepValue(const uint32_t & id, REAL_T h = REAL_T(0.00000000000001)) const {
            return std::complex<REAL_T>(lhs_m) + rhs_m.ComplexStepValue(id, h);
        }

        inline void PushIds(et4ad::IDSet & ids) const {
            rhs_m.PushIds(ids);
        }

        inline void PushStatements(std::vector<Statement<REAL_T> > &storage) const {
            storage.push_back(Statement<REAL_T > (CONSTANT, lhs_m));
            rhs_m.PushStatements(storage);
            storage.push_back(Statement<REAL_T > (PLUS, GetValue()));
        }



        const REAL_T& lhs_m;
        const RHS& rhs_m;
        const REAL_T value_m;
    };

    /**
     * Operator for adding a constant value to a expression templates.
     * @param lhs
     * @param rhs
     * @return 
     */
    template <class REAL_T, class RHS >
    inline const ConstantAdd< REAL_T, RHS > operator+(const REAL_T& lhs,
            const ExpressionBase<REAL_T, RHS> &rhs) {
        return ConstantAdd<REAL_T, RHS > (lhs, rhs.Cast());
    }

}




#endif	/* ADDITION_HPP */

