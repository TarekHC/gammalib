/***************************************************************************
 *          GCTAObservation.i - CTA Observation class interface            *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2010-2015 by Juergen Knoedlseder                         *
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
 * @file GCTAObservation.i
 * @brief CTA observation interface definition
 * @author Juergen Knoedlseder
 */
%{
/* Put headers and other declarations here that are needed for compilation */
#include "GCTAObservation.hpp"
%}


/***********************************************************************//**
 * @class GCTAObservation
 *
 * @brief CTA observation class
 ***************************************************************************/
class GCTAObservation : public GObservation {
public:
    // Constructors and destructors
    GCTAObservation(void);
    explicit GCTAObservation(const std::string& instrument);
    GCTAObservation(const std::string& cntcube,
                    const std::string& expcube,
                    const std::string& psfcube,
                    const std::string& bkgcube);
    GCTAObservation(const GCTAObservation& obs);
    virtual ~GCTAObservation(void);

    // Implemented pure virtual base class methods
    virtual void                clear(void);
    virtual GCTAObservation*    clone(void) const;
    virtual std::string         classname(void) const;
    virtual void                response(const GResponse& rsp);
    virtual const GCTAResponse* response(void) const;
    virtual std::string         instrument(void) const;
    virtual double              ontime(void) const;
    virtual double              livetime(void) const;
    virtual double              deadc(const GTime& time) const;
    virtual void                read(const GXmlElement& xml);
    virtual void                write(GXmlElement& xml) const;

    // Overwrite virtual base class methods
    virtual const GEvents* events(void) const;
    virtual void           events(const GEvents& events);

    // Other methods
    bool                has_response(void) const;
    bool                has_events(void) const;
    void                read(const GFits& fits);
    void                write(GFits& fits) const;
    void                load(const std::string& filename);
    void                load(const std::string& cntcube,
                             const std::string& expcube,
                             const std::string& psfcube,
                             const std::string& bkgcube);
    void                save(const std::string& filename,
                             const bool& clobber = false) const;
    void                response(const std::string& rspname,
                                 const GCaldb& caldb);
    void                response(const GCTACubeExposure& expcube,
                                 const GCTACubePsf&      psfcube,
                                 const GCTACubeBackground& bkgcube);
    void                pointing(const GCTAPointing& pointing);
    const GCTAPointing& pointing(void) const;
    GCTARoi             roi(void) const;
    GGti                gti(void) const;
    GEbounds            ebounds(void) const;
    void                obs_id(const int& id);
    const int&          obs_id(void) const;
    void                ra_obj(const double& ra);
    const double&       ra_obj(void) const;
    void                dec_obj(const double& dec);
    const double&       dec_obj(void) const;
    void                ontime(const double& ontime);
    void                livetime(const double& livetime);
    void                deadc(const double& deadc);
    void                eventfile(const std::string& filename);
    const std::string&  eventfile(void) const;
    void                dispose_events(void);
    const double&       lo_user_thres(void) const;
    const double&       hi_user_thres(void) const;
    void                n_tels(const int& tels);
    const int&          n_tels(void) const;
};


/***********************************************************************//**
 * @brief GCTAObservation class extension
 ***************************************************************************/
%extend GCTAObservation {
    GCTAObservation copy() {
        return (*self);
    }
};
