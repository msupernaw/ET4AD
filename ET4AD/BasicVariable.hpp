/* 
 * File:   BasicVariable.hpp
 * Author: matthewsupernaw
 *
 * Created on August 26, 2014, 9:00 AM
 */

#ifndef BASICVariable_HPP
#define	BASICVariable_HPP

#include <stack>
#include "IDGenerator.hpp"
#include "STD.hpp"
#include "AlignedAllocator.hpp"
#include <boost/unordered_map.hpp>
#include "GradientFile.hpp"


namespace et4ad {

    template<class REAL_T, size_t SIZE = 10 >
    struct StackStorage {
        REAL_T data[SIZE];
    public:

        StackStorage() {
            memset(data, 0, sizeof (data));
        }

        size_t Size() const {
            return SIZE;
        }

        inline REAL_T & Get(const uint32_t & index) {
            return data[index];
        }

        inline const REAL_T & Get(const uint32_t & index) const {
            return data[index];
        }

        void Reset() {
            memset(data, 0, sizeof (data));
        }

        void Prepare(const uint32_t & id) {

        }


    };

    template<class REAL_T >
    struct DynamicStorage {
        std::vector<REAL_T, util::aligned_allocator<REAL_T, 16 > > data;
        REAL_T zero;
        uint32_t gsize;
    public:

        inline explicit DynamicStorage() {
            zero = 0;
            gsize = 0;
        }

        inline size_t Size() const {
            return gsize;
        }

        inline REAL_T & Get(const uint32_t & index) {

            return data[index];
        }

        inline const REAL_T & Get(const uint32_t & index) const {

            return data[index];
        }

        inline void Reset() {
            data.resize(0);
            gsize = 0;
        }

        /**
         * If the max id needs to be known, 
         * handle it here.
         * @param id
         */
        inline void Prepare(const uint32_t & id) {
            if (gsize < id) {
                data.resize(id + 1);
                gsize = data.size();
            }
        }


    };

    template<class REAL_T>
    struct MapStorage {
        typedef boost::unordered_map<uint32_t, REAL_T> Map;
        mutable Map data;
        typedef typename Map::iterator g_it;
    public:

        MapStorage() {

        }

        size_t Size() const {
            return std::numeric_limits<int>::max();
        }

        inline REAL_T & Get(uint32_t index) {
            return data[index];
        }

        inline const REAL_T & Get(uint32_t index) const {
            return data[index];
        }

        void Reset() {
            data.clear();
        }

        void Prepare(const uint32_t & id) {

        }

    };
    //


#define MAX_INPEPEDENDEDNTS 10000

    template<class REAL_T, int group = 0, size_t SIZE = MAX_INPEPEDENDEDNTS >
    class DiskStorage {
        et4ad::GradientFile<REAL_T, group, SIZE>* fdata;
        uint32_t id;
        mutable uint32_t last_index;
        mutable REAL_T last_g;
        mutable REAL_T temp;
        mutable bool needs_write;
    public:

        DiskStorage() {

            fdata = GradientFileInstance<REAL_T, group, SIZE > ();
            id = fdata->Next();
            last_index = 0;
            last_g = 0;
            needs_write = true;
        }

        size_t Size() const {
            return SIZE;
        }

        inline REAL_T & Get(const uint32_t& index) {
            fdata->Set(id, last_index, last_g);
            last_index = index;
            last_g = 0;
            last_g = fdata->Get(id, index);
            needs_write = true;
            return last_g;
        }

        inline const REAL_T & Get(const uint32_t& index) const {

            fdata->Set(id, last_index, last_g);
            temp = fdata->Get(id, index);
            return temp;
        }

        void Reset() {
            fdata->Reset(id);
        }

        void Prepare(const uint32_t& id) {

        }


    };



    template<class REAL_T, //base type
    int group = 0, //group identifier
    class Storage = DynamicStorage<REAL_T> > //storage policy
    class BasicVariable;

    template<class REAL_T, //base type
    int group, class Storage > //group identifier
    class BasicVariable : public ExpressionBase<REAL_T, BasicVariable<REAL_T, group, Storage > > {
        Storage gradient;
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






    public:

        static bool IsRecording() {
            return BasicVariable<REAL_T, group, Storage>::is_recording_g;
        }

        static void SetRecording(bool is_recording_g) {
            BasicVariable<REAL_T, group, Storage>::is_recording_g = is_recording_g;
        }

        static bool IsSupportingArbitraryOrder() {
            return BasicVariable<REAL_T, group, Storage>::is_supporting_arbitrary_order;
        }

        static void SetSupportingArbitraryOrder(bool is_supporting_arbitrary_order) {
            BasicVariable<REAL_T, group, Storage>::is_supporting_arbitrary_order = is_supporting_arbitrary_order;
        }

        BasicVariable() : ExpressionBase<REAL_T, BasicVariable<REAL_T, group, Storage> >(),
        gsize(0),
        value_m(0.0),
        min_boundary_m(std::numeric_limits<REAL_T>::min()),
        max_boundary_m(std::numeric_limits<REAL_T>::max()),
        is_independent_m(false),
        independent_id_m(0),
        bounded_m(false) {
        }

        BasicVariable(const REAL_T &value) : ExpressionBase<REAL_T, BasicVariable<REAL_T, group, Storage> >(),
        gsize(0),
        value_m(value),
        min_boundary_m(std::numeric_limits<REAL_T>::min()),
        max_boundary_m(std::numeric_limits<REAL_T>::max()),
        is_independent_m(false),
        independent_id_m(0),
        bounded_m(false) {
        }

        BasicVariable(const BasicVariable<REAL_T, group, Storage> & orig) {

            //to do move to initializer list.
            gsize = 0;
            bounded_m = orig.bounded_m;
            is_independent_m = orig.is_independent_m;
            independent_id_m = orig.independent_id_m;
            max_boundary_m = orig.max_boundary_m;
            min_boundary_m = orig.min_boundary_m;
            name_m = orig.name_m;
            this->id_m = orig.id_m;

            if (BasicVariable<REAL_T, group, Storage>::is_recording_g) {


                orig.PushIds(ids_set);
                gradient.Prepare(ids_set.max + 1);

                uint32_t* ids = ids_set.Data();
                size_t size = ids_set.Size();
                for (int i = 0; i < size; i++) {
                    orig.Derivative(ids[i], gradient.Get(ids[i]));
                }


                if (BasicVariable<REAL_T, group, Storage>::is_supporting_arbitrary_order) {
                    orig.PushStatements(statements);
                }
            }
            SetValue(orig.GetValue());
        }

        template<class T>
        BasicVariable(const ExpressionBase<REAL_T, T>& expr) : ExpressionBase<REAL_T, BasicVariable<REAL_T, group, Storage> >(0),
        gsize(0),
        value_m(0.0),
        min_boundary_m(std::numeric_limits<REAL_T>::min()),
        max_boundary_m(std::numeric_limits<REAL_T>::max()),
        is_independent_m(false),
        independent_id_m(0),
        bounded_m(false) {



            if (BasicVariable<REAL_T, group, Storage>::is_recording_g) {

                expr.PushIds(ids_set);
                gradient.Prepare(ids_set.max + 1);

                uint32_t* ids = ids_set.Data();
                size_t size = ids_set.Size();
                for (int i = 0; i < size; i++) {
                    expr.Derivative(ids[i], gradient.Get(ids[i]));
                }


                if (BasicVariable<REAL_T, group, Storage>::is_supporting_arbitrary_order) {
                    expr.template PushStatements<>(statements);
                }

            }

            SetValue(expr.GetValue());
        }

        virtual ~BasicVariable() {

        }

        operator REAL_T() {
            return this->GetValue();
        }

        operator REAL_T()const {
            return this->GetValue();
        }


        //operators

        BasicVariable& operator=(const REAL_T& other) {
            SetValue(other);
            gradient.Reset();

            if (BasicVariable<REAL_T, group, Storage>::is_recording_g) {

                if (BasicVariable<REAL_T, group, Storage>::is_supporting_arbitrary_order) {
                    statements.clear();
                }

            }
            return *this;
        }

        //        BasicVariable& operator=(BasicVariable && other) {
        //            std::swap(*this, other);
        //            return *this;
        //        }

        /**
         * Sets the value of this BasicVariable to that of other.
         * 
         * @param rhs
         * @return 
         */
        BasicVariable& operator=(const BasicVariable& other) {


            if (BasicVariable<REAL_T, group, Storage>::is_recording_g) {


                other.PushIds(ids_set);
                gradient.Prepare(ids_set.max + 1);

                uint32_t* ids = ids_set.Data();
                size_t size = ids_set.Size();
                for (int i = 0; i < size; i++) {
                    other.Derivative(ids[i], gradient.Get(ids[i]));
                }


                if (BasicVariable<REAL_T, group, Storage>::is_supporting_arbitrary_order) {
                    std::vector<Statement<REAL_T> > s;
                    other.template PushStatements<>(s);
                    statements.clear();
                    statements.insert(statements.begin(), s.begin(), s.end());
                }

            }


            SetValue(other.GetValue());
            return *this;
        }

        /**
         * Set the BasicVariables value to the result of the expression rhs. 
         * Derivatives are calculated and stored in the encapsulated 
         * gradient map.
         * @param rhs
         * @return 
         */
        template<class T>
        BasicVariable& operator=(const ExpressionBase<REAL_T, T>& expr) {

            if (BasicVariable<REAL_T, group, Storage>::is_recording_g) {


                expr.PushIds(ids_set);
                gradient.Prepare(ids_set.max + 1);

                uint32_t* ids = ids_set.Data();
                size_t size = ids_set.Size();
                for (int i = 0; i < size; i++) {
                    expr.Derivative(ids[i], gradient.Get(ids[i]));
                }


                if (BasicVariable<REAL_T, group, Storage>::is_supporting_arbitrary_order) {
                    std::vector<Statement<REAL_T> > s;
                    expr.PushStatements(s);
                    statements.clear();
                    statements.insert(statements.begin(), s.begin(), s.end());
                }
            }

            SetValue(expr.GetValue());

            return *this;
        }

        template<class T>
        void ReadStatements(const ExpressionBase<REAL_T, T>& rhs) {
            std::vector<Statement<REAL_T> > s;
            PushStatements(s);
            rhs.template PushStatements<REAL_T>(s);
            s.push_back(Statement<REAL_T > (PLUS));
            statements.clear();
            statements.insert(statements.begin(), s.begin(), s.end());
        }

        template<class T>
        BasicVariable& operator+=(const ExpressionBase<REAL_T, T>& rhs) {


            if (BasicVariable<REAL_T, group, Storage>::is_recording_g) {



                rhs.PushIds(ids_set);
                gradient.Prepare(ids_set.max + 1);

                uint32_t* ids = ids_set.Data();
                size_t size = ids_set.Size();
                register int i;
                register int ind;
                REAL_T dx = 0;
                for (i = 0; i < size; i++) {
                    ind = ids[i];
                    rhs.Derivative(ind, dx);
                    gradient.Get(ind) += dx;
                }


                if (BasicVariable<REAL_T, group, Storage>::is_supporting_arbitrary_order) {
                    ReadStatements(rhs);
                    //                    std::vector<Statement<REAL_T> > s;
                    //                    PushStatements(s);
                    //                    rhs.PushStatements(s);
                    //                    s.push_back(Statement<REAL_T > (PLUS));
                    //                    statements.clear();
                    //                    statements.insert(statements.begin(), s.begin(), s.end());
                }

            }

            SetValue(value_m + rhs.GetValue());

            return *this;
        }

        BasicVariable& operator+=(const BasicVariable& rhs) {

            if (BasicVariable<REAL_T, group, Storage>::is_recording_g) {

                rhs.PushIds(ids_set);
                gradient.Prepare(ids_set.max + 1);

                uint32_t* ids = ids_set.Data();
                size_t size = ids_set.Size();


                register int i;
                register int ind;
                REAL_T dx = 0;
                for (i = 0; i < size; i++) {
                    ind = ids[i];
                    rhs.Derivative(ind, dx);
                    gradient.Get(ind) += dx;
                }
                if (BasicVariable<REAL_T, group, Storage>::is_supporting_arbitrary_order) {
                    std::vector<Statement<REAL_T> > s;
                    PushStatements(s);
                    rhs.template PushStatements<>(s);
                    s.push_back(Statement<REAL_T > (PLUS));
                    statements.clear();
                    statements.insert(statements.begin(), s.begin(), s.end());
                }

            }
            SetValue(value_m + rhs.GetValue());
            return *this;
        }

        BasicVariable& operator+=(const REAL_T& rhs) {
            return *this = (*this +rhs);
        }

        template<class T>
        BasicVariable& operator-=(const ExpressionBase<REAL_T, T>& rhs) {

            if (BasicVariable<REAL_T, group, Storage>::is_recording_g) {
                rhs.PushIds(ids_set);
                gradient.Prepare(ids_set.max + 1);

                uint32_t* ids = ids_set.Data();
                size_t size = ids_set.Size();

                register int i;
                register int ind;
                for (i = 0; i < size; i++) {
                    ind = ids[i];
                    gradient.Get(ind) -= rhs.Derivative(ind);
                }

                if (BasicVariable<REAL_T, group, Storage>::is_supporting_arbitrary_order) {
                    std::vector<Statement<REAL_T> > s;
                    PushStatements(s);
                    rhs.template PushStatements<>(s);
                    s.push_back(Statement<REAL_T > (MINUS));
                    statements.clear();
                    statements.insert(statements.begin(), s.begin(), s.end());
                }

            }
            SetValue(GetValue() - rhs.GetValue());
            return *this;
        }

        BasicVariable& operator-=(BasicVariable& rhs) {
            if (BasicVariable<REAL_T, group, Storage>::is_recording_g) {

                rhs.PushIds(ids_set);
                gradient.Prepare(ids_set.max + 1);

                uint32_t* ids = ids_set.Data();
                size_t size = ids_set.Size();


                register int i;
                register int ind;
                REAL_T dx;
                for (i = 0; i < size; i++) {
                    ind = ids[i];
                    rhs.Derivative(ind, dx);
                    gradient.Get(ind) -= dx;
                }

                if (BasicVariable<REAL_T, group, Storage>::is_supporting_arbitrary_order) {
                    std::vector<Statement<REAL_T> > s;
                    PushStatements(s);
                    rhs.template PushStatements<>(s);
                    s.push_back(Statement<REAL_T > (MINUS));
                    statements.clear();
                    statements.insert(statements.begin(), s.begin(), s.end());
                }

            }
            SetValue(GetValue() - rhs.GetValue());
        }

        BasicVariable& operator-=(const REAL_T& rhs) {
            return *this = (*this -rhs);
        }

        template<class T>
        BasicVariable& operator*=(const ExpressionBase<REAL_T, T>& rhs) {

            if (BasicVariable<REAL_T, group, Storage>::is_recording_g) {

                rhs.PushIds(ids_set);
                gradient.Prepare(ids_set.max + 1);

                uint32_t* ids = ids_set.Data();
                size_t size = ids_set.Size();


                register int i;
                REAL_T rhs_value = rhs.GetValue();
                REAL_T dx = 0;
                for (i = 0; i < size; i++) {
                    REAL_T& dx2 = gradient.Get(ids[i]);
                    rhs.Derivative(ids[i], dx);
                    dx2 = (dx2 * rhs_value + GetValue() * dx);
                }

                if (BasicVariable<REAL_T, group, Storage>::is_supporting_arbitrary_order) {
                    std::vector<Statement<REAL_T> > s;
                    PushStatements(s);
                    rhs.template PushStatements<>(s);
                    s.push_back(Statement<REAL_T > (MULTIPLY));
                    statements = s;
                }

            }
            SetValue(GetValue() * rhs.GetValue());
            return *this;
        }

        BasicVariable& operator*=(const BasicVariable& rhs) {
            if (BasicVariable<REAL_T, group, Storage>::is_recording_g) {
                REAL_T rhs_value = rhs.GetValue();

                rhs.PushIds(ids_set);
                gradient.Prepare(ids_set.max + 1);

                uint32_t* ids = ids_set.Data();
                size_t size = ids_set.Size();


                register int i;
                register int ind;
                REAL_T dx = 0;

                for (i = 0; i < size; i++) {
                    ind = ids[i];
                    rhs.Derivative(ind, dx);
                    REAL_T& dx2 = gradient.Get(ind);
                    dx2 = (dx2 * rhs_value + GetValue() * dx);
                }

                if (BasicVariable<REAL_T, group, Storage>::is_supporting_arbitrary_order) {
                    std::vector<Statement<REAL_T> > s;
                    PushStatements(s);
                    rhs.template PushStatements<>(s);
                    s.push_back(Statement<REAL_T > (MULTIPLY));
                    statements = s;
                }

            }

            SetValue(GetValue() * rhs.GetValue());
            return *this;
        }

        BasicVariable& operator*=(const REAL_T& rhs) {
            return *this = (*this * rhs);
        }

        template<class T>
        BasicVariable& operator/=(const ExpressionBase<REAL_T, T>& rhs) {
            if (BasicVariable<REAL_T, group, Storage>::is_recording_g) {


                rhs.PushIds(ids_set);
                gradient.Prepare(ids_set.max + 1);

                uint32_t* ids = ids_set.Data();
                size_t size = ids_set.Size();


                register int i;
                register int ind;
                REAL_T mult = 1.0 / (rhs.GetValue() * rhs.GetValue());
                REAL_T rhs_value = rhs.GetValue();
                for (i = 0; i < size; i++) {
                    ind = ids[i];
                    REAL_T& dx = gradient.Get(ind);
                    dx = (dx * rhs_value - GetValue() * rhs.Derivative(ind)) * mult;
                }

                if (BasicVariable<REAL_T, group, Storage>::is_supporting_arbitrary_order) {
                    std::vector<Statement<REAL_T> > s;
                    PushStatements(s);
                    rhs.template PushStatements<REAL_T>(s);
                    s.push_back(Statement<REAL_T > (DIVIDE));
                    statements = s;
                }

            }
            SetValue(GetValue() / rhs.GetValue());
            return *this;
        }

        BasicVariable& operator/=(const BasicVariable& rhs) {
            if (BasicVariable<REAL_T, group, Storage>::is_recording_g) {

                rhs.PushIds(ids_set);
                gradient.Prepare(ids_set.max + 1);

                uint32_t* ids = ids_set.Data();
                size_t size = ids_set.Size();

                register int i;
                REAL_T mult = 1.0 / (rhs.GetValue() * rhs.GetValue());
                REAL_T rhs_value = rhs.GetValue();
                REAL_T dx = 0;
                for (i = 0; i < size; i++) {
                    rhs.Derivative(ids[i], dx);
                    REAL_T& dx2 = gradient.Get(ids[i]);
                    dx2 = (dx2 * rhs_value - GetValue() * dx) * mult;
                }


                if (BasicVariable<REAL_T, group, Storage>::is_supporting_arbitrary_order) {
                    std::vector<Statement<REAL_T> > s;
                    PushStatements(s);
                    rhs.template PushStatements<>(s);
                    s.push_back(Statement<REAL_T > (DIVIDE));
                    statements = s;
                }

            }
            SetValue(GetValue() / rhs.GetValue());
        }

        BasicVariable& operator/=(const REAL_T& rhs) {
            return *this = (*this / rhs);
        }

        const REAL_T WRT(const BasicVariable<REAL_T, group, Storage> &x) {
            return x.GetId() < gradient.Size() ? gradient.Get(x.GetId()) : 0.0;
        }

        const REAL_T WRT(const BasicVariable<REAL_T, group, Storage> &x) const {
            return x.GetId() < gradient.Size() ? gradient.Get(x.GetId()) : 0.0;
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
            for (size_t i = 0; i < this->gradient.Size(); i++) {
                dif = this->gradient.Get(i);
                temp += dif * dif * squared_epsilon;
            }

            return std::sqrt(temp);

        }

        /**
         * Returns a BasicVariable that is the derivative w.r.t x.
         * Requires support for arbitrary. see SetSupportingArbitraryOrder(bool)
         * 
         * @param x
         * @return 
         */
        const REAL_T DiffV(const BasicVariable<REAL_T, group, Storage> &x) {
            if (statements.size() == 0) {
                return 0.0;
            }
            std::vector<std::pair<REAL_T, REAL_T > > v;
            v.reserve(statements.size());
            std::stack<std::pair<REAL_T, REAL_T >,
                    std::vector<std::pair<REAL_T, REAL_T > > > stack;

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

            return stack.top().second;
        }

        /**
         * Returns a BasicVariable that is the derivative w.r.t x.
         * Requires support for arbitrary. see SetSupportingArbitraryOrder(bool)
         * 
         * @param x
         * @return 
         */
        const BasicVariable<REAL_T, group, Storage> Diff(const BasicVariable<REAL_T, group, Storage> &x) {
            if (statements.size() == 0) {
                return 0.0;
            }



            typedef std::pair<BasicVariable<REAL_T, group, Storage>, BasicVariable<REAL_T, group, Storage> > VPair;


            std::vector<VPair > v;
            v.reserve(statements.size()*200);
            std::stack<VPair,
                    std::vector<VPair > > stack(v);
            bool found = false;
            int size = statements.size();
            VPair lhs = VPair(BasicVariable<REAL_T, group, Storage > (0), BasicVariable<REAL_T, group, Storage > (0));
            VPair rhs = VPair(BasicVariable<REAL_T, group, Storage > (0), BasicVariable<REAL_T, group, Storage > (0));


            BasicVariable<REAL_T, group, Storage> temp;
            for (int i = 0; i < size; i++) {


                temp = 0.0;

                switch (statements[i].op_m) {

                    case CONSTANT:
                        stack.push(VPair(statements[i].value_m, 0.0));
                        break;
                    case VARIABLE:
                        if (statements[i].id_m == x.GetId()) {
                            found = true;
                            //f(x) = x
                            //f'(x) = 1
                            stack.push(VPair(statements[i].value_m, 1.0));
                            stack.top().first.value_m = statements[i].value_m;
                            stack.top().first.id_m = statements[i].id_m;
                            stack.top().first.independent_id_m = statements[i].id_m;
                            stack.top().first.is_independent_m = true;
                        } else {//constant
                            //f(x) = C
                            //f'(x) = 0
                            stack.push(VPair(statements[i].value_m, 0.0));
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
                        stack.push(VPair(lhs.first + rhs.first, lhs.second + rhs.second));

                        break;
                    case MINUS:
                        rhs = stack.top();
                        stack.pop();
                        lhs = stack.top();
                        stack.pop();
                        stack.push(VPair(lhs.first - rhs.first, lhs.second - rhs.second));


                        break;
                    case MULTIPLY:
                        rhs = stack.top();
                        stack.pop();
                        lhs = stack.top();
                        stack.pop();
                        temp = lhs.second * rhs.first + lhs.first * rhs.second;

                        stack.push(VPair(lhs.first * rhs.first, temp));

                        break;
                    case DIVIDE:
                        rhs = stack.top();
                        stack.pop();
                        lhs = stack.top();
                        stack.pop();

                        temp = (lhs.second * rhs.first - lhs.first * rhs.second) / (rhs.first * rhs.first);
                        stack.push(VPair(lhs.first / rhs.first, temp));
                        break;

                    case SIN:
                        lhs = stack.top();
                        stack.pop();
                        if (found) {
                            stack.push(VPair(std::sin(lhs.first), lhs.second * std::cos(lhs.first)));
                        } else {
                            stack.push(VPair(std::sin(lhs.first), 0));
                        }

                        break;
                    case COS:

                        lhs = stack.top();
                        stack.pop();
                        if (found) {
                            stack.push(VPair(std::cos(lhs.first), lhs.second * (static_cast<REAL_T> (-1.0)) * std::sin(lhs.first)));
                        } else {
                            stack.push(VPair(std::cos(lhs.first), 0));
                        }

                        break;
                    case TAN:
                        lhs = stack.top();
                        stack.pop();
                        if (found) {
                            temp = lhs.second * ((static_cast<REAL_T> (1.0) / std::cos(lhs.first))*(static_cast<REAL_T> (1.0) / std::cos(lhs.first)));
                            stack.push(VPair(std::tan(lhs.first), temp));
                        } else {
                            stack.push(VPair(std::tan(lhs.first), 0));
                        }

                        break;
                    case ASIN:
                        lhs = stack.top();
                        stack.pop();
                        if (found) {
                            temp = (lhs.second * static_cast<REAL_T> (1.0) / std::pow((static_cast<REAL_T> (1.0) - std::pow(lhs.first, static_cast<REAL_T> (2.0))), static_cast<REAL_T> (0.5)));
                            stack.push(VPair(std::asin(lhs.first), temp));
                        } else {
                            stack.push(VPair(std::asin(lhs.first), 0));
                        }

                        break;
                    case ACOS:
                        lhs = stack.top();
                        stack.pop();
                        if (found) {
                            temp = (lhs.second * (static_cast<REAL_T> (-1.0)) / std::pow((static_cast<REAL_T> (1.0) - std::pow(lhs.first, static_cast<REAL_T> (2.0))), static_cast<REAL_T> (0.5)));
                            stack.push(VPair(std::acos(lhs.first), temp));
                        } else {
                            stack.push(VPair(std::acos(lhs.first), (0)));
                        }

                        break;
                    case ATAN:
                        lhs = stack.top();
                        stack.pop();
                        if (found) {
                            temp = (lhs.second * static_cast<REAL_T> ((1.0)) / (lhs.first * lhs.first + static_cast<REAL_T> ((1.0))));
                            stack.push(VPair(std::atan(lhs.first), temp));
                        } else {
                            stack.push(VPair(std::atan(lhs.first), (0)));
                        }

                        break;
                    case ATAN2:
                        rhs = stack.top();
                        stack.pop();
                        lhs = stack.top();
                        stack.pop();
                        if (found) {
                            temp = (rhs.first * lhs.second / (lhs.first * lhs.first + (rhs.first * rhs.first)));
                            stack.push(VPair(std::atan2(lhs.first, rhs.first), temp));
                        } else {
                            stack.push(VPair(std::atan2(lhs.first, rhs.first), (0)));
                        }

                        break;
                    case SQRT:
                        lhs = stack.top();
                        stack.pop();
                        if (found) {
                            temp = lhs.second * static_cast<REAL_T> ((.5)) / std::sqrt(lhs.first);
                            stack.push(VPair(std::sqrt(lhs.first), temp));
                        } else {
                            stack.push(VPair(std::sqrt(lhs.first), (0)));
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
                            stack.push(VPair(std::pow(lhs.first, rhs.first), temp));
                        } else {
                            stack.push(VPair(std::pow(lhs.first, rhs.first), (0)));
                        }

                        break;
                    case LOG:
                        lhs = stack.top();
                        stack.pop();
                        if (found) {
                            temp = (lhs.second * (Constant<REAL_T > (1.0))) / lhs.first;
                            stack.push(VPair(std::log(lhs.first), temp));
                        } else {
                            stack.push(VPair(std::log(lhs.first), (0)));
                        }

                        break;
                    case LOG10:
                        lhs = stack.top();
                        stack.pop();
                        if (found) {
                            temp = (lhs.second * static_cast<REAL_T> ((1.0))) / (lhs.first * std::log(static_cast<REAL_T> ((10.0))));
                            stack.push(VPair(std::log10(lhs.first), temp));
                        } else {
                            stack.push(VPair(std::log10(lhs.first), (0)));
                        }

                        break;
                    case EXP:
                        lhs = stack.top();
                        stack.pop();
                        if (found) {
                            temp = lhs.second * std::exp(lhs.first);
                            stack.push(VPair(std::exp(lhs.first), temp));
                        } else {
                            stack.push(VPair(std::exp(lhs.first), (0)));
                        }

                        break;
                    case MFEXP:
                        lhs = stack.top();
                        stack.pop();
                        if (found) {
                            temp = lhs.second * std::mfexp(lhs.first);
                            stack.push(VPair(std::mfexp(lhs.first), temp));
                        } else {
                            stack.push(VPair(std::mfexp(lhs.first), (0)));
                        }

                        break;
                    case SINH:
                        lhs = stack.top();
                        stack.pop();
                        if (found) {
                            temp = lhs.second * std::cosh(lhs.first);
                            stack.push(VPair(std::sinh(lhs.first), temp));
                        } else {
                            stack.push(VPair(std::sinh(lhs.first), (0)));
                        }

                        break;
                    case COSH:
                        lhs = stack.top();
                        stack.pop();
                        if (found) {
                            temp = lhs.second * std::sinh(lhs.first);
                            stack.push(VPair(std::cosh(lhs.first), temp));
                        } else {
                            stack.push(VPair(std::cosh(lhs.first), (0)));
                        }

                        break;
                    case TANH:
                        lhs = stack.top();
                        stack.pop();
                        if (found) {
                            temp = lhs.second * (static_cast<REAL_T> ((1.0)) / std::cosh(lhs.first))*(static_cast<REAL_T> ((1.0)) / std::cosh(lhs.first));
                            stack.push(VPair(std::tanh(lhs.first), temp));
                        } else {
                            stack.push(VPair(std::tanh(lhs.first), (0)));
                        }

                        break;
                    case FABS:
                        lhs = stack.top();
                        stack.pop();
                        if (found) {
                            temp = (lhs.second * lhs.first) /
                                    std::fabs(lhs.first);
                            stack.push(VPair(std::fabs(lhs.first), temp));
                        } else {
                            stack.push(VPair(std::fabs(lhs.first), (0)));
                        }

                        break;
                    case ABS:
                        lhs = stack.top();
                        stack.pop();
                        if (found) {
                            temp = (lhs.second * lhs.first) /
                                    std::fabs(lhs.first);
                            stack.push(VPair(std::fabs(lhs.first), temp));
                        } else {
                            stack.push(VPair(std::fabs(lhs.first), (0)));
                        }

                        break;
                    case FLOOR:
                        lhs = stack.top();
                        stack.pop();
                        if (found) {

                            temp = (0);
                            stack.push(VPair(std::floor(lhs.first), temp));
                        } else {
                            stack.push(VPair(std::floor(lhs.first), (0)));
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

        /**
         * Returns the name of this BasicVariable. Names are not initialized and 
         * if it is not set this function will return a empty string.
         * @return 
         */
        std::string GetName() const {
            return name_m;
        }

        /**
         * Set the name of this BasicVariable.
         * 
         * @param name
         */
        void SetName(std::string name) {
            name_m = name;
        }

        /**
         * Returns true if this BasicVariable is bounded. Otherwise,
         * false.
         * 
         * @return 
         */
        bool IsBounded() {
            return bounded_m;
        }

        /**
         * Set the min and max boundary for this BasicVariable. 
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
         * Return the minimum boundary for this BasicVariable. The default value 
         * is the results of std::numeric_limits<REAL_t>::min().
         * 
         * @return 
         */
        const REAL_T GetMinBoundary() {
            return min_boundary_m;
        }

        /**
         * Return the maximum boundary for this BasicVariable. The default value 
         * is the results of std::numeric_limits<REAL_t>::max().
         * 
         * @return 
         */
        const REAL_T GetMaxBoundary() {
            return max_boundary_m;
        }

        /**
         * Make this BasicVariable an independent BasicVariable. If set to true,
         * the BasicVariables unique identifier is registered in the static set
         * of independent BasicVariables. During function evaluations, gradient 
         * information is accumulated in post-order wrt to BasicVariables in the 
         * static set of independent BasicVariables. If set false, this BasicVariables 
         * unique identifier will be removed from the set, if it existed.
         * 
         * 
         * To access the derivative wrt to a BasicVariable, call function:
         * 
         * const REAL_t wrt(const BasicVariable & ind)
         * 
         * 
         * @param is_independent
         */
        void SetAsIndependent(const bool &is_independent) {

            if (!is_independent_m && is_independent && independent_id_m == 0) {
                independent_id_m = IndependentVariableIdGenerator::instance()->next();
                this->id_m = independent_id_m;
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
         * Sets the value of this BasicVariable. If the BasicVariable is bounded, 
         * the value will be set between the min and max boundary. If the
         * value is less than the minimum boundary, the BasicVariables value is 
         * set to minimum boundary. If the  value is greater than the maximum
         * boundary, the BasicVariables value is set to maximum boundary. If the 
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

        inline size_t Size() const {
            return is_independent_m ? this->id_m : gsize;
        }

        inline const REAL_T Derivative(const uint32_t id, bool &found) const {

            if (this->ids_set.Has(id)) {
                found = true;
            } else {
                found = false;
            }

            return this->ids_set.Has(id) ?
                    gradient.Get(unsigned(id)) :
                    static_cast<REAL_T> ((unsigned(id) == unsigned(this->id_m)));

            return 0.0;
        }

        inline void Derivative(const uint32_t& id, REAL_T& dx) const {
            dx = (unsigned(gradient.Size()) > unsigned(id)) ?
                    gradient.Get(unsigned(id)) :
                    static_cast<REAL_T> ((unsigned(id) == unsigned(this->id_m)));
        }

        /**
         * Return the derivative wrt to parameter id. If the gradient vector is
         * larger than id, return the value at index id, else check
         * if this id is equal to the provided id. If it is equal return 1, else
         * return 0.
         * @param id
         * @return 
         */
        inline const REAL_T Derivative(const uint32_t& id) const {
   
            return this->ids_set.Has(id) ?
                    gradient.Get(unsigned(id)) :
                    static_cast<REAL_T> ((unsigned(id) == unsigned(this->id_m)));

        }

        inline void Derivative(std::vector<REAL_T>& gradient) const {
            gradient.clear();
            gradient.insert(gradient.begin(), this->gradient.begin(), this->gradient.end());

        }

        std::vector<REAL_T>& Gradient() {
            return gradient;
        }

        inline void PushIds(et4ad::IDSet & ids) const {
            if (!unsigned(this->id_m)) {

                const uint32_t* ids_m = ids_set.Data();
                const size_t size = ids_set.Size();
                int i;
                for (i = 0; i < size; i++) {
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


    };





    template <class REAL_T, int group, class Storage >
    bool BasicVariable<REAL_T, group, Storage>::is_recording_g = true;

    template <class REAL_T, int group, class Storage>
    bool BasicVariable<REAL_T, group, Storage>::is_supporting_arbitrary_order = false;
    //
    //    template<class REAL_T, class T, class TT>
    //    inline const int operator==(const et4ad::ExpressionBase<REAL_T, T>& lhs, const et4ad::ExpressionBase<REAL_T, TT>& rhs) {
    //        return lhs.GetValue() == rhs.GetValue();
    //    }
    //
    //    template<class REAL_T, class T, class TT>
    //    inline const int operator!=(const et4ad::ExpressionBase<REAL_T, T>& lhs, const et4ad::ExpressionBase<REAL_T, TT>& rhs) {
    //        return lhs.GetValue() != rhs.GetValue();
    //    }
    //
    //    template<class REAL_T, class T, class TT>
    //    inline const int operator<(const et4ad::ExpressionBase<REAL_T, T>& lhs, const et4ad::ExpressionBase<REAL_T, TT>& rhs) {
    //        return lhs.GetValue() < rhs.GetValue();
    //    }
    //
    //    template<class REAL_T, class T, class TT>
    //    inline const int operator>(const et4ad::ExpressionBase<REAL_T, T>& lhs, const et4ad::ExpressionBase<REAL_T, TT>& rhs) {
    //        return lhs.GetValue() > rhs.GetValue();
    //    }
    //
    //    template<class REAL_T, class T, class TT>
    //    inline const int operator<=(const et4ad::ExpressionBase<REAL_T, T>& lhs, const et4ad::ExpressionBase<REAL_T, TT>& rhs) {
    //        return lhs.GetValue() <= rhs.GetValue();
    //    }
    //
    //    template<class REAL_T, class T, class TT>
    //    inline const int operator>=(const et4ad::ExpressionBase<REAL_T, T>& lhs, const et4ad::ExpressionBase<REAL_T, TT>& rhs) {
    //        return lhs.GetValue() >= rhs.GetValue();
    //    }
    //
    //    template<class REAL_T, class T>
    //    inline const int operator==(const REAL_T &lhs, const et4ad::ExpressionBase<REAL_T, T>& rhs) {
    //        return lhs == rhs.GetValue();
    //    }
    //
    //    template<class REAL_T, class T>
    //    inline const int operator!=(const REAL_T &lhs, const et4ad::ExpressionBase<REAL_T, T>& rhs) {
    //        return lhs != rhs.GetValue();
    //    }
    //
    //    template<class REAL_T, class T>
    //    inline const int operator<(const REAL_T &lhs, const et4ad::ExpressionBase<REAL_T, T>& rhs) {
    //        return lhs < rhs.GetValue();
    //    }
    //
    //    template<class REAL_T, class T>
    //    inline const int operator>(const REAL_T &lhs, const et4ad::ExpressionBase<REAL_T, T>& rhs) {
    //        return lhs > rhs.GetValue();
    //    }
    //
    //    template<class REAL_T, class T>
    //    inline const int operator<=(const REAL_T &lhs, const et4ad::ExpressionBase<REAL_T, T>& rhs) {
    //        return lhs <= rhs.GetValue();
    //    }
    //
    //    template<class REAL_T, class T>
    //    inline const int operator>=(const REAL_T &lhs, const et4ad::ExpressionBase<REAL_T, T>& rhs) {
    //        return lhs >= rhs.GetValue();
    //    }
    //
    //    template<class REAL_T, class T>
    //    inline const int operator==(const et4ad::ExpressionBase<REAL_T, T>& lhs, const REAL_T &rhs) {
    //        return lhs.GetValue() == rhs;
    //    }
    //
    //    template<class REAL_T, class T>
    //    inline const int operator!=(const et4ad::ExpressionBase<REAL_T, T>& lhs, const REAL_T &rhs) {
    //        return lhs.GetValue() != rhs;
    //    }
    //
    //    template<class REAL_T, class T>
    //    inline const int operator<(const et4ad::ExpressionBase<REAL_T, T>& lhs, const REAL_T &rhs) {
    //        return lhs.GetValue() <= rhs;
    //    }
    //
    //    template<class REAL_T, class T>
    //    inline const int operator>(const et4ad::ExpressionBase<REAL_T, T>& lhs, const REAL_T &rhs) {
    //        return lhs.GetValue() > rhs;
    //    }
    //
    //    template<class REAL_T, class T>
    //    inline const int operator<=(const et4ad::ExpressionBase<REAL_T, T>& lhs, const REAL_T &rhs) {
    //        return lhs.GetValue() <= rhs;
    //    }
    //
    //    template<class REAL_T, class T>
    //    inline const int operator>=(const et4ad::ExpressionBase<REAL_T, T>& lhs, const REAL_T &rhs) {
    //        return lhs.GetValue() >= rhs;
    //    }

    /**
     * A wrapper class for BasicVariables. Provides synchronized access for situations
     * where thread safety is desired.
     */




}





#endif	/* BASICVariable_HPP */

