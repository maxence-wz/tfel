---
title: Release notes of the 3.4.1 version of `TFEL`, `MFront` and `MTest`
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
tabPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

# Tickets fixed

## Ticket #263: `GenericBehaviourInterface`: `rotateArrayOfTangentOperatorBlocks` is not correctly implemented

For more details, see: <https://sourceforge.net/p/tfel/tickets/263/>

## Ticket #262: Missing headers in packages generated by `automake`

For more details, see: <https://sourceforge.net/p/tfel/tickets/262/>

## Ticket #258: Missing `ndarray.cxx` source

For more details, see: <https://sourceforge.net/p/tfel/tickets/258/>

## Ticket #240: Ability to export the thermal expansion coefficients as material properties with the ‘cyrano’ interface

When the thermal expansion is handled by a behaviour, the thermal
expansion coefficients are now automatically exported as material
properties when compiling the behaviour behaviour with the `Cast3M` and
`Cyrano` interfaces.

The names of the functions associated with those thermal expansion
coefficients can be retrieved by:

- the `getUMATLinearThermalExpansionCoefficientsEntryPoints` method of
  the `ExternalLibraryManager` class.
- the `linear_thermal_expansion_coefficients_epts` data member of the
  `ExternalBehaviourData` class.

For more details, see: <https://sourceforge.net/p/tfel/tickets/240/>