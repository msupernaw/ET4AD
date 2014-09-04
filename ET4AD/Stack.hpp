/* 
 * File:   Stack.hpp
 * Author: matthewsupernaw
 *
 * Created on July 23, 2014, 11:17 AM
 */

#ifndef STACK_HPP
#define	STACK_HPP
#include <vector>
#include <stdint.h>
namespace et4ad {

    template<class REAL_T>
    class StackStatement {
    public:
        REAL_T value_m;
        REAL_T factor_m;
        uint32_t id_m;

        StackStatement(const REAL_T& value,
                const REAL_T& factor,
                uint32_t id = 1) :
        value_m(value),
        factor_m(factor),
        id_m(id) {

        }

    };

    template <class REAL_T>
    class Stack {
        std::vector<StackStatement<REAL_T> > entries;
        typedef typename std::vector<StackStatement<REAL_T> >::iterator stack_iterator;

    public:

        void Add(const REAL_T& value, REAL_T factor = 1.0, uint32_t id = 1) {
            entries.push_back(StackStatement<REAL_T> (value, factor, id));
        }

        const REAL_T adjoint(uint32_t id) {
            stack_iterator it;
            REAL_T dx = 0.0;
            REAL_T mult = 1;
            for (it = entries.begin(); it != entries.end(); ++it) {


//                if(it->id_m > 0){
//                    if(it->id_m != id){
//                        continue;
//                    }
//                }
//                
                dx += it->factor_m * it->value_m*mult;
            }

            return dx;

        }




    };


}





#endif	/* STACK_HPP */

