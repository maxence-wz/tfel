/*!
 * \file  mfront/include/MFront/MaterialKnowledgeDescription.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 19 mars 2014
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_MATERIALKNOWLEDGEDESCRIPTION_HXX
#define LIB_MFRONT_MATERIALKNOWLEDGEDESCRIPTION_HXX

#include <map>
#include <string>
#include <vector>
#include <string_view>
#include "TFEL/Material/OutOfBoundsPolicy.hxx"
#include "MFront/MFrontConfig.hxx"
#include "MFront/MaterialKnowledgeAttribute.hxx"

namespace mfront {

  /*!
   * \brief base class for the description of material knowledge
   */
  struct MFRONT_VISIBILITY_EXPORT MaterialKnowledgeDescription {
    //! \brief standard option and attribute name
    static const char* const defaultOutOfBoundsPolicy;
    //! \brief attribute name
    static const char* const parametersAsStaticVariables;
    //! \brief attribute name
    static const char* const initializeParametersFromFile;
    //! \brief attribute name
    static const char* const buildIdentifier;
    /*!
     * \brief throw an exception saying that no attribute with the given name
     * exists
     */
    [[noreturn]] static void throwUndefinedAttribute(const std::string_view);
    /*!
     * \brief insert a new attribute
     * \param[in] n : name
     * \param[in] a : attribute
     * \param[in] b : don't throw if the the
     *                attribute already exists.
     *                The attribute is left unchanged.
     *                However the type of the attribute is checked.
     */
    void setAttribute(const std::string&,
                      const MaterialKnowledgeAttribute&,
                      const bool);
    /*!
     * \brief update an existing new attribute
     * \param[in] n: name
     * \param[in] a: attribute
     */
    void updateAttribute(const std::string_view,
                         const MaterialKnowledgeAttribute&);
    /*!
     * \return true if an attribute with the given name as been registred
     * \param[in] n : name
     */
    bool hasAttribute(const std::string_view) const;
    /*!
     * \return the attribute with the given name
     * \param[in] n : name
     */
    template <typename T>
    std::enable_if_t<isMaterialKnowledgeAttributeType<T>(), T&> getAttribute(
        const std::string_view);
    /*!
     * \return the attribute with the given name
     * \param[in] n : name
     */
    template <typename T>
    std::enable_if_t<isMaterialKnowledgeAttributeType<T>(), const T&>
    getAttribute(const std::string_view) const;
    /*!
     * \return the attribute with the given name
     * \param[in] n: name
     */
    template <typename T>
    std::enable_if_t<isMaterialKnowledgeAttributeType<T>(), T> getAttribute(
        const std::string_view, const T&) const;
    /*!
     * \return all the attribute registred
     * \param[in] n : name
     */
    const std::map<std::string, MaterialKnowledgeAttribute, std::less<>>&
    getAttributes() const;

   protected:
    //! \brief behaviour attributes
    std::map<std::string, MaterialKnowledgeAttribute, std::less<>> attributes;
  };  // end of struct MaterialKnowledgeDescription

  /*!
   * \brief set the default out of bounds policy
   * \param[out] d: material knowledge description
   * \param[in] policy: out of bounds policy
   */
  MFRONT_VISIBILITY_EXPORT tfel::material::OutOfBoundsPolicy
  setDefaultOutOfBoundsPolicy(const MaterialKnowledgeDescription&,
                              const std::string&);
  /*!
   * \brief this function returns the value of the
   * `MaterialKnowledgeDescription::defaultOutOfBoundsPolicy`
   * attribute if it is defined, `tfel::material::None` otherwise.
   * \param[in] d: material knowledge description
   */
  MFRONT_VISIBILITY_EXPORT tfel::material::OutOfBoundsPolicy
  getDefaultOutOfBoundsPolicy(const MaterialKnowledgeDescription&);
  /*!
   * \brief this function returns the value of the
   * `MaterialKnowledgeDescription::defaultOutOfBoundsPolicy`
   * attribute if it is defined, `None` otherwise.
   * \param[in] d: material knowledge description
   */
  MFRONT_VISIBILITY_EXPORT std::string getDefaultOutOfBoundsPolicyAsString(
      const MaterialKnowledgeDescription&);
  /*!
   * \brief this function returns the value of the
   * `MaterialKnowledgeDescription::defaultOutOfBoundsPolicy`
   * attribute if it is defined, `NONE` otherwise.
   * \param[in] d: material knowledge description
   */
  MFRONT_VISIBILITY_EXPORT std::string
  getDefaultOutOfBoundsPolicyAsUpperCaseString(
      const MaterialKnowledgeDescription&);
  /*!
   * \brief this function returns the value of the
   * `MaterialKnowledgeDescription::parametersAsStaticVariables`
   * attribute if it is defined, `false` otherwise.
   * \return if the parameters are treated as static variables.
   * \param[in] d: material knowledge description
   */
  MFRONT_VISIBILITY_EXPORT bool areParametersTreatedAsStaticVariables(
      const MaterialKnowledgeDescription&);
  /*!
   * \brief this function returns the value of the
   * `MaterialKnowledgeDescription::initializeParametersFromFile`
   * attribute if it is defined, `true` otherwise.
   * \return if the initialization of parameters from a file shall be allowed.
   * \param[in] d: material knowledge description
   */
  MFRONT_VISIBILITY_EXPORT bool allowsParametersInitializationFromFile(
      const MaterialKnowledgeDescription&);

}  // end of namespace mfront

#include "MFront/MaterialKnowledgeDescription.ixx"

#endif /* LIB_MFRONT_MATERIALKNOWLEDGEDESCRIPTION_HXX */
