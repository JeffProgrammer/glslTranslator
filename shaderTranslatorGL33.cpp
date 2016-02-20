//------------------------------------------------------------------------------
// Copyright (c) 2016, Jeff Hutchinson
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the name of glslTranslator nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
//	SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------

#include "shaderTranslatorGL33.h"

#define FRAG_OUTPUT "GEN_OUTPUT_FINAL_COLOR"

const std::string SHADER_GL33_VERTEX_HEADER = "#version 330 core\n#define GL33\n\n";
const std::string SHADER_GL33_FRAGMENT_HEADER = "#version 330 core\n#define GL33\n\nout vec4 " + std::string(FRAG_OUTPUT) + ";\n\n";

const std::string ShaderTranslatorGL33::translate(const std::string &str, ShaderType shaderType) {
	// first tokenize
	tokenize(str);

	size_t size = mTokens.size();
	for (size_t i = 0; i < size; i++) {
		if (shaderType == VERTEX) {
			if (mTokens[i] == "attribute") {
				// check for the attribute keyword. in GLSL core profile, it is changed
				// to the in keyword.
				mTokens[i] = "in";
				continue;
			}
		}
		else if (shaderType == FRAGMENT) {
			if (mTokens[i].find("gl_") != std::string::npos) {
				// Handle built in variables for GLSL here in fragment shaders.

				// Handle output of color.
				if (mTokens[i] == "gl_FragColor") {
					mTokens[i] = FRAG_OUTPUT;
				}

				continue;
			}
		}

		if (mTokens[i] == "varying") {
			// In vertex shaders, varying turns to out.
			// in fragment shaders, varying turns to in.
			if (shaderType == ShaderType::VERTEX)
				mTokens[i] = "out";
			else if (shaderType == ShaderType::FRAGMENT)
				mTokens[i] = "in";
		}
		else if (isFunctionCallAtPos("texture", i)) {
			// texture2D, textureCube, texture2DLod, ect is handled here. Basically
			// this case handles texture functions
			// In GLSL core profile, texture() is overloaded.
			mTokens[i] = "texture";
		}
	}

	// create the shader and return it.
	// first add our shader header.
	std::string shader;
	if (shaderType == ShaderType::VERTEX)
		shader = SHADER_GL33_VERTEX_HEADER;
	else if (shaderType == ShaderType::FRAGMENT)
		shader = SHADER_GL33_FRAGMENT_HEADER;

	for (const auto &tok : mTokens) {
		shader += tok.c_str();
	}
	return shader;
}