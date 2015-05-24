#! /usr/bin/env python
# ==========================================================================
# This script simulates the PSF distribution.
#
# If matplotlib is installed, the PSF will be displayed on the screen.
#
# --------------------------------------------------------------------------
#
# Copyright (C) 2013-2014 Juergen Knoedlseder
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# ==========================================================================
from gammalib import *
from math import *


# ============ #
# Simulate PSF #
# ============ #
def sim_psf(response, energy, r_max=0.8, rbins=1000, nmc=1000000):
    """
    Simulate PSF and show results using matplotlib (if available).
    """
    # Check if matplotlib is available
    try:
        import matplotlib.pyplot as plt
        has_matplotlib = True
    except ImportError:
        print("Matplotlib is not (correctly) installed on your system.")
        has_matplotlib = False

    # Continue only if matplotlib is available
    if has_matplotlib:

        # Create figure
        plt.figure(1)
        plt.title("MC simulated PSF ("+str(energy)+" TeV)")

        # Set log10(energy)
        logE = log10(energy)

        # Create delta axis
        delta = []
        dr    = r_max/rbins
        for i in range(rbins):
            delta.append((i+0.5)*dr)

        # Reset histogram
        counts = [0.0 for i in range(rbins)]

        # Allocate random number generator
        ran = GRan()

        # Simulate offsets
        for i in range(nmc):
            offset         = rsp.psf().mc(ran, logE) * 180.0/pi
            #print(offset)
            index          = int(offset/dr)
            if (index < rbins):
                counts[index] += 1.0

        # Create error bars
        error = [sqrt(c) for c in counts]

        # Get expected PSF
        sum = 0.0
        psf = []
        for i in range(rbins):
            r     = delta[i]*pi/180.0
            if r <= rsp.psf().delta_max(logE):
                value = rsp.psf()(r, logE) * 2.0*pi * sin(r) * dr * pi/180.0 * nmc
            else:
                value = 0.0
            sum += value
            psf.append(value)
        print(sum)

        # Plot simulated data
        plt.plot(delta, counts, 'ro')
        plt.errorbar(delta, counts, error, ecolor='r')

        # Plot PSF
        plt.plot(delta, psf, 'b-')
        #plt.semilogy(delta, psf, 'b-')
        #plt.xlim([0.4,0.45])
        #plt.ylim([0,1.0])
        
        # Set axes
        plt.xlabel("Offset angle (degrees)")
        plt.ylabel("Number of counts")

        # Notify
        print("PLEASE CLOSE WINDOW TO CONTINUE ...")

        # Show plot
        plt.show()

    # Return
    return


#==========================#
# Main routine entry point #
#==========================#
if __name__ == '__main__':
    """
    Simulate PSF.
    """
    # Dump header
    print("")
    print("****************")
    print("* Simulate PSF *")
    print("****************")

    # Load response
    #rsp = GCTAResponseIrf("irf_file.fits",
    #                      GCaldb("../caldb/data/cta/e/bcf/IFAE20120510_50h_King"))
    rsp = GCTAResponseIrf("irf_file.fits",
                          GCaldb("../caldb/data/cta/e/bcf/IFAE20120510_50h"))
    #rsp = GCTAResponseIrf("cta_dummy_irf",
    #                      GCaldb("../caldb"))

    # Simulate PSF
    sim_psf(rsp, 1.0)
    #sim_psf(rsp, 1.0, r_max=0.2)
