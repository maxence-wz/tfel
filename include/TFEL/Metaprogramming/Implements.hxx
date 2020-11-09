/*!
 * \file   include/TFEL/Metaprogramming/Implements.hxx
 * \brief  This file describes the Implements metafunction  
 * \author Thomas Helfer
 * \date   28 Aug 2006
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_TFEL_META_IMPLEMENTS_HXX
#define LIB_TFEL_META_IMPLEMENTS_HXX 

#include<type_traits>

#include"TFEL/Config/TFELConfig.hxx"

namespace tfel{

  namespace meta{
    
    /*!
     * \class Implements
     *
     * \param T, class to be tested.
     * \param Concept, concept template class. 
     * \return bool.
     *
     * \see concepts_sec for more details about how concepts are modelled in TFEL.
     *
     * \brief the Implements metafunction returns true if the 
     * class T is a subclass of Concept<T>.
     * \author Thomas Helfer
     * \date   28 Aug 2006
     */
    template<typename T, template<typename> class Concept>
    struct TFEL_VISIBILITY_LOCAL Implements
    {
      //!
      using decay_type = typename std::decay<T>::type;
      /*!
       * The result of the metafunction.
       */
      static constexpr bool cond = std::is_base_of<Concept<decay_type>,
						   decay_type>::value;
    };

    template<typename T, template<typename> class Concept>
    constexpr bool implements(){
      return Implements<T,Concept>::cond;
    }
    
  } // end of namespace meta

} // end of namespace tfel

#endif /* LIB_TFEL_META_IMPLEMENTS_HXX */

