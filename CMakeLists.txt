cmake_minimum_required(VERSION 2.6)
project(GLSLTranslatorSln)

set (GLSLTRANSLATOR_SRC
	shaderTranslator.cpp
	shaderTranslator.h
	shaderTranslatorGL21.cpp
	shaderTranslatorGL21.h
	shaderTranslatorGL33.cpp
	shaderTranslatorGL33.h
)
add_library(GLSLTranslator ${GLSLTRANSLATOR_SRC})

add_executable(GLSLTest glslCross.cpp)
target_link_libraries(GLSLTest GLSLTranslator)