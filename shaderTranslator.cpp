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

#include "shaderTranslator.h"

ShaderTranslator::ShaderTranslator() {

}

void ShaderTranslator::tokenize(const std::string &str) {
	size_t len = str.length();
	size_t i = 0;
	while (i < len) {
		if (ispunct(str[i]) != 0 || !isWordcharacter(str[i]))
			mTokens.push_back(str.substr(i, 1));
		else {
			// grab full word and push that as a token.
			size_t l = 0;
			size_t start = i;
			while ((start + l) < len && isWordcharacter(str[start + l]))
				l++;
			
			std::string tmp = str.substr(start, l);
			mTokens.push_back(tmp);
			
			// Only add if we are greater than 0. If we are zero, we will just
			// grab the increment below.
			if (l > 0) {
				i += l;
				continue;
			}
		}
		i++;
	}
}

const std::string ShaderTranslator::translate(const std::string &str, ShaderType shaderType) {
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
		} else if (shaderType == FRAGMENT) {
			if (mTokens[i] == "void" && mTokens[i + 2] == "main") {
				// We need to add a token here for output color.
				// TODO: MRT if we need it with gl_FragData[]
				mTokens[i] = "out vec4 GEN_OUTPUT_FINAL_COLOR;\n\nvoid";
				continue;
			} else if (mTokens[i].find("gl_") != std::string::npos) {
				// Handle built in variables for GLSL here in fragment shaders.

				// Handle output of color.
				if (mTokens[i] == "gl_FragColor") {
					mTokens[i] = "GEN_OUTPUT_FINAL_COLOR";
				}

				continue;
			}
		}

		if (mTokens[i] == "#") {
			// check for preprocessor tokens.
			i += preproccessor(i);
		} else if (mTokens[i] == "varying") {
			// In vertex shaders, varying turns to out.
			// in fragment shaders, varying turns to in.
			if (shaderType == ShaderType::VERTEX)
				mTokens[i] = "out";
			else if (shaderType == ShaderType::FRAGMENT)
				mTokens[i] = "in";
		} else if (isFunctionCallAtPos("texture", i)) {
			// texture2D, textureCube, texture2DLod, ect is handled here. Basically
			// this case handles texture functions
			// In GLSL core profile, texture() is overloaded.
			mTokens[i] = "texture";
		}
	}
	
	// create the shader and return it.
	std::string shader = "";
	for (const auto &tok : mTokens) {
		shader += tok.c_str();
	}
	return shader;
}

int ShaderTranslator::preproccessor(int currentToken) {
	const size_t size = mTokens.size();
	for (size_t i = currentToken + 1; i < size; i++) {
		if (mTokens[i] == "version") {
			// version number token.
			// check to make sure that the next number is in fact a verison number
			if (i+2 >= size) {
				return -1;
			}
			// tokens[i+1] is a space
			if (!isspace(mTokens[i+1][0])) {
				printf("verison needs a space between #version and the number\n");
				return -1;
			}
			mTokens[i + 2] = "330 core";
			currentToken += 2;
			break;
		}
	}
	
	return currentToken;
}