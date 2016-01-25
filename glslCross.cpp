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

#include <iostream>
#include <string>
#include <vector>

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
 * Handles the preprocessor tokens of the shader stream.
 * Anything that has a # symbol is a preprocessor.
 * @param tokens The shader stream, split up into a tokenized list.
 * @param currentToken The current position of where we are in the token list.
 * @return the position on where to continue processing the shader stream
 *  after the preprocessing line is done, or -1 if an error occurred.
 */
int preproccessor(std::vector<std::string> &tokens, int currentToken) {
	const size_t size = tokens.size();
	for (int i = currentToken + 1; i < size; i++) {
		if (tokens[i] == "version") {
			// version number token.
			// check to make sure that the next number is in fact a verison number
			if (i+2 >= size) {
				return -1;
			}
			// tokens[i+1] is a space
			if (!std::isspace(tokens[i+1][0])) {
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

/**
 * Translates the shader in it's tokenized list form from GLSL 120 to GLSL
 * 330. It will print out the old shader and the new shader once the tokenization
 * process is completed.
 * @param tokens A vector of shader tokens that are going to be parsed.
 * @param shaderType The type of shader stream that is being parsed, such as a
 *  vertex shader or a fragment (pixel) shader.s
 */
void translate(std::vector<std::string> &tokens, ShaderType shaderType) {
	// print out original input stream
	printf("Original GLSL 120:\n\n");
	for (auto t : tokens)
		printf("%s", t.c_str());
	
	size_t size = tokens.size();
	for (int i = 0; i < size; i++) {
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
		}
	}
	
	printf("\n");
	printf("Cross compiled to GLSL 330\n\n");
	for (auto t : tokens)
		printf("%s", t.c_str());
}

/**
 * Determines if character 'x' is a word character and is not a space.
 * @param x The character to check if it is a word character.
 * @return true if 'x' is a word character and is not a space, false otherwise.
 */
inline bool isWordcharacter(char x) {
	return !std::isspace(x) && std::iswalnum(x);
}

/**
 * Tokenizes a stream of shader source and then parses it.
 * @param str The stream of shader source to be tokenized and parsed.
 * @param type The type of shader, whether it be a vertex shader or a fragment
 * (pixel) shader.
 */
void tokenize(const std::string &str, ShaderType type) {
	std::string token = " \n\t\r";
	
	std::vector<std::string> tokens;

	
	size_t len = str.length();
	for (size_t i = 0; i < len; i++) {
		if (std::ispunct(str[i]) != 0 || !isWordcharacter(str[i]))
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
	
	// Translate from GLSL 120 to GLSL 330
	translate(tokens, type);
}

int main(int argc, const char * argv[]) {
	std::string glsl = "#version 120\nattribute vec3 pos;\nvarying vec3 col;\nuniform vec4 mvp;\nvoid main() {\n   gl_Position = vec4(pos, 1) * mvp;\n   col = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n}\n";
	// test vertex shader translation from GLSL 120 to GLSL 330 core profile
	tokenize(glsl, ShaderType::VERTEX);
	return 0;
}
