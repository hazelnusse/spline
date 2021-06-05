# spline

spline is a portable and efficient C++ library for creating and querying
splines in n-dimensional vector spaces.

## Features

* Memory allocation must be done by callers.
* Core algorithms don't depend on C++ standard library, aiding portability to
  constrained environments.
* Points can be any C++ type which supports addition with another point and
  multiplication by a real number.
* Project structure is derived from
  [ModernCppStarter](https://github.com/TheLartians/ModernCppStarter) and
  imposes minimal policy on consumers of library.

## Project status

The following features are implemented or awaiting implementation:

* [x] de Casteljau's algorithm with subdivision
* [ ] de Casteljau's algorithm without subdivision
* [ ] Generic spline class (interpolating or smoothing)
* [ ] 2d and 3d linear, quadratic, and cubic splines
* [ ] Bernstein polynomial operations:
  * [ ] Derivative
  * [ ] Integral
  * [ ] Degree elevation
  * [ ] Addition
  * [ ] Subtraction
  * [ ] Transformation of domain
  * [ ] Multiplication
  * [ ] Division
  * [ ] Root finding on [0, 1]
* [ ] C API wrapper around C++ implementation
* [ ] Python language bindings ([pybind11](https://github.com/pybind/pybind11))
