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
        virtual const REAL_T Value(const REAL_T &x) = 0;
        virtual const REAL_T Derivative(const REAL_T &x, uint32_t id = 0) = 0;
    };

    template<class REAL_T>
    struct Add : public ChainEntry<REAL_T> {
        ChainEntry<REAL_T>* lhs_m;
        ChainEntry<REAL_T>* rhs_m;

        Add(ChainEntry<REAL_T>* lhs, ChainEntry<REAL_T>* rhs) : lhs_m(lhs), rhs_m(rhs) {

        }

         const REAL_T Value(const REAL_T &x) {
             return lhs_m->Value(x)+ rhs_m->Value(x);
        }

         const REAL_T Derivative(const REAL_T &x, uint32_t id = 0) {
             return lhs_m->Derivative(x,id)+rhs_m->Derivative(x,id);
        }


    };


}



#endif	/* CHAIN_HPP */

