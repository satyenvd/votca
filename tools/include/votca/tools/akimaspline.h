/*
 * Copyright 2009-2020 The VOTCA Development Team (http://www.votca.org)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef VOTCA_TOOLS_AKIMASPLINE_H
#define VOTCA_TOOLS_AKIMASPLINE_H

// Standard includes
#include <iostream>

// Local VOTCA includes
#include "eigen.h"
#include "floatingpointcomparison.h"
#include "spline.h"

namespace votca {
namespace tools {

/**
 *
 * \brief An Akima Spline Class
 *
 * does Akima interpolation based on the paper
 * "A new method of interpolation and smooth curve fitting based on local
 * procedures"
 *
 * Fitting is not supported. In order to fit data, do linear fitting and
 * interpolate the linear fitted values by Akima interpolation.
 */

class AkimaSpline : public Spline {
 public:
  // default constructor
  AkimaSpline() = default;

  // destructor
  ~AkimaSpline() override = default;

  /**
   * \brief Calculate the slope according to the original Akima paper ("A New
   * Method of Interpolation and Smooth Curve Fitting Based on Local
   * Procedures")
   * \param m1 slope of the first line segment connecting the five
   * data points
   * \param m2 slope of the second line segment connecting the five
   * data points
   * \param m3 slope of the third line segment connecting the five
   * data points
   * \param m4 slope of the fourth line segment connecting the five
   * data points
   * \return slope handles all special cases to determine the slope
   *  based on slopes m1,m2,m3,m4
   */
  double getSlope(double m1, double m2, double m3, double m4);

  // construct an interpolation spline
  // x, y are the the points to construct interpolation, both vectors must be of
  // same size
  void Interpolate(const Eigen::VectorXd &x, const Eigen::VectorXd &y) override;

  // fit spline through noisy data
  // x,y are arrays with noisy data, both vectors must be of same size
  void Fit(const Eigen::VectorXd &x, const Eigen::VectorXd &y) override;

  // Calculate the function value
  double Calculate(double r) override;

  // Calculate the function derivative
  double CalculateDerivative(double r) override;
  using Spline::Calculate;
  using Spline::CalculateDerivative;

 protected:
  // p1,p2,p3,p4 and t1,t2 (same identifiers as in Akima paper, page 591)
  Eigen::VectorXd p0;
  Eigen::VectorXd p1;
  Eigen::VectorXd p2;
  Eigen::VectorXd p3;
  Eigen::VectorXd t;
};

}  // namespace tools
}  // namespace votca

#endif  // VOTCA_TOOLS_AKIMASPLINE_H
