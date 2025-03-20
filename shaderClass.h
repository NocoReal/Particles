#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <GL/glew.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cerrno>

std::string get_file_contents(const char* filename);

class Shader {
	public:
		GLuint ID;
		Shader(const char* veertexFile, const char* fragmentFile);

		void Activate();
		void Delete();
};

#endif