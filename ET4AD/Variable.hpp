/* 
 * File:   Variable.hpp
 * Author: matthewsupernaw
 *
 * Created on June 18, 2014, 1:05 PM
 */

#ifndef ET4AD_VARIABLE_HPP
#define	ET4AD_VARIABLE_HPP
#include <stack>
//#include <boost/unordered_map.hpp>
#include "VariableStorage.hpp"
//#include "Expression.hpp"
#include "STD.hpp"
#include <fstream>



#define H_V 1.0E-20

//#define USE_COMPLEX_STEP_DIFFERNTIATION

//#define ET4AD_TEST_AND_VERIFY_RUNTIME

#if defined(ET4AD_TEST_AND_VERIFY_RUNTIME) && !defined(ET4AD_TEST_AND_VERIFY)
#define ET4AD_TEST_AND_VERIFY
#endif
#ifdef ET4AD_TEST_AND_VERIFY
#define ET4AD_TEST_AND_VERIFY_PRECISION 100000.0
#endif


namespace et4ad {

#ifdef ET4AD_TEST_AND_VERIFY

    std::ofstream et4ad_test_and_verify_log("et4ad_derivative_test.log");

    template <class REAL_T>
    static void Derivative_Test(const REAL_T& a, const REAL_T& b, char* func = "not supplied") {

        if (!(std::trunc(ET4AD_TEST_AND_VERIFY_PRECISION * a) == std::trunc(ET4AD_TEST_AND_VERIFY_PRECISION * b))) {
            et4ad_test_and_verify_log.precision(50);
            et4ad_test_and_verify_log << "derivative test failed from function \"" << func << "\": " << a << " != " << b << "\n";
        }
    }

#endif

    template<class T>
    bool operator<(const std::complex<T>& a, const std::complex<T>& b) {
        return a.real() < b.real();
    }

    template<class T>
    bool operator>(const std::complex<T>& a, const std::complex<T>& b) {
        return a.real() > b.real();
    }


    template<class REAL_T, //base type
    int group = 0 > //group identifier
    class Variable;

    template<class T>
    struct IDTrait {

        void SetId(T& t, uint32_t id) {

        }
    };

    template<class REAL_T, //base type
    int group > //group identifier
    class Variable : public ExpressionBase<REAL_T, Variable<REAL_T, group > > {
        template<class T> friend struct IDTrait;
        std::vector<REAL_T > gradient;
        mutable IDSet ids_set;
        size_t gsize;
        REAL_T value_m;

        REAL_T min_boundary_m;
        REAL_T max_boundary_m;

        bool is_independent_m;
        uint32_t independent_id_m;

        std::string name_m;
        bool bounded_m;



        //static members
        static bool is_recording_g;
        static bool is_supporting_arbitrary_order;
        IDSet::id_iterator it;
        mutable std::vector<Statement<REAL_T> > statements;
        //IDSet::id_iterator it;

        inline void ResizeGradientVector() {
            //should make this power of 2
            size_t size = IndependentVariableIdGenerator::instance()->current() + 1;
            if (gsize < size) {
                gradient.resize(size);
                gsize = size;
            }
        }

        inline void ResizeGradientVector(size_t size) {
            //should make this power of 2

            if (gsize < size) {
                gradient.resize(size);
                gsize = size;
            }
        }




    public:

        static bool IsRecording() {
            return Variable<REAL_T, group>::is_recording_g;
        }

        static void SetRecording(bool is_recording_g) {
            Variable<REAL_T, group>::is_recording_g = is_recording_g;
        }

        static bool IsSupportingArbitraryOrder() {
            return Variable<REAL_T, group>::is_supporting_arbitrary_order;
        }

        static void SetSupportingArbitraryOrder(bool is_supporting_arbitrary_order) {
            Variable<REAL_T, group>::is_supporting_arbitrary_order = is_supporting_arbitrary_order;
        }

        Variable() : ExpressionBase<REAL_T, Variable<REAL_T, group> >(),
        gsize(0),
        value_m(0.0),
        min_boundary_m(std::numeric_limits<REAL_T>::min()),
        max_boundary_m(std::numeric_limits<REAL_T>::max()),
        is_independent_m(false),
        independent_id_m(0),
        bounded_m(false) {
        }

        Variable(const REAL_T &value/*, bool independent = false*/) : ExpressionBase<REAL_T, Variable<REAL_T, group> >(),
        gsize(0),
        value_m(value),
        min_boundary_m(std::numeric_limits<REAL_T>::min()),
        max_boundary_m(std::numeric_limits<REAL_T>::max()),
        is_independent_m(false),
        independent_id_m(0),
        bounded_m(false) {
            // SetAsIndependent(independent);
        }

        Variable(const Variable<REAL_T> & orig) {

            //to do move to initializer list.
            gsize = 0;
            bounded_m = orig.bounded_m;
            is_independent_m = orig.is_independent_m;
            //            this->SetAsIndependent()
            independent_id_m = orig.independent_id_m;
            max_boundary_m = orig.max_boundary_m;
            min_boundary_m = orig.min_boundary_m;
            name_m = orig.name_m;
            this->id_m = orig.id_m;

            if (Variable<REAL_T, group>::is_recording_g) {

                orig.PushIds(ids_set);

                ResizeGradientVector(ids_set.max + 1);
                uint32_t* ids = ids_set.Data();
                size_t size = ids_set.Size();

                for (int i = 0; i < size; i++) {
                    //                    
#ifdef USE_COMPLEX_STEP_DIFFERNTIATION
                    gradient[ids[i]] = orig.ComplexStepValue(ids[i], H_V).imag() / H_V;
#else
                    gradient[ids[i]] = orig.Derivative(ids[i]);
#endif
                }
                //                IDSet::id_iterator it;
                //                for (it = ids_set.begin(); it != ids_set.end(); it++) {
                //                    //                    REAL_T dx = orig.Derivative((*it));
                //                    //                    gradient[(*it)].first = 1;
                //                    gradient[(*it)] = orig.Derivative((*it));
                //                }
                //
                //
                //                if (Variable<REAL_T, group>::is_supporting_arbitrary_order) {
                //                    orig.PushStatements(statements);
                //                }
            }
            SetValue(orig.GetValue());
        }

        Variable(Variable<REAL_T> & orig) {

            //to do move to initializer list.
            gsize = orig.gsize;
            bounded_m = orig.bounded_m;
            is_independent_m = orig.is_independent_m;
            //            this->SetAsIndependent()
            independent_id_m = orig.independent_id_m;
            max_boundary_m = orig.max_boundary_m;
            min_boundary_m = orig.min_boundary_m;
            name_m = orig.name_m;
            this->id_m = orig.id_m;
            ids_set = orig.ids_set;
            value_m = orig.value_m;
            gradient = orig.gradient;
            //            if (Variable<REAL_T, group>::is_recording_g) {
            //
            //                orig.PushIds(ids_set);
            //
            //                ResizeGradientVector(ids_set.max + 1);
            //                uint32_t* ids = ids_set.Data();
            //                size_t size = ids_set.Size();
            //
            //                for (int i = 0; i < size; i++) {
            //                    gradient[ids[i]] = orig.Derivative(ids[i]);
            //                }
            //                //                IDSet::id_iterator it;
            //                //                for (it = ids_set.begin(); it != ids_set.end(); it++) {
            //                //                    //                    REAL_T dx = orig.Derivative((*it));
            //                //                    //                    gradient[(*it)].first = 1;
            //                //                    gradient[(*it)] = orig.Derivative((*it));
            //                //                }
            //
            //
            //                if (Variable<REAL_T, group>::is_supporting_arbitrary_order) {
            //                    orig.PushStatements(statements);
            //                }
            //            }
            //            SetValue(orig.GetValue());
        }

        template<class T>
        Variable(const ExpressionBase<REAL_T, T>& expr) : ExpressionBase<REAL_T, Variable<REAL_T, group> >(0),
        gsize(0),
        value_m(0.0),
        min_boundary_m(std::numeric_limits<REAL_T>::min()),
        max_boundary_m(std::numeric_limits<REAL_T>::max()),
        is_independent_m(false),
        independent_id_m(0),
        bounded_m(false) {



            if (Variable<REAL_T, group>::is_recording_g) {
                expr.PushIds(ids_set);
                //                IDSet::id_iterator it;

                //                gradient.clear();
                ResizeGradientVector(ids_set.max + 1);
                uint32_t* ids = ids_set.Data();
                size_t size = ids_set.Size();
                int i;
                for (i = 0; i < size; i++) {

                    //                    gradient[ids[i]] = expr.Derivative(ids[i]);
                    //                    gradient[ids[i]] = expr.ComplexStepValue(ids[i], H_V).imag() / H_V;
#ifdef USE_COMPLEX_STEP_DIFFERNTIATION
                    gradient[ids[i]] = expr.ComplexStepValue(ids[i], H_V).imag() / H_V;
#else
                    gradient[ids[i]] = expr.Derivative(ids[i]);
#endif
                }

                //IDSet::id_iterator it;
                //                for (it = ids_set.begin(); it != ids_set.end(); ++it) {
                //                    gradient[(*it)] = expr.Derivative((*it));
                //                }
                //
                //                if (Variable<REAL_T, group>::is_supporting_arbitrary_order) {
                //                    expr.PushStatements(statements);
                //                }

            }

            SetValue(expr.GetValue());
        }

        virtual ~Variable() {

        }

        //        operator REAL_T() {
        //            return this->GetValue();
        //        }
        //
        //        operator REAL_T()const {
        //            return this->GetValue();
        //        }


        //operators

        Variable& operator=(const REAL_T& other) {
            SetValue(other);
            gradient.resize(0);
            //            ResizeGradientVector(ids_set.max+1);

            //            t¬…≥÷his->ids_set.clear();
            if (Variable<REAL_T, group>::is_recording_g) {

                //                if (Variable<REAL_T, group>::is_supporting_arbitrary_order) {
                //                    statements.clear();
                //                }

            }
            return *this;
        }

        /**
         * Sets the value of this Variable to that of other.
         * 
         * @param rhs
         * @return 
         */
        Variable& operator=(const Variable& other) {


            if (Variable<REAL_T, group>::is_recording_g) {

                other.PushIds(ids_set);
                ResizeGradientVector(ids_set.max + 1);

                uint32_t* ids = ids_set.Data();
                size_t size = ids_set.Size();
                int i;
                for (i = 0; i < size; i++) {
                    //                    gradient[ids[i]] = other.Derivative(ids[i]);
#ifdef USE_COMPLEX_STEP_DIFFERNTIATION
                    gradient[ids[i]] = other.ComplexStepValue(ids[i], H_V).imag() / H_V;
#else
                    gradient[ids[i]] = other.Derivative(ids[i]);
#endif
                }

                //                for (it = ids_set.begin(); it != ids_set.end(); it++) {
                //                    gradient[(*it)] = other.Derivative((*it));
                //                }

                //
                //                if (Variable<REAL_T, group>::is_supporting_arbitrary_order) {
                //                    std::vector<Statement<REAL_T> > s;
                //                    other.PushStatements(s);
                //                    statements.clear();
                //                    statements.insert(statements.begin(), s.begin(), s.end());
                //                }

            }


            SetValue(other.GetValue());
            return *this;
        }

        /**
         * Set the Variables value to the result of the expression rhs. 
         * Derivatives are calculated and stored in the encapsulated 
         * gradient map.
         * @param rhs
         * @return 
         */
        template<class T>
        Variable& operator=(const ExpressionBase<REAL_T, T>& expr) {

            if (Variable<REAL_T, group>::is_recording_g) {

                expr.PushIds(ids_set);

                ResizeGradientVector(ids_set.max + 1);
                uint32_t* ids = ids_set.Data();
                size_t size = ids_set.Size();

                int i;
                for (i = 0; i < size; i++) {
                    //                    gradient[ids[i]] = expr.Derivative(ids[i]);
#ifdef USE_COMPLEX_STEP_DIFFERNTIATION
                    gradient[ids[i]] = expr.ComplexStepValue(ids[i], H_V).imag() / H_V;
#else
                    gradient[ids[i]] = expr.Derivative(ids[i]);
#endif

                }
                //                for (it = ids_set.begin(); it != ids_set.end(); it++) {
                //                    gradient[(*it)] = expr.Derivative((*it));
                //                }

                //                if (Variable<REAL_T, group>::is_supporting_arbitrary_order) {
                //                    std::vector<Statement<REAL_T> > s;
                //                    expr.PushStatements(s);
                //                    statements.clear();
                //                    statements.insert(statements.begin(), s.begin(), s.end());
                //                }
            }

            SetValue(expr.GetValue());

            return *this;
        }

        template<class T>
        void ReadStatements(const ExpressionBase<REAL_T, T>& rhs) {
            std::vector<Statement<REAL_T> > s;
            PushStatements(s);
            rhs.PushStatements(s);
            s.push_back(Statement<REAL_T > (PLUS));
            statements.clear();
            statements.insert(statements.begin(), s.begin(), s.end());
        }

        template<class T>
        Variable& operator+=(const ExpressionBase<REAL_T, T>& rhs) {


            if (Variable<REAL_T, group>::is_recording_g) {

                rhs.PushIds(ids_set);
                const IDSet::id_iterator ite = ids_set.end();
                ResizeGradientVector(ids_set.max + 1);

                //                int length = ids_set.Size();
                //
                //                int i = 0;
                //                for (i = 0; i < length - 3; i += 4) {
                ////                    std::cout<<"i = "<<i<<", "<<(*(ids_set.begin() + (i + 1)))<<" "<<length<<std::endl;
                //                    gradient[*(ids_set.begin() + i)] += rhs.Derivative(*(ids_set.begin() + i));
                //                    gradient[*(ids_set.begin() + (i + 1))] += rhs.Derivative(*(ids_set.begin() + (i + 1)));
                //                    gradient[*(ids_set.begin() + (i + 2))] += rhs.Derivative(*(ids_set.begin() + (i + 2)));
                //                    gradient[*(ids_set.begin() + (i + 3))] += rhs.Derivative(*(ids_set.begin() + (i + 3)));
                //
                //                }
                //                for (; i < length; i++) {
                //                    gradient[*(ids_set.begin() + i)] += rhs.Derivative(*(ids_set.begin() + i));
                //                }
                uint32_t* ids = ids_set.Data();
                size_t size = ids_set.Size();
                int i;
                int ind;
                for (i = 0; i < size; i++) {
                    ind = ids[i];
                    //                    gradient[ind] += rhs.Derivative(ind);
#ifdef USE_COMPLEX_STEP_DIFFERNTIATION
                    gradient[ind] += rhs.ComplexStepValue(ind, H_V).imag() / H_V;
#else
                    gradient[ind] += rhs.Derivative(ind);
#endif

                }
                //                for (it = ids_set.begin(); it != ite; ++it) {
                //                    gradient[(*it)] += rhs.Derivative((*it));
                //                }

                //                if (Variable<REAL_T, group>::is_supporting_arbitrary_order) {
                //                    ReadStatements(rhs);
                //                    //                    std::vector<Statement<REAL_T> > s;
                //                    //                    PushStatements(s);
                //                    //                    rhs.PushStatements(s);
                //                    //                    s.push_back(Statement<REAL_T > (PLUS));
                //                    //                    statements.clear();
                //                    //                    statements.insert(statements.begin(), s.begin(), s.end());
                //                }

            }

            SetValue(value_m + rhs.GetValue());

            return *this;
        }

        Variable& operator+=(const Variable& rhs) {

            if (Variable<REAL_T, group>::is_recording_g) {

                rhs.PushIds(ids_set);

                IDSet::id_iterator ite = ids_set.end();
                ResizeGradientVector(ids_set.max + 1);
                uint32_t* ids = ids_set.Data();
                size_t size = ids_set.Size();
                int i;
                int ind;
                for (i = 0; i < size; i++) {
                    ind = ids[i];
                    //                    gradient[ind] += rhs.Derivative(ind);
#ifdef USE_COMPLEX_STEP_DIFFERNTIATION
                    gradient[ind] += rhs.ComplexStepValue(ind, H_V).imag() / H_V;
#else
                    gradient[ind] += rhs.Derivative(ind);
#endif
                }
                //                for (it = ids_set.begin(); it != ite; ++it) {
                //
                //                    gradient[(*it)] += rhs.Derivative((*it));
                //                }

                //                if (Variable<REAL_T, group>::is_supporting_arbitrary_order) {
                //                    std::vector<Statement<REAL_T> > s;
                //                    PushStatements(s);
                //                    rhs.PushStatements(s);
                //                    s.push_back(Statement<REAL_T > (PLUS));
                //                    statements.clear();
                //                    statements.insert(statements.begin(), s.begin(), s.end());
                //                }

            }
            SetValue(value_m + rhs.GetValue());
            return *this;
        }

        Variable& operator+=(const REAL_T& rhs) {
            return *this = (*this +rhs);
        }

        template<class T>
        Variable& operator-=(const ExpressionBase<REAL_T, T>& rhs) {

            if (Variable<REAL_T, group>::is_recording_g) {

                rhs.PushIds(ids_set);

                ResizeGradientVector(ids_set.max + 1);
                uint32_t* ids = ids_set.Data();
                size_t size = ids_set.Size();
                int i;
                int ind;
                for (i = 0; i < size; i++) {
                    ind = ids[i];
                    //                    gradient[ind] -= rhs.Derivative(ind);
#ifdef USE_COMPLEX_STEP_DIFFERNTIATION
                    gradient[ind] -= rhs.ComplexStepValue(ind, H_V).imag() / H_V;
#else
                    gradient[ind] -= rhs.Derivative(ind);
#endif
                }

                //                for (it = ids_set.begin(); it != ids_set.end(); ++it) {
                //
                //                    REAL_T dx = gradient[(*it)] - rhs.Derivative((*it));
                //                    gradient[(*it)] = dx;
                //                }

                //
                //                if (Variable<REAL_T, group>::is_supporting_arbitrary_order) {
                //                    std::vector<Statement<REAL_T> > s;
                //                    PushStatements(s);
                //                    rhs.PushStatements(s);
                //                    s.push_back(Statement<REAL_T > (MINUS));
                //                    statements.clear();
                //                    statements.insert(statements.begin(), s.begin(), s.end());
                //                }

            }
            SetValue(GetValue() - rhs.GetValue());
            return *this;
        }

        Variable& operator-=(Variable& rhs) {
            if (Variable<REAL_T, group>::is_recording_g) {

                rhs.PushIds(ids_set);

                ResizeGradientVector(ids_set.max + 1);
                uint32_t* ids = ids_set.Data();
                size_t size = ids_set.Size();
                int i;
                int ind;
                for (i = 0; i < size; i++) {
                    ind = ids[i];
                    //                    gradient[ind] -= rhs.Derivative(ind);
#ifdef USE_COMPLEX_STEP_DIFFERNTIATION
                    gradient[ind] -= rhs.ComplexStepValue(ind, H_V).imag() / H_V;
#else
                    gradient[ind] -= rhs.Derivative(ind);
#endif

                }
                //                for (it = ids_set.begin(); it != ids_set.end(); ++it) {
                //                    gradient[(*it)] -= rhs.Derivative((*it));
                //                }
                //
                //                if (Variable<REAL_T, group>::is_supporting_arbitrary_order) {
                //                    std::vector<Statement<REAL_T> > s;
                //                    PushStatements(s);
                //                    rhs.PushStatements(s);
                //                    s.push_back(Statement<REAL_T > (MINUS));
                //                    statements.clear();
                //                    statements.insert(statements.begin(), s.begin(), s.end());
                //                }

            }
            SetValue(GetValue() - rhs.GetValue());
        }

        Variable& operator-=(const REAL_T& rhs) {
            return *this = (*this -rhs);
        }

        template<class T>
        Variable& operator*=(const ExpressionBase<REAL_T, T>& rhs) {

            if (Variable<REAL_T, group>::is_recording_g) {

                rhs.PushIds(ids_set);

                ResizeGradientVector(ids_set.max + 1);
                uint32_t* ids = ids_set.Data();
                size_t size = ids_set.Size();
                int i;
                for (i = 0; i < size; i++) {
                    REAL_T& dx = gradient[ids[i]];
                    //                    dx = (dx * rhs.GetValue() + GetValue() * rhs.Derivative(ids[i]));
#ifdef USE_COMPLEX_STEP_DIFFERNTIATION
                    dx = (dx * rhs.GetValue() + GetValue() * rhs.ComplexStepValue(ids[i], H_V).imag() / H_V);
#else
                    dx = (dx * rhs.GetValue() + GetValue() * rhs.Derivative(ids[i]));
#endif
                }
                //                for (it = ids_set.begin(); it != ids_set.end(); ++it) {
                //                    gradient[(*it)] = gradient[(*it)] * rhs.GetValue() + GetValue() * rhs.Derivative(*it);
                //                }


                //                if (Variable<REAL_T, group>::is_supporting_arbitrary_order) {
                //                    std::vector<Statement<REAL_T> > s;
                //                    PushStatements(s);
                //                    rhs.PushStatements(s);
                //                    s.push_back(Statement<REAL_T > (MULTIPLY));
                //                    statements = s;
                //                }

            }
            SetValue(GetValue() * rhs.GetValue());
            return *this;
        }

        Variable& operator*=(const Variable& rhs) {
            if (Variable<REAL_T, group>::is_recording_g) {

                rhs.PushIds(ids_set);

                ResizeGradientVector(ids_set.max + 1);

                uint32_t* ids = ids_set.Data();
                size_t size = ids_set.Size();
                int i;
                int ind;
                for (i = 0; i < size; i++) {
                    ind = ids[i];
                    REAL_T& dx = gradient[ind];
                    //                    dx = (dx * rhs.GetValue() + GetValue() * rhs.Derivative(ind));
#ifdef USE_COMPLEX_STEP_DIFFERNTIATION
                    dx = (dx * rhs.GetValue() + GetValue() * rhs.ComplexStepValue(ids[i], H_V).imag() / H_V);
#else
                    dx = (dx * rhs.GetValue() + GetValue() * rhs.Derivative(ind));
#endif
                }

                //                for (it = ids_set.begin(); it != ids_set.end(); ++it) {
                //                    gradient[(*it)] = gradient[(*it)] * rhs.GetValue() + GetValue() * rhs.Derivative(*it);
                //                }
                //
                //                if (Variable<REAL_T, group>::is_supporting_arbitrary_order) {
                //                    std::vector<Statement<REAL_T> > s;
                //                    PushStatements(s);
                //                    rhs.PushStatements(s);
                //                    s.push_back(Statement<REAL_T > (MULTIPLY));
                //                    statements = s;
                //                }

            }

            SetValue(GetValue() * rhs.GetValue());
            return *this;
        }

        Variable& operator*=(const REAL_T& rhs) {
            return *this = (*this * rhs);
        }

        template<class T>
        Variable& operator/=(const ExpressionBase<REAL_T, T>& rhs) {
            if (Variable<REAL_T, group>::is_recording_g) {

                rhs.PushIds(ids_set);

                ResizeGradientVector(ids_set.max + 1);

                uint32_t* ids = ids_set.Data();
                size_t size = ids_set.Size();
                int i;
                int ind;
                for (i = 0; i < size; i++) {
                    ind = ids[i];
                    REAL_T& dx = gradient[ind];
                    //                    dx = (dx * rhs.GetValue() - GetValue() * rhs.Derivative(ind)) / (rhs.GetValue() * rhs.GetValue());
#ifdef USE_COMPLEX_STEP_DIFFERNTIATION
                    dx = (dx * rhs.GetValue() - GetValue() * rhs.ComplexStepValue(ind, H_V).imag() / H_V) / (rhs.GetValue() * rhs.GetValue());
#else
                    dx = (dx * rhs.GetValue() - GetValue() * rhs.Derivative(ind)) / (rhs.GetValue() * rhs.GetValue());
#endif

                }
                //                for (it = ids_set.begin(); it != ids_set.end(); ++it) {
                //
                //                    //                    REAL_T dx = (gradient[(*it)] * rhs.GetValue() - GetValue() * rhs.Derivative(*it)) / (rhs.GetValue() * rhs.GetValue());
                //
                //                    gradient[(*it)] = (gradient[(*it)] * rhs.GetValue() - GetValue() * rhs.Derivative(*it)) / (rhs.GetValue() * rhs.GetValue());
                //                }


                //                if (Variable<REAL_T, group>::is_supporting_arbitrary_order) {
                //                    std::vector<Statement<REAL_T> > s;
                //                    PushStatements(s);
                //                    rhs.PushStatements(s);
                //                    s.push_back(Statement<REAL_T > (DIVIDE));
                //                    statements = s;
                //                }

            }
            SetValue(GetValue() / rhs.GetValue());
            return *this;
        }

        Variable& operator/=(const Variable& rhs) {
            if (Variable<REAL_T, group>::is_recording_g) {

                rhs.PushIds(ids_set);

                ResizeGradientVector(ids_set.max + 1);
                uint32_t* ids = ids_set.Data();
                size_t size = ids_set.Size();
                int i;
                for (i = 0; i < size; i++) {
                    REAL_T& dx = gradient[ids[i]];
                    //                    dx = (dx * rhs.GetValue() - GetValue() * rhs.Derivative(ids[i])) / (rhs.GetValue() * rhs.GetValue());
#ifdef USE_COMPLEX_STEP_DIFFERNTIATION
                    dx = (dx * rhs.GetValue() - GetValue() * rhs.ComplexStepValue(ids[i], H_V).imag() / H_V) / (rhs.GetValue() * rhs.GetValue());
#else
                    dx = (dx * rhs.GetValue() - GetValue() * rhs.ComplexStepValue(ids[i], H_V).imag() / H_V) / (rhs.GetValue() * rhs.GetValue());
#endif

                }

                //                for (it = ids_set.begin(); it != ids_set.end(); ++it) {
                //
                //                    //                    REAL_T dx = (gradient[(*it)] * rhs.GetValue() - GetValue() * rhs.Derivative(*it)) / (rhs.GetValue() * rhs.GetValue());
                //
                //                    gradient[(*it)] = (gradient[(*it)] * rhs.GetValue() - GetValue() * rhs.Derivative(*it)) / (rhs.GetValue() * rhs.GetValue());
                //                }

                //                if (Variable<REAL_T, group>::is_supporting_arbitrary_order) {
                //                    std::vector<Statement<REAL_T> > s;
                //                    PushStatements(s);
                //                    rhs.PushStatements(s);
                //                    s.push_back(Statement<REAL_T > (DIVIDE));
                //                    statements = s;
                //                }

            }
            SetValue(GetValue() / rhs.GetValue());
        }

        Variable& operator/=(const REAL_T& rhs) {
            //            SetValue(GetValue() / rhs);
            //            return *this;
            return *this = (*this / rhs);
        }

        const REAL_T WRT(const Variable<REAL_T> &x) {
            if (x.GetId() != 0 && x.GetId() == this->id_m) {
                return REAL_T(1.0);
            }
            return x.GetId() < gsize ? gradient[x.GetId()] : REAL_T(0.0);
            //            if (x.GetId() < gsize) {
            //                return gradient[x.GetId()];
            //                //                if (gradient[x.GetId()].first) {
            //                //                    return gradient[x.GetId()].second;
            //                //                } else {
            //                //                    return 0.0;
            //                //                }
            //            } else {
            //                return 0.0;
            //            }
        }

        const REAL_T WRT(const Variable<REAL_T> &x) const {
            if (x.GetId() != 0 && x.GetId() == this->id_m) {
                return REAL_T(1.0);
            }
            return x.GetId() < gsize ? gradient[x.GetId()] : REAL_T(0.0);
            //            if (x.GetId() < gsize) {
            //                return gradient[x.GetId()];
            //                //                if (gradient[x.GetId()].first) {
            //                //                    return gradient[x.GetId()].second;
            //                //                } else {
            //                //                    return 0.0;
            //                //                }
            //            } else {
            //                return 0.0;
            //            }
        }

        /**
         * Returns sqrt(sum a = A,B { (df/da)^2*u(a)^2}), where u(a) is 
         * round error.
         * 
         **/
        const REAL_T GetUncertainty() {



            REAL_T temp = REAL_T(0);
            REAL_T squared_epsilon = std::numeric_limits<REAL_T>::epsilon() * std::numeric_limits<REAL_T>::epsilon();
            REAL_T dif;
            for (size_t i = 0; i < this->gradient.size(); i++) {
                dif = this->gradient[i];
                temp += dif * dif * squared_epsilon;
            }

            return std::sqrt(temp);

        }

        /**
         * Returns the name of this variable. Names are not initialized and 
         * if it is not set this function will return a empty string.
         * @return 
         */
        std::string GetName() const {
            return name_m;
        }

        /**
         * Set the name of this variable.
         * 
         * @param name
         */
        void SetName(std::string name) {
            name_m = name;
        }

        /**
         * Returns true if this variable is bounded. Otherwise,
         * false.
         * 
         * @return 
         */
        bool IsBounded() {
            return bounded_m;
        }

        /**
         * Set the min and max boundary for this variable. 
         * @param min
         * @param max
         */
        void SetBounds(const REAL_T& min, const REAL_T& max) {
            bounded_m = true;
            max_boundary_m = max;
            min_boundary_m = min;

            if (GetValue() < min || GetValue() > max) {
                value_m = (min + max) / 2.0;
            }

        }

        /**
         * Return the minimum boundary for this variable. The default value 
         * is the results of std::numeric_limits<REAL_t>::min().
         * 
         * @return 
         */
        const REAL_T GetMinBoundary() {
            return min_boundary_m;
        }

        /**
         * Return the maximum boundary for this variable. The default value 
         * is the results of std::numeric_limits<REAL_t>::max().
         * 
         * @return 
         */
        const REAL_T GetMaxBoundary() {
            return max_boundary_m;
        }

        /**
         * Make this Variable an independent variable. If set to true,
         * the Variables unique identifier is registered in the static set
         * of independent variables. During function evaluations, gradient 
         * information is accumulated in post-order wrt to variables in the 
         * static set of independent variables. If set false, this Variables 
         * unique identifier will be removed from the set, if it existed.
         * 
         * 
         * To access the derivative wrt to a Variable, call function:
         * 
         * const REAL_t wrt(const Variable & ind)
         * 
         * 
         * @param is_independent
         */
        void SetAsIndependent(const bool &is_independent) {
            IDTrait<REAL_T> idt;
            if (!is_independent_m && is_independent && independent_id_m == 0) {
                independent_id_m = IndependentVariableIdGenerator::instance()->next();
                this->id_m = independent_id_m;
                idt.SetId(value_m, this->id_m);
                is_independent_m = true;
            } else if (is_independent_m && !is_independent && independent_id_m != 0) {
                this->id_m = 0;
                is_independent_m = false;
            } else {
                this->id_m = independent_id_m;
                is_independent_m = true;
            }

        }

        bool IsIndependent() {
            return is_independent_m;
        }

        /**
         * Sets the value of this variable. If the variable is bounded, 
         * the value will be set between the min and max boundary. If the
         * value is less than the minimum boundary, the variables value is 
         * set to minimum boundary. If the  value is greater than the maximum
         * boundary, the variables value is set to maximum boundary. If the 
         * value is signaling nan, the value is set to the mid point between
         * the min and max boundary. 
         * 
         * @param value
         */
        void SetValue(const REAL_T &value) {

            if (bounded_m) {
                if (value != value) {//nan
                    value_m = min_boundary_m + (max_boundary_m - min_boundary_m) / static_cast<REAL_T> (2.0);

                    return;
                }

                if (value < min_boundary_m) {
                    value_m = min_boundary_m;
                } else if (value > max_boundary_m) {
                    value_m = max_boundary_m;


                } else {
                    value_m = value;
                }
            } else {
                value_m = value;
            }
        }

        inline const REAL_T GetValue() const {
            return value_m;
        }

        inline const REAL_T Derivative(const uint32_t id, bool &found) const {

            //            if (id < gsize) {
            //                if (gradient[id].first) {
            //                    found = true;
            //                    return gradient[id].second;
            //                } else {
            //                    return 0.0;
            //                }
            //            }
            //
            //            if (id_m) {
            //                if (id_m == id) {
            //                    found = true;
            //                    return 1.0;
            //                } else {
            //                    return 0;
            //                }
            //            }

            return 0.0;
        }

        /**
         * Return the derivative wrt to parameter id. If the gradient vector is
         * larger than id, return the value at index id, else check
         * if this id is equal to the provided id. If it is equal return 1, else
         * return 0.
         * @param id
         * @return 
         */
        inline const REAL_T Derivative(const uint32_t &id) const {
            return unsigned(gsize) > unsigned(id) ?
                    gradient[id] :
                    static_cast<REAL_T> ((unsigned(id) == unsigned(this->id_m)));
        }

        inline const std::complex<REAL_T> ComplexStepValue(const uint32_t & id, REAL_T h = REAL_T(0.00000000000001)) const {
            return this->id_m == id ? std::complex<REAL_T>(GetValue(), h) : std::complex<REAL_T>(GetValue());
        }

        inline void PushIds(et4ad::IDSet & ids) const {
            if (!unsigned(this->id_m)) {
                //                IDSet::id_iterator it;
                //                IDSet::id_iterator ite = ids_set.end();
                const uint32_t* ids_m = ids_set.Data();
                const size_t size = ids_set.Size();
                int i;
                //                for (it = ids_set.begin(); it != ite; ++it) {
                for (i = 0; i < size; i++) {
                    //                    ids.insert((*it));
                    ids.insert(ids_m[i]);
                }
            } else {
                ids.insert(this->id_m);
            }
        }

        inline void PushStatements(std::vector<Statement<REAL_T> > &other) const {
            if (statements.size() == 0) {
                other.push_back(Statement<REAL_T > (VARIABLE, GetValue(), this->GetId()));
            } else {
                other.insert(other.end(), statements.begin(), statements.end());
            }

        }

        size_t Size()const {
            return this->gradient.size();
        }

    };

    template<>
    struct IDTrait<et4ad::Variable<double> > {

        void SetId(et4ad::Variable<double>& t, uint32_t id) {
            t.id_m = id;
            t.is_independent_m = true;
        }
    };

    template<>
    struct IDTrait<et4ad::Variable<et4ad::Variable<double> > > {

        void SetId(et4ad::Variable<et4ad::Variable<double> >& t, uint32_t id) {
            t.id_m = id;
            t.is_independent_m = true;
        }
    };

    template <class REAL_T, int group>
    bool Variable<REAL_T, group>::is_recording_g = true;

    template <class REAL_T, int group>
    bool Variable<REAL_T, group>::is_supporting_arbitrary_order = false;

    /**
     * A wrapper class for Variables. Provides synchronized access for situations
     * where thread safety is desired.
     */
    template<class REAL_T, int group = 0 >
    class SynchronizedVariable {
    public:

    private:
        Variable<REAL_T, group> var;

    };

    /**
     * Returns a variable that is the derivative w.r.t x.
     * Requires support for arbitrary. see SetSupportingArbitraryOrder(bool)
     * 
     * @param x
     * @return 
     */
    template<class REAL_T>
    static const REAL_T DiffV(const std::vector<Statement<REAL_T> >& statements, const Variable<REAL_T> &x) {
        if (statements.size() == 0) {
            return 0.0;
        }
        std::vector<std::pair<REAL_T, REAL_T > > v;
        v.reserve(statements.size());
        std::stack<std::pair<REAL_T, REAL_T >,
                std::vector<std::pair<REAL_T, REAL_T > > > stack;
        //            ad::Stack<std::pair<T, T> > stack;
        bool found = false;
        int size = statements.size();
        std::pair<REAL_T, REAL_T > lhs = std::pair<REAL_T, REAL_T > (0, 0);
        std::pair<REAL_T, REAL_T > rhs = std::pair<REAL_T, REAL_T > (0, 0);

        for (int i = 0; i < size; i++) {



            REAL_T temp = REAL_T(0);


            switch (statements[i].op_m) {

                case CONSTANT:
                    stack.push(std::pair<REAL_T, REAL_T > (statements[i].value_m, (0.0)));
                    break;
                case VARIABLE:
                    if (statements[i].id_m == x.GetId()) {
                        found = true;
                        stack.push(std::pair<REAL_T, REAL_T > (statements[i].value_m, REAL_T(1.0)));

                    } else {//constant
                        //f(x) = C
                        //f'(x) = 0
                        stack.push(std::pair<REAL_T, REAL_T > (statements[i].value_m, REAL_T(0.0)));

                    }

                    break;
                case PLUS:
                    rhs = stack.top();
                    stack.pop();
                    lhs = stack.top();
                    stack.pop();
                    stack.push(std::pair<REAL_T, REAL_T > (lhs.first + rhs.first, lhs.second + rhs.second));

                    break;
                case MINUS:
                    rhs = stack.top();
                    stack.pop();
                    lhs = stack.top();
                    stack.pop();
                    stack.push(std::pair<REAL_T, REAL_T > (lhs.first - rhs.first, lhs.second - rhs.second));

                    break;
                case MULTIPLY:
                    rhs = stack.top();
                    stack.pop();
                    lhs = stack.top();
                    stack.pop();
                    temp = lhs.second * rhs.first + lhs.first * rhs.second;


                    stack.push(std::pair<REAL_T, REAL_T > (lhs.first * rhs.first, temp));

                    break;
                case DIVIDE:
                    rhs = stack.top();
                    stack.pop();
                    lhs = stack.top();
                    stack.pop();
                    temp = (lhs.second * rhs.first - lhs.first * rhs.second) / (rhs.first * rhs.first);
                    stack.push(std::pair<REAL_T, REAL_T > (lhs.first / rhs.first, temp));

                    break;

                case SIN:
                    lhs = stack.top();
                    stack.pop();
                    if (found) {
                        stack.push(std::pair<REAL_T, REAL_T > (std::sin(lhs.first), lhs.second * std::cos(lhs.first)));
                    } else {
                        stack.push(std::pair<REAL_T, REAL_T > (std::sin(lhs.first), 0));
                    }

                    break;
                case COS:

                    lhs = stack.top();
                    stack.pop();
                    if (found) {
                        stack.push(std::pair<REAL_T, REAL_T > (std::cos(lhs.first), lhs.second * (-1.0) * std::sin(lhs.first)));
                    } else {
                        stack.push(std::pair<REAL_T, REAL_T > (std::cos(lhs.first), 0));
                    }

                    break;
                case TAN:
                    lhs = stack.top();
                    stack.pop();
                    if (found) {
                        temp = lhs.second * ((1.0 / std::cos(lhs.first))*(1.0 / std::cos(lhs.first)));
                        stack.push(std::pair<REAL_T, REAL_T > (std::tan(lhs.first), temp));
                    } else {
                        stack.push(std::pair<REAL_T, REAL_T > (std::tan(lhs.first), 0));
                    }

                    break;
                case ASIN:
                    lhs = stack.top();
                    stack.pop();
                    if (found) {
                        temp = (lhs.second * 1.0 / std::pow((static_cast<REAL_T> (1.0) - std::pow(lhs.first, static_cast<REAL_T> (2.0))), static_cast<REAL_T> (0.5)));
                        stack.push(std::pair<REAL_T, REAL_T > (std::asin(lhs.first), temp));
                    } else {
                        stack.push(std::pair<REAL_T, REAL_T > (std::asin(lhs.first), 0));
                    }

                    break;
                case ACOS:
                    lhs = stack.top();
                    stack.pop();
                    if (found) {
                        temp = (lhs.second * (-1.0) / std::pow((static_cast<REAL_T> (1.0) - std::pow(lhs.first, static_cast<REAL_T> (2.0))), static_cast<REAL_T> (0.5)));
                        stack.push(std::pair<REAL_T, REAL_T > (std::acos(lhs.first), temp));
                    } else {
                        stack.push(std::pair<REAL_T, REAL_T > (std::acos(lhs.first), (0)));
                    }

                    break;
                case ATAN:
                    lhs = stack.top();
                    stack.pop();
                    if (found) {
                        temp = (lhs.second * (1.0) / (lhs.first * lhs.first + (1.0)));
                        stack.push(std::pair<REAL_T, REAL_T > (std::atan(lhs.first), temp));
                    } else {
                        stack.push(std::pair<REAL_T, REAL_T > (std::atan(lhs.first), (0)));
                    }

                    break;
                case ATAN2:
                    rhs = stack.top();
                    stack.pop();
                    lhs = stack.top();
                    stack.pop();
                    if (found) {
                        temp = (rhs.first * lhs.second / (lhs.first * lhs.first + (rhs.first * rhs.first)));
                        stack.push(std::pair<REAL_T, REAL_T > (std::atan2(lhs.first, rhs.first), temp));
                    } else {
                        stack.push(std::pair<REAL_T, REAL_T > (std::atan2(lhs.first, rhs.first), (0)));
                    }

                    break;
                case SQRT:
                    lhs = stack.top();
                    stack.pop();
                    if (found) {
                        temp = lhs.second * (.5) / std::sqrt(lhs.first);
                        stack.push(std::pair<REAL_T, REAL_T > (std::sqrt(lhs.first), temp));
                    } else {
                        stack.push(std::pair<REAL_T, REAL_T > (std::sqrt(lhs.first), (0)));
                    }

                    break;
                case POW:
                    rhs = stack.top();
                    stack.pop();
                    lhs = stack.top();
                    stack.pop();
                    if (found) {
                        temp = (lhs.second * rhs.first) *
                                std::pow(lhs.first, (rhs.first - static_cast<REAL_T> (1.0)));
                        stack.push(std::pair<REAL_T, REAL_T > (std::pow(lhs.first, rhs.first), temp));
                    } else {
                        stack.push(std::pair<REAL_T, REAL_T > (std::pow(lhs.first, rhs.first), (0)));
                    }

                    break;
                case LOG:
                    lhs = stack.top();
                    stack.pop();
                    if (found) {
                        temp = (lhs.second * ((1.0))) / lhs.first;
                        stack.push(std::pair<REAL_T, REAL_T > (std::log(lhs.first), temp));
                    } else {
                        stack.push(std::pair<REAL_T, REAL_T > (std::log(lhs.first), (0)));
                    }

                    break;
                case LOG10:
                    lhs = stack.top();
                    stack.pop();
                    if (found) {
                        temp = (lhs.second * (1.0)) / (lhs.first * std::log((10.0)));
                        stack.push(std::pair<REAL_T, REAL_T > (std::log10(lhs.first), temp));
                    } else {
                        stack.push(std::pair<REAL_T, REAL_T > (std::log10(lhs.first), (0)));
                    }

                    break;
                case EXP:
                    lhs = stack.top();
                    stack.pop();
                    if (found) {
                        temp = lhs.second * std::exp(lhs.first);
                        stack.push(std::pair<REAL_T, REAL_T > (std::exp(lhs.first), temp));
                    } else {
                        stack.push(std::pair<REAL_T, REAL_T > (std::exp(lhs.first), (0)));
                    }

                    break;
                case MFEXP:
                    lhs = stack.top();
                    stack.pop();
                    if (found) {
                        temp = lhs.second * std::mfexp(lhs.first);
                        stack.push(std::pair<REAL_T, REAL_T > (std::mfexp(lhs.first), temp));
                    } else {
                        stack.push(std::pair<REAL_T, REAL_T > (std::mfexp(lhs.first), (0)));
                    }

                    break;
                case SINH:
                    lhs = stack.top();
                    stack.pop();
                    if (found) {
                        temp = lhs.second * std::cosh(lhs.first);
                        stack.push(std::pair<REAL_T, REAL_T > (std::sinh(lhs.first), temp));
                    } else {
                        stack.push(std::pair<REAL_T, REAL_T > (std::sinh(lhs.first), (0)));
                    }

                    break;
                case COSH:
                    lhs = stack.top();
                    stack.pop();
                    if (found) {
                        temp = lhs.second * std::sinh(lhs.first);
                        stack.push(std::pair<REAL_T, REAL_T > (std::cosh(lhs.first), temp));
                    } else {
                        stack.push(std::pair<REAL_T, REAL_T > (std::cosh(lhs.first), (0)));
                    }

                    break;
                case TANH:
                    lhs = stack.top();
                    stack.pop();
                    if (found) {
                        temp = lhs.second * ((1.0) / std::cosh(lhs.first))*((1.0) / std::cosh(lhs.first));
                        stack.push(std::pair<REAL_T, REAL_T > (std::tanh(lhs.first), temp));
                    } else {
                        stack.push(std::pair<REAL_T, REAL_T > (std::tanh(lhs.first), (0)));
                    }

                    break;
                case FABS:
                    lhs = stack.top();
                    stack.pop();
                    if (found) {
                        temp = (lhs.second * lhs.first) /
                                std::fabs(lhs.first);
                        stack.push(std::pair<REAL_T, REAL_T > (std::fabs(lhs.first), temp));
                    } else {
                        stack.push(std::pair<REAL_T, REAL_T > (std::fabs(lhs.first), (0)));
                    }

                    break;
                case ABS:
                    lhs = stack.top();
                    stack.pop();
                    if (found) {
                        temp = (lhs.second * lhs.first) /
                                std::fabs(lhs.first);
                        stack.push(std::pair<REAL_T, REAL_T > (std::fabs(lhs.first), temp));
                    } else {
                        stack.push(std::pair<REAL_T, REAL_T > (std::fabs(lhs.first), (0)));
                    }

                    break;
                case FLOOR:
                    lhs = stack.top();
                    stack.pop();
                    if (found) {

                        temp = (0); //lhs.second * T(std::floor(lhs.first));
                        stack.push(std::pair<REAL_T, REAL_T > (std::floor(lhs.first), temp));
                    } else {
                        stack.push(std::pair<REAL_T, REAL_T > (std::floor(lhs.first), (0)));
                    }

                    break;
                case NONE:
                    std::cout << "nothing to do here.\n";
                    break;
                default:
                    break;

            }


        }

        //            std::cout << "returning " << stack.top().second << "\n\n";
        return stack.top().second;
    }

    /**
     * Returns a variable that is the derivative w.r.t x.
     * Requires support for arbitrary. see SetSupportingArbitraryOrder(bool)
     * 
     * @param x
     * @return 
     */
    template<class REAL_T>
    static Variable<REAL_T> Diff(const std::vector<Statement<REAL_T> >& statements, const Variable<REAL_T> &x) {
        if (statements.size() == 0) {
            return 0.0;
        }
        std::vector<std::pair<Variable<REAL_T>, Variable<REAL_T> > > v;
        v.reserve(statements.size()*200);
        std::stack<std::pair<Variable<REAL_T>, Variable<REAL_T> >,
                std::vector<std::pair<Variable<REAL_T>, Variable<REAL_T> > > > stack(v);
        //                        et4ad::Stack<std::pair<Variable<REAL_T>, Variable<REAL_T> > > stack;
        bool found = false;
        int size = statements.size();
        std::pair<Variable<REAL_T>, Variable<REAL_T> > lhs = std::pair<Variable<REAL_T>, Variable<REAL_T> > (0, 0);
        std::pair<Variable<REAL_T>, Variable<REAL_T> > rhs = std::pair<Variable<REAL_T>, Variable<REAL_T> > (0, 0);


        Variable<REAL_T> temp;
        for (int i = 0; i < size; i++) {


            temp = 0.0;

            switch (statements[i].op_m) {

                case CONSTANT:
                    stack.push(std::pair<Variable<REAL_T>, Variable<REAL_T> > (statements[i].value_m, 0.0));
                    break;
                case VARIABLE:
                    if (statements[i].id_m == x.GetId()) {
                        found = true;
                        //f(x) = x
                        //f'(x) = 1
                        stack.push(std::pair<Variable<REAL_T>, Variable<REAL_T> > (statements[i].value_m, 1.0));
                        stack.top().first.value_m = statements[i].value_m;
                        stack.top().first.id_m = statements[i].id_m;
                        stack.top().first.independent_id_m = statements[i].id_m;
                        stack.top().first.is_independent_m = true;
                    } else {//constant
                        //f(x) = C
                        //f'(x) = 0
                        stack.push(std::pair<Variable<REAL_T>, Variable<REAL_T> > (statements[i].value_m, 0.0));
                        stack.top().first.value_m = statements[i].value_m;
                        stack.top().first.id_m = statements[i].id_m;
                        stack.top().first.independent_id_m = statements[i].id_m;
                        if (statements[i].id_m > 0) {
                            stack.top().first.is_independent_m = true;
                        }
                    }

                    break;
                case PLUS:
                    rhs = stack.top();
                    stack.pop();
                    lhs = stack.top();
                    stack.pop();
                    stack.push(std::pair<Variable<REAL_T>, Variable<REAL_T> > (lhs.first + rhs.first, lhs.second + rhs.second));

                    break;
                case MINUS:
                    rhs = stack.top();
                    stack.pop();
                    lhs = stack.top();
                    stack.pop();
                    stack.push(std::pair<Variable<REAL_T>, Variable<REAL_T> > (lhs.first - rhs.first, lhs.second - rhs.second));


                    break;
                case MULTIPLY:
                    rhs = stack.top();
                    stack.pop();
                    lhs = stack.top();
                    stack.pop();
                    temp = lhs.second * rhs.first + lhs.first * rhs.second;

                    stack.push(std::pair<Variable<REAL_T>, Variable<REAL_T> > (lhs.first * rhs.first, temp));

                    break;
                case DIVIDE:
                    rhs = stack.top();
                    stack.pop();
                    lhs = stack.top();
                    stack.pop();

                    temp = (lhs.second * rhs.first - lhs.first * rhs.second) / (rhs.first * rhs.first);
                    stack.push(std::pair<Variable<REAL_T>, Variable<REAL_T> > (lhs.first / rhs.first, temp));
                    break;

                case SIN:
                    lhs = stack.top();
                    stack.pop();
                    if (found) {
                        stack.push(std::pair<Variable<REAL_T>, Variable<REAL_T> > (std::sin(lhs.first), lhs.second * std::cos(lhs.first)));
                    } else {
                        stack.push(std::pair<Variable<REAL_T>, Variable<REAL_T> > (std::sin(lhs.first), 0));
                    }

                    break;
                case COS:

                    lhs = stack.top();
                    stack.pop();
                    if (found) {
                        stack.push(std::pair<Variable<REAL_T>, Variable<REAL_T> > (std::cos(lhs.first), lhs.second * (static_cast<REAL_T> (-1.0)) * std::sin(lhs.first)));
                    } else {
                        stack.push(std::pair<Variable<REAL_T>, Variable<REAL_T> > (std::cos(lhs.first), 0));
                    }

                    break;
                case TAN:
                    lhs = stack.top();
                    stack.pop();
                    if (found) {
                        temp = lhs.second * ((static_cast<REAL_T> (1.0) / std::cos(lhs.first))*(static_cast<REAL_T> (1.0) / std::cos(lhs.first)));
                        stack.push(std::pair<Variable<REAL_T>, Variable<REAL_T> > (std::tan(lhs.first), temp));
                    } else {
                        stack.push(std::pair<Variable<REAL_T>, Variable<REAL_T> > (std::tan(lhs.first), 0));
                    }

                    break;
                case ASIN:
                    lhs = stack.top();
                    stack.pop();
                    if (found) {
                        temp = (lhs.second * static_cast<REAL_T> (1.0) / std::pow((static_cast<REAL_T> (1.0) - std::pow(lhs.first, static_cast<REAL_T> (2.0))), static_cast<REAL_T> (0.5)));
                        stack.push(std::pair<Variable<REAL_T>, Variable<REAL_T> > (std::asin(lhs.first), temp));
                    } else {
                        stack.push(std::pair<Variable<REAL_T>, Variable<REAL_T> > (std::asin(lhs.first), 0));
                    }

                    break;
                case ACOS:
                    lhs = stack.top();
                    stack.pop();
                    if (found) {
                        temp = (lhs.second * (static_cast<REAL_T> (-1.0)) / std::pow((static_cast<REAL_T> (1.0) - std::pow(lhs.first, static_cast<REAL_T> (2.0))), static_cast<REAL_T> (0.5)));
                        stack.push(std::pair<Variable<REAL_T>, Variable<REAL_T> > (std::acos(lhs.first), temp));
                    } else {
                        stack.push(std::pair<Variable<REAL_T>, Variable<REAL_T> > (std::acos(lhs.first), (0)));
                    }

                    break;
                case ATAN:
                    lhs = stack.top();
                    stack.pop();
                    if (found) {
                        temp = (lhs.second * static_cast<REAL_T> ((1.0)) / (lhs.first * lhs.first + static_cast<REAL_T> ((1.0))));
                        stack.push(std::pair<Variable<REAL_T>, Variable<REAL_T> > (std::atan(lhs.first), temp));
                    } else {
                        stack.push(std::pair<Variable<REAL_T>, Variable<REAL_T> > (std::atan(lhs.first), (0)));
                    }

                    break;
                case ATAN2:
                    rhs = stack.top();
                    stack.pop();
                    lhs = stack.top();
                    stack.pop();
                    if (found) {
                        temp = (rhs.first * lhs.second / (lhs.first * lhs.first + (rhs.first * rhs.first)));
                        stack.push(std::pair<Variable<REAL_T>, Variable<REAL_T> > (std::atan2(lhs.first, rhs.first), temp));
                    } else {
                        stack.push(std::pair<Variable<REAL_T>, Variable<REAL_T> > (std::atan2(lhs.first, rhs.first), (0)));
                    }

                    break;
                case SQRT:
                    lhs = stack.top();
                    stack.pop();
                    if (found) {
                        temp = lhs.second * static_cast<REAL_T> ((.5)) / std::sqrt(lhs.first);
                        stack.push(std::pair<Variable<REAL_T>, Variable<REAL_T> > (std::sqrt(lhs.first), temp));
                    } else {
                        stack.push(std::pair<Variable<REAL_T>, Variable<REAL_T> > (std::sqrt(lhs.first), (0)));
                    }

                    break;
                case POW:
                    rhs = stack.top();
                    stack.pop();
                    lhs = stack.top();
                    stack.pop();
                    if (found) {
                        temp = (lhs.second * rhs.first) *
                                std::pow(lhs.first, (rhs.first - static_cast<REAL_T> (1.0)));
                        stack.push(std::pair<Variable<REAL_T>, Variable<REAL_T> > (std::pow(lhs.first, rhs.first), temp));
                    } else {
                        stack.push(std::pair<Variable<REAL_T>, Variable<REAL_T> > (std::pow(lhs.first, rhs.first), (0)));
                    }

                    break;
                case LOG:
                    lhs = stack.top();
                    stack.pop();
                    if (found) {
                        temp = (lhs.second * (Constant<REAL_T > (1.0))) / lhs.first;
                        stack.push(std::pair<Variable<REAL_T>, Variable<REAL_T> > (std::log(lhs.first), temp));
                    } else {
                        stack.push(std::pair<Variable<REAL_T>, Variable<REAL_T> > (std::log(lhs.first), (0)));
                    }

                    break;
                case LOG10:
                    lhs = stack.top();
                    stack.pop();
                    if (found) {
                        temp = (lhs.second * static_cast<REAL_T> ((1.0))) / (lhs.first * std::log(static_cast<REAL_T> ((10.0))));
                        stack.push(std::pair<Variable<REAL_T>, Variable<REAL_T> > (std::log10(lhs.first), temp));
                    } else {
                        stack.push(std::pair<Variable<REAL_T>, Variable<REAL_T> > (std::log10(lhs.first), (0)));
                    }

                    break;
                case EXP:
                    lhs = stack.top();
                    stack.pop();
                    if (found) {
                        temp = lhs.second * std::exp(lhs.first);
                        stack.push(std::pair<Variable<REAL_T>, Variable<REAL_T> > (std::exp(lhs.first), temp));
                    } else {
                        stack.push(std::pair<Variable<REAL_T>, Variable<REAL_T> > (std::exp(lhs.first), (0)));
                    }

                    break;
                case MFEXP:
                    lhs = stack.top();
                    stack.pop();
                    if (found) {
                        temp = lhs.second * std::mfexp(lhs.first);
                        stack.push(std::pair<Variable<REAL_T>, Variable<REAL_T> > (std::mfexp(lhs.first), temp));
                    } else {
                        stack.push(std::pair<Variable<REAL_T>, Variable<REAL_T> > (std::mfexp(lhs.first), (0)));
                    }

                    break;
                case SINH:
                    lhs = stack.top();
                    stack.pop();
                    if (found) {
                        temp = lhs.second * std::cosh(lhs.first);
                        stack.push(std::pair<Variable<REAL_T>, Variable<REAL_T> > (std::sinh(lhs.first), temp));
                    } else {
                        stack.push(std::pair<Variable<REAL_T>, Variable<REAL_T> > (std::sinh(lhs.first), (0)));
                    }

                    break;
                case COSH:
                    lhs = stack.top();
                    stack.pop();
                    if (found) {
                        temp = lhs.second * std::sinh(lhs.first);
                        stack.push(std::pair<Variable<REAL_T>, Variable<REAL_T> > (std::cosh(lhs.first), temp));
                    } else {
                        stack.push(std::pair<Variable<REAL_T>, Variable<REAL_T> > (std::cosh(lhs.first), (0)));
                    }

                    break;
                case TANH:
                    lhs = stack.top();
                    stack.pop();
                    if (found) {
                        temp = lhs.second * (static_cast<REAL_T> ((1.0)) / std::cosh(lhs.first))*(static_cast<REAL_T> ((1.0)) / std::cosh(lhs.first));
                        stack.push(std::pair<Variable<REAL_T>, Variable<REAL_T> > (std::tanh(lhs.first), temp));
                    } else {
                        stack.push(std::pair<Variable<REAL_T>, Variable<REAL_T> > (std::tanh(lhs.first), (0)));
                    }

                    break;
                case FABS:
                    lhs = stack.top();
                    stack.pop();
                    if (found) {
                        temp = (lhs.second * lhs.first) /
                                std::fabs(lhs.first);
                        stack.push(std::pair<Variable<REAL_T>, Variable<REAL_T> > (std::fabs(lhs.first), temp));
                    } else {
                        stack.push(std::pair<Variable<REAL_T>, Variable<REAL_T> > (std::fabs(lhs.first), (0)));
                    }

                    break;
                case ABS:
                    lhs = stack.top();
                    stack.pop();
                    if (found) {
                        temp = (lhs.second * lhs.first) /
                                std::fabs(lhs.first);
                        stack.push(std::pair<Variable<REAL_T>, Variable<REAL_T> > (std::fabs(lhs.first), temp));
                    } else {
                        stack.push(std::pair<Variable<REAL_T>, Variable<REAL_T> > (std::fabs(lhs.first), (0)));
                    }

                    break;
                case FLOOR:
                    lhs = stack.top();
                    stack.pop();
                    if (found) {

                        temp = (0);
                        stack.push(std::pair<Variable<REAL_T>, Variable<REAL_T> > (std::floor(lhs.first), temp));
                    } else {
                        stack.push(std::pair<Variable<REAL_T>, Variable<REAL_T> > (std::floor(lhs.first), (0)));
                    }

                    break;
                case NONE:
                    std::cout << "nothing to do here.\n";
                    break;
                default:
                    break;

            }



        }


        return stack.top().second;
    }


}






#endif	/* VARIABLE_HPP */

