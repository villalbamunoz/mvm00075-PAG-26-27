#include "Modelo.h"

#include <iostream>
#include <stdexcept>
#include <glm/gtx/transform.hpp>

namespace PAG {
    Modelo::~Modelo() {}

    void Modelo::liberarRecursos() {
        if ( idVBOcoordenadas != 0 )
        { glDeleteBuffers ( 1, &idVBOcoordenadas );
        }
        if ( idIBO != 0 )
        { glDeleteBuffers ( 1, &idIBO );
        }
        if ( idVAO != 0 )
        { glDeleteVertexArrays ( 1, &idVAO );
        }
    }

    /**
     * Añade una transformación de escalado a la matriz de transformación y modelado del modelo
     * @param agrandar Booleano que indica si aumentaremos o reduciremos el tamaño del modelo
     * @param proporcion Flotante que indica cuanta transformación aplicaremos
     */
    void Modelo::escalar(bool agrandar,  glm::float32 proporcion) {
        if (agrandar) {
            matrizTransformacionModelado = glm::scale(matrizTransformacionModelado,
                glm::vec3(proporcion, proporcion, proporcion));
        } else {
            matrizTransformacionModelado = glm::scale(matrizTransformacionModelado,
                glm::vec3(proporcion, proporcion, proporcion));
        }
    }

    /**
     * Añade una transformación de rotación sobre el eje X del modelo a la matriz de transformación y modelado del modelo
     * @param sentido Booleano que indica si rotamos en positivo o negativo sobre el eje
     * @param grados Flotante que indica cuantos grados rotaremos
     */
    void Modelo::rotarSobreEjeX(bool sentido, glm::float32 grados) {
         /*Si queremos rotar un modelo tomando como centro de rotación un punto p arbitrario en el espacio,
         *deberemos en primer lugar trasladar el modelo utilizando como vector de traslación (O - p), siendo O el
         *origen de coordenadas. Una vez hecha esta traslación, aplicaremos la rotación, y finalmente, volveremos a
         *trasladar el modelo utilizando como vector de traslación (p - O).*/

        // Matriz de traslación (O - p): Movemos el centro del objeto al origen
        glm::mat4 mTraslacion1 = glm::translate( glm::vec3(0,0,0) - puntoPosModelo);

        // Matriz de rotación
        glm::float32 radianesRotacion = glm::radians(grados);
        if (!sentido) {
            radianesRotacion = -radianesRotacion;
        }
        glm::mat4 matrizRotacion= glm::rotate(radianesRotacion, glm::vec3({1,0,0}));

        // Matriz de traslación (p - 0): Devolvemos el centro del objeto a su posición inicial
        glm::mat4 mTraslacion2 = glm::translate( puntoPosModelo - glm::vec3(0,0,0));

        // Multiplicamos las matrices en el orden inverso al que se van a aplicar
        glm::mat4 mFinal = mTraslacion2 * matrizRotacion * mTraslacion1;

        // Combinamos la matriz final con la de transformación y modelado actual
        matrizTransformacionModelado = mFinal * matrizTransformacionModelado;

        // Actualizamos el punto centro del objeto
        puntoPosModelo = mFinal * glm::vec4(puntoPosModelo, 1.0f);

    }


    /**
     * Añade una transformación de rotación sobre el eje Y del modelo a la matriz de transformación y modelado del modelo
     * @param sentido Booleano que indica si rotamos en positivo o negativo sobre el eje
     * @param grados Flotante que indica cuantos grados rotaremos
     */
    void Modelo::rotarSobreEjeY(bool sentido, glm::float32 grados) {
        /*Si queremos rotar un modelo tomando como centro de rotación un punto p arbitrario en el espacio,
          *deberemos en primer lugar trasladar el modelo utilizando como vector de traslación (O - p), siendo O el
          *origen de coordenadas. Una vez hecha esta traslación, aplicaremos la rotación, y finalmente, volveremos a
          *trasladar el modelo utilizando como vector de traslación (p - O).*/

        // Matriz de traslación (O - p): Movemos el centro del objeto al origen
        glm::mat4 mTraslacion1 = glm::translate( glm::vec3(0,0,0) - puntoPosModelo);

        // Matriz de rotación
        glm::float32 radianesRotacion = glm::radians(grados);
        if (!sentido) {
            radianesRotacion = -radianesRotacion;
        }
        glm::mat4 matrizRotacion= glm::rotate(radianesRotacion, glm::vec3({0,1,0}));

        // Matriz de traslación (p - 0): Devolvemos el centro del objeto a su posición inicial
        glm::mat4 mTraslacion2 = glm::translate( puntoPosModelo - glm::vec3(0,0,0));

        // Multiplicamos las matrices en el orden inverso al que se van a aplicar
        glm::mat4 mFinal = mTraslacion2 * matrizRotacion * mTraslacion1;

        // Combinamos la matriz final con la de transformación y modelado actual
        matrizTransformacionModelado = mFinal * matrizTransformacionModelado;

        // Actualizamos el punto centro del objeto
        puntoPosModelo = mFinal * glm::vec4(puntoPosModelo, 1.0f);
    }

    /**
     * Añade una transformación de rotación sobre el eje Z del modelo a la matriz de transformación y modelado del modelo
     * @param sentido Booleano que indica si rotamos en positivo o negativo sobre el eje
     * @param grados Flotante que indica cuantos grados rotaremos
     */
    void Modelo::rotarSobreEjeZ(bool sentido, glm::float32 grados) {
        /*Si queremos rotar un modelo tomando como centro de rotación un punto p arbitrario en el espacio,
         *deberemos en primer lugar trasladar el modelo utilizando como vector de traslación (O - p), siendo O el
         *origen de coordenadas. Una vez hecha esta traslación, aplicaremos la rotación, y finalmente, volveremos a
         *trasladar el modelo utilizando como vector de traslación (p - O).*/

        // Matriz de traslación (O - p): Movemos el centro del objeto al origen
        glm::mat4 mTraslacion1 = glm::translate( glm::vec3(0,0,0) - puntoPosModelo);

        // Matriz de rotación
        glm::float32 radianesRotacion = glm::radians(grados);
        if (!sentido) {
            radianesRotacion = -radianesRotacion;
        }
        glm::mat4 matrizRotacion= glm::rotate(radianesRotacion, glm::vec3({0,0,1}));

        // Matriz de traslación (p - 0): Devolvemos el centro del objeto a su posición inicial
        glm::mat4 mTraslacion2 = glm::translate( puntoPosModelo - glm::vec3(0,0,0));

        // Multiplicamos las matrices en el orden inverso al que se van a aplicar
        glm::mat4 mFinal = mTraslacion2 * matrizRotacion * mTraslacion1;

        // Combinamos la matriz final con la de transformación y modelado actual
        matrizTransformacionModelado = mFinal * matrizTransformacionModelado;

        // Actualizamos el punto centro del objeto
        puntoPosModelo = mFinal * glm::vec4(puntoPosModelo, 1.0f);
    }

    /**
     * Añade una transformación de traslación sobre el eje X del sistema de coordenadas global a la matriz de transformación y modelado del modelo
     * @param sentido Booleano que indica si nos movemos en positivo o negativo sobre el eje
     * @param distancia Flotante que indica cuanto avanzaremos en el eje
     */
    void Modelo::trasladarEnEjeX(bool sentido, glm::float32 distancia) {
        if (sentido) {
            glm::mat4 matrizTraslacion = glm::translate(glm::vec3(distancia,0,0));

            // Aplicamos la traslación tanto a la matriz de modelado como al punto de posición
            matrizTransformacionModelado = matrizTraslacion * matrizTransformacionModelado;
            puntoPosModelo = matrizTraslacion * glm::vec4(puntoPosModelo, 1.0f);
        } else {
            glm::mat4 matrizTraslacion = glm::translate(glm::vec3(-distancia,0,0));

            // Aplicamos la traslación tanto a la matriz de modelado como al punto de posición
            matrizTransformacionModelado = matrizTraslacion * matrizTransformacionModelado;
            puntoPosModelo = matrizTraslacion * glm::vec4(puntoPosModelo, 1.0f);
        }
    }

    /**
     * Añade una transformación de traslación sobre el eje X del sistema de coordenadas global a la matriz de transformación y modelado del modelo
     * @param sentido Booleano que indica si nos movemos en positivo o negativo sobre el eje
     * @param distancia Flotante que indica cuanto avanzaremos en el eje
     */
    void Modelo::trasladarEnEjeY(bool sentido, glm::float32 distancia) {
        if (sentido) {
            glm::mat4 matrizTraslacion = glm::translate(glm::vec3(0,distancia,0));

            // Aplicamos la traslación tanto a la matriz de modelado como al punto de posición
            matrizTransformacionModelado = matrizTraslacion * matrizTransformacionModelado;
            puntoPosModelo = matrizTraslacion * glm::vec4(puntoPosModelo, 1.0f);
        } else {
            glm::mat4 matrizTraslacion = glm::translate(glm::vec3(0,-distancia,0));

            // Aplicamos la traslación tanto a la matriz de modelado como al punto de posición
            matrizTransformacionModelado = matrizTraslacion * matrizTransformacionModelado;
            puntoPosModelo = matrizTraslacion * glm::vec4(puntoPosModelo, 1.0f);
        }
    }

    /**
    * Añade una transformación de traslación sobre el eje X del sistema de coordenadas global a la matriz de transformación y modelado del modelo
    * @param sentido Booleano que indica si nos movemos en positivo o negativo sobre el eje
    * @param distancia Flotante que indica cuanto avanzaremos en el eje
    */
    void Modelo::trasladarEnEjeZ(bool sentido, glm::float32 distancia) {
        if (sentido) {
            glm::mat4 matrizTraslacion = glm::translate(glm::vec3(0,0, distancia));

            // Aplicamos la traslación tanto a la matriz de modelado como al punto de posición
            matrizTransformacionModelado = matrizTraslacion * matrizTransformacionModelado;
            puntoPosModelo = matrizTraslacion * glm::vec4(puntoPosModelo, 1.0f);
        } else {
            glm::mat4 matrizTraslacion = glm::translate(glm::vec3(0,0, -distancia));

            // Aplicamos la traslación tanto a la matriz de modelado como al punto de posición
            matrizTransformacionModelado = matrizTraslacion * matrizTransformacionModelado;
            puntoPosModelo = matrizTraslacion * glm::vec4(puntoPosModelo, 1.0f);
        }
    }

    /**
     * A partir del atributo nombreShaders crea el objeto ProgramShader que usaremos para renderizar el modelo
     */
    void Modelo::creaShaderProgram() {
        // Obtenemos los nombres de los shaders
        std::string nombre_vs;
        std::string nombre_fs;

        // Un nombre puede ser ../pag03-vs.glsl
        nombre_vs.append("../");
        nombre_fs.append("../");
        nombre_vs.append(nombreShaders);
        nombre_fs.append(nombreShaders);
        nombre_vs.append("-vs.glsl");
        nombre_fs.append("-fs.glsl");

        // Creamos los shaders
        programShader = new ProgramShader();
        programShader->cargarVertexShader(nombre_vs);
        programShader->cargarFragmentShader(nombre_fs);
        programShader->crearProgramShader();
    }

    /**
     * Procesa un nodo durante la carga del modelo con Assimp
     * @param nodo Puntero al nodo a procesar
     * @param escena Puntero a la escena donde nos encontramos
     */
    void Modelo::procesarNodo(aiNode *nodo, const aiScene *escena) {
        for(unsigned int i = 0; i < nodo->mNumMeshes; i++) { // if (nodo->mNumMeshes > 0) {
            // Procesamos la única mesh del nodo, pues trabajamos con modelos que tienen una sola mesh
            aiMesh *mesh = escena->mMeshes[nodo->mMeshes[i]];

            for(unsigned int j = 0; j < mesh->mNumVertices; j++) {
                // Procesamos las coordenadas de posición y normales del vector
                // Con las siguientes líneas comentadas podemos cargar la normal
                // mesh->mNormals[i].x; mesh->mNormals[i].y; mesh->mNormals[i].z;

                // Introducimos el vértice relleno en la ED de vértices
                vertices.push_back({ mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z});
            }

            // Procesamos los índices
            for(unsigned int k = 0; k < mesh->mNumFaces; k++) {
                aiFace cara = mesh->mFaces[k];
                for(unsigned int j = 0; j < cara.mNumIndices; j++)
                    indices.push_back(cara.mIndices[j]);
            }

        }

        // Hacemos lo mismo para cada hijo del nodo
        for(unsigned int i = 0; i < nodo->mNumChildren; i++) {
            procesarNodo(nodo->mChildren[i], escena);
        }

    }

    /**
     * Carga el modelo indicado en el atributo rutaOBJ usando Assimp
     */
    void Modelo::cargarModelo() {
        Assimp::Importer importador;
        const aiScene* escena = importador.ReadFile ( rutaOBJ,
        aiProcess_JoinIdenticalVertices | aiProcess_Triangulate
        | aiProcess_GenSmoothNormals);

        if(!escena || escena->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !escena->mRootNode)
        {
            // Lanzamos una excepción con la información adecuada sobre el error
            std::string mensajeError = "Error al importar el modelo: ";
            mensajeError.append(importador.GetErrorString());
            throw std::runtime_error(mensajeError);
        }

        procesarNodo(escena->mRootNode, escena);

        // ------ VAO ------
        // Creamos el VAO y lo enlazamos. A partir de aquí, todas las órdenes se referirían al VAO
        glGenVertexArrays ( 1, &idVAO );
        glBindVertexArray ( idVAO );

        // ------ VBOs ------
        // Creamos el VBO y lo enlazamos A partir de aquí, todas las órdenes se refieren él
        glGenBuffers ( 1, &idVBOcoordenadas );
        glBindBuffer ( GL_ARRAY_BUFFER, idVBOcoordenadas );

        // Entregamos la información para añadir al VBO el atributo coordenadas
        glBufferData ( GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices.front(), GL_STATIC_DRAW );

        // Indicamos a OpenGL cómo están organizados los datos del atributo coordenadas de vértice en el VBO activo
        // Usamos el índice 0 para indicar atributo coordenadas
        glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr );


        // Activamos el atributo coordenadas almacenado en el VBO
        glEnableVertexAttribArray ( 0 );

        // ------ IBO ------
        // Creamos el IBO y lo enlazamos. A partir de aquí, todas las órdenes se refieren al IBO
        glGenBuffers ( 1, &idIBO );
        glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, idIBO );

        // Entregamos los índices para llenar el IBO
        glBufferData ( GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(GLuint), &indices.front(), GL_STATIC_DRAW );
    }

    /**
     * Vuelve a renderizar el modelo, pasando los datos oportunos a los shaders
     * @param matrizCompuesta Matriz de visión y proyección a aplicar. Se combina con la de modelado del propio modelo
     * antes de pasarse como uniform a los shaders.
     */
    void Modelo::refrescarModelo(glm::mat4  matrizCompuesta) {
        // Combinamos la matriz compuesta global (cámara) con la específica de este modelo
        glm::mat4 matrizFinal = matrizCompuesta * matrizTransformacionModelado;

        // Activamos nuestro shader program
        glUseProgram ( programShader->getIdOpenGL());

        // Pasamos la matriz como uniform al vertex shader
        std::string nombreUniform = "matrizModeladoVisionProyeccion"; // Mismo nombre que hemos puesto en el shader
        GLint posicion = glGetUniformLocation ( programShader->getIdOpenGL(), nombreUniform.c_str () );

        if ( posicion != -1 )
        {  // El uniform existe y se ha podido localizar correctamente
            glUniformMatrix4fv ( posicion, 1, GL_FALSE, &matrizFinal[0][0] );
        }

        // Activamos nuestro VAO
        glBindVertexArray (idVAO);

        // Indicamos el IBO a usar
        glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, idIBO );

        // Comenzamos el proceso de rendering
        glDrawElements ( GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr );

    }
} // PAG