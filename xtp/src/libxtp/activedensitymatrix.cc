/*
 *            Copyright 2009-2023 The VOTCA Development Team
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

/*
 * References- (1) A fast intrinsic localization procedure applicable for ab
 *initio and semiempirical linear combination of atomic orbital wave functions.
 *Janos Pipek and Paul G. Mezey. J. Chem. Phys. 90, 4916 (1989);
 *https://doi.org/10.1063/1.456588
 *(2) A Simple, Exact Density-Functional-Theory
 *Embedding Scheme. Frederick R. Manby, Martina Stella, Jason D. Goodpaster, and
 *Thomas F. Miller Journal of Chemical Theory and Computation 2012 8 (8),
 *2564-2568 DOI: 10.1021/ct300544e
 */
#include "votca/xtp/activedensitymatrix.h"
#include "votca/xtp/aomatrix.h"
namespace votca {
namespace xtp {

std::array<Eigen::MatrixXd, 3> ActiveDensityMatrix::compute_Dmat_A() {
  Eigen::MatrixXd localized_mo_coeff = orbitals_.getLMOs();
  return activedensitymatrix(localized_mo_coeff);
}

std::array<Eigen::MatrixXd, 3> ActiveDensityMatrix::activedensitymatrix(
    const Eigen::MatrixXd &localized_mo_coeff) {
  AOBasis aobasis = orbitals_.getDftBasis();
  AOOverlap overlap;
  overlap.Fill(aobasis);
  Index numOfActiveOrbs = 0;
  Index numOfInactiveOrbs = 0;
  std::vector<Index> numfuncpatom = aobasis.getFuncPerAtom();
  Eigen::MatrixXd active_mo_coeff;
  Eigen::MatrixXd inactive_mo_coeff;

  for (Index LocMoCoeff_col_i = 0; LocMoCoeff_col_i < localized_mo_coeff.cols();
       LocMoCoeff_col_i++) {
    // Calculate orbital wise Mulliken population
    const Eigen::MatrixXd orbital_wise_population =
        localized_mo_coeff.col(LocMoCoeff_col_i).transpose() *
        overlap.Matrix() *
        localized_mo_coeff.col(LocMoCoeff_col_i).asDiagonal();
    const Eigen::RowVectorXd MullikenPop_per_basisset =
        orbital_wise_population.colwise().sum();
    Index start = 0;
    bool inactive = true;
    for (Index atom_id = 0; atom_id < Index(numfuncpatom.size()); atom_id++) {
      const double MullikenPop_per_atom =
          MullikenPop_per_basisset.segment(start, numfuncpatom[atom_id]).sum();
      if ((std::find(activeatoms_.begin(), activeatoms_.end(), atom_id) !=
           activeatoms_.end()) &&
          MullikenPop_per_atom > threshold_) {
        active_mo_coeff.conservativeResize(localized_mo_coeff.rows(),
                                           numOfActiveOrbs + 1);
        active_mo_coeff.col(numOfActiveOrbs) =
            localized_mo_coeff.col(LocMoCoeff_col_i);
        numOfActiveOrbs++;
        inactive = false;
        break;
      }
      start += numfuncpatom[atom_id];
    }
    if (inactive) {
      inactive_mo_coeff.conservativeResize(localized_mo_coeff.rows(),
                                           numOfInactiveOrbs + 1);
      inactive_mo_coeff.col(numOfInactiveOrbs) =
          localized_mo_coeff.col(LocMoCoeff_col_i);
      numOfInactiveOrbs++;
    }
  }
  const Eigen::MatrixXd dmat_active =
      2 * active_mo_coeff * active_mo_coeff.transpose();
  std::array<Eigen::MatrixXd, 3> result;
  result[0] = dmat_active;
  result[1] = active_mo_coeff;
  result[2] = inactive_mo_coeff;
  return result;
}
}  // namespace xtp
}  // namespace votca