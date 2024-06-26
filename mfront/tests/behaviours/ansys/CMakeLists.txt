set(mfront_tests_SOURCES
  InitializationFailureTest
  MaterialPropertiesBoundsCheck
  Elasticity
  OrthotropicElastic
  HosfordPerfectPlasticity
  JohnsonCook_s
  JohnsonCook_ssr
  JohnsonCook_ssrt
  RusinekKlepaczko_ssrt
  SaintVenantKirchhoffElasticity
  SaintVenantKirchhoffElasticity2
  SaintVenantKirchhoffElasticity3
  OrthotropicSaintVenantKirchhoffElasticity
  LogarithmicStrainElasticity
  LogarithmicStrainElasticity2
  LogarithmicStrainElasticity3
  LogarithmicStrainNortonCreep
  ComputeThermalExpansion
  ComputeThermalExpansion2
  ComputeThermalExpansion3
  ParameterTest
  ParameterTest2)
mfront_dependencies(MFrontAnsysBehaviours
  ThermalExpansionCoefficientTest
  ThermalExpansionCoefficientTest2
  ThermalExpansionCoefficientTest_1
  ThermalExpansionCoefficientTest_2
  ThermalExpansionCoefficientTest_3)

mfront_behaviour_check_library(MFrontAnsysBehaviours  ansys ${mfront_tests_SOURCES})
target_link_libraries(MFrontAnsysBehaviours
  PRIVATE AnsysInterface)

if(WIN32)
  if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    set_target_properties(MFrontAnsysBehaviours
      PROPERTIES LINK_FLAGS "-Wl,--kill-at -Wl,--no-undefined")
  endif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
endif(WIN32)

macro(test_ansys test_arg)
  set(_NO_XML_OUTPUT )
  set(_WILL_FAIL )
  foreach(_ARG ${ARGN})
    if ( ${_ARG} MATCHES NO_XML_OUTPUT )
      set ( _NO_XML_OUTPUT ON)
    elseif ( ${_ARG} MATCHES WILL_FAIL)
      set ( _WILL_FAIL ON)
    else()
      message(FATAL_ERROR "test_ansys: unsupported option '${_ARG}'")
    endif()
  endforeach(_ARG ${ARGN})
  foreach(rm ${IEEE754_ROUNDING_MODES})
    if(CMAKE_CONFIGURATION_TYPES)
      foreach(conf ${CMAKE_CONFIGURATION_TYPES})
	set(file "ansys${test_arg}-${rm}-${conf}.mtest")
	set(test "ansys${test_arg}_${rm}_${conf}_mtest")
	get_property(MFrontAnsysBehavioursBuildPath TARGET MFrontAnsysBehaviours PROPERTY LOCATION_${conf})
	if(HAVE_CASTEM)
	  get_property(MFrontMaterialPropertiesCastemBuildPath TARGET MFrontMaterialProperties-castem PROPERTY LOCATION_${conf})
	endif(HAVE_CASTEM)
	configure_file(ansys${test_arg}.mtest.in ${file} @ONLY)
	if(_NO_XML_OUTPUT)
	  add_test(NAME ${test} CONFIGURATIONS ${conf}
	    COMMAND mtest --rounding-direction-mode=${rm} --verbose=level0 --xml-output=false --result-file-output=false ${file})
	else(_NO_XML_OUTPUT)
	  add_test(NAME ${test} CONFIGURATIONS ${conf}
	    COMMAND mtest --rounding-direction-mode=${rm} --verbose=level0 --xml-output=true --result-file-output=false ${file})
	endif(_NO_XML_OUTPUT)
	if(_WILL_FAIL)
	  set_tests_properties(${test} PROPERTIES WILL_FAIL true)
	endif(_WILL_FAIL)
	if((CMAKE_HOST_WIN32) AND (NOT MSYS))
          set_property(TEST ${test}
            PROPERTY DEPENDS MFrontAnsysBehaviours MFrontMaterialProperties-ansys mtest
	    PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELNUMODIS>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELConfig>\;$<TARGET_FILE_DIR:TFELUnicodeSupport>\;$ENV{PATH}")
	elseif((CMAKE_HOST_WIN32) AND (NOT MSYS))
          set_property(TEST ${test}
            PROPERTY DEPENDS MFrontAnsysBehaviours MFrontMaterialProperties-castem mtest)
	endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
	if(TFEL_APPEND_SUFFIX)
	  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
	    DESTINATION "share/doc/mfront-${TFEL_SUFFIX}/tests/behaviours/ansys"
	    COMPONENT mtest)
	else(TFEL_APPEND_SUFFIX)
	  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
	    DESTINATION "share/doc/mfront/tests/behaviours/ansys"
	    COMPONENT mtest)
	endif(TFEL_APPEND_SUFFIX)
      endforeach(conf ${CMAKE_CONFIGURATION_TYPES})
    else(CMAKE_CONFIGURATION_TYPES)
      set(file "ansys${test_arg}-${rm}.mtest")
      set(test "ansys${test_arg}_${rm}_mtest")
      get_property(MFrontAnsysBehavioursBuildPath TARGET MFrontAnsysBehaviours PROPERTY LOCATION)
      if(HAVE_CASTEM)
	get_property(MFrontMaterialPropertiesCastemBuildPath TARGET MFrontMaterialProperties-castem PROPERTY LOCATION)
      endif(HAVE_CASTEM)
      configure_file(ansys${test_arg}.mtest.in ${file} @ONLY)
      if(_NO_XML_OUTPUT)
	add_test(NAME ${test}
	  COMMAND mtest --rounding-direction-mode=${rm} --verbose=level0 --xml-output=false --result-file-output=false ${file})
      else(_NO_XML_OUTPUT)
	add_test(NAME ${test}
	  COMMAND mtest --rounding-direction-mode=${rm} --verbose=level0 --xml-output=true --result-file-output=false ${file})
      endif(_NO_XML_OUTPUT)
      if(_WILL_FAIL)
	set_tests_properties(${test} PROPERTIES WILL_FAIL true)
      endif(_WILL_FAIL)
      set_tests_properties(${test} PROPERTIES DEPENDS MFrontAnsysBehaviours)
      set_tests_properties(${test} PROPERTIES DEPENDS mtest)
      if(TFEL_APPEND_SUFFIX)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
	  DESTINATION "share/doc/mfront-${TFEL_SUFFIX}/tests/behaviours/ansys"
	  COMPONENT mtest)
      else(TFEL_APPEND_SUFFIX)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}
	  DESTINATION "share/doc/mfront/tests/behaviours/ansys"
	  COMPONENT mtest)
      endif(TFEL_APPEND_SUFFIX)
    endif(CMAKE_CONFIGURATION_TYPES)
  endforeach(rm ${IEEE754_ROUNDING_MODES})
endmacro(test_ansys)

test_ansys(initializationfailuretest
  NO_XML_OUTPUT WILL_FAIL)
test_ansys(materialpropertiesboundscheck
  NO_XML_OUTPUT WILL_FAIL)
test_ansys(materialpropertiesboundscheck2 
  NO_XML_OUTPUT WILL_FAIL)
test_ansys(materialpropertiesboundscheck3)
test_ansys(materialpropertiesboundscheck4)
test_ansys(materialpropertiesboundscheck5 
  NO_XML_OUTPUT WILL_FAIL)
test_ansys(materialpropertiesboundscheck6)

test_ansys(elasticity)
test_ansys(elasticity2)
if(HAVE_CASTEM)
  test_ansys(elasticity3)
endif(HAVE_CASTEM)
test_ansys(elasticity4)
test_ansys(elasticity7)
test_ansys(elasticity15)
test_ansys(elasticity16)

test_ansys(elasticity10)
test_ansys(elasticity11)
test_ansys(elasticity12)
test_ansys(elasticity13)
test_ansys(elasticity14)
test_ansys(elasticity24)
test_ansys(elasticity25)
test_ansys(elasticity26)
test_ansys(elasticity27)
test_ansys(elasticity33)
test_ansys(elasticity5)
test_ansys(elasticity6)
test_ansys(elasticity8)
test_ansys(elasticity9)

test_ansys(saintvenantkirchhoffelasticity-shear-1-2D)
test_ansys(saintvenantkirchhoffelasticity-shear-1-3D)
test_ansys(saintvenantkirchhoffelasticity-shear-2-2D)
test_ansys(saintvenantkirchhoffelasticity-shear-2-3D)
test_ansys(saintvenantkirchhoffelasticity-shear-3-3D)
test_ansys(saintvenantkirchhoffelasticity-shear-4-3D)
test_ansys(saintvenantkirchhoffelasticity-shear-5-3D)
test_ansys(saintvenantkirchhoffelasticity-shear-6-3D)
test_ansys(saintvenantkirchhoffelasticity-uniaxialloading-1-2D)
test_ansys(saintvenantkirchhoffelasticity-uniaxialloading-1-3D)
test_ansys(saintvenantkirchhoffelasticity-uniaxialloading-2-2D)
test_ansys(saintvenantkirchhoffelasticity-uniaxialloading-2-3D)
test_ansys(saintvenantkirchhoffelasticity-uniaxialloading-3-2D)
test_ansys(saintvenantkirchhoffelasticity-uniaxialloading-3-3D)
test_ansys(saintvenantkirchhoffelasticity-uniaxialloading-sig11-2D)
test_ansys(saintvenantkirchhoffelasticity-uniaxialloading-sig11-3D)
test_ansys(saintvenantkirchhoffelasticity-uniaxialloading-sig22-2D)
test_ansys(saintvenantkirchhoffelasticity-uniaxialloading-sig22-3D)
test_ansys(saintvenantkirchhoffelasticity-uniaxialloading-sig33-2D)
test_ansys(saintvenantkirchhoffelasticity-uniaxialloading-sig33-3D)

test_ansys(saintvenantkirchhoffelasticity2-shear-1-2D)
test_ansys(saintvenantkirchhoffelasticity2-shear-1-3D)
test_ansys(saintvenantkirchhoffelasticity2-shear-2-2D)
test_ansys(saintvenantkirchhoffelasticity2-shear-2-3D)
test_ansys(saintvenantkirchhoffelasticity2-shear-3-3D)
test_ansys(saintvenantkirchhoffelasticity2-shear-4-3D)
test_ansys(saintvenantkirchhoffelasticity2-shear-5-3D)
test_ansys(saintvenantkirchhoffelasticity2-shear-6-3D)
test_ansys(saintvenantkirchhoffelasticity2-uniaxialloading-1-2D)
test_ansys(saintvenantkirchhoffelasticity2-uniaxialloading-1-3D)
test_ansys(saintvenantkirchhoffelasticity2-uniaxialloading-2-2D)
test_ansys(saintvenantkirchhoffelasticity2-uniaxialloading-2-3D)
test_ansys(saintvenantkirchhoffelasticity2-uniaxialloading-3-2D)
test_ansys(saintvenantkirchhoffelasticity2-uniaxialloading-3-3D)
test_ansys(saintvenantkirchhoffelasticity3-shear-1-2D)
test_ansys(saintvenantkirchhoffelasticity3-shear-1-3D)
test_ansys(saintvenantkirchhoffelasticity3-shear-2-2D)
test_ansys(saintvenantkirchhoffelasticity3-shear-2-3D)
test_ansys(saintvenantkirchhoffelasticity3-shear-3-3D)
test_ansys(saintvenantkirchhoffelasticity3-shear-4-3D)
test_ansys(saintvenantkirchhoffelasticity3-shear-5-3D)
test_ansys(saintvenantkirchhoffelasticity3-shear-6-3D)
test_ansys(saintvenantkirchhoffelasticity3-uniaxialloading-1-2D)
test_ansys(saintvenantkirchhoffelasticity3-uniaxialloading-1-3D)
test_ansys(saintvenantkirchhoffelasticity3-uniaxialloading-2-2D)
test_ansys(saintvenantkirchhoffelasticity3-uniaxialloading-2-3D)
test_ansys(saintvenantkirchhoffelasticity3-uniaxialloading-3-2D)
test_ansys(saintvenantkirchhoffelasticity3-uniaxialloading-3-3D)
test_ansys(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-3D)
test_ansys(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-2-3D)
test_ansys(mieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-3-3D)
test_ansys(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-1-3D)
test_ansys(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-2-3D)
test_ansys(mieheapellambrechtlogarithmicstrainelasticity2-uniaxialloading-3-3D)
test_ansys(mieheapellambrechtlogarithmicstrainelasticity3-uniaxialloading-1-3D)
test_ansys(mieheapellambrechtlogarithmicstrainelasticity3-uniaxialloading-2-3D)
test_ansys(mieheapellambrechtlogarithmicstrainelasticity3-uniaxialloading-3-3D)
test_ansys(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-1-3D)
test_ansys(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-2-3D)
test_ansys(mieheapellambrechtlogarithmicstrainnortoncreep-uniaxialloading-3-3D)
test_ansys(mieheapellambrechtlogarithmicstrainelasticity-shear-2D)
test_ansys(mieheapellambrechtlogarithmicstrainelasticity-shear-FXY-3D)
test_ansys(mieheapellambrechtlogarithmicstrainelasticity-shear-FXZ-3D)
test_ansys(mieheapellambrechtlogarithmicstrainelasticity-shear-FYX-3D)
test_ansys(mieheapellambrechtlogarithmicstrainelasticity-shear-FYZ-3D)
test_ansys(mieheapellambrechtlogarithmicstrainelasticity-shear-FZX-3D)
test_ansys(mieheapellambrechtlogarithmicstrainelasticity-shear-FZY-3D)
test_ansys(parametertest)
test_ansys(parametertest2)

