#ifndef MVM00075_PAG_RENDERER_H
#define MVM00075_PAG_RENDERER_H
#include <string>
#include "glad/glad.h"
#include <glm/glm.hpp>
#include "ProgramShader.h"

namespace PAG {
    class Renderer {
    private:
        static  Renderer* instancia;
        float colorBorradoFrameBuffer[3] = {0.5f, 0.5f, 0.5f};
        ProgramShader* programShader;
        std::string nombreShaders = "pag05"; // Inicializamos a un shader predeterminado


        // Identificadores de objetos de OpenGL
        GLuint idVAO = 0; // Identificador del vertex array object
        GLuint idVBOcoordenadas = 0; // Identificador del vertex buffer object de coordenadas
        GLuint idVBOcolores = 0; // Identificador del vertex buffer object de colores
        GLuint idIBO = 0; // Identificador del index buffer object

        Renderer();


    public:
        static Renderer& getInstancia();
        virtual ~Renderer();


        void refrescar_escena(glm::vec3 puntoPosCamara, glm::vec3 puntoLA, glm::vec3 upVector, glm::float32 fovYGrados,
            glm::float32 aspectRatio, glm::float32 distZNear, glm::float32 distZFar);
        void callback_resize(int width, int height);
        void habilitar_profundidad();

        // Getters y setters
        void set_color_borrado_frame_buffer(float r, float g, float b, float a);
        float* get_color_borrado_frame_buffer();
        std::string get_propiedades_del_contexto();
        void set_nombre_shaders(std::string texto) {nombreShaders = texto;};
        std::string* get_direccion_nombre_shaders() {return &nombreShaders;};

        // Modelado básico
        void creaShaderProgram();
        void creaModelo();
        void inicializaOpenGL();

    };
} // PAG

#endif //MVM00075_PAG_RENDERER_H