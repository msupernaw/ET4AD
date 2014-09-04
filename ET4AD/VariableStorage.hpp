/* 
 * File:   VariableStorage.hpp
 * Author: matthewsupernaw
 *
 * Created on June 18, 2014, 11:19 AM
 */

#ifndef VARIABLESTORAGE_HPP
#define	VARIABLESTORAGE_HPP


//#include <boost/unordered_set.hpp>
//#include <boost/any.hpp>
#include <iostream>

#include "Expression.hpp"
#include "Statement.hpp"
#include "IDGenerator.hpp"
#include <set>
#include <vector>
#include <sstream>
namespace et4ad {


    template <class REAL_T, class T>
    struct ExpressionBase;

    template<class REAL_T, class ID_STORAGE = std::set<uint32_t>,
    class DERIVATIVE_STORAGE = std::vector<std::pair<bool, REAL_T> >,
    class STATEMENT_STORAGE = std::vector<Statement<REAL_T> > >
    class VariableStorage {
    public:
        typedef ID_STORAGE IdSet;
        typedef typename IdSet::iterator ids_terator;
        typedef typename IdSet::const_iterator const_ids_iterator;
         IdSet ids;

        typedef DERIVATIVE_STORAGE GradientVector;
        typedef typename GradientVector::iterator gradient_iterator;
        mutable GradientVector gradient;

        typedef STATEMENT_STORAGE StatementVector;
        mutable StatementVector statements;

        template<class T, int, class STORAGE>
        friend class Variable;
    public:

        VariableStorage() {
            //statements.reserve(1000000);
        }

        //        VariableStorage(const VariableStorage &orig ) {
        //            this->gradient = GradientVector(orig.gradient);
        //            this->ids = IdSet(orig.ids);
        //            this->statements = StatementVector(orig.statements);
        //
        //        }

        virtual inline void PushIds(VariableStorage<REAL_T> &other)  {
            ids.insert(
                    other.ids.begin(),
                    other.ids.end());
        }

        virtual inline void PushStatements(const VariableStorage<REAL_T> &other) {
            other.statements.insert(other.statements.end(),
                    this->statements.begin(),
                    this->statements.end());



        }

        virtual inline void PushAll(VariableStorage<REAL_T> &other) {
            other.ids.insert(this->ids.begin(), this->ids.end());
            other.gradient.insert(other.gradient.begin(), this->gradient.begin(), this->gradient.end());
        }

        inline IdSet& GetIds() {
            return ids;
        }

        inline GradientVector& GetGradient() {
            return gradient;
        }

        inline StatementVector& GetStatements() {
            return statements;
        }

        void Reset()const {
            this->gradient.clear();
            this->ids.clear();
            this->statements.clear();
        }

        template<class T>
        inline void ComputeDerivatives(const ExpressionBase<REAL_T, T> &expr) {
            //            expr.PushIds(*this);
            //            std::cout<<__func__<<"\n";
            gradient.resize(IndependentVariableIdGenerator::instance()->current() + 1);
            ids_terator it;
            bool found = false;
            for (it = this->ids.begin(); it != ids.end(); ++it) {
                //                                std::cout << *it << "<<---\n";
                 found = false;
                this->gradient[*it].first = true;
                //                REAL_T dx =  expr.Derivative(*it, found);
                this->gradient[*it].second = expr.Derivative(*it, found);
                //   
               
//                std::cout<< *it <<" id dx = " << this->gradient[*it].second << "<--\n";
            }

        }

        inline const REAL_T GetDerivative(const uint32_t &id, bool &found) {
            if (id < gradient.size()) {
                if (gradient[id].first) {
                    found = true;
                    return gradient[id].second;
                } else {
                    return 0.0;
                }
            } else {
                return 0.0;
            }
        }

        inline const REAL_T GetDerivative(const uint32_t &id) {
            if (id < gradient.size()) {
                if (gradient[id].first) {
                    return gradient[id].second;
                } else {
                    return 0.0;
                }
            } else {
                return 0.0;
            }
        }

        const std::string ToString() {

            std::stringstream ss;
            ss << "Variable Storage:\n";
            ss << "Independent Variables by Id:\n";
            ids_terator it;
            for (it = this->ids.begin(); it != ids.end(); ++it) {
                ss << *it << "\n";
            }

            ss << "Gradient info:\n";
            ss << "Id\tderivative\n";
            for (int i = 1; i < gradient.size(); i++) {
                ss << i << "\t" << gradient[i].second << "\n";
            }



        }

    };

}







#endif	/* VARIABLESTORAGE_HPP */

