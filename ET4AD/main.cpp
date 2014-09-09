/* 
 * File:   main.cpp
 * Author: matthewsupernaw
 *
 * Created on June 18, 2014, 11:16 AM
 */

#include <cstdlib>
#include <iostream>
#include "Stack.hpp"
#include "ET4AD.hpp"
#include "Chain.hpp"
#include <vector>
#include <complex>

template<class T>
T F(const T& x, const T& y) {
    return std::pow(x,2.0) + std::pow(y,4.0);
}

int main(int argc, char** argv) {

    
    std::cout<<1.0/H_V<<"\n";
//    exit(0);
//    double h = .00000000000001;
//    std::complex<double> x(M_PI / 4.0, h);
//    std::complex<double> y(M_PI / 2.0);
//    std::cout << F<std::complex<double> >(x,y).real() << " " << F<std::complex<double> >(x,y).imag() / h << "\n";
//
//    et4ad::Variable<double> xv;
//    xv.SetAsIndependent(true);
//    xv = M_PI / 4.0;
//
//    et4ad::Variable<double> yv;
//    yv.SetAsIndependent(true);
//    yv = M_PI / 2.0;
//
//    std::cout << F<et4ad::Variable<double> >(xv,yv) << " " << F<et4ad::Variable<double> >(xv,yv).WRT(xv) << "\n";
//    exit(0);
    //    std::vector< et4ad::ChainEntry<double> * > chain;
    //
    //    et4ad::ChainEntry<double> * x = new et4ad::ChainVariable<double>(3.1456,1);
    //    et4ad::ChainEntry<double> * y = new et4ad::ChainVariable<double>(1.12323, 2);
    //    et4ad::ChainMultiply<double> * z = new et4ad::ChainMultiply<double>(x, y);
    //    //    chain.push_back(x);
    //    //     chain.push_back(y);
    //    chain.push_back((et4ad::ChainEntry<double> *) z);
    //    double dx = 1.0;
    //    double xvv = 3.1459;
    //    for (int i = 0; i < chain.size(); i++) {
    //        dx *= chain[i]->Derivative(xvv, 1);
    //    }
    //
    //
    //    et4ad::Variable<double> xv;
    //
    //    xv.SetAsIndependent(true);
    //    xv = 3.1456;
    //    et4ad::Variable<double> yv;
    //    yv.SetAsIndependent(true);
    //    yv = 1.12323;
    //    et4ad::Variable<double>zv = xv*yv;
    //
    //    std::cout << zv.WRT(xv) << " " << dx << "\n";
    //

        typedef double Real;
//        //    et4ad::Stack<double> s;
//        //    double x = 3.14;
//        //    double y = 2.12;
//        //    double z = 100.12;
//        //    
//        //    
//        //    
//        //    
//        ////    s.Add(y,1.0/(y*y),1);//x
//        ////    s.Add(x,-1.0/(y*y),1);//y
//        ////    s.Add(1,1,2);
//        ////    s.Add(1,-1,2);
//        //    s.Add(1,1.0,1);
//        //    s.Add(1,1.0,2);
//        //    s.Add(-std::sin(y),1,0);
//        //    std::cout<<s.adjoint(2)<<"\n";
//        //    
//    srand (time(NULL));
            srand (40909012);

        int SIZE = 50;
        std::vector<et4ad::Variable<Real >* > params;
        et4ad::Variable<Real > xv;
        xv.SetAsIndependent(true);
        params.push_back(&xv);
//        std::cout << "id = " << xv.GetValue().GetId() << "\n";
        xv = Real(M_PI / 4.0);
        et4ad::Variable<Real > yv;
        yv.SetAsIndependent(true);
        yv =Real(M_PI / 2.0);
        params.push_back(&yv);
        std::cout<<xv.GetValue()<<" - "<<yv.GetValue()<<"\n";
        std::vector<et4ad::Variable<Real > > v(SIZE);
        for (int i = 0; i < SIZE; i++) {
            v[i] = et4ad::Variable<Real >();
            v[i].SetAsIndependent(true);
            v[i] =  Real((double) rand() / (RAND_MAX));
//            std::cout<<"v["<<i<<"] = "<<v[i].GetValue()<<"\n";
            params.push_back(&v[i]);
        }
//    
    
        et4ad::Variable<Real > zv =std::pow(xv,Real(2.0))+std::pow(yv,Real(4.0));
        for (int i = 0; i < SIZE; i++) {
//            std::cout<<i<<"\n";
            zv += std::exp(v[i]);
        }
        std::setprecision(100);
    //    std::cout<<zv.WRT(yv)<<"\n";
        for (int i = 0; i < params.size(); i++) {
//            for (int j = 0; j < params.size(); j++) {
    //            for (int k = 0; k < params.size(); k++) {
                Real d = zv.WRT(*params[i]);
                std::cout<<d<<" ";
    //            std::cout<<zv.WRT(*params[i]).WRT(*params[j])<<" ";
    //        }
    //        std::cout<<"\n";
//        }
        }

}