/*!
 * Copyright (C) tkornuta, IBM Corporation 2015-2019
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

#include "Matrix.hpp"
#include "Rand.hpp"
#include <fstream>

class GRU{
public:
  GRU(){};
  GRU(const int inputDim, const int hiddenDim);

  class State;
  class Grad;

  MatD Wxr, Whr, br;
  MatD Wxz, Whz, bz;
  MatD Wxu, Whu, bu;

  void init(Rand& rnd, const double scale = 1.0);
  virtual void forward(const MatD& xt, const GRU::State* prev, GRU::State* cur);
  virtual void backward(GRU::State* prev, GRU::State* cur, GRU::Grad& grad, const MatD& xt);
  void sgd(const GRU::Grad& grad, const double learningRate);
  void save(std::ofstream& ofs);
  void load(std::ifstream& ifs);
};

class GRU::State{
public:
  MatD h, u, r, z;
  MatD rh;

  MatD delh, delx; //for backprop

  void clear();
};

class GRU::Grad{
public:
  Grad(){}
  Grad(const GRU& gru);

  MatD Wxr, Whr, br;
  MatD Wxz, Whz, bz;
  MatD Wxu, Whu, bu;

  void init();
  double norm();

  void operator += (const GRU::Grad& grad);
  void operator /= (const double val);
};
