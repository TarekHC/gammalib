/***************************************************************************
 *         GCTAModelRadialRegistry.i - Radial model registry class         *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2011-2015 by Juergen Knoedlseder                         *
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
 * @file GCTAModelRadialRegistry.i
 * @brief Radial model registry class definition
 * @author Juergen Knoedlseder
 */
%{
/* Put headers and other declarations here that are needed for compilation */
#include "GCTAModelRadialRegistry.hpp"
#include "GTools.hpp"
%}


/***********************************************************************//**
 * @class GCTAModelRadialRegistry
 *
 * @brief Interface definition for the CTA radial model registry class
 ***************************************************************************/
class GCTAModelRadialRegistry : public GRegistry {

public:
    // Constructors and destructors
    GCTAModelRadialRegistry(void);
    GCTAModelRadialRegistry(const GCTAModelRadial* model);
    GCTAModelRadialRegistry(const GCTAModelRadialRegistry& registry);
    virtual ~GCTAModelRadialRegistry(void);

    // Methods
    std::string      classname(void) const;
    int              size(void) const;
    GCTAModelRadial* alloc(const std::string& name) const;
    std::string      name(const int& index) const;
};


/***********************************************************************//**
 * @brief GCTAModelRadialRegistry class extension
 ***************************************************************************/
%extend GCTAModelRadialRegistry {
};
