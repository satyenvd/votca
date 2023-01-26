/*
 * Copyright 2009 The VOTCA Development Team (http://www.votca.org)
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

// Standard includes
#include <cstdlib>

// VOTCA includes
#include <votca/tools/average.h>
#include <votca/tools/tokenizer.h>

// Local VOTCA includes
#include <votca/csg/csgapplication.h>

using namespace std;
using namespace votca::csg;

class CsgRadiiApp : public CsgApplication {
  string ProgramName() override { return "radii"; }
  void HelpText(ostream &out) override {
    out << "calculate gyration- and hydrodynamic radius for a specific "
           "molecule or molecule type";
  }

  // some program options are added here
  void Initialize() override;

  // we want to process a trajectory
  bool DoTrajectory() override { return true; }

  // write out results in EndEvaluate
  void EndEvaluate() override;
  // do calculation in this function
  void EvalConfiguration(Topology *top, Topology *top_ref) override;

 protected:
  // inverse hydrodynamic radius average
  votca::tools::Average<double> inv_r_hydr_;
  // radius of gyration squared average
  votca::tools::Average<double> r_gyr_sq_;
  // mass weighted radius of gyration squared average
  votca::tools::Average<double> r_gyr_m_sq_;
};

int main(int argc, char **argv) {
  CsgRadiiApp app;

  return app.Exec(argc, argv);
}

void CsgRadiiApp::EvalConfiguration(Topology *top, Topology *) {
  // loop over all molecules
  for (const auto &mol : top->Molecules()) {
    // does the id match if given?
    if (OptionsMap().count("mol")) {
      if (OptionsMap()["mol"].as<votca::Index>() != mol.getId() + 1) {
        continue;
      }
    }
    // otherwise does the name pattern match?
    else if (!votca::tools::wildcmp(OptionsMap()["molname"].as<string>(),
                                    mol.getName())) {
      continue;  // if not skip this molecule
    }

    // Number of beads in the molecule
    votca::Index N = mol.BeadCount();

    // sqared tensor of gyration for current snapshot
    double r_gyr_sq = 0;
    // inverse hydrodynamic radius for current snapshot
    double inv_r_hydr = 0;

    // loop over all bead pairs in molecule
    for (votca::Index i = 0; i < N; ++i) {
      for (votca::Index j = i + 1; j < N; ++j) {
        // distance between bead i and j
        Eigen::Vector3d r_ij =
            mol.getBead(i)->getPos() - mol.getBead(j)->getPos();
        // radius of gyration squared
        r_gyr_sq += r_ij.squaredNorm() / (double)(N * N);
        // hydrodynamic radius
        inv_r_hydr += 2. / (r_ij.norm() * (double(N * N)));
      }
    }

    // add calculated values to the averages
    r_gyr_sq_.Process(r_gyr_sq);
    inv_r_hydr_.Process(inv_r_hydr);

    // calculate the mass weighted tensor of gyration
    // first calculate mass + center of mass
    double M = 0;
    Eigen::Vector3d cm(0, 0, 0);
    for (votca::Index i = 0; i < N; ++i) {
      M += mol.getBead(i)->getMass();
      cm += mol.getBead(i)->getPos() * mol.getBead(i)->getMass();
    }
    cm /= M;
    // now tensor of gyration based on cm
    double r_gyr_m_sq = 0;
    for (votca::Index i = 0; i < N; ++i) {
      Eigen::Vector3d r_ij = mol.getBead(i)->getPos() - cm;
      r_gyr_m_sq += mol.getBead(i)->getMass() * r_ij.squaredNorm();
    }
    r_gyr_m_sq /= M;

    // add to average
    r_gyr_m_sq_.Process(r_gyr_m_sq);
  }
}

// output everything when processing frames is done
void CsgRadiiApp::EndEvaluate() {
  cout << "\n\n------------------------------\n";
  cout << "radius of gyration:                  " << sqrt(r_gyr_sq_.getAvg())
       << endl;
  cout << "mass weighted radius of gyration:    " << sqrt(r_gyr_m_sq_.getAvg())
       << endl;
  cout << "hydrodynamic radius:                 " << 1. / inv_r_hydr_.getAvg()
       << endl;
  cout << "------------------------------\n";
}

// add our user program options
void CsgRadiiApp::Initialize() {
  CsgApplication::Initialize();
  // add program option to pick molecule
  AddProgramOptions("Molecule filter options")(
      "mol", boost::program_options::value<votca::Index>(), "molecule number")(
      "molname", boost::program_options::value<string>()->default_value("*"),
      "pattern for molecule name");
}
