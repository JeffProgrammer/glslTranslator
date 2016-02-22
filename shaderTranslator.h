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

typedef std::vector<std::string> ShaderTokenList;

/**
 * A class that translates OpenGL GLSL 120 shaders other high level
 * shading languages. Currently only GLSL 120 and GLSL 330 are supported.
 * Example usage of the library:
 *
 * std::string vertex   = "void main() {\n gl_Position = vec4(1);\n}";
 * std::string fragment = "void main() {\n gl_FragColor = vec4(1, 0, 0, 1);\n}";
 * printf("%s\n", vertex.c_str());
 * printf("%s\n", fragment.c_str());
 *
 * // Tokenize and translate the Vertex Shader.
 * auto vertexTranslator = new ShaderTranslatorGL33();
 * const auto &shader = vertexTranslator->translate(vertex, ShaderTranslator::VERTEX);
 * printf("%s\n", shader.c_str());
 *
 * // Now tokenize and translate the Fragment Shader.
 * auto = fragmentTranslator = new ShaderTranslatorGL33();
 * shader = fragmentTranslator->translate(fragment, ShaderTranslator::FRAGMENT);
 * printf("%s\n", shader.c_str());
 */
class ShaderTranslator {
public:
	virtual ~ShaderTranslator() {}

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
	 * Translates the shader in it's tokenized list form from GLSL 120 to GLSL
	 * 330. It will print out the old shader and the new shader once the tokenization
	 * process is completed.
	 * @param str The stream of shader source to be tokenized and translated..
	 * @param shaderType The type of shader stream that is being parsed, such as a
	 *  vertex shader or a fragment (pixel) shader.
	 * @return the translated shader source, back in a string form.
	 */
	virtual const std::string translate(const std::string &str, ShaderType shaderType) = 0;

	/**
	 * Get's the list of tokens from the tokenizer's job.
	 * @return the list of tokens.
	 */
	const ShaderTokenList getTokens() const {
		return mTokens;
	}
	
protected:
	/**
	 * Tokenizes a stream of shader source.
	 */
	void tokenize(const std::string &str);

	/**
	 * Determines if character 'x' is a word character and is not a space.
	 * @param x The character to check if it is a word character.
	 * @return true if 'x' is a word character and is not a space, false otherwise.
	 */
	static inline bool isWordcharacter(char x) {
		return (!isspace(x) && iswalnum(x)) || x == '_';
	}

	/**
	 * Determines if the token at 'currentId' is calling the fuction or function
	 * pattern.
	 * @param fn The function or pattern we are checking at the token currentId.
	 * @param tokens The vector containing the token list.
	 * @param currentId The current token we are checking.
	 * @return true if we are calling a function of pattern 'fn', false otherwise.
	 * @note A function call grammar we are checking is defined as the following:
	 *       'fn_pattern' '('
	 */
	inline bool isFunctionCallAtPos(const std::string &fn, size_t currentId) {
		return (mTokens[currentId].find(fn) != std::string::npos) && ((currentId + 1) < mTokens.size()) && (mTokens[currentId + 1] == "(");
	}

	/**
	 * A vector of tokens represented as strings.
	 */
	ShaderTokenList mTokens;
};

#endif /* shaderTranslator_h */
