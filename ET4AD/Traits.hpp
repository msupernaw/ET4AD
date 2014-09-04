
#ifndef PROMOTIONTRAITS_HPP
#define	PROMOTIONTRAITS_HPP

#include "Variable.hpp"


//#include "Expression.hpp"
//#include "Variable.hpp"

namespace et4ad {

    template<typename T1, typename T2>
    struct promote_trait {
    };

#define DECLARE_PROMOTION(A, B, C) template<> struct promote_trait<A, B> { typedef C return_type;};

    DECLARE_PROMOTION(float, float, float);
    DECLARE_PROMOTION(double, double, double);
    DECLARE_PROMOTION(double, float, double);
    DECLARE_PROMOTION(float, double, double);
//    DECLARE_PROMOTION(et4ad::Variable<float>, et4ad::Variable<float>, et4ad::Variable<float>);
//    DECLARE_PROMOTION(float, et4ad::Variable<float>, et4ad::Variable<float>);
//    DECLARE_PROMOTION(et4ad::Variable<float>, float, et4ad::Variable<float>);
//     DECLARE_PROMOTION(et4ad::Variable<float>, et4ad::Variable<float>, et4ad::Variable<float>);
//    DECLARE_PROMOTION(double, et4ad::Variable<double>, et4ad::Variable<double>);
//    DECLARE_PROMOTION(et4ad::Variable<double>, double, et4ad::Variable<double>);
//    DECLARE_PROMOTION(et4ad::Variable<float>, et4ad::Variable<double>, et4ad::Variable<double>);
//    
//     template<typename T1>
//    struct id_trait {
//         void SetId(T1& t, uint32_t id){
//             
//         }
//    };
//
//    
//    template<>id_trait<et4ad::Variable<double> >{
//        void SetId(et4ad::Variable<double>& t, uint32_t id){
//            t.id_m = id;
//         }
//    }
//    
    
}


#endif	/* PROMOTIONTRAITS_HPP */

