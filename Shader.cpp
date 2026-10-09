#include "Shader.h"

namespace PAG {
    Shader::~Shader() {
        if (idOpenGL != 0) {
            glDeleteShader ( idOpenGL);
        }

    }
} // PAG