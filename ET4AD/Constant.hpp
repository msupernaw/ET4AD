/* 
 * File:   Constant.hpp
 * Author: matthewsupernaw
 *
 * Created on June 18, 2014, 11:18 AM
 */

#ifndef CONSTANT_HPP
#define	CONSTANT_HPP

#include "Expression.hpp"


namespace et4ad {

    template<class REAL_T>
    class Constant : public et4ad::ExpressionBase<REAL_T, Constant<REAL_T> > {
    public:

        Constant() : value_m(static_cast<REAL_T> (0.0)) {

        }

        Constant(const REAL_T & value) : value_m(value) {

        }
        //        ~Constant(){
        //            std::cout<<"pau\n"<<std::flush;
        //        }

        inline const REAL_T GetValue() const {
            return value_m;
        }

        operator REAL_T() {
            return this->GetValue();
        }

        operator REAL_T()const {
            return this->GetValue();
        }

        const REAL_T Derivative(const uint32_t &id, bool &found) const {
            return static_cast<REAL_T> (0.0);
        }

        const REAL_T Derivative(const uint32_t &id) const {
            return static_cast<REAL_T> (0.0);
        }

        inline void PushIds(et4ad::IDSet &ids) const {
        }

        inline void PushStatements(std::vector<Statement<REAL_T> > &storage) const {
            storage.push_back(Statement<REAL_T > (CONSTANT, this->value_m));
        }

        inline const std::complex<REAL_T> ComplexStepValue(const uint32_t & id, REAL_T h = REAL_T(0.00000000000001)) const {
            return std::complex<REAL_T>(GetValue());
        }


    private:
        REAL_T value_m;
    };



}


#endif	/* CONSTANT_HPP */

