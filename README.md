# VecOps
A header only library built on top of ROOT to perform operations on collections, fast and easily
```
sqrt(v0*v0 + v1*v1) / 3
```

TVec<T> solves two probelms:
1) Allow to naturally express operations on collections of values which are common in analysis
2) Act as a view on other collections with a contiguous memory region associated. An example are TTreeReaderArray's. This indeed allows to solve problem 1) in the context of TDataFrame based analysis.

## How do you do that?
```TVec<T>``` and the related operators are implemented exploiting advanced template metaprogramming: it strives to achieve generality and performance at the same time.
The main class of the library is ```TVec<T>```, which has the following nice properties:
- has a storage associated which is contiguous (yes, almost all operations rely on autovectorisation)
- offers the same interface of ```std::vector<T>```
- supports +, -, *, /, <, <=, ==, >=, >, != operators in combination other TVec<T>s and with *scalars* (for example numbers)
- supports trivial math functions such as the trigonometrical ones, exp, log and sqrt
- plays well with helper functions such as Dot, Sum, Filter
- it is written on ROOT files as a ```std::vector<T>```

## What about some examples?
Please have a look into the test directory!

[![status](https://travis-ci.org/dpiparo/VecOps.svg?branch=master)](https://travis-ci.org/dpiparo/VecOps#)

E. Guiraud, D. Piparo, E. Tejedor
2018 Copyright CERN
