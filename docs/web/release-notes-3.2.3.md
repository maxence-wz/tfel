---
title: Release notes of the 3.2.3 version of `TFEL`, `MFront` and `MTest`
author: Thomas Helfer
date: 2020
lang: en-EN
numbersections: true
documentclass: article
from: markdown+tex_math_single_backslash
geometry:
  - margin=2cm
papersize: a4
link-citations: true
colorlinks: true
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

Version `3.2.3` is mainly a bug fix version of the `3.2` series.

This version was released along with Version `3.1.5` and inherits from
all the fixes of this version.

# Noticeable fixed issues that may affect the results obtained with previous versions

Ticket #256 reported that the scalar product of two unsymmetric tensors
was not properly computed.

This may affect single crystal finite strain computations to a limited
extent, as the Mandel stress tensor is almost symmetric.

# Issues solved

## Ticket #249: Implementation of the Norton-Hoff in the StandardElastoViscoPlasticity brick may lead to negative equivalent plastic strain increment

The implementation of the Norton-Hoff flow rule in the
`StandardElastoViscoPlasticity` brick may in some exceptional cases lead
to negative equivalent plastic strain increment. This was checked by the
brick and led to a divergence of the Newton algorithm, so this flaw
could not induce wrong results.

For more details, see: <https://sourceforge.net/p/tfel/tickets/249/>

## Ticket #242: The factor in front of the hyperbolic sine is discarded when using the `HyperbolicSine` inelastic flow

The optional factor in in front of the hyperbolic sine was not generated when using the `HyperbolicSine` inelastic flow.

For more details, see: <https://sourceforge.net/p/tfel/tickets/242/>

## Ticket #228: Many stress criteria of the StandardElastoViscoplasticity brick fail when compiling in C++-17

For more details, see: <https://sourceforge.net/p/tfel/tickets/228/>

## Ticket #222: Handling stored and dissated energies is broken in `MTest` for `Abaqus/Standard` behaviours

There were a bug in `MTest` in the `Abaqus/Standard` behaviours which
didn't reinitialize the values of the stored and dissipated energies at
the end of the time step to their values at the beginning of the time
step before the behaviour integration.

Thus, the dissipated and stored energies, when computed incrementally,
are the sums of their increments at each iterations.

For more details, see: <https://sourceforge.net/p/tfel/tickets/222/>

## Ticket #203: Cyrano` interface: compilation process hangs when treating behaviours based on the Hencky strain measure in `AxisymmetricalGeneralizedPlaneStress`

The trouble came from an infinite loop when looking for the external
state variable holding the axial stress.

For more details, see: <https://sourceforge.net/p/tfel/tickets/203/>
