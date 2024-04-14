/* Simulation.cpp
Runs a \phi^4 simulation with metropolis and wolff algorithms.

This is based on https://inspirehep.net/literature/1386200 ,
Lattice Simulations of Nonperturbative Quantum Field Theories
by David Schaich
*/

#include <cstdio>
#include <cmath>
#include "Lattice.h"
#include <gsl/gsl_sf_log.h>  // Natural log.
#include <gsl/gsl_math.h>    // Power.