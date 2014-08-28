/* 
 * File:   IDGenerator.hpp
 * Author: matthewsupernaw
 *
 * Created on June 18, 2014, 1:43 PM
 */

#ifndef ET4AD_IDGENERATOR_HPP
#define	ET4AD_IDGENERATOR_HPP

#include <stdint.h>
//#include <boost/unordered_map.hpp>
namespace et4ad {

    /*!
     * Creates a unique identifier.
     * @return 
     */
    class IndependentVariableIdGenerator {
//        typedef boost::unordered_map<uint32_t, uint32_t > id_set;
//        id_set ids;
    public:
        static IndependentVariableIdGenerator * instance();

        const uint32_t next() {
//            ids.insert(std::pair<uint32_t, uint32_t > (++_id, 0));
            return ++_id;
        }

        const uint32_t current() {
            return _id;
        }

//        id_set getIds() const {
//            return ids;
//        }

    private:

        IndependentVariableIdGenerator() : _id(0) {
        }

        uint32_t _id;
    };

    static IndependentVariableIdGenerator* only_copy;

    inline IndependentVariableIdGenerator *
    IndependentVariableIdGenerator::instance() {

        if (!only_copy) {
            only_copy = new IndependentVariableIdGenerator();
        }

        return only_copy;
    }
}




#endif	/* IDGENERATOR_HPP */

