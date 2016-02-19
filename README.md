Machine Intelligence Core: Algorithms
=========================================

Description
-----------

A subproject of Machine Intelligence Core framework.

The library contains core types and several machine intelligence algorithms, such as HTM, RBM accompanied by some useful modules and functions.

MIC dependencies
------------
   * MIToolchain - core library of MIC.

External dependencies
------------
Additionally it depends on the following external libraries:
   * Boost - library of free (open source) peer-reviewed portable C++ source libraries.
   * Eigen - a C++ template library for linear algebra: matrices, vectors, numerical solvers, and related algorithms.
   
Optional external dependencies 
------------
   * OpenBlas - an optimized BLAS library based on GotoBLAS2 1.13 BSD version. If present - used for fastening operation on matrices.

Main modules
------------
   * types - core types and classes (image, tensor, matrix, vector etc.)
   * data_io - data i/o classes and functions (e.g. data importers)
   * data_utils - data utils (e.g. functions for interpolations, generation of series, random generator etc.)
   * auto_encoders - auto (as well as some simple) encoders
   * classifiers - different classifiers (naive bayes, softmax etc.)
   * htm - Hierarchical Temporal Memory 
   * rbm - Restricted Boltzmann Machine

Applications
------------
   * char_encoder_test examplary 1-of-k Char Encoder test application

Maintainer
----------

tkornuta