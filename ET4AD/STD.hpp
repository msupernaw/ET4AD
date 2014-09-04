/* 
 * File:   StandardNamespace.hpp
 * Author: matthewsupernaw
 *
 * Created on June 18, 2014, 12:08 PM
 */

#ifndef STANDARDNAMESPACE_HPP
#define	STANDARDNAMESPACE_HPP

#include <ostream>

#include "Constant.hpp"
#include "Add.hpp"
#include "Subtract.hpp"
#include "Multiply.hpp"
#include "Divide.hpp"
#include "Sin.hpp"
#include "Cos.hpp"
#include "Tan.hpp"
#include "ASin.hpp"
#include "ACos.hpp"
#include "ATan.hpp"
#include "ATan2.hpp"
#include "Sqrt.hpp"
#include "Pow.hpp"
#include "Log.hpp"
#include "Log10.hpp"
#include "Exp.hpp"
#include "MFExp.hpp"
#include "Sinh.hpp"
#include "Cosh.hpp"
#include "Tanh.hpp"
#include "Fabs.hpp"
#include "Floor.hpp"
#include "Ceil.hpp"

/**
 * Utility functions added to the standard name space. Mostly cmath overloads 
 * for expressions.
 */
namespace std {

     inline const double mfexp(const double& value) {
        double x = value;
        double b = double(60.0);
        if (x <= b && x >= double(-1) * b) {
            return std::exp(x);
        } else if (x > b) {
            return std::exp(b)*(double(1.) + double(2.) * (x - b)) / (double(1.) + x - b);
        } else {

            return std::exp(double(-1) * b)*(double(1.) - x - b) / (double(1.) + double(2.) * (double(-1) * x - b));
        }
    }
    
      /**
     * Write the expression value to std::ostream out.
     * @param out
     * @param exp
     * @return 
     */
    template<class REAL_T, class A>
    inline std::ostream& operator<<(std::ostream &out, const et4ad::ExpressionBase<REAL_T, A> &exp) {
        out << exp.GetValue();
        return out;
    }

     
#ifdef USE_STD_FILE
   

    /**
     * Override for the sin function in namespace std.
     * 
     * @param expr
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const et4ad::Sin<REAL_T, EXPR> sin(const et4ad::ExpressionBase<REAL_T, EXPR>& expr) {
        return et4ad::Sin<REAL_T, EXPR > (expr.Cast());
    }

    /**
     * Override for the cos function in namespace std.
     * 
     * @param expr
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const et4ad::Cos<REAL_T, EXPR> cos(const et4ad::ExpressionBase<REAL_T, EXPR>& expr) {
        return et4ad::Cos<REAL_T, EXPR > (expr.Cast());
    }

    /**
     * Override for the tan function in namespace std.
     * 
     * @param expr
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const et4ad::Tan<REAL_T, EXPR> tan(const et4ad::ExpressionBase<REAL_T, EXPR>& expr) {
        return et4ad::Tan<REAL_T, EXPR > (expr.Cast());
    }

    /**
     * Override for the asin function in namespace std.
     * 
     * @param expr
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const et4ad::ASin<REAL_T, EXPR> asin(const et4ad::ExpressionBase<REAL_T, EXPR>& expr) {
        return et4ad::ASin<REAL_T, EXPR > (expr.Cast());
    }

    /**
     * Override for the asin function in namespace std.
     * 
     * @param expr
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const et4ad::ACos<REAL_T, EXPR> acos(const et4ad::ExpressionBase<REAL_T, EXPR>& expr) {
        return et4ad::ACos<REAL_T, EXPR > (expr.Cast());
    }

    /**
     * Override for the atan function in namespace std.
     * 
     * @param expr
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const et4ad::ATan<REAL_T, EXPR> atan(const et4ad::ExpressionBase<REAL_T, EXPR>& a) {
        return et4ad::ATan<REAL_T, EXPR > (a.Cast());
    }

    /**
     * Override for the atan2 function in namespace std.
     * 
     * @param expr1
     * @param expr2
     * @return 
     */
    template <class REAL_T, class EXPR1, class EXPR2>
    inline
    et4ad::ATan2<REAL_T, EXPR1, EXPR2> atan2(const et4ad::ExpressionBase<REAL_T, EXPR1>& expr1,
    const et4ad::ExpressionBase<REAL_T, EXPR2>& expr2) {
        return et4ad::ATan2<REAL_T, EXPR1, EXPR2 > (expr1.Cast(), expr2.Cast());
    }

    /**
     * Override for the atan2 function in namespace std.
     * 
     * @param expr1
     * @param val
     * @return 
     */
    template <class REAL_T, class EXPR>
    inline
    et4ad::ATan2Constant<REAL_T, EXPR > atan2(const et4ad::ExpressionBase<REAL_T, EXPR>& expr,
    const REAL_T& val) {
        return et4ad::ATan2Constant<REAL_T, EXPR> (expr.Cast(), val);
    }

    /**
     * Override for the atan2 function in namespace std.
     * 
     * @param expr1
     * @param expr2
     * @return 
     */
    template <class REAL_T, class EXPR>
    inline
    et4ad::ConstantATan2<REAL_T, EXPR> atan2(const REAL_T& val,
    const et4ad::ExpressionBase<REAL_T, EXPR>& expr) {
        return et4ad::ConstantATan2<REAL_T,  EXPR > (val, expr.Cast());
    }

    /**
     * Override for the sqrt function in namespace std.
     * 
     * @param expr
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const et4ad::Sqrt<REAL_T, EXPR> sqrt(const et4ad::ExpressionBase<REAL_T, EXPR>& expr) {
        return et4ad::Sqrt<REAL_T, EXPR > (expr.Cast());
    }

    /**
     * Override for the pow function in namespace std.
     * 
     * @param expr1
     * @param expr2
     * @return 
     */
    template <class REAL_T, class EXPR1, class EXPR2>
    inline
    et4ad::Pow<REAL_T, EXPR1, EXPR2> pow(const et4ad::ExpressionBase<REAL_T, EXPR1>& expr1,
    const et4ad::ExpressionBase<REAL_T, EXPR2>& expr2) {
        return et4ad::Pow<REAL_T, EXPR1, EXPR2 > (expr1.Cast(), expr2.Cast());
    }

    /**
     * Override for the pow function in namespace std.
     * 
     * @param expr1
     * @param val
     * @return 
     */
    template <class REAL_T, class EXPR>
    inline
    et4ad::PowConstant< REAL_T, EXPR > pow(const et4ad::ExpressionBase<REAL_T, EXPR>& expr,
    const REAL_T& val) {
        return et4ad::PowConstant< REAL_T, EXPR > (expr.Cast(), val);
    }

    /**
     * Override for the pow function in namespace std.
     * 
     * @param val
     * @param expr2
     * @return 
     */
    template <class REAL_T, class EXPR>
    inline
    et4ad::ConstantPow<REAL_T, EXPR> pow(const REAL_T& val,
    const et4ad::ExpressionBase<REAL_T, EXPR>& expr) {

        return et4ad::ConstantPow<REAL_T, EXPR > (val, expr.Cast());
    }

    /**
     * Override for the log function in namespace std.
     * 
     * @param expr
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const et4ad::Log<REAL_T, EXPR> log(const et4ad::ExpressionBase<REAL_T, EXPR>& expr) {

        return et4ad::Log<REAL_T, EXPR > (expr.Cast());
    }

    /**
     * Override for the log function in namespace std.
     * 
     * @param expr
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const et4ad::Log10<REAL_T, EXPR> log10(const et4ad::ExpressionBase<REAL_T, EXPR>& expr) {

        return et4ad::Log10<REAL_T, EXPR > (expr.Cast());
    }

    /**
     * Override for the exp function in namespace std.
     * 
     * @param expr
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const et4ad::Exp<REAL_T, EXPR> exp(const et4ad::ExpressionBase<REAL_T, EXPR>& expr) {

        return et4ad::Exp<REAL_T, EXPR > (expr.Cast());
    }

    template<class REAL_T, class EXPR>
    inline const et4ad::MFExp<REAL_T, EXPR> mfexp(const et4ad::ExpressionBase<REAL_T, EXPR>& expr) {

        return et4ad::MFExp<REAL_T, EXPR > (expr.Cast());
    }

    inline const double mfexp(const double& value) {
        double x = value;
        double b = double(60.0);
        if (x <= b && x >= double(-1) * b) {
            return std::exp(x);
        } else if (x > b) {
            return std::exp(b)*(double(1.) + double(2.) * (x - b)) / (double(1.) + x - b);
        } else {

            return std::exp(double(-1) * b)*(double(1.) - x - b) / (double(1.) + double(2.) * (double(-1) * x - b));
        }
    }

    /**
     * Override for the sinh function in namespace std.
     * 
     * @param expr
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const et4ad::Sinh<REAL_T, EXPR> sinh(const et4ad::ExpressionBase<REAL_T, EXPR>& expr) {

        return et4ad::Sinh<REAL_T, EXPR > (expr.Cast());
    }

    /**
     * Override for the cosh function in namespace std.
     * 
     * @param expr
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const et4ad::Cosh<REAL_T, EXPR> cosh(const et4ad::ExpressionBase<REAL_T, EXPR>& expr) {

        return et4ad::Cosh<REAL_T, EXPR > (expr.Cast());
    }

    /**
     * Override for the tanh function in namespace std.
     * 
     * @param expr
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const et4ad::Tanh<REAL_T, EXPR> tanh(const et4ad::ExpressionBase<REAL_T, EXPR>& expr) {

        return et4ad::Tanh<REAL_T, EXPR > (expr.Cast());
    }

    /**
     * Override for the fabs function in namespace std.
     * 
     * @param expr
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const et4ad::Fabs<REAL_T, EXPR> fabs(const et4ad::ExpressionBase<REAL_T, EXPR>& expr) {

        return et4ad::Fabs<REAL_T, EXPR > (expr.Cast());
    }

    /**
     * Override for the floor function in namespace std.
     * 
     * @param expr
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const et4ad::Floor<REAL_T, EXPR> floor(const et4ad::ExpressionBase<REAL_T, EXPR>& expr) {

        return et4ad::Floor<REAL_T, EXPR > (expr.Cast());
    }

    /**
     * Override for the ceil function in namespace std.
     * 
     * @param expr
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const et4ad::Ceil<REAL_T, EXPR> ceil(const et4ad::ExpressionBase<REAL_T, EXPR>& expr) {
        return et4ad::Ceil<REAL_T, EXPR > (expr.Cast());
    }


#endif
}


/**
     * Error function.
     * Reference: Handbook of Mathematical Functions by Abramowitz and Stegun. 
     * 
     * @param x
     * @return 
     */
    template<class T>
    const T erf(const T &x) {
        // constants
        const double a1(0.254829592);
        const double a2(-0.284496736);
        const double a3(1.421413741);
        const double a4(-1.453152027);
        const double a5(1.061405429);
        const double p(0.3275911);
        const double one(1.0);

        // Save the sign of x
        double sign(1.0);
        if (x < 0.0)
            sign = -1.0;
        double xx = std::fabs(x);

        // A&S formula 7.1.26
        const double t = one / (one + p * xx);
        const double y = one - (((((a5 * t + a4) * t) + a3) * t + a2) * t + a1) * t * std::exp((-1.0) * xx * xx);

        return sign*y;
    }

    /**
     * Complimentary Error function.
     * Reference: Handbook of Mathematical Functions by Abramowitz and Stegun. 
     * 
     * @param x
     * @return 
     */
    template<class T>
    static T erfc(T &x) {
        return double(1) - erf<T > (x);
    }


#endif	/* STANDARDNAMESPACE_HPP */

