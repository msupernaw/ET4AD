/* 
 * File:   IDSet.hpp
 * Author: matthewsupernaw
 *
 * Created on June 23, 2014, 8:04 AM
 */

#ifndef IDSET_HPP
#define	IDSET_HPP

#include "IDGenerator.hpp"
#include <queue>
#include <algorithm>

namespace et4ad {

    /**
     * This class simply holds a list of independent variable 
     * ids for a variables expression.
     */
    class IDSet {
    private:
        std::vector<uint32_t> list_m;
        std::vector<int> has_m;
        size_t has_size;



        //        template<class T, int group>
        //        friend class Variable;
    public:
        typedef std::vector<uint32_t>::iterator id_iterator;
        uint32_t max;

        IDSet() {
            //            idsss = IndependentVariableIdGenerator::instance()->getIds();
            has_m.reserve(IndependentVariableIdGenerator::instance()->current() + 1);
            has_size = has_m.size();
            list_m.reserve(IndependentVariableIdGenerator::instance()->current() + 1);
            max = 0;
        }

        inline void insert(const uint32_t &id) {
            if (unsigned(id) > unsigned(max)) {
                max = id;
            }

            //            idsss[id] = 1;
            if (unsigned(has_size) <= unsigned(id)) {
                has_m.resize(IndependentVariableIdGenerator::instance()->current() + 1);
                has_size = has_m.size();
                list_m.reserve(IndependentVariableIdGenerator::instance()->current() + 1);
            }

            if (!unsigned(has_m[id])) {
                has_m[id] = 1;
//                list_m.insert(list_m.end(),id);
                list_m.push_back(id);
//                                this->SortSet();
            }


        }

        id_iterator begin() {
            return this->list_m.begin();
        }

        id_iterator end() {
            return this->list_m.end();
        }

        size_t Size() {
            return this->list_m.size();
        }

        uint32_t* Data() {
            return this->list_m.data();
        }

        void clear() {
            this->has_size = 0;
            this->has_m.resize(0);
            this->list_m.resize(0);
            has_m.resize(IndependentVariableIdGenerator::instance()->current() + 1);
        }

        void SortSet() {
            std::sort(this->list_m.begin(), this->list_m.end());
        }




    };


}


#endif	/* IDSET_HPP */

