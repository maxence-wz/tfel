configure_file("${PROJECT_SOURCE_DIR}/mfront/tests/behaviours/references/PolyCrystalsAngles.txt"
    PolyCrystalsAngles.txt @ONLY)

macro(test_crossed2delta test_arg)
  if(CMAKE_CONFIGURATION_TYPES)
    foreach(conf ${CMAKE_CONFIGURATION_TYPES})
      set(file "crossed2delta${test_arg}-${conf}.mtest")
      get_property(MFrontCastemBehavioursBuildPath TARGET MFrontCastemBehaviours PROPERTY LOCATION_${conf})
      get_property(MFrontMaterialPropertiesCastemBuildPath TARGET MFrontMaterialProperties-castem PROPERTY LOCATION_${conf})
      configure_file(crossed2delta${test_arg}.mtest.in
	${file} @ONLY)
      add_test(NAME crossed2delta${test_arg}_${conf}_mtest
	CONFIGURATIONS ${conf}
	COMMAND mtest --verbose=level0 --xml-output=true --result-file-output=false ${file})
      if((CMAKE_HOST_WIN32) AND (NOT MSYS))
        set_property(TEST crossed2delta${test_arg}_${conf}_mtest
          PROPERTY DEPENDS "MFrontCastemBehaviours MFrontMaterialProperties-castem mtest"
	  PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELNUMODIS>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELConfig>\;$<TARGET_FILE_DIR:TFELUnicodeSupport>\;$ENV{PATH}")
      elseif((CMAKE_HOST_WIN32) AND (NOT MSYS))
        set_property(TEST crossed2delta${test_arg}_${conf}_mtest
          PROPERTY DEPENDS "MFrontCastemBehaviours MFrontMaterialProperties-castem mtest")
      endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
      if(TFEL_APPEND_SUFFIX)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
	  DESTINATION "share/doc/mfront-${TFEL_SUFFIX}/tests/behaviours/crossed2delta"
	  CONFIGURATIONS ${conf}
	  COMPONENT mtest)
      else(TFEL_APPEND_SUFFIX)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
	  DESTINATION "share/doc/mfront/tests/behaviours/crossed2delta"
	  CONFIGURATIONS ${conf}
	  COMPONENT mtest)
      endif(TFEL_APPEND_SUFFIX)
    endforeach(conf ${CMAKE_CONFIGURATION_TYPES})
  else(CMAKE_CONFIGURATION_TYPES)
    get_property(MFrontCastemBehavioursBuildPath TARGET MFrontCastemBehaviours PROPERTY LOCATION_${conf})
    configure_file(crossed2delta${test_arg}.mtest.in
      crossed2delta${test_arg}.mtest @ONLY)
    add_test(NAME crossed2delta${test_arg}_mtest
      COMMAND mtest --verbose=level0 --xml-output=true --result-file-output=false crossed2delta${test_arg}.mtest)
    set_tests_properties(crossed2delta${test_arg}_mtest
      PROPERTIES DEPENDS MFrontCastemBehaviours)
    set_tests_properties(crossed2delta${test_arg}_mtest
      PROPERTIES DEPENDS MFrontMaterialProperties-castem)
    set_tests_properties(crossed2delta${test_arg}_mtest
      PROPERTIES DEPENDS mtest)
    if(TFEL_APPEND_SUFFIX)
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/crossed2delta${test_arg}.mtest
	DESTINATION "share/doc/mfront-${TFEL_SUFFIX}/tests/behaviours/crossed2delta"
	COMPONENT mtest)
    else(TFEL_APPEND_SUFFIX)
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/crossed2delta${test_arg}.mtest
	DESTINATION "share/doc/mfront/tests/behaviours/crossed2delta"
	COMPONENT mtest)
    endif(TFEL_APPEND_SUFFIX)
  endif(CMAKE_CONFIGURATION_TYPES)
endmacro(test_crossed2delta)

test_crossed2delta(chaboche)
test_crossed2delta(ddif2-2)
test_crossed2delta(ddif2-3)
test_crossed2delta(ddif2-4)
test_crossed2delta(ddif2)
test_crossed2delta(druckerprager)
test_crossed2delta(ellipticcreep)
test_crossed2delta(finitestrainsinglecrystal-001)
test_crossed2delta(finitestrainsinglecrystal-012-2)
test_crossed2delta(finitestrainsinglecrystal-012)
test_crossed2delta(finitestrainsinglecrystal-159)
test_crossed2delta(finitestrainsinglecrystal2-001)
test_crossed2delta(finitestrainsinglecrystal_numericaljacobian-001)
test_crossed2delta(finitestrainsinglecrystal_numericaljacobian-012-2)
test_crossed2delta(finitestrainsinglecrystal_numericaljacobian-012)
test_crossed2delta(finitestrainsinglecrystal_numericaljacobian-159)
test_crossed2delta(hayhurst)
test_crossed2delta(implicithayhurst)
test_crossed2delta(implicitmonocrystal)
test_crossed2delta(implicitnorton3)
test_crossed2delta(implicitnorton-planestress2)
test_crossed2delta(lorentz)
test_crossed2delta(monocrystal_dd_cc_irradiation)
test_crossed2delta(monocrystal_dd_cc_irradiation_numericaljacobian)
test_crossed2delta(monocrystal_dd_cfc_irradiation)
test_crossed2delta(monocrystal_dd_cfc)
test_crossed2delta(monocrystal_dd_cfc_numericaljacobian)
test_crossed2delta(monocrystal)
test_crossed2delta(norton2)
test_crossed2delta(norton-euler)
test_crossed2delta(norton)
test_crossed2delta(norton-rk2)
test_crossed2delta(nortonrk2)
test_crossed2delta(nortonrk3)
test_crossed2delta(norton-rk4)
test_crossed2delta(nortonrk4)
test_crossed2delta(plasisocin)
test_crossed2delta(plasticity)
test_crossed2delta(strainhardeningcreep)
test_crossed2delta(stressrelaxation)
test_crossed2delta(T91-satohtest)
test_crossed2delta(T91-viscositytest)
test_crossed2delta(viscoplasisocin)

# test_crossed2delta(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-3-3D)
# test_crossed2delta(monocrystal_dd_cc)
# test_crossed2delta(monocrystal_dd_cc_numericaljacobian)
