#ifndef MVM00075_PAG_MODELO_H
#define MVM00075_PAG_MODELO_H
#include <string>
#include <vector>

#include "ProgramShader.h"
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> // Esta es necesaria aunque Clion indique lo contrario
#include "enumerados.h"



namespace PAG {

    class Modelo {
    private:
        std::string rutaOBJ = ""; // Aquí incluimos el modelo predeterminado
        ProgramShader* programShader = nullptr;
        std::string nombreShaders = "pag05"; // Aquí incluimos los shaders predeterminados
        glm::mat4 matrizTransformacionModelado = glm::mat4(1); // Este constructor crea la identity matrix
        glm::vec3 puntoPosModelo = {0.0f,0.0f,0.0f}; // Posicion del modelo


        // EEDD para guardar los datos del modelo en sí
        std::vector<glm::vec3> vertices;
        std::vector<GLuint> indices;


        // Identificadores de objetos de OpenGL
        GLuint idVAO = 0; // Identificador del vertex array object
        GLuint idVBOcoordenadas = 0; // Identificador del vertex buffer object de coordenadas
        GLuint idIBO = 0; // Identificador del index buffer object

        // Parámetros para indicar la cantidad de las transformaciones de modelado
        glm::float32 cantidadEscalarPos = 1.5f;
        glm::float32 cantidadEscalarNeg = 0.75f;
        glm::float32 gradosRotacion = 1.0f;
        glm::float32 distTraslacion = 0.01f;


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

        // Getters y setters
        std::string getRuta() {return rutaOBJ;};

    };
} // PAG

#endif //MVM00075_PAG_MODELO_H