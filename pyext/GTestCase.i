/***************************************************************************
 *                     GTestCase.i - Test case class                       *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2012-2015 Jean-Baptiste Cayrou                           *
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
 * @file GTestCase.i
 * @brief Test case class interface definition
 * @author Jean-Baptiste Cayrou
 */
%{
/* Put headers and other declarations here that are needed for compilation */
#include "GTestCase.hpp"
%}


/***********************************************************************//**
 * @class GTestCase
 *
 * @brief Test case Python interface definition
 ***************************************************************************/
class GTestCase : public GBase {
public:
    // Public enumerators
    enum ErrorKind {
        FAIL_TEST,
        ERROR_TEST
    };

    // Constructors and destructors
    GTestCase(void);
    GTestCase(const ErrorKind& kind, const std::string& name = "");
    GTestCase(const GTestCase& test);
    virtual ~GTestCase(void);

    // Methods
    void               clear(void);
    GTestCase*         clone(void) const;
    std::string        classname(void) const;
    const std::string& name(void) const;
    void               name(const std::string& name);
    const std::string& message(void) const;
    void               message( const std::string& message);
    const std::string& type(void) const;
    void               type( const std::string& type);
    const ErrorKind&   kind(void) const;
    void               kind(const ErrorKind& kind);
    const bool&        has_passed(void) const;
    void               has_passed(const bool& has_passed);
    const double&      duration(void) const;
    void               duration(const double& duration);
};


/***********************************************************************//**
 * @brief GTestCase class extension
 ***************************************************************************/
%extend GTestCase {
    GTestCase copy() {
        return (*self);
    }
};
