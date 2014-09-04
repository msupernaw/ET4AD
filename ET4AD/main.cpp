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

int main(int argc, char** argv) {
    typedef et4ad::Variable<et4ad::Variable<double> >  Real;
    //    et4ad::Stack<double> s;
    //    double x = 3.14;
    //    double y = 2.12;
    //    double z = 100.12;
    //    
    //    
    //    
    //    
    ////    s.Add(y,1.0/(y*y),1);//x
    ////    s.Add(x,-1.0/(y*y),1);//y
    ////    s.Add(1,1,2);
    ////    s.Add(1,-1,2);
    //    s.Add(1,1.0,1);
    //    s.Add(1,1.0,2);
    //    s.Add(-std::sin(y),1,0);
    //    std::cout<<s.adjoint(2)<<"\n";
    //    
srand (time(NULL));
    int SIZE = 100;
    std::vector<et4ad::Variable<Real >* > params;
    et4ad::Variable<Real > xv;
    xv.SetAsIndependent(true);
    params.push_back(&xv);
    std::cout << "id = " << xv.GetValue().GetId() << "\n";
    xv = Real(1.0);
    et4ad::Variable<Real > yv;
    yv.SetAsIndependent(true);
    yv =Real(1.0);
    params.push_back(&yv);
    std::cout<<xv.GetValue()<<" - "<<yv.GetValue()<<"\n";
    std::vector<et4ad::Variable<Real > > v(SIZE);
    for (int i = 0; i < SIZE; i++) {
        v[i] = et4ad::Variable<Real >();
        v[i].SetAsIndependent(true);
        v[i] =  Real((double) rand() / (RAND_MAX));
        std::cout<<"v["<<i<<"] = "<<v[i].GetValue()<<"\n";
        params.push_back(&v[i]);
    }


    et4ad::Variable<Real > zv =std::pow(xv,Real(2.0))*std::pow(yv,Real(4.0));
    for (int i = 0; i < SIZE; i++) {
        std::cout<<i<<"\n";
        zv *= std::mfexp(v[i]);
    }
//    std::cout<<zv.WRT(yv)<<"\n";
    for (int i = 0; i < params.size(); i++) {
        for (int j = 0; j < params.size(); j++) {
//            for (int k = 0; k < params.size(); k++) {
           std::cout<< zv.WRT(*params[i]).WRT(*params[j]);
//            std::cout<<zv.WRT(*params[i]).WRT(*params[j])<<" ";
//        }
//        std::cout<<"\n";
    }
    }

}