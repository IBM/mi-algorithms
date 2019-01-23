# Machine Intelligence Core: Algorithms

![Language](https://img.shields.io/badge/language-C%2B%2B-blue.svg)
[![GitHub license](https://img.shields.io/github/license/IBM/mi-toolchain.svg)](https://github.com/IBM/mi-toolchain/blob/master/LICENSE)
[![Build Status](https://travis-ci.com/IBM/mi-algorithms.svg?branch=master)](https://travis-ci.com/IBM/mi-algorithms)


## Description

A subproject of Machine Intelligence Core framework.

The project contains core types and several machine intelligence algorithms, such as HTM, RBM accompanied by some useful modules and functions.

## MIC dependencies

   * [MIToolchain](https://github.com/IBM/mi-toolchain) - the core of MIC framework.

## External dependencies

Additionally it depends on the following external libraries:
   * Boost - library of free (open source) peer-reviewed portable C++ source libraries.
   * Eigen - a C++ template library for linear algebra: matrices, vectors, numerical solvers, and related algorithms.
   * OpenBlas (optional) - An optimized library implementing BLAS routines. If present - used for fastening operation on matrices.
   * Doxygen (optional) - Tool for generation of documentation.
   * GTest (optional) - Framework for unit testing.

### Installation of the dependencies/required tools

#### On Linux (Ubuntu 14.04):

    sudo apt-get install git cmake doxygen libboost1.54-all-dev libeigen3-dev

#### On Mac (OS X 10.14): (last tested on: Jan/22/2019)

    brew install git cmake doxygen boost eigen

To install GTest on Mac OS X:

    brew install --HEAD https://gist.githubusercontent.com/Kronuz/96ac10fbd8472eb1e7566d740c4034f8/raw/gtest.rb



## Main modules

   * types - core types and classes (image, tensor, matrix, vector etc.)
   * data_io - data i/o classes and functions (e.g. data importers)
   * data_utils - data utils (e.g. functions for interpolations, generation of series, random generator etc.)
   * auto_encoders - auto (as well as some simple) encoders
   * classifiers - different classifiers (naive bayes, softmax etc.)
   * htm - Hierarchical Temporal Memory
   * rbm - Restricted Boltzmann Machine

## Applications

   * char_encoder_test - examplary 1-of-k Char Encoder test application.
   * data_collector_test - program for testing data collector.
   * tensor_test - program for testing tensor functionality.

## Unit tests

   *  types/matrixTestsRunner -- dense (Eigen-derived) matrix unit tests
   *  types/matrixArrayTestsRunner -- dense array of matrices matrix unit tests
 
## Installation

In order to download, configure, make and install new "clean" version of mi-algorithms please execute the following:

    cd ~/workspace
    git clone git@github.com:ibm/mi-algorithms.git
    cd mi-algorithms
    mkdir build
    cd build
    cmake .. -DCMAKE_INSTALL_PREFIX=~/workspace/mic/
    make -j4 install

## Documentation

In order to generate a "living" documentation of the code please run Doxygen:

    cd ~/workspace/mi-algorithms
    doxygen mi-algorithms.doxyfile
    firefox html/index.html

The current documentation (generated straight from the code) is available at github pages:

https://ibm.github.io/mi-algorithms/

## Maintainer

Tomasz Kornuta (tkornut@us.ibm.com)

