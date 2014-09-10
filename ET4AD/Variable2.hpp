/* 
 * File:   Variable2.hpp
 * Author: matthewsupernaw
 *
 * Created on September 9, 2014, 11:08 AM
 */

#ifndef VARIABLE2_HPP
#define	VARIABLE2_HPP
#include <limits>

#include "Expression.hpp"
#include "IDSet.hpp"
#include "IDGenerator.hpp"

namespace et4ad2 {

    template<class T>
    struct IDTrait {

        void SetId(T& t, uint32_t id) {

        }
    };

    enum ACCUMULATION_TYPE {
        EXPRESSION_LEVEL = 0,
        EXPRESSION_STACK,
        ADJOINT,
        TANGENT_LINEAR,
        COMPLEX_STEP
    };

    template<class REAL_T, //base type
    int group = 0, //group identifier
    int ACCUMULATOR = EXPRESSION_LEVEL> //accumulation method
    class DefaultVariable;

    template<class REAL_T, class T>
    static const REAL_T DiffValue(const std::vector<Statement<REAL_T> >& statements, const et4ad::ExpressionBase<REAL_T, T> &x);

    template<class REAL_T, //base type
    int group, //group identifier
    int ACCUMULATOR > //accumulation method
    class DefaultVariable : public et4ad::ExpressionBase<REAL_T, DefaultVariable<REAL_T, group, ACCUMULATOR > > {
        template<class T> friend struct IDTrait;
        std::vector<REAL_T > gradient;
        mutable et4ad::IDSet ids_set;
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
        mutable std::vector<Statement<REAL_T> > statements;
        //IDSet::id_iterator it;

        inline void ResizeGradientVector() {
            //should make this power of 2
            size_t size = et4ad::IndependentVariableIdGenerator::instance()->current() + 1;
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
            return DefaultVariable<REAL_T, group, ACCUMULATOR>::is_recording_g;
        }

        static void SetRecording(bool is_recording_g) {
            DefaultVariable<REAL_T, group, ACCUMULATOR>::is_recording_g = is_recording_g;
        }

        DefaultVariable() : et4ad::ExpressionBase<REAL_T, DefaultVariable<REAL_T, group, ACCUMULATOR> >(),
        gsize(0),
        value_m(0.0),
        min_boundary_m(std::numeric_limits<REAL_T>::min()),
        max_boundary_m(std::numeric_limits<REAL_T>::max()),
        is_independent_m(false),
        independent_id_m(0),
        bounded_m(false) {
        }

        DefaultVariable(const REAL_T &value/*, bool independent = false*/) : et4ad::ExpressionBase<REAL_T, DefaultVariable<REAL_T, group, ACCUMULATOR> >(0),
        gsize(0),
        value_m(value),
        min_boundary_m(std::numeric_limits<REAL_T>::min()),
        max_boundary_m(std::numeric_limits<REAL_T>::max()),
        is_independent_m(false),
        independent_id_m(0),
        bounded_m(false) {

            // SetAsIndependent(independent);
        }

        DefaultVariable(const DefaultVariable<REAL_T, group, ACCUMULATOR> & orig) {
            gsize = 0;
            bounded_m = orig.bounded_m;
            is_independent_m = orig.is_independent_m;
            //            this->SetAsIndependent()
            independent_id_m = orig.independent_id_m;
            max_boundary_m = orig.max_boundary_m;
            min_boundary_m = orig.min_boundary_m;
            name_m = orig.name_m;
            this->id_m = orig.id_m;

            if (DefaultVariable<REAL_T, group, ACCUMULATOR>::is_recording_g) {

                orig.PushIds(ids_set);
                uint32_t* ids;
                size_t size;

                switch (ACCUMULATOR) {
                    case EXPRESSION_LEVEL:

                        ResizeGradientVector(ids_set.max + 1);
                        ids = ids_set.Data();
                        size = ids_set.Size();

                        for (int i = 0; i < size; i++) {
                            gradient[ids[i]] = orig.Derivative(ids[i]);
                        }

                        break;

                    case EXPRESSION_STACK:
                        orig.PushStatements(statements);
                        break;
                    case ADJOINT:
                        std::cout << "Adjoint(reverse) accumulation method not yet implemented.";
                        exit(0);
                        break;
                    case TANGENT_LINEAR:
                        std::cout << "Tangent linear(forward) accumulation method not yet implemented.";
                        exit(0);
                        break;

                    case COMPLEX_STEP:

                        ResizeGradientVector(ids_set.max + 1);
                        ids = ids_set.Data();
                        size = ids_set.Size();

                        for (int i = 0; i < size; i++) {
                            gradient[ids[i]] = orig.ComplexStepValue(ids[i], H_V).imag() / H_V;
                        }
                        break;

                    default:
                        ResizeGradientVector(ids_set.max + 1);
                        ids = ids_set.Data();
                        size = ids_set.Size();

                        for (int i = 0; i < size; i++) {
                            gradient[ids[i]] = orig.Derivative(ids[i]);
                        }

                }
            }
            SetValue(orig.GetValue());
        }

        template<class T >
        DefaultVariable(const et4ad::ExpressionBase<REAL_T, T>& expr) : et4ad::ExpressionBase<REAL_T, DefaultVariable<REAL_T, group> >(0),
        gsize(0),
        value_m(0.0),
        min_boundary_m(std::numeric_limits<REAL_T>::min()),
        max_boundary_m(std::numeric_limits<REAL_T>::max()),
        is_independent_m(false),
        independent_id_m(0),
        bounded_m(false) {



            if (DefaultVariable<REAL_T, group, ACCUMULATOR>::is_recording_g) {
                expr.PushIds(ids_set);
                uint32_t* ids;
                size_t size;

                switch (ACCUMULATOR) {
                    case EXPRESSION_LEVEL:

                        ResizeGradientVector(ids_set.max + 1);
                        ids = ids_set.Data();
                        size = ids_set.Size();

                        for (int i = 0; i < size; i++) {
                            gradient[ids[i]] = expr.Derivative(ids[i]);
                        }

                        break;

                    case EXPRESSION_STACK:
                        expr.PushStatements(statements);
                        break;
                    case ADJOINT:
                        std::cout << "Adjoint(reverse) accumulation method not yet implemented.";
                        exit(0);
                        break;
                    case TANGENT_LINEAR:
                        std::cout << "Tangent linear(forward) accumulation method not yet implemented.";
                        exit(0);
                        break;
                    case COMPLEX_STEP:

                        ResizeGradientVector(ids_set.max + 1);
                        ids = ids_set.Data();
                        size = ids_set.Size();

                        for (int i = 0; i < size; i++) {
                            gradient[ids[i]] = expr.ComplexStepValue(ids[i], H_V).imag() / H_V;
                        }
                        break;

                    default:
                        ResizeGradientVector(ids_set.max + 1);
                        ids = ids_set.Data();
                        size = ids_set.Size();

                        for (int i = 0; i < size; i++) {
                            gradient[ids[i]] = expr.Derivative(ids[i]);
                        }

                }
            }

            SetValue(expr.GetValue());
        }

        virtual ~DefaultVariable() {

        }

        //        operator REAL_T() {
        //            return this->GetValue();
        //        }
        //
        //        operator REAL_T()const {
        //            return this->GetValue();
        //        }


        //operators

        DefaultVariable<REAL_T, group, ACCUMULATOR>& operator=(const REAL_T & other) {
            SetValue(other);
            gradient.resize(0);
            statements.resize(0);

            return *this;
        }

        /**
         * Sets the value of this Variable to that of other.
         * 
         * @param rhs
         * @return 
         */
        DefaultVariable<REAL_T, group, ACCUMULATOR>& operator=(const DefaultVariable<REAL_T, group, ACCUMULATOR> & other) {

            if (DefaultVariable<REAL_T, group, ACCUMULATOR>::is_recording_g) {

                other.PushIds(ids_set);
                uint32_t* ids;
                size_t size;

                switch (ACCUMULATOR) {
                    case EXPRESSION_LEVEL:

                        ResizeGradientVector(ids_set.max + 1);
                        ids = ids_set.Data();
                        size = ids_set.Size();

                        for (int i = 0; i < size; i++) {
                            gradient[ids[i]] = other.Derivative(ids[i]);
                        }

                        break;

                    case EXPRESSION_STACK:
                        statements.resize(0);
                        other.PushStatements(statements);
                        break;
                    case ADJOINT:
                        std::cout << "Adjoint(reverse) accumulation method not yet implemented.";
                        exit(0);
                        break;
                    case TANGENT_LINEAR:
                        std::cout << "Tangent linear(forward) accumulation method not yet implemented.";
                        exit(0);
                        break;

                    case COMPLEX_STEP:

                        ResizeGradientVector(ids_set.max + 1);
                        ids = ids_set.Data();
                        size = ids_set.Size();

                        for (int i = 0; i < size; i++) {
                            gradient[ids[i]] = other.ComplexStepValue(ids[i], H_V).imag() / H_V;
                        }
                        break;

                    default:
                        ResizeGradientVector(ids_set.max + 1);
                        ids = ids_set.Data();
                        size = ids_set.Size();

                        for (int i = 0; i < size; i++) {
                            gradient[ids[i]] = other.Derivative(ids[i]);
                        }

                }
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
        template<class T >
        DefaultVariable<REAL_T, group, ACCUMULATOR>& operator=(const et4ad::ExpressionBase<REAL_T, T>& expr) {

            if (DefaultVariable<REAL_T, group, ACCUMULATOR>::is_recording_g) {

                expr.PushIds(ids_set);
                uint32_t* ids;
                size_t size;

                switch (ACCUMULATOR) {
                    case EXPRESSION_LEVEL:

                        ResizeGradientVector(ids_set.max + 1);
                        ids = ids_set.Data();
                        size = ids_set.Size();

                        for (int i = 0; i < size; i++) {
                            gradient[ids[i]] = expr.Derivative(ids[i]);
                        }

                        break;

                    case EXPRESSION_STACK:
                        statements.resize(0);
                        expr.PushStatements(statements);
                        break;
                    case ADJOINT:
                        std::cout << "Adjoint(reverse) accumulation method not yet implemented.";
                        exit(0);
                        break;
                    case TANGENT_LINEAR:
                        std::cout << "Tangent linear(forward) accumulation method not yet implemented.";
                        exit(0);
                        break;

                    case COMPLEX_STEP:

                        ResizeGradientVector(ids_set.max + 1);
                        ids = ids_set.Data();
                        size = ids_set.Size();

                        for (int i = 0; i < size; i++) {
                            gradient[ids[i]] = expr.ComplexStepValue(ids[i], H_V).imag() / H_V;
                        }
                        break;

                    default:
                        ResizeGradientVector(ids_set.max + 1);
                        ids = ids_set.Data();
                        size = ids_set.Size();

                        for (int i = 0; i < size; i++) {
                            gradient[ids[i]] = expr.Derivative(ids[i]);
                        }

                }
            }
            SetValue(expr.GetValue());

            return *this;
        }

        template<class T >
        DefaultVariable<REAL_T, group, ACCUMULATOR>& operator+=(const et4ad::ExpressionBase<REAL_T, T>& rhs) {


            if (DefaultVariable<REAL_T, group, ACCUMULATOR>::is_recording_g) {

                rhs.PushIds(ids_set);
                uint32_t* ids;
                size_t size;

                switch (ACCUMULATOR) {
                    case EXPRESSION_LEVEL:

                        ResizeGradientVector(ids_set.max + 1);
                        ids = ids_set.Data();
                        size = ids_set.Size();

                        for (int i = 0; i < size; i++) {
                            std::cout << gradient[ids[i]] << "+=" << rhs.Derivative(ids[i]) << "\n";

                            gradient[ids[i]] += rhs.Derivative(ids[i]);
                        }

                        break;

                    case EXPRESSION_STACK:
                        rhs.PushStatements(statements);
                        statements.push_back(Statement<REAL_T>(PLUS));
                        break;
                    case ADJOINT:
                        std::cout << "Adjoint(reverse) accumulation method not yet implemented.";
                        exit(0);
                        break;
                    case TANGENT_LINEAR:
                        std::cout << "Tangent linear(forward) accumulation method not yet implemented.";
                        exit(0);
                        break;

                    case COMPLEX_STEP:

                        ResizeGradientVector(ids_set.max + 1);
                        ids = ids_set.Data();
                        size = ids_set.Size();

                        for (int i = 0; i < size; i++) {
                            gradient[ids[i]] += rhs.ComplexStepValue(ids[i], H_V).imag() / H_V;
                        }
                        break;

                    default:
                        ResizeGradientVector(ids_set.max + 1);
                        ids = ids_set.Data();
                        size = ids_set.Size();

                        for (int i = 0; i < size; i++) {
                            gradient[ids[i]] += rhs.Derivative(ids[i]);
                        }

                }
            }


            SetValue(value_m + rhs.GetValue());

            return *this;
        }

        DefaultVariable<REAL_T, group, ACCUMULATOR>& operator+=(const DefaultVariable & rhs) {

            if (DefaultVariable<REAL_T, group, ACCUMULATOR>::is_recording_g) {

                rhs.PushIds(ids_set);
                uint32_t* ids;
                size_t size;

                switch (ACCUMULATOR) {
                    case EXPRESSION_LEVEL:

                        ResizeGradientVector(ids_set.max + 1);
                        ids = ids_set.Data();
                        size = ids_set.Size();

                        for (int i = 0; i < size; i++) {
                            std::cout << gradient[ids[i]] << "+=" << rhs.Derivative(ids[i]) << "\n";
                            gradient[ids[i]] += rhs.Derivative(ids[i]);
                            std::cout << gradient[ids[i]] << "\n";
                        }

                        break;

                    case EXPRESSION_STACK:
                        rhs.PushStatements(statements);
                        statements.push_back(Statement<REAL_T>(PLUS_EQUALS));
                        break;
                    case ADJOINT:
                        std::cout << "Adjoint(reverse) accumulation method not yet implemented.";
                        exit(0);
                        break;
                    case TANGENT_LINEAR:
                        std::cout << "Tangent linear(forward) accumulation method not yet implemented.";
                        exit(0);
                        break;

                    case COMPLEX_STEP:

                        ResizeGradientVector(ids_set.max + 1);
                        ids = ids_set.Data();
                        size = ids_set.Size();

                        for (int i = 0; i < size; i++) {
                            gradient[ids[i]] += rhs.ComplexStepValue(ids[i], H_V).imag() / H_V;
                        }
                        break;

                    default:
                        ResizeGradientVector(ids_set.max + 1);
                        ids = ids_set.Data();
                        size = ids_set.Size();

                        for (int i = 0; i < size; i++) {
                            gradient[ids[i]] += rhs.Derivative(ids[i]);
                        }

                }
            }
            SetValue(value_m + rhs.GetValue());
            return *this;
        }

        DefaultVariable<REAL_T, group, ACCUMULATOR>& operator+=(const REAL_T & rhs) {
            return *this = (*this +rhs);
        }

        template<class T >
        DefaultVariable<REAL_T, group, ACCUMULATOR>& operator-=(const et4ad::ExpressionBase<REAL_T, T>& rhs) {

            if (DefaultVariable<REAL_T, group, ACCUMULATOR>::is_recording_g) {

                rhs.PushIds(ids_set);
                uint32_t* ids;
                size_t size;

                switch (ACCUMULATOR) {
                    case EXPRESSION_LEVEL:

                        ResizeGradientVector(ids_set.max + 1);
                        ids = ids_set.Data();
                        size = ids_set.Size();

                        for (int i = 0; i < size; i++) {
                            gradient[ids[i]] -= rhs.Derivative(ids[i]);
                        }

                        break;

                    case EXPRESSION_STACK:
                        rhs.PushStatements(statements);
                        statements.push_back(Statement<REAL_T>(MINUS_EQUALS));
                        break;

                    case ADJOINT:
                        std::cout << "Adjoint(reverse) accumulation method not yet implemented.";
                        exit(0);
                        break;
                    case TANGENT_LINEAR:
                        std::cout << "Tangent linear(forward) accumulation method not yet implemented.";
                        exit(0);
                        break;

                    case COMPLEX_STEP:

                        ResizeGradientVector(ids_set.max + 1);
                        ids = ids_set.Data();
                        size = ids_set.Size();

                        for (int i = 0; i < size; i++) {
                            gradient[ids[i]] -= rhs.ComplexStepValue(ids[i], H_V).imag() / H_V;
                        }
                        break;

                    default:
                        ResizeGradientVector(ids_set.max + 1);
                        ids = ids_set.Data();
                        size = ids_set.Size();

                        for (int i = 0; i < size; i++) {
                            gradient[ids[i]] -= rhs.Derivative(ids[i]);
                        }

                }
            }
            SetValue(GetValue() - rhs.GetValue());
            return *this;
        }

        DefaultVariable<REAL_T, group, ACCUMULATOR>& operator-=(DefaultVariable<REAL_T, group, ACCUMULATOR> & rhs) {
            if (DefaultVariable<REAL_T, group, ACCUMULATOR>::is_recording_g) {

                rhs.PushIds(ids_set);
                uint32_t* ids;
                size_t size;

                switch (ACCUMULATOR) {
                    case EXPRESSION_LEVEL:

                        ResizeGradientVector(ids_set.max + 1);
                        ids = ids_set.Data();
                        size = ids_set.Size();

                        for (int i = 0; i < size; i++) {
                            gradient[ids[i]] -= rhs.Derivative(ids[i]);
                        }

                        break;

                    case EXPRESSION_STACK:
                        rhs.PushStatements(statements);
                        statements.push_back(Statement<REAL_T>(MINUS_EQUALS));
                        break;

                    case ADJOINT:
                        std::cout << "Adjoint(reverse) accumulation method not yet implemented.";
                        exit(0);
                        break;
                    case TANGENT_LINEAR:
                        std::cout << "Tangent linear(forward) accumulation method not yet implemented.";
                        exit(0);
                        break;

                    case COMPLEX_STEP:

                        ResizeGradientVector(ids_set.max + 1);
                        ids = ids_set.Data();
                        size = ids_set.Size();

                        for (int i = 0; i < size; i++) {
                            gradient[ids[i]] -= rhs.ComplexStepValue(ids[i], H_V).imag() / H_V;
                        }
                        break;

                    default:
                        ResizeGradientVector(ids_set.max + 1);
                        ids = ids_set.Data();
                        size = ids_set.Size();

                        for (int i = 0; i < size; i++) {
                            gradient[ids[i]] -= rhs.Derivative(ids[i]);
                        }

                }
            }
            SetValue(GetValue() - rhs.GetValue());
            return *this;
        }

        DefaultVariable& operator-=(const REAL_T & rhs) {
            return *this = (*this -rhs);
        }

        template<class T >
        DefaultVariable<REAL_T, group, ACCUMULATOR>& operator*=(const et4ad::ExpressionBase<REAL_T, T>& rhs) {

            if (DefaultVariable<REAL_T, group, ACCUMULATOR>::is_recording_g) {

                rhs.PushIds(ids_set);
                uint32_t* ids;
                size_t size;

                switch (ACCUMULATOR) {
                    case EXPRESSION_LEVEL:

                        ResizeGradientVector(ids_set.max + 1);
                        ids = ids_set.Data();
                        size = ids_set.Size();

                        for (int i = 0; i < size; i++) {
                            gradient[ids[i]] = (gradient[ids[i]] * rhs.GetValue() + GetValue() * rhs.ComplexStepValue(ids[i], H_V).imag() / H_V);
                        }

                        break;

                    case EXPRESSION_STACK:
                        rhs.PushStatements(statements);
                        statements.push_back(Statement<REAL_T>(TIMES_EQUALS));
                        break;

                    case ADJOINT:
                        std::cout << "Adjoint(reverse) accumulation method not yet implemented.";
                        exit(0);
                        break;
                    case TANGENT_LINEAR:
                        std::cout << "Tangent linear(forward) accumulation method not yet implemented.";
                        exit(0);
                        break;


                    case COMPLEX_STEP:

                        ResizeGradientVector(ids_set.max + 1);
                        ids = ids_set.Data();
                        size = ids_set.Size();

                        for (int i = 0; i < size; i++) {
                            gradient[ids[i]] = (gradient[ids[i]] * rhs.GetValue() + GetValue() * rhs.ComplexStepValue(ids[i], H_V).imag() / H_V);
                        }

                        break;

                    default:
                        ResizeGradientVector(ids_set.max + 1);
                        ids = ids_set.Data();
                        size = ids_set.Size();

                        for (int i = 0; i < size; i++) {
                            gradient[ids[i]] = (gradient[ids[i]] * rhs.GetValue() + GetValue() * rhs.ComplexStepValue(ids[i], H_V).imag() / H_V);
                        }
                }
            }

            SetValue(GetValue() * rhs.GetValue());
            return *this;
        }

        DefaultVariable<REAL_T, group, ACCUMULATOR>& operator*=(const DefaultVariable<REAL_T, group, ACCUMULATOR> & rhs) {
            if (DefaultVariable<REAL_T, group, ACCUMULATOR>::is_recording_g) {

                rhs.PushIds(ids_set);
                uint32_t* ids;
                size_t size;

                switch (ACCUMULATOR) {
                    case EXPRESSION_LEVEL:

                        ResizeGradientVector(ids_set.max + 1);
                        ids = ids_set.Data();
                        size = ids_set.Size();

                        for (int i = 0; i < size; i++) {
                            gradient[ids[i]] = (gradient[ids[i]] * rhs.GetValue() + GetValue() * rhs.ComplexStepValue(ids[i], H_V).imag() / H_V);
                        }

                        break;

                    case EXPRESSION_STACK:
                        rhs.PushStatements(statements);
                        statements.push_back(Statement<REAL_T>(TIMES_EQUALS));
                        break;

                    case ADJOINT:
                        std::cout << "Adjoint(reverse) accumulation method not yet implemented.";
                        exit(0);
                        break;
                    case TANGENT_LINEAR:
                        std::cout << "Tangent linear(forward) accumulation method not yet implemented.";
                        exit(0);
                        break;

                    case COMPLEX_STEP:

                        ResizeGradientVector(ids_set.max + 1);
                        ids = ids_set.Data();
                        size = ids_set.Size();

                        for (int i = 0; i < size; i++) {
                            gradient[ids[i]] = (gradient[ids[i]] * rhs.GetValue() + GetValue() * rhs.ComplexStepValue(ids[i], H_V).imag() / H_V);
                        }

                        break;

                    default:
                        ResizeGradientVector(ids_set.max + 1);
                        ids = ids_set.Data();
                        size = ids_set.Size();

                        for (int i = 0; i < size; i++) {
                            gradient[ids[i]] = (gradient[ids[i]] * rhs.GetValue() + GetValue() * rhs.ComplexStepValue(ids[i], H_V).imag() / H_V);
                        }
                }
            }

            SetValue(GetValue() * rhs.GetValue());
            return *this;
        }

        DefaultVariable<REAL_T, group, ACCUMULATOR>& operator*=(const REAL_T & rhs) {
            return *this = (*this * rhs);
        }

        template<class T >
        DefaultVariable<REAL_T, group, ACCUMULATOR>& operator/=(const et4ad::ExpressionBase<REAL_T, T>& rhs) {
            if (DefaultVariable<REAL_T, group, ACCUMULATOR>::is_recording_g) {

                rhs.PushIds(ids_set);
                uint32_t* ids;
                size_t size;

                switch (ACCUMULATOR) {
                    case EXPRESSION_LEVEL:

                        ResizeGradientVector(ids_set.max + 1);
                        ids = ids_set.Data();
                        size = ids_set.Size();

                        for (int i = 0; i < size; i++) {
                            gradient[ids[i]] = (gradient[ids[i]] * rhs.GetValue() - GetValue() * rhs.Derivative(gradient[ids[i]])) / (rhs.GetValue() * rhs.GetValue());
                        }

                        break;

                    case EXPRESSION_STACK:
                        rhs.PushStatements(statements);
                        statements.push_back(Statement<REAL_T>(DIVIDE_EQUALS));
                        break;

                    case ADJOINT:
                        std::cout << "Adjoint(reverse) accumulation method not yet implemented.";
                        exit(0);
                        break;
                    case TANGENT_LINEAR:
                        std::cout << "Tangent linear(forward) accumulation method not yet implemented.";
                        exit(0);
                        break;

                    case COMPLEX_STEP:

                        ResizeGradientVector(ids_set.max + 1);
                        ids = ids_set.Data();
                        size = ids_set.Size();

                        for (int i = 0; i < size; i++) {
                            gradient[ids[i]] = (gradient[ids[i]] * rhs.GetValue() - GetValue() * rhs.ComplexStepValue(ids[i], H_V).imag() / H_V) / (rhs.GetValue() * rhs.GetValue());
                        }

                        break;

                    default:
                        ResizeGradientVector(ids_set.max + 1);
                        ids = ids_set.Data();
                        size = ids_set.Size();

                        for (int i = 0; i < size; i++) {
                            gradient[ids[i]] = (gradient[ids[i]] * rhs.GetValue() - GetValue() * rhs.Derivative(gradient[ids[i]])) / (rhs.GetValue() * rhs.GetValue());
                        }
                }
            }

            SetValue(GetValue() / rhs.GetValue());
            return *this;
        }

        DefaultVariable<REAL_T, group, ACCUMULATOR>& operator/=(const DefaultVariable<REAL_T, group, ACCUMULATOR> & rhs) {
            if (DefaultVariable<REAL_T, group, ACCUMULATOR>::is_recording_g) {

                rhs.PushIds(ids_set);
                uint32_t* ids;
                size_t size;

                switch (ACCUMULATOR) {
                    case EXPRESSION_LEVEL:

                        ResizeGradientVector(ids_set.max + 1);
                        ids = ids_set.Data();
                        size = ids_set.Size();

                        for (int i = 0; i < size; i++) {
                            gradient[ids[i]] = (gradient[ids[i]] * rhs.GetValue() - GetValue() * rhs.Derivative(gradient[ids[i]])) / (rhs.GetValue() * rhs.GetValue());
                        }

                        break;

                    case EXPRESSION_STACK:
                        rhs.PushStatements(statements);
                        statements.push_back(Statement<REAL_T>(DIVIDE_EQUALS));
                        break;

                    case ADJOINT:
                        std::cout << "Adjoint(reverse) accumulation method not yet implemented.";
                        exit(0);
                        break;
                    case TANGENT_LINEAR:
                        std::cout << "Tangent linear(forward) accumulation method not yet implemented.";
                        exit(0);
                        break;

                    case COMPLEX_STEP:

                        ResizeGradientVector(ids_set.max + 1);
                        ids = ids_set.Data();
                        size = ids_set.Size();

                        for (int i = 0; i < size; i++) {
                            gradient[ids[i]] = (gradient[ids[i]] * rhs.GetValue() - GetValue() * rhs.ComplexStepValue(ids[i], H_V).imag() / H_V) / (rhs.GetValue() * rhs.GetValue());
                        }

                        break;

                    default:
                        ResizeGradientVector(ids_set.max + 1);
                        ids = ids_set.Data();
                        size = ids_set.Size();

                        for (int i = 0; i < size; i++) {
                            gradient[ids[i]] = (gradient[ids[i]] * rhs.GetValue() - GetValue() * rhs.Derivative(gradient[ids[i]])) / (rhs.GetValue() * rhs.GetValue());
                        }
                }
            }

            SetValue(GetValue() / rhs.GetValue());
        }

        DefaultVariable<REAL_T, group, ACCUMULATOR>& operator/=(const REAL_T & rhs) {
            return *this = (*this / rhs);
        }

        template<class T>
        const REAL_T WRT(const et4ad::ExpressionBase<REAL_T, T> &x) {
            switch (ACCUMULATOR) {
                case EXPRESSION_LEVEL:
                    //                    if (x.GetId() != 0 && x.GetId() == this->id_m) {
                    //                        return REAL_T(1.0);
                    //                    }
                    return x.GetId() < gsize ? gradient[x.GetId()] : REAL_T(0.0);

                case EXPRESSION_STACK:
                    if (x.GetId() != 0 && x.GetId() == this->id_m) {
                        return REAL_T(1.0);
                    }
                    return et4ad2::DiffValue<REAL_T>(statements, x);
                    break;

                case ADJOINT:
                    std::cout << "Adjoint(reverse) accumulation method not yet implemented.";
                    exit(0);
                    break;
                case TANGENT_LINEAR:
                    std::cout << "Tangent linear(forward) accumulation method not yet implemented.";
                    exit(0);
                    break;

                case COMPLEX_STEP:
                    if (x.GetId() != 0 && x.GetId() == this->id_m) {
                        return REAL_T(1.0);
                    }
                    return x.GetId() < gsize ? gradient[x.GetId()] : REAL_T(0.0);
                default:
                    if (x.GetId() != 0 && x.GetId() == this->id_m) {
                        return REAL_T(1.0);
                    }
                    return x.GetId() < gsize ? gradient[x.GetId()] : REAL_T(0.0);
            }

        }

        template<class T>
        const REAL_T WRT(const et4ad::ExpressionBase<REAL_T, T> &x) const {

            switch (ACCUMULATOR) {
                case EXPRESSION_LEVEL:
                    //                    if (x.GetId() != 0 && x.GetId() == this->id_m) {
                    //                        return REAL_T(1.0);
                    //                    }
                    return x.GetId() < gsize ? gradient[x.GetId()] : REAL_T(0.0);

                case EXPRESSION_STACK:
                    if (x.GetId() != 0 && x.GetId() == this->id_m) {
                        return REAL_T(1.0);
                    }
                    return et4ad2::DiffValue(statements, x);

                    break;

                case ADJOINT:
                    std::cout << "Adjoint(reverse) accumulation method not yet implemented.";
                    exit(0);
                    break;
                case TANGENT_LINEAR:
                    std::cout << "Tangent linear(forward) accumulation method not yet implemented.";
                    exit(0);
                    break;

                case COMPLEX_STEP:
                    if (x.GetId() != 0 && x.GetId() == this->id_m) {
                        return REAL_T(1.0);
                    }
                    return x.GetId() < gsize ? gradient[x.GetId()] : REAL_T(0.0);
                default:
                    if (x.GetId() != 0 && x.GetId() == this->id_m) {
                        return REAL_T(1.0);
                    }
                    return x.GetId() < gsize ? gradient[x.GetId()] : REAL_T(0.0);
            }

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
        void SetBounds(const REAL_T& min, const REAL_T & max) {
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
            et4ad::IDTrait<REAL_T> idt;
            if (!is_independent_m && is_independent && independent_id_m == 0) {
                independent_id_m = et4ad::IndependentVariableIdGenerator::instance()->next();
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
        void SetValue(const REAL_T & value) {

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
        inline const REAL_T Derivative(const uint32_t & id) const {
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
                statements.push_back(Statement<REAL_T > (VARIABLE, GetValue(), this->id_m));
            }
            other.insert(other.end(), statements.begin(), statements.end());

        }

        size_t Size()const {
            return this->gradient.size();
        }

    };


    template <class REAL_T, int group, int ACCUMULATOR>
    bool DefaultVariable<REAL_T, group, ACCUMULATOR>::is_recording_g = true;

    /**
     * Returns a variable that is the derivative w.r.t x.
     * Requires support for arbitrary. see SetSupportingArbitraryOrder(bool)
     * 
     * @param x
     * @return 
     */
    template<class REAL_T, class T>
    const REAL_T DiffValue(const std::vector<Statement<REAL_T> >& statements, const et4ad::ExpressionBase<REAL_T, T> &x) {

        if (statements.size() == 0 || x.GetId() == 0) {
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
                case PLUS_EQUALS:
                    rhs = stack.top();
                    stack.pop();
                    lhs = stack.top();
                    stack.pop();
                    stack.push(std::pair<REAL_T, REAL_T > (lhs.first + rhs.first, lhs.second + rhs.second));
                    break;
                case MINUS_EQUALS:
                    rhs = stack.top();
                    stack.pop();
                    lhs = stack.top();
                    stack.pop();
                    stack.push(std::pair<REAL_T, REAL_T > (lhs.first - rhs.first, lhs.second - rhs.second));

                    break;
                case TIMES_EQUALS:
                    std::cout<<"Statement handler TIMES_EQUALS not yet implemented!\n";
                    break;
                case DIVIDE_EQUALS:
                    std::cout<<"Statement handler DIVIDE_EQUALS not yet implemented!\n";
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

    template<>
    struct IDTrait<DefaultVariable<double> > {

        void SetId(DefaultVariable<double>& t, uint32_t id) {
            t.id_m = id;
            t.is_independent_m = true;
        }
    };

    template<>
    struct IDTrait<DefaultVariable<DefaultVariable<double> > > {

        void SetId(DefaultVariable<DefaultVariable<double> >& t, uint32_t id) {
            t.id_m = id;
            t.is_independent_m = true;
        }
    };

}




#endif	/* VARIABLE2_HPP */

