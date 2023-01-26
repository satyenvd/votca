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

#ifndef VOTCA_CSG_DLPOLYTRAJECTORYREADER_PRIVATE_H
#define VOTCA_CSG_DLPOLYTRAJECTORYREADER_PRIVATE_H

// Standard includes
#include <fstream>
#include <iostream>
#include <string>

// VOTCA includes
#include <votca/tools/unitconverter.h>

// Local VOTCA includes
#include "votca/csg/trajectoryreader.h"

namespace votca {
namespace csg {

/**
    \brief class for reading dlpoly trajectory and configuration files

    This class encapsulates the dlpoly trajectory and configuration reading
   function and provides an interface to fill a topology class

*/

class DLPOLYTrajectoryReader : public TrajectoryReader {
 public:
  const tools::DistanceUnit distance_unit = tools::DistanceUnit::angstroms;
  const tools::MassUnit mass_unit = tools::MassUnit::atomic_mass_units;
  const tools::TimeUnit time_unit = tools::TimeUnit::picoseconds;
  const tools::ChargeUnit charge_unit = tools::ChargeUnit::e;
  const tools::MolarEnergyUnit energy_unit =
      tools::MolarEnergyUnit::joules_per_mole;
  const tools::VelocityUnit velocity_unit =
      tools::VelocityUnit::angstroms_per_picosecond;
  const tools::MolarForceUnit force_unit =
      tools::MolarForceUnit::kilojoules_per_mole_angstrom;

  /// open original trajectory file
  bool Open(const std::string &file) override;
  /// read in the first frame
  bool FirstFrame(Topology &conf) override;
  /// read in the next frame
  bool NextFrame(Topology &conf) override;
  /// close original trajectory file
  void Close() override;

  /// set/get the original configuration or trajectory file name:
  /// name.dlpc/name.dlph (convention: ".dlpc"="CONFIG", ".dlph"="HISTORY")
  void setFname(std::string name) {
    fname_ = name;
    return;
  }
  std::string getFname() { return fname_; }

  /// set/check the flag for the read-in file as configuration, i.e. not
  /// trajectory format
  void setIsConfig(bool isConf) {
    isConfig_ = isConf;
    return;
  }
  bool getIsConfig() { return isConfig_; }

 private:
  std::ifstream fl_;
  std::string fname_;
  bool first_frame_;
  bool isConfig_;
};

}  // namespace csg
}  // namespace votca

#endif  // VOTCA_CSG_DLPOLYTRAJECTORYREADER_PRIVATE_H
