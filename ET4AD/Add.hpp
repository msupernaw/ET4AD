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

        inline explicit Add(const ExpressionBase<REAL_T, LHS>& lhs, const ExpressionBase<REAL_T, RHS>& rhs)
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

        inline void Derivative(const uint32_t& id, REAL_T & dx) const {
            lhs_m.Derivative(id, dx);
            dx += rhs_m.Derivative(id);
        }

        inline const REAL_T Derivative(const uint32_t & id) const {
            return lhs_m.Derivative(id) + rhs_m.Derivative(id);
        }

        inline void Derivative(std::vector<REAL_T>& gradient) const {
            lhs_m.Derivative(gradient);
            std::vector<REAL_T> tmp;
            rhs_m.Derivative(tmp);

            tmp.size() > gradient.size() ? gradient.resize(tmp.size()) : tmp.resize(gradient.size());

            for (int i = 0; i < gradient.size(); i++) {
                gradient[i] += tmp[i];
            }
        }

        inline size_t Size() const {
            return lhs_m.Size() > rhs_m.Size() ? lhs_m.Size() : rhs_m.Size();
        }

        inline void PushIds(et4ad::VariableStorage<REAL_T> &storage) const {
            lhs_m.PushIds(storage);
            rhs_m.PushIds(storage);
        }

        inline void PushIds(et4ad::IDSet & ids) const {
            lhs_m.PushIds(ids);
            rhs_m.PushIds(ids);
        }

        inline void PushStatements(std::vector<Statement<REAL_T> > &storage) const {
            lhs_m.PushStatements(storage);
            rhs_m.PushStatements(storage);
            storage.push_back(Statement<REAL_T > (PLUS));
        }

        inline void PushAll(et4ad::VariableStorage<REAL_T> &storage) const {
            lhs_m.PushAll(storage);
            rhs_m.PushAll(storage);
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

        inline explicit AddConstant(const ExpressionBase<REAL_T, LHS>& lhs, const REAL_T & rhs)
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

        inline void Derivative(const uint32_t& id, REAL_T & dx) const {
            lhs_m.Derivative(id, dx);

        }

        inline const REAL_T Derivative(const uint32_t & id) const {
            return lhs_m.Derivative(id);
        }

        inline void Derivative(std::vector<REAL_T>& gradient) const {
            lhs_m.Derivative(gradient);
        }

        inline size_t Size() const {
            return lhs_m.Size();
        }

        inline void PushIds(et4ad::VariableStorage<REAL_T> &storage) const {
            lhs_m.PushIds(storage);
        }

        inline void PushIds(et4ad::IDSet & ids) const {
            lhs_m.PushIds(ids);
        }

        inline void PushStatements(std::vector<Statement<REAL_T> > &storage) const {
            lhs_m.PushStatements(storage);
            storage.push_back(Statement<REAL_T > (CONSTANT, rhs_m));
            storage.push_back(Statement<REAL_T > (PLUS));
        }

        inline void PushAll(et4ad::VariableStorage<REAL_T> &storage) const {
            lhs_m.PushAll(storage);
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

        inline explicit ConstantAdd(const REAL_T& lhs, const ExpressionBase<REAL_T, RHS>& rhs)
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

        inline void Derivative(const uint32_t& id, REAL_T & dx) const {
            rhs_m.Derivative(id, dx);
        }

        inline const REAL_T Derivative(const uint32_t & id) const {
            return rhs_m.Derivative(id);
        }

        inline void Derivative(std::vector<REAL_T>& gradient) const {
            rhs_m.Derivative(gradient);
        }

        inline size_t Size() const {
            return rhs_m.Size();
        }

        inline void PushIds(et4ad::VariableStorage<REAL_T> &storage) const {
            rhs_m.PushIds(storage);
        }

        inline void PushIds(et4ad::IDSet & ids) const {
            rhs_m.PushIds(ids);
        }

        inline void PushStatements(std::vector<Statement<REAL_T> > &storage) const {
            storage.push_back(Statement<REAL_T > (CONSTANT, lhs_m));
            rhs_m.PushStatements(storage);
            storage.push_back(Statement<REAL_T > (PLUS));
        }

        inline void PushAll(et4ad::VariableStorage<REAL_T> &storage) const {
            rhs_m.PushAll(storage);
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
