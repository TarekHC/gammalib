/***************************************************************************
 *              GEventBin.cpp  -  Event bin abstract base class            *
 * ----------------------------------------------------------------------- *
 *  copyright            : (C) 2009 by Jurgen Knodlseder                   *
 * ----------------------------------------------------------------------- *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 * ----------------------------------------------------------------------- *
 ***************************************************************************/
/**
 * @file GEventBin.cpp
 * @brief GEventBin abstract base class implementation.
 * @author J. Knodlseder
 */

/* __ Includes ___________________________________________________________ */
#include <iostream>
#include <iomanip.h>
#include "GException.hpp"
#include "GEventBin.hpp"

/* __ Method name definitions ____________________________________________ */

/* __ Macros _____________________________________________________________ */

/* __ Coding definitions _________________________________________________ */

/* __ Debug definitions __________________________________________________ */


/*==========================================================================
 =                                                                         =
 =                      GEventBin constructors/destructors                 =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Constructor
 ***************************************************************************/
GEventBin::GEventBin() : GEvent()
{
    // Initialise class members for clean destruction
    init_members();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Copy constructor
 *
 * @param[in] atom Event bin from which the instance should be built.
 ***************************************************************************/
GEventBin::GEventBin(const GEventBin& bin) : GEvent(bin)
{
    // Initialise class members for clean destruction
    init_members();

    // Copy members
    copy_members(bin);

    // Return
    return;
}


/***********************************************************************//**
 * @brief Destructor
 ***************************************************************************/
GEventBin::~GEventBin()
{
    // Free members
    free_members();

    // Return
    return;
}


/*==========================================================================
 =                                                                         =
 =                            GEventBin operators                          =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Assignment operator
 *
 * @param[in] bin Event bin to be assigned.
 ***************************************************************************/
GEventBin& GEventBin::operator= (const GEventBin& bin)
{
    // Execute only if object is not identical
    if (this != &bin) {

        // Copy base class members
        this->GEvent::operator=(bin);

        // Free members
        free_members();

        // Initialise private members for clean destruction
        init_members();

        // Copy members
        copy_members(bin);

    } // endif: object was not identical

    // Return this object
    return *this;
}


/*==========================================================================
 =                                                                         =
 =                         GEventBin public methods                        =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Converts event into string
 ***************************************************************************/
std::string GEventBin::string(void) const
{
    // Allocate buffer for event
    char buffer[80];
    
    // Get Right Ascension and Declination
//    GSkyDir dir = m_dir;
    
    // Put event in buffer
//    sprintf(buffer, "Time=%14.3f Energy=%12.3f MeV (RA,Dec)=(%7.3f,%7.3f)",
//            m_time, m_energy, dir.ra_deg(), dir.dec_deg());
    sprintf(buffer, ".");
    
    // Return as string
    return std::string(buffer);
}


/*==========================================================================
 =                                                                         =
 =                         GEventBin private methods                       =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Initialise class members
 ***************************************************************************/
void GEventBin::init_members(void)
{
    // Initialise attributes

    // Return
    return;
}


/***********************************************************************//**
 * @brief Copy class members
 *
 * @param[in] bin GEventBin members which should be copied.
 ***************************************************************************/
void GEventBin::copy_members(const GEventBin& bin)
{
    // Copy attributes

    // Return
    return;
}


/***********************************************************************//**
 * @brief Delete class members
 ***************************************************************************/
void GEventBin::free_members(void)
{
    // Return
    return;
}


/*==========================================================================
 =                                                                         =
 =                             GEventBin friends                           =
 =                                                                         =
 ==========================================================================*/

/*==========================================================================
 =                                                                         =
 =                    Other functions used by GEventBin                    =
 =                                                                         =
 ==========================================================================*/
