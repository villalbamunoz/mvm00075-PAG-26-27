#ifndef MVM00075_PAG_RENDERER_H
#define MVM00075_PAG_RENDERER_H
#include <list>
#include <string>
#include "glad/glad.h"
#include <glm/glm.hpp>
#include "enumerados.h"


#include "Modelo.h"
#include "ProgramShader.h"

namespace PAG {

    class Renderer {
    private:
        // Necesidades del singleton
        Renderer();
        static  Renderer* instancia;

        // Color del frame buffer, el del fondo de la pantalla
        float colorBorradoFrameBuffer[3] = {0.5f, 0.5f, 0.5f};

        // Gestión de modelo
        std::vector<Modelo> modelosEnEscena;

        // Identificadores de objetos de OpenGL
        GLuint idVAO = 0; // Identificador del vertex array object
        GLuint idVBOcoordenadas = 0; // Identificador del vertex buffer object de coordenadas
        GLuint idVBOcolores = 0; // Identificador del vertex buffer object de colores
        GLuint idIBO = 0; // Identificador del index buffer object

        // Parámetros para indicar la cantidad de las transformaciones de modelado
        glm::float32 cantidadEscalarPos = 1.5f;
        glm::float32 cantidadEscalarNeg = 0.75f;
        glm::float32 gradosRotacion = 1.0f;
        glm::float32 distTraslacion = 0.01f;

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
        void set_nombre_shaders(std::string texto, int posModelo) {}; // TODO Implementar en cpp y documentar
        std::vector<std::string> getRutasModelos();

        // TODO Quitar placeholder. Deben poderse hacer shaders diferentes para cada modelo.
        std::string placeholder = "pag05";
        std::string* get_direccion_nombre_shaders() {return &placeholder;}

        // Generales
        void creaShadersPrograms();
        void cargarModelosPredeterminados();
        void inicializaOpenGL();

        // Gestión de modelos en escena
        void anadirModelo(std::string ruta);
        void anadirModelosPredeterminados();
        void aplicarTransformacion(std::string rutaModelo, TransformacionModelado transformacion, bool sentido);
        void borrarModelo(std::string ruta);

    };
} // PAG

#endif //MVM00075_PAG_RENDERER_H