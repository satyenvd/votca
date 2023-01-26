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

#ifndef VOTCA_CSG_DLPOLYTOPOLOGYREADER_H
#define VOTCA_CSG_DLPOLYTOPOLOGYREADER_H

// Standard includes
#include <string>

// VOTCA includes
#include <votca/tools/unitconverter.h>

// Local VOTCA includes
#include "votca/csg/topology.h"
#include "votca/csg/topologyreader.h"

namespace votca {
namespace csg {

/**
    \brief class for reading dlpoly topology files

    This class encapsulates the dlpoly topology reading functions and provides
   an interface to fill a topolgy class

*/

class DLPOLYTopologyReader : public TopologyReader {
 public:
  DLPOLYTopologyReader() = default;

  /// Set default units
  const tools::DistanceUnit distance_unit = tools::DistanceUnit::angstroms;
  const tools::MassUnit mass_unit = tools::MassUnit::atomic_mass_units;
  const tools::TimeUnit time_unit = tools::TimeUnit::picoseconds;
  const tools::ChargeUnit charge_unit = tools::ChargeUnit::e;
  const tools::MolarEnergyUnit molar_energy_unit =
      tools::MolarEnergyUnit::joules_per_mole;
  const tools::VelocityUnit velocity_unit =
      tools::VelocityUnit::angstroms_per_picosecond;
  const tools::MolarForceUnit force_unit =
      tools::MolarForceUnit::kilojoules_per_mole_angstrom;
  /// read a topology file
  bool ReadTopology(std::string file, Topology &top) override;

  /// set the topology file name: name.dlpf (convention: ".dlpf"="FIELD")
  void setFname(std::string name) {
    fname_ = name;
    return;
  }
  /// get the topology file name: name.dlpf (convention: ".dlpf"="FIELD")
  std::string getFname() { return fname_; }

 private:
  std::string fname_;
  /// function to find and read the next line starting with a keyword/directive
  /// (skipping comments starting with "#" or ";")
  std::string NextKeyline_(std::ifstream &fs, const char *wspace);
  /// function to read the next line containing only a given keyword and an
  /// integer value after it (only skipping comments!)
  std::string NextKeyInt_(std::ifstream &fs, const char *wspace,
                          const std::string &word, Index &ival);
  /// function to check if the given (last read) directive line starts with a
  /// given keyword and has an integer value at the end
  bool isKeyInt_(const std::string &line, const char *wspace,
                 const std::string &word, Index &ival);
};

}  // namespace csg
}  // namespace votca

#endif  // VOTCA_CSG_DLPOLYTOPOLOGYREADER_H
