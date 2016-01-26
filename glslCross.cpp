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
#include "shaderTranslator.h"

int main(int argc, const char * argv[]) {
	std::string vertex = "#version 120\nattribute vec3 pos;\nvarying vec3 col;\nvarying vec2 uv;\nuniform vec4 mvp;\nvoid main() {\n   gl_Position = vec4(pos, 1) * mvp;\n   col = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n   uv = vec2(0, 0);\n}\n";
	
	// test vertex shader translation from GLSL 120 to GLSL 330 core profile
	auto tokens = ShaderTranslator::tokenize(vertex);
	const auto &translatedSource = ShaderTranslator::translate(tokens, ShaderTranslator::ShaderType::VERTEX);
	printf("%s\n", translatedSource.c_str());

	std::string frag = "#version 120\nvarying vec4 col;\nvarying vec2 uv;\nuniform sampler2D diffuseTexture;\nvoid main() {\n   gl_FragColor = texture2D(diffuseTexture.st, uv) * color;\n}\n";

	// test vertex shader translation from GLSL 120 to GLSL 330 core profile
	tokens = ShaderTranslator::tokenize(frag);
	const auto &translatedFragSource = ShaderTranslator::translate(tokens, ShaderTranslator::ShaderType::FRAGMENT);
	printf("%s\n", translatedFragSource.c_str());

#ifdef _WIN32
	system("PAUSE");
#endif
	return 0;
}
