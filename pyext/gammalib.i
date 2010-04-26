/***************************************************************************
 *                         gammalib - SWIG file                            *
 * ----------------------------------------------------------------------- *
 *  copyright            : (C) 2008-2010 by Jurgen Knodlseder              *
 * ----------------------------------------------------------------------- *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 * ----------------------------------------------------------------------- *
 * Usage:                                                                  *
 * swig -c++ -python -Wall -includeall gammalib.i                          *
 ***************************************************************************/
%module gammalib

/* Support */
#include "GException.i"
#include "GNodeArray.i"
/*#include "GHealpix.i"*/

/* Sky classes */
#include "GSkyDir.i"
#include "GSkymap.i"

/* Observation classes */
#include "GData.i"
#include "GResponse.i"

/* Fermi LAT classes */
#include "GLATObservation.i"
#include "GLATResponse.i"

/* FITS file class */
#include "GFits.i"
#include "GFitsHDU.i"
#include "GFitsHeader.i"
#include "GFitsHeaderCard.i"
#include "GFitsData.i"
#include "GFitsImage.i"
#include "GFitsImageDbl.i"
#include "GFitsTable.i"
#include "GFitsAsciiTable.i"
#include "GFitsBinTable.i"
#include "GFitsTableCol.i"
#include "GFitsTableLogCol.i"
#include "GFitsTableDblCol.i"
#include "GFitsTableFltCol.i"
#include "GFitsTableLngCol.i"
#include "GFitsTableShtCol.i"
#include "GFitsTableStrCol.i"

/* Model fitting */
/*#include "GFitPar.i"*/

/* Linear Algebra */
#include "GVector.i"
#include "GMatrixBase.i"
#include "GMatrix.i"
#include "GSparseMatrix.i"
#include "GSymMatrix.i"

