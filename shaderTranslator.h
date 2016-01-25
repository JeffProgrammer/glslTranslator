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

#ifndef shaderTranslator_h
#define shaderTranslator_h

#include <iostream>
#include <string>
#include <vector>

/**
 * A static class that translates OpenGL GLSL 120 shaders to GLSL 330 shaders.
 * Example usage of the library:
 *
 * std::string vertex   = "#version 120\nvoid main() {\n gl_Position = vec4(1);\n}";
 * std::string fragment = "#version 120\nvoid main() {\n gl_FragColor = vec4(1, 0, 0, 1);\n}";
 * printf("%s\n", vertex.c_str());
 * printf("%s\n", fragment.c_str());
 *
 * // Tokenize and translate the Vertex Shader.
 * auto tokens = ShaderTranslator::tokenize(vertex);
 * const auto &shader = ShaderTranslator::translate(tokens, ShaderTranslator::VERTEX);
 * printf("%s\n", shader.c_str());
 *
 * // Now tokenize and translate the Fragment Shader.
 * tokens = ShaderTranslator::tokenize(fragment);
 * shader = ShaderTranslator::translate(tokens, ShaderTranslator::FRAGMENT);
 * printf("%s\n", shader.c_str());
 */
class ShaderTranslator {
public:
	/**
	 * Enum that handles different types of shader streams.
	 * Vertex shaders are shader files that process every geometric vertex.
	 * Fragment shaders are shader files that process every pixel of the geometry.
	 */
	enum ShaderType {
		VERTEX,
		FRAGMENT
	};

	/**
	 * Tokenizes a stream of shader source.
	 * @param str The stream of shader source to be tokenized.
	 * @return the tokenized list of the shader.
	 */
	static std::vector<std::string> tokenize(const std::string &str);
	
	/**
	 * Translates the shader in it's tokenized list form from GLSL 120 to GLSL
	 * 330. It will print out the old shader and the new shader once the tokenization
	 * process is completed.
	 * @param tokens A vector of shader tokens that are going to be parsed.
	 * @param shaderType The type of shader stream that is being parsed, such as a
	 *  vertex shader or a fragment (pixel) shader.
	 * @return the translated shader source, back in a string form.
	 */
	static const std::string translate(std::vector<std::string> &tokens, ShaderType shaderType);
	
protected:
	/**
	 * Handles the preprocessor tokens of the shader stream.
	 * Anything that has a # symbol is a preprocessor.
	 * @param tokens The shader stream, split up into a tokenized list.
	 * @param currentToken The current position of where we are in the token list.
	 * @return the position on where to continue processing the shader stream
	 *  after the preprocessing line is done, or -1 if an error occurred.
	 */
	static int preproccessor(std::vector<std::string> &tokens, int currentToken);
	
	/**
	 * Determines if character 'x' is a word character and is not a space.
	 * @param x The character to check if it is a word character.
	 * @return true if 'x' is a word character and is not a space, false otherwise.
	 */
	static inline bool isWordcharacter(char x) {
		return !std::isspace(x) && std::iswalnum(x);
	}
};

#endif /* shaderTranslator_h */
