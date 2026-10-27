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
    public: // TODO Hacer private
        std::string rutaOBJ;
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



    public:
        // Constructores y destructores
        Modelo(std::string nuevaRuta, std::string nuevoNombre="") : rutaOBJ(nuevaRuta), nombreShaders(nuevoNombre) {}
        ~Modelo();

        void liberarRecursos();

        // Méto_dos para añadir transformaciones de modelado
        void escalar(bool agrandar, glm::float32 proporcion);
        void rotarSobreEjeX(bool sentido, glm::float32 grados);
        void rotarSobreEjeY(bool sentido, glm::float32 grados);
        void rotarSobreEjeZ(bool sentido, glm::float32 grados);
        void trasladarEnEjeX(bool sentido, glm::float32 distancia);
        void trasladarEnEjeY(bool sentido, glm::float32 distancia);
        void trasladarEnEjeZ(bool sentido, glm::float32 distancia);

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