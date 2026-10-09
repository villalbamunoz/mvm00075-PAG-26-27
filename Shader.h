#ifndef MVM00075_PAG_SHADER_H
#define MVM00075_PAG_SHADER_H
#include "glad/glad.h"


namespace PAG {
    class Shader {
    private:
        GLuint idOpenGL = 0; // Identificador del shader en OpenGL

    public:
        GLuint getIdOpenGL() { return idOpenGL;};
        void setIdOpenGL(GLuint id) { idOpenGL = id; };

        virtual ~Shader();

    };
} // PAG

#endif //MVM00075_PAG_SHADER_H