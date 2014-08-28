ET4AD
=====

Expression Templates For Automatic Differentiation. 


Essentially the same algorithm as ADNumber using template metaprogramming to speed things up. Calculates expression level derivatives using intermediate derivative storage rather than the traditional stack or tape method. An expression can also be "recorded" in a stack and provide partial derivatives of arbitrary order. There are several policies for strorage at the individual variable level, to include:

1. StackStorage   - gradients are stored in a stack allocated array.
2. DynamicStorage - gradients are stored in a dynamically allocated array.
3. MapStorage     - gradients are stored in a associative container.
3. DiskStorage    - gradients are stored on the disk to reduce ram consumption.

Example:

```cpp
    typedef et4ad::DiskStorage<double, 1000> Storage_Type;
    //typedef et4ad::DynamicStorage<double> Storage_Type;
    //et4ad::BasicVariable<data type, group identifier, storage type>                      
    typedef et4ad::BasicVariable<double, 0, Storage_Type> Var;

    int size = 10;

    et4ad::Array<Var > x(size);
    
    Var slope;
    slope = 2.1212;
    slope.SetAsIndependent(true); //gives slope a unique identifer and stops it from recording derivative info.
    
    Var intercept;
    intercept = 21.12312321;
    intercept.SetAsIndependent(true); //gives intercept a unique identifer and stops it from recording derivative info.
    
    std::cout << "starting computation..." << std::endl;
    for (int i = 0; i < a.Size(0); i++) {
        x(i) = i;
    }

    et4ad::Array<Var > z(size);
    

    for (int i = 0; i < size; i++) {
        z(i) += slope + x(i) + intercept;
        //get the derivatice w.r.t. the slope.
        std::cout << z(i).WRT(slope) << std::endl;//should = 1
    }
```
