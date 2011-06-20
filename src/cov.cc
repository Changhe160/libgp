/**************************************************************
 libgp - Gaussian Process library for Machine Learning
 Copyright (C) 2011 Universität Freiburg
 Author: Manuel Blum
 
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 ***************************************************************/

#include "cov.h"
#include "gp_utils.h"

namespace libgp
{
  
  size_t CovarianceFunction::get_param_dim()
  {
    return param_dim;
  }
  
  size_t CovarianceFunction::get_input_dim()
  {
    return input_dim;
  }
  
  Eigen::VectorXd CovarianceFunction::get_loghyper()
  {
    return loghyper;
  }
  
  void CovarianceFunction::set_loghyper(const Eigen::VectorXd &p)
  {
    assert(p.size() == loghyper.size());
    loghyper = p;
  }
  
  void CovarianceFunction::set_loghyper(const double p[])
  {
    Eigen::Map<const Eigen::VectorXd> p_vec_map(p, param_dim);
    set_loghyper(p_vec_map);
  }

  
  Eigen::VectorXd CovarianceFunction::draw_random_sample(Eigen::MatrixXd &X)
  {
    assert (X.cols() == int(input_dim));  
    int n = X.rows();
    Eigen::MatrixXd K(n, n);
    Eigen::LLT<Eigen::MatrixXd> solver;
    Eigen::VectorXd y(n);
    // compute kernel matrix (lower triangle)
    for(int i = 0; i < n; ++i) {
      for(int j = i; j < n; ++j) {
        K(j, i) = get(X.row(j), X.row(i));
      }
      y(i) = Utils::randn();
    }
    // perform cholesky factorization
    solver = K.llt();  
    return solver.matrixL() * y;
  }
  
  double CovarianceFunction::get_threshold()
  {
    return INFINITY;
  }
  
  void CovarianceFunction::set_threshold(double threshold) 
  {
    std::cerr << "warning: thresholding is not supported for " << to_string() 
      << " covariance function." << std::endl;
  }
}