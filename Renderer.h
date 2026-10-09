#ifndef MVM00075_PAG_RENDERER_H
#define MVM00075_PAG_RENDERER_H
#include <string>
#include "glad/glad.h"
#include "ProgramShader.h"

namespace PAG {
    class Renderer {
    private:
        static  Renderer* instancia;
        float colorBorradoFrameBuffer[3] = {0.5f, 0.5f, 0.5f};
        ProgramShader* programShader;
        Renderer();


        // Identificadores de objetos de OpenGL
        GLuint idVAO = 0; // Identificador del vertex array object
        GLuint idVBOcoordenadas = 0; // Identificador del vertex buffer object de coordenadas
        GLuint idVBOcolores = 0; // Identificador del vertex buffer object de colores
        GLuint idIBO = 0; // Identificador del index buffer object


    public:
        static Renderer& getInstancia();
        virtual ~Renderer();


        void refrescar_escena();
        void callback_resize(int width, int height);
        void habilitar_profundidad();

        void set_color_borrado_frame_buffer(float r, float g, float b, float a);
        float* get_color_borrado_frame_buffer();

        std::string get_propiedades_del_contexto();

        // Modelado básico
        void creaShaderProgram();
        void creaModelo();
        void inicializaOpenGL();

    };
} // PAG

#endif //MVM00075_PAG_RENDERER_H