tfel_library(TFELGlossary
  GlossaryEntry.cxx
  Glossary.cxx)
target_link_libraries(TFELGlossary
	PRIVATE TFELUtilities)
target_include_directories(TFELGlossary
   PUBLIC 
   $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
   $<INSTALL_INTERFACE:include>)
if(enable-static)
  target_link_libraries(TFELGlossary-static
	  PRIVATE TFELUtilities-static)
  target_include_directories(TFELGlossary-static
     PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)
endif(enable-static)