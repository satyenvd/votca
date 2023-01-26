/*
 *            Copyright 2009-2020 The VOTCA Development Team
 *                       (http://www.votca.org)
 *
 *      Licensed under the Apache License, Version 2.0 (the "License")
 *
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef VOTCA_TOOLS_CALCULATOR_H
#define VOTCA_TOOLS_CALCULATOR_H

// Local VOTCA includes
#include "globals.h"
#include "property.h"
#include "propertyiomanipulator.h"

namespace votca {
namespace tools {

/**
 * \brief Base class for all calculators
 *
 * Calculators are grouped in CalculatorFactories and are run by Threads
 * or Applications. Every calculator has a description (an XML file) installed
 * in VOTCASHARE which is used to compile HELP.
 * This XML file also contains default values.
 *
 */
class Calculator {
 public:
  Calculator() = default;
  virtual ~Calculator() = default;
  /**
   * \brief Calculator name
   *
   * This name is used to register a calculator in a Factory
   * It the name of the XML file with the default calculator options
   * stored in VOTCASHARE
   *
   * @return calculator name
   */
  virtual std::string Identify() const = 0;

  /**
   * \brief Package name
   *
   * This name is the name of the package the calculator belongs to, e.g. csg,
   * xtp, etc..
   *
   * @return Package name
   */
  virtual std::string Package() const = 0;

  /**
   * \brief Initializes a calculator from an XML file with options
   *
   * Options are passed to a calculator by the Application
   * These option overwrite defaults
   *
   * @param user_options Property object passed by the application to a
   * calculator
   */
  virtual void Initialize(const Property &user_options) = 0;
  /**
   * \brief Sets number of threads to use
   *
   * If only one thread is used, this calculator behaves as a master
   *
   * @param nThreads number of threads running this calculator
   *
   */
  void setnThreads(Index nThreads) {
    nThreads_ = nThreads;
    maverick_ = (nThreads_ == 1) ? true : false;
  }

 protected:
  Index nThreads_;
  bool maverick_;
};

}  // namespace tools
}  // namespace votca

#endif  // VOTCA_TOOLS_CALCULATOR_H
