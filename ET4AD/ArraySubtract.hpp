/* 
 * File:   ArraySubtract.hpp
 * Author: matthewsupernaw
 *
 * Created on August 25, 2014, 1:17 PM
 */

#ifndef ARRAYSUBTRACT_HPP
#define	ARRAYSUBTRACT_HPP
#include "Array.hpp"

namespace et4ad {

    template<class T, class T2, class LHS, class RHS>
    struct ArraySubtract : ArrayExpression<T, ArraySubtract<T, T2, LHS, RHS> > {
        const LHS& lhs_m;
        const RHS& rhs_m;

        inline explicit ArraySubtract(const ArrayExpression<T, LHS>& lhs, const ArrayExpression<T2, RHS> & rhs) : lhs_m(lhs.Cast()), rhs_m(rhs.Cast()) {


#ifdef ENABLE_BOUNDS_CHECKING
            for (int i = 0; i < 7; i++) {
                assert(lhs_m.Size(i) == rhs_m.Size(i));
            }
#endif

        }

        inline const size_t Size(const int32_t & dimension) const {
            return lhs_m.Size(dimension) < rhs_m.Size(dimension) ? lhs_m.Size(dimension) : rhs_m.Size(dimension);
        }

        inline const size_t Dimensions() const {
            return lhs_m.Dimensions() < rhs_m.Dimensions() ? lhs_m.Dimensions() : rhs_m.Dimensions();
        }

        inline const T operator()(const uint32_t & i) const {
            return lhs_m(i) - rhs_m(i);
        }

        inline const T operator()(const uint32_t& i, const uint32_t & j) const {
            return lhs_m(i, j) - rhs_m(i, j);
        }

        inline const T operator()(const uint32_t& i, const uint32_t & j, const uint32_t & k) const {
            return lhs_m(i, j, k) - rhs_m(i, j, k);
        }

        inline const T operator()(const uint32_t& i, const uint32_t & j, const uint32_t & k, const uint32_t & l)const {
            return lhs_m(i, j, k, l) - rhs_m(i, j, k, l);
        }

        inline const T operator()(const uint32_t& i, const uint32_t & j, const uint32_t & k, const uint32_t & l, const uint32_t & m) const {
            return lhs_m(i, j, k, j, m) - rhs_m(i, j, k, l, m);
        }

        inline const T operator()(const uint32_t& i, const uint32_t & j, const uint32_t & k, const uint32_t & l, const uint32_t & m, const uint32_t & n) const {
            return lhs_m(i, j, k, j, m, n) - rhs_m(i, j, k, l, m, n);
        }

        inline const T operator()(const uint32_t& i, const uint32_t & j, const uint32_t & k, const uint32_t & l, const uint32_t & m, const uint32_t & n, const uint32_t & o) const {
            return lhs_m(i, j, k, j, m, n, o) - rhs_m(i, j, k, l, m, n, o);
        }




    };

    template<class T, class T2, class LHS>
    struct ArraySubtractScalar : ArrayExpression<T, ArraySubtractScalar<T, T2, LHS> > {
        const LHS& lhs_m;
        const T2& rhs_m;

        inline explicit ArraySubtractScalar(const ArrayExpression<T, LHS>& lhs, const T2 & rhs) : lhs_m(lhs.Cast()), rhs_m(rhs) {



        }

        inline const size_t Size(const int32_t & dimension) const {
            return lhs_m.Size(dimension);
        }

        inline const size_t Dimensions() const {
            return lhs_m.Dimensions();
        }

        inline const T operator()(const uint32_t & i) const {
            return lhs_m(i) - rhs_m;
        }

        inline const T operator()(const uint32_t& i, const uint32_t & j) const {
            return lhs_m(i, j) - rhs_m;
        }

        inline const T operator()(const uint32_t& i, const uint32_t & j, const uint32_t & k) const {
            return lhs_m(i, j, k) - rhs_m;
        }

        inline const T operator()(const uint32_t& i, const uint32_t & j, const uint32_t & k, const uint32_t & l)const {
            return lhs_m(i, j, k, l) - rhs_m;
        }

        inline const T operator()(const uint32_t& i, const uint32_t & j, const uint32_t & k, const uint32_t & l, const uint32_t & m) const {
            return lhs_m(i, j, k, j, m) - rhs_m;
        }

        inline const T operator()(const uint32_t& i, const uint32_t & j, const uint32_t & k, const uint32_t & l, const uint32_t & m, const uint32_t & n) const {
            return lhs_m(i, j, k, j, m, n) - rhs_m;
        }

        inline const T operator()(const uint32_t& i, const uint32_t & j, const uint32_t & k, const uint32_t & l, const uint32_t & m, const uint32_t & n, const uint32_t & o) const {
            return lhs_m(i, j, k, j, m, n, o) - rhs_m;
        }




    };

    template<class T, class T2, class RHS>
    struct ArrayScalarSubtract : ArrayExpression<T, ArrayScalarSubtract<T, T2, RHS> > {
        const T& lhs_m;
        const RHS& rhs_m;

        inline explicit ArrayScalarSubtract(const T& lhs, const ArrayExpression<T2, RHS> & rhs) : lhs_m(lhs), rhs_m(rhs.Cast()) {



        }

        inline const size_t Size(const int32_t & dimension) const {
            return rhs_m.Size(dimension);
        }

        inline const size_t Dimensions() const {
            return rhs_m.Dimensions();
        }

        inline const T operator()(const uint32_t & i) const {
            return lhs_m - rhs_m(i);
        }

        inline const T operator()(const uint32_t& i, const uint32_t & j) const {
            return lhs_m - rhs_m(i, j);
        }

        inline const T operator()(const uint32_t& i, const uint32_t & j, const uint32_t & k) const {
            return lhs_m -rhs_m(i, j, k);
        }

        inline const T operator()(const uint32_t& i, const uint32_t & j, const uint32_t & k, const uint32_t & l)const {
            return lhs_m -rhs_m(i, j, k, l);
        }

        inline const T operator()(const uint32_t& i, const uint32_t & j, const uint32_t & k, const uint32_t & l, const uint32_t & m) const {
            return lhs_m - rhs_m(i, j, k, j, m);
        }

        inline const T operator()(const uint32_t& i, const uint32_t & j, const uint32_t & k, const uint32_t & l, const uint32_t & m, const uint32_t & n) const {
            return lhs_m - rhs_m(i, j, k, j, m, n);
        }

        inline const T operator()(const uint32_t& i, const uint32_t & j, const uint32_t & k, const uint32_t & l, const uint32_t & m, const uint32_t & n, const uint32_t & o) const {
            return lhs_m - rhs_m(i, j, k, j, m, n, o);
        }




    };

    template <class T, class T2, class LHS, class RHS>
    inline ArraySubtract<T, T2, LHS, RHS> operator-(const ArrayExpression<T, LHS>& a,
    const ArrayExpression<T2, RHS>& b) {
       
        return ArraySubtract<T, T2, LHS, RHS > (a.Cast(), b.Cast());
    }

    template <class T, class LHS, class RHS>
    inline ArraySubtract<et4ad::Variable<T>, T, RHS, LHS> operator-(const ArrayExpression<T, LHS>& a,
    const ArrayExpression<et4ad::Variable<T>, RHS>& b) {
       
        return ArraySubtract<et4ad::Variable<T>, T, RHS, LHS > (b.Cast(), a.Cast());
    }

    template <class T, class LHS, class RHS>
    inline ArraySubtract<et4ad::Variable<T>, T, LHS, RHS> operator-(const ArrayExpression<et4ad::Variable<T>, LHS>& a,
    const ArrayExpression<T, RHS>& b) {
       
        return ArraySubtract<et4ad::Variable<T>, T, LHS, RHS > (a.Cast(), b.Cast());
    }

    template<class T, class LHS>
    inline ArrayScalarSubtract<T, T, LHS> operator -(const ArrayExpression<T, LHS>& a, const T& b) {
        return ArrayScalarSubtract<T, T, LHS > (a.Cast(), b);
    }

    template<class T, class LHS>
    inline ArrayScalarSubtract<T, T, LHS> operator -(const T& a, const ArrayExpression<T, LHS>& b) {
        return ArrayScalarSubtract<T, T, LHS > (a.Cast(), b);
    }

    template<class T, class LHS>
    inline ArraySubtractScalar<et4ad::Variable<T>, T, LHS> operator -(const ArrayExpression<T, LHS>& a, const et4ad::Variable<T>& b) {
        return ArraySubtractScalar<et4ad::Variable<T>, T, LHS > (a.Cast(), b);
    }

    template<class T, class LHS>
    inline ArrayScalarSubtract<et4ad::Variable<T>, T, LHS> operator -(const et4ad::Variable<T>& a, const ArrayExpression<T, LHS>& b) {
        return ArrayScalarSubtract<et4ad::Variable<T>, T, LHS > (a, b.Cast());
    }

    template<class T, class LHS>
    inline ArraySubtractScalar<et4ad::Variable<T>, T, LHS> operator -(const ArrayExpression<et4ad::Variable<T>, LHS>& a, const T& b) {
        return ArraySubtractScalar<et4ad::Variable<T>, T, LHS > (a.Cast(), b);
    }

    template<class T, class LHS>
    inline ArrayScalarSubtract<et4ad::Variable<T>, et4ad::Variable<T>, LHS> operator -(const T& a, const ArrayExpression<et4ad::Variable<T>, LHS>& b) {
        return ArrayScalarSubtract<et4ad::Variable<T>, et4ad::Variable<T>, LHS > (a, b.Cast());
    }

}


#endif	/* ARRAYSUBTRACT_HPP */

