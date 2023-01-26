/*
 * Copyright 2009-2023 The VOTCA Development Team (http://www.votca.org)
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

/**

  \mainpage VOTCA C++ reference

  \section intro_sec Introduction

  This page is the C++ code documentation of the VOTCA package
  (http://www.votca.org). The main target of VOTCA-CSG is the implementation of
  systematic coarse-graining techniques. However, it offers a powerful,
  object-oriented framework to develop analysis tools for particle based
  molecular simulations.

  A later addition is the VOTCA-XTP package for charge/excition transport and
  electronic structure calculations using GW-BSE. Here we focus on CSG first.

  \section started_sec Getting started
  To start developing custom analysis tools, a good place to start is the
  csgapps folder:

  https://github.com/votca/votca/tree/master/csg/src/csgapps

  It contains several small analysis tools which were implemented based upon the
  VOTCA-CSG framework. We highly recomment to use an IDE such as Visual Studio
  Code for development since it offers lots of guides to get started with new
  code (code completion, code documentation popups, navigation through code,
  ...).

  The main container for the whole structure is the Topology, so it is a good
  advise to get comfortable with this class. Also the standard applications in
  csg/src/tools might help.

  \section beginner_sec For beginners: how to avoid frustration

  For those not familiar with object oriented code: don't try to dig into every
  single function in order to understand what exactly is going on. This strategy
  only works for very small projects and is not intended for oject oriented
  programs. Think about the code in layers of abstraction! Your main focus
  should be on the global structure and understand how objects relate to each
  other. The code was designed that you don't have to redo and understand all
  the nasty details!

 */

#ifndef VOTCA_CSG_VERSION_H
#define VOTCA_CSG_VERSION_H

// Standard includes
#include <string>

namespace votca {
namespace csg {
void HelpTextHeader(const std::string &tool_name);
}  // namespace csg
}  // namespace votca

#endif /*  VOTCA_CSG_VERSION_H */
