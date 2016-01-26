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

std::vector<std::string> ShaderTranslator::tokenize(const std::string &str) {
	std::vector<std::string> tokens;
	
	size_t len = str.length();
	for (size_t i = 0; i < len; i++) {
		if (ispunct(str[i]) != 0 || !isWordcharacter(str[i]))
			tokens.push_back(str.substr(i, 1));
		else {
			// grab full word and push that as a token.
			size_t l = 0;
			size_t start = i;
			while ((start + l) < len && isWordcharacter(str[start + l]))
				l++;
			
			std::string tmp = str.substr(start, l);
			tokens.push_back(tmp);
			
			if (l > 0)
				i += l - 1;
		}
	}

	return tokens;
}

const std::string ShaderTranslator::translate(std::vector<std::string> &tokens, ShaderType shaderType) {
	size_t size = tokens.size();
	for (size_t i = 0; i < size; i++) {
		if (tokens[i] == "#") {
			// check for preprocessor tokens.
			i += preproccessor(tokens, i);
		} else if (tokens[i] == "attribute") {
			// check for the attribute keyword. in GLSL core profile, it is changed
			// to the in keyword.
			tokens[i] = "in";
		} else if (tokens[i] == "varying") {
			// In vertex shaders, varying turns to out.
			// in fragment shaders, varying turns to in.
			if (shaderType == ShaderType::VERTEX)
				tokens[i] = "out";
			else if (shaderType == ShaderType::FRAGMENT)
				tokens[i] = "in";
		} else if (isFunctionCallAtPos("texture", tokens, i)) {
			// texture2D, textureCube, texture2DLod, ect is handled here. Basically
			// this case handles texture functions
			// In GLSL core profile, texture() is overloaded.
			tokens[i] = "texture";
		}
	}
	
	// create the shader and return it.
	std::string shader = "";
	for (const auto &tok : tokens) {
		shader += tok.c_str();
	}
	return shader;
}

int ShaderTranslator::preproccessor(std::vector<std::string> &tokens, int currentToken) {
	const size_t size = tokens.size();
	for (size_t i = currentToken + 1; i < size; i++) {
		if (tokens[i] == "version") {
			// version number token.
			// check to make sure that the next number is in fact a verison number
			if (i+2 >= size) {
				return -1;
			}
			// tokens[i+1] is a space
			if (!isspace(tokens[i+1][0])) {
				printf("verison needs a space between #version and the number\n");
				return -1;
			}
			tokens[i + 2] = "330 core";
			currentToken += 2;
			break;
		}
	}
	
	return currentToken;
}