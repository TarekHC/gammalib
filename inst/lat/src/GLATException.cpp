/***************************************************************************
 *                 GLATException.cpp  - LAT exception handler              *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2010-2011 by Jurgen Knodlseder                           *
 * ----------------------------------------------------------------------- *
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
 * @file GLATException.cpp
 * @brief LAT exception handler interface implementation.
 * @author J. Knodlseder
 */

/* __ Includes ___________________________________________________________ */
#include "GLATException.hpp"
#include "GTools.hpp"


/***********************************************************************//**
 * @brief Exception message.
 ***************************************************************************/
const char* GLATExceptionHandler::what() const throw()
{
    // Set error message
    std::string message = "*** ERROR in " + m_origin + ": " + m_message;

    // Return message as C character array
    return gammalib::tochar(message);
}


/***********************************************************************//**
 * @brief Error while opening file.
 *
 * @param[in] origin Name of method that has thrown the exception.
 * @param[in] filename Name of file that could not be opened.
 * @param[in] message Optional error message.
 ***************************************************************************/
GLATException::file_open_error::file_open_error(std::string origin,
                                                std::string filename,
                                                std::string message)
{
    m_origin  = origin;
    m_message = "Unable to open file '"+filename+"'. "+message;
    return;
}


/***********************************************************************//**
 * @brief Member not set.
 *
 * @param[in] origin Name of method that has thrown the exception.
 * @param[in] message Error message.
 ***************************************************************************/
GLATException::no_member::no_member(const std::string& origin,
                                    const std::string& message)
{
    // Set origin and message
    m_origin  = origin;
    m_message = message;

    // Return
    return;
}


/***********************************************************************//**
 * @brief No sky pixels found.
 *
 * @param[in] origin Name of method that has thrown the exception.
 * @param[in] message Optional error message.
 ***************************************************************************/
GLATException::no_sky::no_sky(std::string origin, std::string message)
{
    m_origin  = origin;
    m_message = "No sky pixels have been found. "+message;
    return;
}


/***********************************************************************//**
 * @brief No energy boundary information found.
 *
 * @param[in] origin Name of method that has thrown the exception.
 * @param[in] message Optional error message.
 ***************************************************************************/
GLATException::no_ebds::no_ebds(std::string origin, std::string message)
{
    m_origin  = origin;
    m_message = "No energy boundaries have been found. "+message;
    return;
}


/***********************************************************************//**
 * @brief No Good Time Intervals found.
 *
 * @param[in] origin Name of method that has thrown the exception.
 * @param[in] message Optional error message.
 ***************************************************************************/
GLATException::no_gti::no_gti(std::string origin, std::string message)
{
    m_origin  = origin;
    m_message = "No Good Time Intervals (GTIs) have been found. "+message;
    return;
}


/***********************************************************************//**
 * @brief No Livetime Cube found.
 *
 * @param[in] origin Name of method that has thrown the exception.
 * @param[in] message Optional error message.
 ***************************************************************************/
GLATException::no_ltcube::no_ltcube(std::string origin, std::string message)
{
    m_origin  = origin;
    m_message = "No Livetime Cube have been found. "+message;
    return;
}


/***********************************************************************//**
 * @brief No energies set
 *
 * @param[in] origin Name of method that has thrown the exception.
 * @param[in] message Optional error message.
 ***************************************************************************/
GLATException::no_energies::no_energies(std::string origin, std::string message)
{
    // Set origin
    m_origin = origin;

    // Set message
    m_message = "Energy vector has not been setup."
                " Cannot access event information.";
    if (message.length() > 0)
        m_message += " "+message;

    // Return
    return;
}


/***********************************************************************//**
 * @brief No sky directions set
 *
 * @param[in] origin Name of method that has thrown the exception.
 * @param[in] message Optional error message.
 ***************************************************************************/
GLATException::no_dirs::no_dirs(std::string origin, std::string message)
{
    // Set origin
    m_origin = origin;

    // Set message
    m_message = "Sky direction vector has not been setup."
                " Cannot access event information.";
    if (message.length() > 0)
        m_message += " "+message;

    // Return
    return;
}


/***********************************************************************//**
 * @brief ROI is not a LAT ROI
 *
 * @param[in] origin Name of method that has thrown the exception.
 * @param[in] message Optional error message.
 ***************************************************************************/
GLATException::bad_roi_type::bad_roi_type(std::string origin, std::string message)
{
    // Set origin
    m_origin = origin;

    // Set message
    m_message = "Specified ROI is not of type GLATRoi.";
    if (message.length() > 0)
        m_message += " "+message;

    // Return
    return;
}


/***********************************************************************//**
 * @brief Instrument direction is not a CTA instrument direction
 *
 * @param[in] origin Name of method that has thrown the exception.
 * @param[in] message Optional error message.
 ***************************************************************************/
GLATException::bad_instdir_type::bad_instdir_type(std::string origin,
                                                  std::string message)
{
    // Set origin
    m_origin = origin;

    // Set message
    m_message = "Instrument direction is not of type GLATInstDir.";
    if (message.length() > 0)
        m_message += " "+message;

    // Return
    return;
}


/***********************************************************************//**
 * @brief Invalid response found
 *
 * @param[in] origin Name of method that has thrown the exception.
 * @param[in] message Optional error message.
 ***************************************************************************/
GLATException::invalid_response::invalid_response(std::string origin,
                                                  std::string message)
{
    m_origin  = origin;
    m_message = "Invalid response encountered. " + message;
    return;
}


/***********************************************************************//**
 * @brief Incompatible source map
 *
 * @param[in] origin Name of method that has thrown the exception.
 * @param[in] name Map name.
 * @param[in] message Optional error message.
 ***************************************************************************/
GLATException::wcs_incompatible::wcs_incompatible(std::string origin,
                                                  std::string name,
                                                  std::string message)
{
    m_origin  = origin;
    m_message = "Source map \""+name+"\" incompatible with counts map. " +
                message;
    return;
}


/***********************************************************************//**
 * @brief Diffuse model not found
 *
 * @param[in] origin Name of method that has thrown the exception.
 * @param[in] name Diffuse model.
 * @param[in] message Optional error message.
 ***************************************************************************/
GLATException::diffuse_not_found::diffuse_not_found(std::string origin,
                                                    std::string name,
                                                    std::string message)
{
    m_origin  = origin;
    m_message = "Diffuse model \""+name+"\" not found. " +
                message;
    return;
}


/***********************************************************************//**
 * @brief Inconsistent response table
 *
 * @param[in] origin Name of method that has thrown the exception.
 * @param[in] size Size.
 * @param[in] expect Expected size.
 * @param[in] message Optional error message.
 ***************************************************************************/
GLATException::inconsistent_response::inconsistent_response(std::string origin,
                                                            int         size,
                                                            int         expect,
                                                            std::string message)
{
    m_origin  = origin;
    m_message = "Inconsistent response table found. Expected "+gammalib::str(expect)+
                " elements, found "+gammalib::str(size)+". " + message;
    return;
}


/***********************************************************************//**
 * @brief Response is not a LAT response
 *
 * @param[in] origin Name of method that has thrown the exception.
 * @param[in] message Optional error message.
 ***************************************************************************/
GLATException::bad_response_type::bad_response_type(std::string origin,
                                                    std::string message)
{
    // Set origin
    m_origin = origin;

    // Set message
    m_message = "Response is not of type GLATResponse.";
    if (message.length() > 0)
        m_message += " "+message;

    // Return
    return;
}
