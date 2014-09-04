/* 
 * File:   Expression.hpp
 * Author: matthewsupernaw
 *
 * Created on June 18, 2014, 11:16 AM
 */

#ifndef EXPRESSION_HPP
#define	EXPRESSION_HPP
#include <stdint.h>
#include <complex>
#include "IDset.hpp"
#include "Statement.hpp"

namespace et4ad {

    /**
     * Base class for expression types.
     */
    template<class REAL_T, class A>
    struct ExpressionBase {
        uint32_t id_m;

        ExpressionBase() : id_m(0) {

        }

        ExpressionBase(const uint32_t & id) : id_m(id) {

        }

        /**
         * Cast this expression template to it's child
         * representation.
         * 
         * @return 
         */
        const A & Cast() const {
            return static_cast<const A&> (*this);
        }

        inline const REAL_T GetValue() const {
            return Cast().GetValue();
        }

        operator REAL_T() {
            return Cast().GetValue();
        }

        operator REAL_T()const {
            return Cast().GetValue();
        }

        inline const uint32_t GetId() const {
            return id_m;
        }

        /**
         * Compute or get the stored derivative with respect to the unique 
         * identifier id.
         * 
         * @param id
         * @param found
         * @return 
         */
        const REAL_T Derivative(const uint32_t &id, bool &found) const {
            return Cast().Derivative(id, found);
        }

        /**
         * Compute or get the stored derivative with respect to the unique 
         * identifier id.
         * 
         * @param id
         * @param found
         * @return 
         */
        inline const REAL_T Derivative(const uint32_t & id) const {
            return Cast().Derivative(id);
        }

        /**
         * Complex Step value. Used for complex step derivative computations.
         * Complex step differentiation is a technique that employs complex 
         * arithmetic to obtain the numerical value of the first derivative of 
         * a real valued analytic function of a real variable, avoiding the 
         * loss of precision inherent in traditional finite differences.
         * 
         * For Example:
         * 
         * F(x+ih) = cos(x+ih)
         * dF/dx+ih = F(x+ih).imag()/h
         * 
         * @param id
         * @param h
         * @return 
         */
        inline const std::complex<REAL_T> ComplexStepValue(const uint32_t & id, REAL_T h = REAL_T(0.00000000000001)) const {
            return Cast().ComplexStepValue(id, h);
        }

        /**
         * Push the ids for independent variables to a storage object.
         * 
         * @param storage
         */
        inline void PushIds(et4ad::IDSet & ids) const {
            Cast().PushIds(ids);
        }

        inline void PushStatements(std::vector<Statement<REAL_T> > &storage) const {
            Cast().PushStatements(storage);
        }


        //
        //
        //
        //    private:
        //

        ExpressionBase& operator=(const ExpressionBase & exp) const {
            return *this;
        }
    };

    template<class REAL_T, class T, class TT>
    inline const int operator==(const et4ad::ExpressionBase<REAL_T, T>& lhs, const et4ad::ExpressionBase<REAL_T, TT>& rhs) {
        return lhs.GetValue() == rhs.GetValue();
    }

    template<class REAL_T, class T, class TT>
    inline const int operator!=(const et4ad::ExpressionBase<REAL_T, T>& lhs, const et4ad::ExpressionBase<REAL_T, TT>& rhs) {
        return lhs.GetValue() != rhs.GetValue();
    }

    template<class REAL_T, class T, class TT>
    inline const int operator<(const et4ad::ExpressionBase<REAL_T, T>& lhs, const et4ad::ExpressionBase<REAL_T, TT>& rhs) {
        return lhs.GetValue() < rhs.GetValue();
    }

    template<class REAL_T, class T, class TT>
    inline const int operator>(const et4ad::ExpressionBase<REAL_T, T>& lhs, const et4ad::ExpressionBase<REAL_T, TT>& rhs) {
        return lhs.GetValue() > rhs.GetValue();
    }

    template<class REAL_T, class T, class TT>
    inline const int operator<=(const et4ad::ExpressionBase<REAL_T, T>& lhs, const et4ad::ExpressionBase<REAL_T, TT>& rhs) {
        return lhs.GetValue() <= rhs.GetValue();
    }

    template<class REAL_T, class T, class TT>
    inline const int operator>=(const et4ad::ExpressionBase<REAL_T, T>& lhs, const et4ad::ExpressionBase<REAL_T, TT>& rhs) {
        return lhs.GetValue() >= rhs.GetValue();
    }

    template<class REAL_T, class T>
    inline const int operator==(const REAL_T &lhs, const et4ad::ExpressionBase<REAL_T, T>& rhs) {
        return lhs == rhs.GetValue();
    }

    template<class REAL_T, class T>
    inline const int operator!=(const REAL_T &lhs, const et4ad::ExpressionBase<REAL_T, T>& rhs) {
        return lhs != rhs.GetValue();
    }

    template<class REAL_T, class T>
    inline const int operator<(const REAL_T &lhs, const et4ad::ExpressionBase<REAL_T, T>& rhs) {
        return lhs < rhs.GetValue();
    }

    template<class REAL_T, class T>
    inline const int operator>(const REAL_T &lhs, const et4ad::ExpressionBase<REAL_T, T>& rhs) {
        return lhs > rhs.GetValue();
    }

    template<class REAL_T, class T>
    inline const int operator<=(const REAL_T &lhs, const et4ad::ExpressionBase<REAL_T, T>& rhs) {
        return lhs <= rhs.GetValue();
    }

    template<class REAL_T, class T>
    inline const int operator>=(const REAL_T &lhs, const et4ad::ExpressionBase<REAL_T, T>& rhs) {
        return lhs >= rhs.GetValue();
    }

    template<class REAL_T, class T>
    inline const int operator==(const et4ad::ExpressionBase<REAL_T, T>& lhs, const REAL_T &rhs) {
        return lhs.GetValue() == rhs;
    }

    template<class REAL_T, class T>
    inline const int operator!=(const et4ad::ExpressionBase<REAL_T, T>& lhs, const REAL_T &rhs) {
        return lhs.GetValue() != rhs;
    }

    template<class REAL_T, class T>
    inline const int operator<(const et4ad::ExpressionBase<REAL_T, T>& lhs, const REAL_T &rhs) {
        return lhs.GetValue() <= rhs;
    }

    template<class REAL_T, class T>
    inline const int operator>(const et4ad::ExpressionBase<REAL_T, T>& lhs, const REAL_T &rhs) {
        return lhs.GetValue() > rhs;
    }

    template<class REAL_T, class T>
    inline const int operator<=(const et4ad::ExpressionBase<REAL_T, T>& lhs, const REAL_T &rhs) {
        return lhs.GetValue() <= rhs;
    }

    template<class REAL_T, class T>
    inline const int operator>=(const et4ad::ExpressionBase<REAL_T, T>& lhs, const REAL_T &rhs) {
        return lhs.GetValue() >= rhs;
    }
}
namespace std{
    template<class T,class A>
    bool isfinite(et4ad::ExpressionBase<T,A>& v){
        return false;
    }
    
    template<class T,class A>
    bool isinf(et4ad::ExpressionBase<T,A>& v){
        return false;
    }
}

#endif	/* EXPRESSION_HPP */

