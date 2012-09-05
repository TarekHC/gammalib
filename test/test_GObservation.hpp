/***************************************************************************
 *             test_GObservation.hpp  -  Test observation module           *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2012 by Jean-Baptiste Cayrou                             *
 * ----------------------------------------------------------------------- *
 *                                                                         *
 *  This program is free software: you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, either version 3 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                         *
 ***************************************************************************/
/**
 * @file test_GObservation.hpp
 * @brief Test observation module
 * @author J.-B. Cayrou
 */

#ifndef TEST_GOBSERVATION_HPP
#define TEST_GOBSERVATION_HPP

/* __ Includes ___________________________________________________________ */
#include "GammaLib.hpp"


#ifdef _OPENMP
/***********************************************************************//**
 * @class TestOpenMP
 *
 * @brief Test suite for OpenMP testing
 ***************************************************************************/
class TestOpenMP : public GTestSuite {
    
public:
    // Constructors and destructors
    TestOpenMP(void) : GTestSuite() {}
    virtual ~TestOpenMP(void) {}

    // Methods
    virtual void set(void);
    void         test_observations_optimizer_unbinned_1();
    void         test_observations_optimizer_unbinned_10();
    void         test_observations_optimizer_binned_1();
    void         test_observations_optimizer_binned_10();
    GModelPar&   test_observations_optimizer(int mode=0);
};
#endif

#endif /* TEST_GOPTIMIZER_HPP */