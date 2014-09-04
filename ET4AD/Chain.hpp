/* 
 * File:   Chain.hpp
 * Author: matthewsupernaw
 *
 * Created on September 2, 2014, 3:39 PM
 */

#ifndef CHAIN_HPP
#define	CHAIN_HPP

namespace et4ad {

    template<class REAL_T>
    struct ChainEntry {
        virtual const REAL_T Value(const REAL_T &x, uint32_t id = 0) = 0;
        virtual const REAL_T Derivative(const REAL_T &x, uint32_t id = 0) = 0;
    };

    template<class REAL_T>
    struct ChainVariable : public ChainEntry<REAL_T> {
        uint32_t id_m;

         REAL_T value_m;

        ChainVariable(const REAL_T& value, uint32_t id = 0)
        : value_m(value) {

        }

        const REAL_T Value(const REAL_T &x, uint32_t id = 0) {
            return value_m;
        }

        const REAL_T Derivative(const REAL_T &x, uint32_t id = 0) {
//            return id_m == id ? REAL_T(1.0) : REAL_T(0.0);
                        return REAL_T(1.0);
        }
    };

    template<class REAL_T>
    struct ChainDependentVariable : public ChainEntry<REAL_T> {
        REAL_T value_m;

        ChainDependentVariable(const REAL_T& value, uint32_t id = 0)
        : value_m(value) {

        }

        const REAL_T Value(const REAL_T &x) {
            return value_m;
        }

        const REAL_T Derivative(const REAL_T &x, uint32_t id = 0) {
            return REAL_T(0.0);
        }
    };

    template<class REAL_T>
    struct ChainAdd : public ChainEntry<REAL_T> {
        ChainEntry<REAL_T>* lhs_m;
        ChainEntry<REAL_T>* rhs_m;

        ChainAdd(ChainEntry<REAL_T>* lhs,
                ChainEntry<REAL_T>* rhs)
        : lhs_m(lhs), rhs_m(rhs) {

        }

        const REAL_T Value(const REAL_T &x, uint32_t id = 0) {
            return lhs_m->Value(x) + rhs_m->Value(x);
        }

        const REAL_T Derivative(const REAL_T &x, uint32_t id = 0) {
            return lhs_m->Derivative(x, id) + rhs_m->Derivative(x, id);
        }


    };

    template<class REAL_T>
    struct ChainSubtract : public ChainEntry<REAL_T> {
        ChainEntry<REAL_T>* lhs_m;
        ChainEntry<REAL_T>* rhs_m;

        ChainSubtract(ChainEntry<REAL_T>* lhs,
                ChainEntry<REAL_T>* rhs)
        : lhs_m(lhs), rhs_m(rhs) {

        }

        const REAL_T Value(const REAL_T &x, uint32_t id = 0) {
            return lhs_m->Value(x) - rhs_m->Value(x);
        }

        const REAL_T Derivative(const REAL_T &x, uint32_t id = 0) {
            return lhs_m->Derivative(x, id) - rhs_m->Derivative(x, id);
        }


    };

    template<class REAL_T>
    struct ChainMultiply : public ChainEntry<REAL_T> {
        ChainEntry<REAL_T>* lhs_m;
        ChainEntry<REAL_T>* rhs_m;

        ChainMultiply(ChainEntry<REAL_T>* lhs,
                ChainEntry<REAL_T>* rhs)
        : lhs_m(lhs), rhs_m(rhs) {

        }

        const REAL_T Value(const REAL_T &x, uint32_t id = 0) {
            return lhs_m->Value(x) * rhs_m->Value(x);
        }

        const REAL_T Derivative(const REAL_T &x, uint32_t id = 0) {
            return lhs_m->Derivative(x, id) * rhs_m->Value(x) + rhs_m->Derivative(x, id) * lhs_m->Value(x);
        }


    };

    template<class REAL_T>
    struct ChainDivide : public ChainEntry<REAL_T> {
        ChainEntry<REAL_T>* lhs_m;
        ChainEntry<REAL_T>* rhs_m;

        ChainDivide(ChainEntry<REAL_T>* lhs,
                ChainEntry<REAL_T>* rhs)
        : lhs_m(lhs), rhs_m(rhs) {

        }

        const REAL_T Value(const REAL_T &x, uint32_t id = 0) {
            return lhs_m->Value(x) * rhs_m->Value(x);
        }

        const REAL_T Derivative(const REAL_T &x, uint32_t id = 0) {
            return lhs_m->Derivative(x, id) * rhs_m->Value(x) -
                    rhs_m->Derivative(x, id) * lhs_m->Value(x)
                    / rhs_m->Value(x) / rhs_m->Value(x);
        }


    };

    template<class REAL_T>
    ChainAdd<REAL_T>* operator+(ChainEntry<REAL_T>& a, ChainEntry<REAL_T>& b) {
        return new ChainAdd<REAL_T>(&a, &b);
    }

    template<class REAL_T>
    ChainSubtract<REAL_T>* operator-(ChainEntry<REAL_T>& a, ChainEntry<REAL_T>& b) {
        return new ChainSubtract<REAL_T>(&a, &b);
    }

    template<class REAL_T>
    ChainMultiply<REAL_T>* operator*(ChainEntry<REAL_T>& a, ChainEntry<REAL_T>& b) {
        return new ChainMultiply<REAL_T>(&a, &b);
    }

    template<class REAL_T>
    ChainDivide<REAL_T>* operator*(ChainEntry<REAL_T>& a, ChainEntry<REAL_T>& b) {
        return new ChainDivide<REAL_T>(&a, &b);
    }




}



#endif	/* CHAIN_HPP */

