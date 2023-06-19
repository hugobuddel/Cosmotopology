# CMB Topo

CMBTopo is a tool to create, to create realizations of the Cosmic Microwave Background for flat universes with a non-trivial topology.

## Cosmotopology

Cosmotopology is the field of studying the topology of the cosmos.

### Background

A geometrically flat universe can still be topologically closed by 'folding back' into itself like a 3-dimensional flat torus.

An effect of such a non-trivial topology is that the universe will appear to repeat itself, just like the old pacman games where pacman reappears on the other side if he leaves the screen.

This apparent repetition should be visible in the Cosmic Microwave Background radiation if the physical universe is smaller than the observable universe. 

CMBTopo simulates the Cosmic Microwave Background for different topologies and offers interactive 3D visualizations for exploration.

### Context

This code is an implementation of the algorithms described by [Riazuelo, Uzan, Lehoucq and Weeks (2004)](http://adsabs.harvard.edu/abs/2004PhRvD..69j3514R). The same notation and terminology is used as in their paper.

The results of this code is used in the [master thesis of Hugo Buddelmeijer](https://www.astro.rug.nl/~buddel/go/thesis.pdf).

### Algorithm

The primordial density field that can be seen in the Cosmic Microwave Background follows a Gaussian power spectrum. Only those modes that 'fit' the topology are allowed in a multi-connected universe.

CMBTopo simulates the Cosmic Microwave Background in three steps:

1. The eigenvalues of the Cartesian eigenmodes of the primordial density field are calculated.
2. These are converted to eigenvalues in spherical coordinates.
3. A CMB realization is computed from these spherical coordinates.


## Installation

### Requirements

CMBTopo requires the following packages:

 * SDL (Simple DirectMedia Layer)
 * GSL (GNU Scientific Library)
 * CImg (CImg Library)

### Compilation

Compile CMBTopo simply with

    make

## Usage

### Invocation

The default is a toroidal universe:

    ./cmbtopo

Differentn topologies can be specified using a number as command line argument, e.g.:

    ./cmbtopo 3

All flat orientable topologies are supported:

 1. toroidal space
 2. half-turn space
 3. quarter-turn space
 4. third-turn space
 5. chimney space
 6. slab space
 7. sixth-turn space
 8. Hantzsche-Wendt space
 
### Usage

CMBTopo shows an interactive visualization of the Cosmic Microwave Background realization. The following keyboard commands are available:

User interface:

 * `Esc`, `space`, `q` quit
 * `F1` toggle full screen
 * `y` toggle between 3D display and Aitoff projection
 * `z` print values

Move camera (3D mode only):

 * `q` rotate along x-axis
 * `a` counter rotate along x-axis
 * `w` rotate along y-axis
 * `s` counter rotate along y-axis
 * `e` rotate along z-axis
 * `d` counter rotate along z-axis
 * `r` increase distance
 * `f` decrease distance

Show CMB copies:

 * `m` toggle showing CMB copies along x-axis
 * `u` increase x-axis offset
 * `j` decrease x-axis offset
 * `,` toggle showing CMB copies along y-axis
 * `i` increase y-axis offset
 * `k` decrease y-axis offset
 * `.` toggle showing CMB copies along z-axis
 * `o` increase z-axis offset
 * `l` decrease z-axis offset

By default only the CMB copies along the z-axis are shown. The copies should connect to each other almost seamlessly (limited by resolution). Changing the offsets will break this connection.


## Code

The code is structured in different files which mostly contain a single class with the same name.

User interface:
 
 * `cmbtopo` provides the main function.
 * `Cmb3D` provides an OpenGL visualization of the CMB.

Data and algorithm:

 * `Ek` holds the Cartesian eigenvalues.
 * `Alm` holds the spherical eigenvalues.
 * `Cmbdata` holds the CMB realization.
 * `Cmb` performs the algorithm.

Helpers:

 * `Coordinate` converts between Cartesian and spherical coordinates.
 * `Counter` provides a progress bar.
 * `Random` provides a random number generator.
 * `NegVector` (header only) provides the base class for `Ek` and `Alm`.

 
 

