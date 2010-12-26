/***************************************************************************
 *    GObservation.i  -  Observation abstract base class SWIG interface    *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2008-2010 by Jurgen Knodlseder                           *
 * ----------------------------------------------------------------------- *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/**
 * @file GObservation.i
 * @brief GObservation class SWIG file.
 * @author J. Knodlseder
 */
%{
/* Put headers and other declarations here that are needed for compilation */
#include "GObservation.hpp"
%}


/***********************************************************************//**
 * @class GObservation
 *
 * @brief Abstract interface for the observation classes.
 ***************************************************************************/
class GObservation {

    // Friend classes
    friend class GObservations;

public:
    // Constructors and destructors
    GObservation(void);
    GObservation(const GObservation& obs);
    virtual ~GObservation(void);

    // Pure virtual methods
    virtual void          clear(void) = 0;
    virtual GObservation* clone(void) const = 0;
    virtual GResponse*    response(void) const = 0;
    virtual GPointing*    pointing(const GTime& time) const = 0;
    virtual std::string   instrument(void) const = 0;

    // Virtual methods
    virtual double model(const GModels& models, const GEvent& event,
                         GVector* gradient = NULL) const;
    virtual double npred(const GModels& models, GVector* gradient = NULL) const;

    // Implemented methods
    void        obsname(const std::string& obsname) { m_obsname=obsname; return; }
    void        roi(const GRoi& roi) { m_roi=roi.clone(); return; }
    void        ebounds(const GEbounds& ebounds) { m_ebounds=ebounds; return; }
    void        gti(const GGti& gti) { m_gti=gti; return; }
    void        statistics(const std::string& statistics);
    std::string obsname(void) const { return m_obsname; }
    GTime       tstart(void) const { return m_gti.tstart(); }
    GTime       tstop(void) const { return  m_gti.tstop(); }
    GEnergy     emin(void) const { return m_ebounds.emin(); }
    GEnergy     emax(void) const { return m_ebounds.emax(); }
    GRoi*       roi(void) { return m_roi; }
    GEbounds*   ebounds(void) { return &m_ebounds; }
    GGti*       gti(void) { return &m_gti; }
    GEvents*    events(void) const { return m_events; }
    std::string statistics(void) const { return m_statistics; }
};


/***********************************************************************//**
 * @brief GObservation class extension
 ***************************************************************************/
%extend GObservation {
    char *__str__() {
        static std::string result = self->print();
        return ((char*)result.c_str());
    }
};
