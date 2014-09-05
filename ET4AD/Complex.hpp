/* 
 * File:   Complex.hpp
 * Author: matthewsupernaw
 *
 * Created on September 4, 2014, 4:33 PM
 */

#ifndef COMPLEX_HPP
#define	COMPLEX_HPP

#include <cmath>
#include <iostream>
#include <fstream>
#include <iomanip>

template <class T>
class Complex {
public:
	enum { D = 2 };

	Complex(const Complex<T> &S) : a(S.a), b(S.b) { }
	Complex(const T U[D]) : a(U[0]), b(U[1]) { }
	Complex(const T &sa = (T)0, const T &sb = (T)0) : a(sa), b(sb) { }

	const Complex<T> & set(const T &, const T &);

	const Complex<T> operator + (const Complex<T> &) const;
	const Complex<T> operator - (const Complex<T> &) const;
	const Complex<T> operator * (const Complex<T> &) const;
	const Complex<T> operator / (const Complex<T> &) const;
	const Complex<T> operator + (const T &) const;
	const Complex<T> operator - (const T &) const;
	const Complex<T> operator * (const T &) const;
	const Complex<T> operator / (const T &) const;

	const Complex<T> & operator = (const Complex<T> &);
	const Complex<T> & operator += (const Complex<T> &);
	const Complex<T> & operator -= (const Complex<T> &);
	const Complex<T> & operator *= (const Complex<T> &);
	const Complex<T> & operator /= (const Complex<T> &);
	const Complex<T> & operator = (const T &);
	const Complex<T> & operator += (const T &);
	const Complex<T> & operator -= (const T &);
	const Complex<T> & operator *= (const T &);
	const Complex<T> & operator /= (const T &);

	const Complex<T> operator - () const;
	const T & operator [] (const int) const;
	T & operator [] (const int);

	std::ofstream & saveBin(std::ofstream &) const;
	std::ifstream & loadBin(std::ifstream &);

	T normalize(void);

	~Complex(void) { }

	union
    {
        T V[D];
        struct
        {
            T a, b;
        };
    };
};

template <class T>
inline const T abs(const Complex<T> &A)
{
    return sqrt(A.a*A.a + A.b*A.b);
}

template <class T>
inline const T absSq(const Complex<T> &A)
{
    return A.a*A.a + A.b*A.b;
}

template <class T>
inline const T arg(const Complex<T> &A)
{
    return atan2(A.b, A.a);
}

template <class T>
const Complex<T> conj(const Complex<T> &A)
{
    return Complex<T>(A.a, -A.b);
}

template <class T>
inline const T re(const Complex<T> &A)
{
    return A.a;
}

template <class T>
inline const T im(const Complex<T> &A)
{
    return A.b;
}

template <class T>
inline Complex<T> sqrt(const Complex<T> &C)
{
	T l = abs©;
	T p = 0.5*arg©;

	return Complex<T>(l*cos(p), l*sin(p));
}

template <class T>
inline Complex<T> exp(const Complex<T> &C)
{
	T k = exp(C.a);
	return Complex<T>(cos(C.b)*k, sin(C.b)*k);
}

template <class T>
inline Complex<T> pow(const Complex<T> &C, const T &m)
{
	T l = pow(abs©, m);
	T p = m*arg©;

	return Complex<T>(l*cos(p), l*sin(p));
}

template <class T>
inline Complex<T> pow(const T &a, const Complex<T> &C)
{
	T p = pow(a, C.a);
	T l = log(C.a);

	return Complex<T>(p*cos(C.b*l), p*sin(C.b*l));
}

template <class T>
std::ostream & operator << (std::ostream &vout, const Complex<T> &Q)
{
    return vout << "" << std::setw(14) << Q.a << " + " << std::setw(14) << Q.b << "i";
}

template <class T>
inline const Complex<T> operator + (const T &l, const Complex<T> &R)
{
    return Complex<T>(l + R.a, R.b);
}

template <class T>
inline const Complex<T> operator - (const T &l, const Complex<T> &R)
{
    return Complex<T>(l - R.a, -R.b);
}

template <class T>
inline const Complex<T> operator * (const T &l, const Complex<T> &R)
{
    return Complex<T>(l*R.a, l*R.b);
}

template <class T>
inline const Complex<T> operator / (const T &l, const Complex<T> &R)
{
    T z = absSq®;
    return Complex<T>(l*R.a/z, -l*R.b/z);
}

template <class T>
inline const Complex<T> & Complex<T>::set(const T &sa, const T &sb)
{
	a = sa; b = sb;
	return *this;
}

template <class T>
inline const Complex<T> Complex<T>::operator + (const Complex<T> &R) const
{
    return Complex<T>(a + R.a, b + R.b);
}

template <class T>
inline const Complex<T> Complex<T>::operator - (const Complex<T> &R) const
{
    return Complex<T>(a - R.a, b - R.b);
}

template <class T>
inline const Complex<T> Complex<T>::operator * (const Complex<T> &R) const
{
    return Complex<T>(a*R.a - b*R.b, a*R.b + b*R.a);
}

template <class T>
inline const Complex<T> Complex<T>::operator / (const Complex<T> &R) const
{
    T z = abs®;
    return Complex<T>((a*R.a + b*R.b)/z, (b*R.a - a*R.b)/z);
}

template <class T>
inline const Complex<T> Complex<T>::operator + (const T &r) const
{
    return Complex<T>(a + r, b);
}

template <class T>
inline const Complex<T> Complex<T>::operator - (const T &r) const
{
    return Complex<T>(a - r, b);
}

template <class T>
inline const Complex<T> Complex<T>::operator * (const T &r) const
{
    return Complex<T>(a*r, b*r);
}

template <class T>
inline const Complex<T> Complex<T>::operator / (const T &r) const
{
    return Complex<T>(a/r, b/r);
}

template <class T>
inline const Complex<T> & Complex<T>::operator = (const Complex<T> &R)
{
    return set(R.a, R.b);
}

template <class T>
inline const Complex<T> & Complex<T>::operator += (const Complex<T> &R)
{
    return set(a + R.a, b + R.b);
}

template <class T>
inline const Complex<T> & Complex<T>::operator -= (const Complex<T> &R)
{
    return set(a - R.a, b - R.b);
}

template <class T>
inline const Complex<T> & Complex<T>::operator *= (const Complex<T> &R)
{
    return set(a*R.a - a*R.b, a*R.b + b*R.a);
}

template <class T>
inline const Complex<T> & Complex<T>::operator /= (const Complex<T> &R)
{
    T z = abs®;
    return set((a*R.a + a*R.b)/z, (b*R.a - a*R.b)/z);
}

template <class T>
inline const Complex<T> & Complex<T>::operator = (const T &r)
{
    return set(r, static_cast<T>(0));
}

template <class T>
inline const Complex<T> & Complex<T>::operator += (const T &r)
{
    return set(a + r, b);
}

template <class T>
inline const Complex<T> & Complex<T>::operator -= (const T &r)
{
    return set(a - r, b);
}

template <class T>
inline const Complex<T> & Complex<T>::operator *= (const T &r)
{
    return set(a*r, b*r);
}

template <class T>
inline const Complex<T> & Complex<T>::operator /= (const T &r)
{
    return set(a/r, b/r);
}

template <class T>
inline const Complex <T> Complex<T>::operator - () const
{
	return Complex(-a, -b);
}

template <class T>
inline const T & Complex<T>::operator [] (const int en) const
{
	return V[en];
}

template <class T>
inline T & Complex<T>::operator [] (const int en)
{
	return V[en];
}

template <class T>
inline std::ofstream & Complex<T>::saveBin(std::ofstream &savf) const
{
	savf.write((char *)V, D*sizeof(T));
	return savf;
}

template <class T>
inline std::ifstream & Complex<T>::loadBin(std::ifstream &loaf)
{
	loaf.read((char *)V, D*sizeof(T));
	return loaf;
}


template <class T>
inline T Complex<T>::normalize(void)
{
	T l = sqrt(a*a + b*b);

    if(fabs(l) > (T)0)
    {
        a /= l;
        b /= l;
    }

	return l;
}


#endif	/* COMPLEX_HPP */

