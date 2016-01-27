Machine Intelligence Algorithms
=========================================

Description
-----------

Subproject of Machine Intelligence Core.

The library core types and several machine intelligence algorithms, such as HTM, RBM accompanied by some useful modules and functions.

Dependencies
------------
   * MIToolchain - core library of MIC.

Additionally it depends on the following external libraries:
   * Boost - library of free (open source) peer-reviewed portable C++ source libraries.
   * OpenBlas - an optimized BLAS library based on GotoBLAS2 1.13 BSD version.

Main modules
------------
   * core_types - core types and classes (image, tensor, matrix, vector etc.)
   * data_io - data i/o classes and functions (e.g. data importers)
   * data_utils - data utils (e.g. functions for interpolations, generation of series, random generator etc.)
   * auto_encoders - auto (as well as some simple) encoders
   * classifiers - different classifiers (naive bayes, softmax etc.)
   * htm - Hierarchical Temporal Memory 
   * rbm - Restricted Boltzmann Machine


Maintainer
----------

tkornuta