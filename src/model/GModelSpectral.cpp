/***************************************************************************
 *         GModelSpectral.cpp - Abstract spectral model base class         *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2009-2015 by Juergen Knoedlseder                         *
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
 * @file GModelSpectral.cpp
 * @brief Abstract spectral model base class implementation
 * @author Juergen Knoedlseder
 */

/* __ Includes ___________________________________________________________ */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "GException.hpp"
#include "GModelSpectral.hpp"

/* __ Method name definitions ____________________________________________ */
#define G_ACCESS                   "GModelSpectral::operator[](std::string&)"
#define G_AT                            "GModelPar& GModelSpectral::at(int&)"

/* __ Macros _____________________________________________________________ */

/* __ Coding definitions _________________________________________________ */

/* __ Debug definitions __________________________________________________ */


/*==========================================================================
 =                                                                         =
 =                        Constructors/destructors                         =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Void constructor
 ***************************************************************************/
GModelSpectral::GModelSpectral(void)
{
    // Initialise members
    init_members();
  
    // Return
    return;
}


/***********************************************************************//**
 * @brief Copy constructor
 *
 * @param[in] model Spectral model.
 ***************************************************************************/
GModelSpectral::GModelSpectral(const GModelSpectral& model)
{ 
    // Initialise members
    init_members();

    // Copy members
    copy_members(model);

    // Return
    return;
}


/***********************************************************************//**
 * @brief Destructor
 ***************************************************************************/
GModelSpectral::~GModelSpectral(void)
{
    // Free members
    free_members();

    // Return
    return;
}


/*==========================================================================
 =                                                                         =
 =                               Operators                                 =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Assignment operator
 *
 * @param[in] model Spectral model.
 * @return Spectral model.
 ***************************************************************************/
GModelSpectral& GModelSpectral::operator=(const GModelSpectral& model)
{ 
    // Execute only if object is not identical
    if (this != &model) {

        // Free members
        free_members();

        // Initialise private members for clean destruction
        init_members();

        // Copy members
        copy_members(model);

    } // endif: object was not identical
  
    // Return
    return *this;
}


/***********************************************************************//**
 * @brief Returns reference to model parameter
 *
 * @param[in] name Parameter name.
 *
 * @exception GException::par_not_found
 *            Parameter with specified name not found in container.
 ***************************************************************************/
GModelPar& GModelSpectral::operator[](const std::string& name)
{
    // Get parameter index
    int index = 0;
    for (; index < size(); ++index) {
        if (m_pars[index]->name() == name) {
            break;
        }
    }

    // Throw exception if parameter name was not found
    if (index >= size()) {
        throw GException::par_not_found(G_ACCESS, name);
    }

    // Return reference
    return *(m_pars[index]);
}


/***********************************************************************//**
 * @brief Returns reference to model parameter (const version)
 *
 * @param[in] name Parameter name.
 *
 * @exception GException::par_not_found
 *            Parameter with specified name not found in container.
 ***************************************************************************/
const GModelPar& GModelSpectral::operator[](const std::string& name) const
{
    // Get parameter index
    int index = 0;
    for (; index < size(); ++index) {
        if (m_pars[index]->name() == name) {
            break;
        }
    }

    // Throw exception if parameter name was not found
    if (index >= size()) {
        throw GException::par_not_found(G_ACCESS, name);
    }

    // Return reference
    return *(m_pars[index]);
}


/*==========================================================================
 =                                                                         =
 =                             Public methods                              =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Returns model parameter
 *
 * @param[in] index Parameter index [0,...,size()-1].
 * @return Model parameter.
 *
 * @exception GException::out_of_range
 *            Parameter index is out of range.
 *
 * Returns model parameter with @p index range checking.
 ***************************************************************************/
GModelPar& GModelSpectral::at(const int& index)
{
    // Compile option: raise exception if index is out of range
    if (index < 0 || index >= size()) {
        throw GException::out_of_range(G_AT, index, 0, size()-1);
    }

    // Return reference
    return *(m_pars[index]);
}


/***********************************************************************//**
 * @brief Returns model parameter (const version)
 *
 * @param[in] index Parameter index [0,...,size()-1].
 * @return Model parameter.
 *
 * @exception GException::out_of_range
 *            Parameter index is out of range.
 *
 * Returns model parameter with @p index range checking.
 ***************************************************************************/
const GModelPar& GModelSpectral::at(const int& index) const
{
    // Compile option: raise exception if index is out of range
    if (index < 0 || index >= size()) {
        throw GException::out_of_range(G_AT, index, 0, size()-1);
    }

    // Return reference
    return *(m_pars[index]);
}


/***********************************************************************//**
 * @brief Checks if parameter name exists
 *
 * @param[in] name Parameter name.
 * @return True if parameter with specified @p name exists.
 *
 * Searches all parameter names for a match with the specified @p name. If
 * the specified name has been found, true is returned.
 ***************************************************************************/
bool GModelSpectral::has_par(const std::string& name) const
{
    // Default found flag to false
    bool found = false;

    // Search for parameter name
    for (int i = 0; i < size(); ++i) {
        if (m_pars[i]->name() == name) {
            found = true;
            break;
        }
    }

    // Return
    return found;
}


/***********************************************************************//**
 * @brief Autoscale parameters
 *
 * Sets the scale factors for all parameters so that the values are unity.
 ***************************************************************************/
void GModelSpectral::autoscale(void)
{
    // Loop over all parameters
    for (int i = 0; i < m_pars.size(); ++i) {
        if (m_pars[i] != NULL) {
            m_pars[i]->autoscale();
        }
    }

    // Return
    return;
}


/*==========================================================================
 =                                                                         =
 =                             Private methods                             =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Initialise class members
 ***************************************************************************/
void GModelSpectral::init_members(void)
{
    // Initialise members
    m_pars.clear();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Copy class members
 *
 * @param[in] model Spectral model.
 ***************************************************************************/
void GModelSpectral::copy_members(const GModelSpectral& model)
{
    // Copy members
    m_pars = model.m_pars;

    // Return
    return;
}


/***********************************************************************//**
 * @brief Delete class members
 ***************************************************************************/
void GModelSpectral::free_members(void)
{
    // Return
    return;
}
