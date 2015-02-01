/***************************************************************************
 *                 GModelRegistry.i - Model registry class                 *
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
 * @file GModelRegistry.i
 * @brief Model registry class definition
 * @author Juergen Knoedlseder
 */
%{
/* Put headers and other declarations here that are needed for compilation */
#include "GModelRegistry.hpp"
#include "GTools.hpp"
%}


/***********************************************************************//**
 * @class GModelRegistry
 *
 * @brief Interface definition for the model registry class
 ***************************************************************************/
class GModelRegistry : public GRegistry {

public:
    // Constructors and destructors
    GModelRegistry(void);
    GModelRegistry(const GModel* model);
    GModelRegistry(const GModelRegistry& registry);
    virtual ~GModelRegistry(void);

    // Methods
    std::string classname(void) const;
    int         size(void) const;
    GModel*     alloc(const std::string& name) const;
    std::string name(const int& index) const;
};


/***********************************************************************//**
 * @brief GModelRegistry class extension
 ***************************************************************************/
%extend GModelRegistry {
};
