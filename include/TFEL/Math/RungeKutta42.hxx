/*!
 * \file   include/TFEL/Math/RungeKutta42.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   04 oct 2007
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_RUNGEKUTTA42_HXX
#define LIB_TFEL_MATH_RUNGEKUTTA42_HXX

#include <type_traits>
#include "TFEL/Math/tvector.hxx"

namespace tfel::math {

  template <unsigned short N, typename Child, typename Scalar = double>
  struct RungeKutta42 {
    typedef typename std::conditional<N == 1, Scalar, tvector<N, Scalar>>::type
        VariableType;
    void setInitialValue(const VariableType&);
    void setInitialTime(const Scalar);
    void setFinalTime(const Scalar);
    void setInitialTimeIncrement(const Scalar);
    void setCriterium(const Scalar);
    void iterate();
    const VariableType& getValue() const;
    const Scalar& getTimeIncrement() const;

   private:
    struct AbsoluteSommation;
    VariableType y;
    Scalar ti;
    Scalar tf;
    Scalar dt;
    Scalar epsilon;
  };  // end of struct RungeKutta42

}  // end of namespace tfel::math

#include "TFEL/Math/RungeKutta/RungeKutta42.ixx"

#endif /* LIB_TFEL_MATH_RUNGEKUTTA42_HXX */
