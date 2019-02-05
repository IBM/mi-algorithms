# Machine Intelligence Core: Algorithms

![Language](https://img.shields.io/badge/language-C%2B%2B-blue.svg)
[![GitHub license](https://img.shields.io/github/license/IBM/mi-toolchain.svg)](https://github.com/IBM/mi-toolchain/blob/master/LICENSE)
![](https://img.shields.io/github/release/IBM/mi-algorithms.svg)
[![Build Status](https://travis-ci.com/IBM/mi-algorithms.svg?branch=master)](https://travis-ci.com/IBM/mi-algorithms)
[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/IBM/mi-algorithms.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/IBM/mi-algorithms/context:cpp)
[![Total alerts](https://img.shields.io/lgtm/alerts/g/IBM/mi-algorithms.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/IBM/mi-algorithms/alerts/)

## Description

A subproject of Machine Intelligence Core framework.

The project contains core types and several tool useful when training different models and working with different problems, such as importers ()

### Main modules

   * types - core types and classes (sample, batch, matrix, tensor, position2D, action2D etc.)
   * data_io - data i/o classes and functions (e.g. various data importers, data collector)
   * data_utils - data utils (random generator, timer etc.)
   * encoders - tools for changing data format from one to another (mostly from/to Matrix)

### Applications

   * char_encoder_test - examplary 1-of-k Char Encoder test application.
   * data_collector_test - program for testing data collector.
   * tensor_test - program for testing tensor functionality.

### Unit tests

   *  types/matrixTestsRunner -- dense (Eigen-derived) matrix unit tests
   *  types/matrixArrayTestsRunner -- dense array of matrices matrix unit tests


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

To install GTest on Ubuntu:

    sudo apt-get install libgtest-dev

#### On Mac (OS X 10.14): (last tested on: Jan/22/2019)

    brew install git cmake doxygen boost eigen

To install GTest on Mac OS X:

    brew install --HEAD https://gist.githubusercontent.com/Kronuz/96ac10fbd8472eb1e7566d740c4034f8/raw/gtest.rb

## MIC dependencies

   * [MIToolchain](https://github.com/IBM/mi-toolchain) - the core of MIC framework.

### Installation of all MIC dependencies (optional)

This step is required only when not downloaded/installed the listed MIC dependencies earlier.

In directory scripts one can find script that will download and install all required MIC modules.

    git clone git@github.com:IBM/mi-algorithms.git
    cd mi-algorithms
    ./srcipt/install_mic_deps.sh ../mic

Then one can install the module by calling the following.

    ./scripts/build_mic_module.sh ../mic

Please note that it will create a directory 'deps' and download all sources into that directory.
After compilation all dependencies will be installed in the directory '../mic'.

### Installation of MI-algorithms
The following assumes that all MIC dependencies are installed in the directory '../mic'.

    git clone git@github.com:IBM/mi-algorithms.git
    cd mi-algorithms
    ./scripts/build_mic_module.sh ../mic

### Make commands

   * make install - install applications to ../mic/bin, headers to ../mic/include, libraries to ../mic/lib, cmake files to ../mic/share

## Documentation

In order to generate a "living" documentation of the code please run Doxygen:

    cd mi-algorithms
    doxygen mi-algorithms.doxyfile
    firefox html/index.html

The current documentation (generated straight from the code and automatically uploaded to github pages by Travis) is available at:

https://ibm.github.io/mi-algorithms/

## Maintainer

[tkornuta](http://github.com/tkornut)

[![HitCount](http://hits.dwyl.io/tkornut/ibm/mi-algorithms.svg)](http://hits.dwyl.io/tkornut/ibm/mi-algorithms)