#ifndef MVM00075_PAG_MODELO_H
#define MVM00075_PAG_MODELO_H
#include <string>
#include <vector>

#include "ProgramShader.h"
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>



namespace PAG {

    class Modelo {
    private:
        std::string rutaOBJ = ""; // Aquí incluimos el modelo predeterminado
        ProgramShader* programShader = nullptr;
        std::string nombreShaders = "pag05"; // Aquí incluimos los shaders predeterminados
        glm::mat4 matrizTransformacionModelado;

        // EEDD para guardar los datos del modelo en sí
        std::vector<glm::vec3> vertices;
        std::vector<GLuint> indices;


        // Identificadores de objetos de OpenGL
        GLuint idVAO = 0; // Identificador del vertex array object
        GLuint idVBOcoordenadas = 0; // Identificador del vertex buffer object de coordenadas
        GLuint idIBO = 0; // Identificador del index buffer object



    public:

        Modelo(std::string nuevaRuta) : rutaOBJ(nuevaRuta) {}

        // Méto_dos para añadir transformaciones de modelado
        void escalar(bool agrandar);
        void rotarSobreEjeX(bool sentido);
        void rotarSobreEjeY(bool sentido);
        void rotarSobreEjeZ(bool sentido);
        void trasladarEnEjeX(bool sentido);
        void trasladarEnEjeY(bool sentido);
        void trasladarEnEjeZ(bool sentido);

        // Modelado básico
        void creaShaderProgram();
        void cargarModelo();
        void refrescarModelo(glm::mat4  matrizCompuesta);

        // Función auxiliar para carga de modelos .obj
        void procesarNodo(aiNode *nodo, const aiScene *escena);

    };
} // PAG

#endif //MVM00075_PAG_MODELO_H