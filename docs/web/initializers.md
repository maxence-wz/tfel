---
title: Initialize state of behaviours
author: Thomas Helfer
date: 16/11/2021
lang: en-EN
link-citations: true
colorlinks: true
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

The `@InitializeFunction` keyword introduces a code block that can be
used to initialize internal state variables at the very beginning of the
computation. Initalize functions may have user so-called
*initialize funtion variables*.

In this version, only the `generic` interface generates functions
associated with initialize functions (See Section
@sec:tfel_4.1:system:elm:initialize_functions to see how to retrieve
the initialize functions generated by the `generic` interfaces).

Because initialize functions are called before any behaviour
integration, special care to the meaning of the variables must be taken:

- The gradients and external state variable will have their values at
  the beginning of the time step and their increments will be null.
- The thermodynamic forces will have their values at the beginning of
  the time step.
- The state variables and auxiliary state variables will have undefined
  values. The associated increments, if defined, will be null.

Concerning material properties, they have their values at the beginning
of the time step.

> **About initialisation of local variables**
>
> The code block defined by the `@InitLocalVariables` code block
> shall be called before the execution of an initialize function.

### Example of usage

The following code defines an initializer function which initializes the
elastic strain from the value of stress:

~~~~{.cxx}
@InitializeFunction ElasticStrainFromInitialStress{
  const auto K = 2 / (3 * (1 - 2 * nu));
  const auto pr = trace(sig) / 3;
  const auto s = deviator(sig);
  eel = eval((pr / K) * Stensor::Id() + s / mu);
}
~~~~

### About initialize function variables

Initialize function variables are introduced by the
`@InitializeFunctionVariable` keyword.

Initialize function variables are only defined in initialize functions,
and can't be used in the other code blocks.

Contrary most variables (internal state variables, external state
variables, etc.), initialize function variables can be defined after the
first code block. However, care must be taken to declare initialize
function variables **before** their use in an initialize function.

Note that an initialize function variable can be used in differents
initialize function.
