/* 
 * File:   Expression.hpp
 * Author: matthewsupernaw
 *
 * Created on June 18, 2014, 11:16 AM
 */

#ifndef EXPRESSION_HPP
#define	EXPRESSION_HPP
#include <stdint.h>

#include "VariableStorage.hpp"
//#include "BigFloat.hpp"
#include "IDSet.hpp"


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

        //        /**
        //         * Push the ids for independent variables to a storage object.
        //         * 
        //         * @param storage
        //         */
        //        inline void PushIds(et4ad::VariableStorage<REAL_T> &storage) const {
        //            Cast().PushIds(storage);
        //        }

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

        /**
         *  Push all elements (derivatives, ids, statements) 
         * to a storage object.
         * 
         * @param storage
         */
        inline void PushAll(et4ad::VariableStorage<REAL_T> &storage) const {
            Cast().PushAll(storage);
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
}


#endif	/* EXPRESSION_HPP */

