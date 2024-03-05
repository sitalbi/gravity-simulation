#pragma once
#include <GL/glew.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);
