#ifndef MVM00075_PAG_PROGRAMSHADER_H
#define MVM00075_PAG_PROGRAMSHADER_H

#include <string>
#include "Shader.h"


namespace PAG {
    class ProgramShader {
    private:
        GLuint idOpenGL = 0;
        Shader* vertexShader = nullptr;
        Shader* fragmentShader = nullptr;


    public:
        void cargarVertexShader(std::string ruta);
        void cargarFragmentShader(std::string ruta);

        void crearProgramShader();

        virtual ~ProgramShader();

        GLuint getIdOpenGL() {return idOpenGL;}
    };
} // PAG

#endif //MVM00075_PAG_PROGRAMSHADER_H