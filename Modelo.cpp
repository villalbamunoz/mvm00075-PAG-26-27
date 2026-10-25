#include "Modelo.h"

#include <iostream>
#include <stdexcept>


namespace PAG {


    /**
     * TODO Documentar
     * @param agrandar
     */
    void Modelo::escalar(bool agrandar) {
        // TODO Implementar
        std::cout << "escalar"  << std::endl;
    }

    /**
     * TODO Documentar
     */
    void Modelo::rotarSobreEjeX(bool sentido) {
        // TODO Implementar
        std::cout << "rotarSobreEjeX"  << std::endl;
    }


    /**
     * TODO Documentar
     */
    void Modelo::rotarSobreEjeY(bool sentido) {
        // TODO Implementar
        std::cout << "rotarSobreEjeY"  << std::endl;
    }

    /**
     * TODO Documentar
     */
    void Modelo::rotarSobreEjeZ(bool sentido) {
        // TODO Implementar
        std::cout << "rotarSobreEjeZ"  << std::endl;
    }

    /**
     * TODO Documentar
     */
    void Modelo::trasladarEnEjeX(bool sentido) {
        // TODO Implementar
        std::cout << "trasladarEnEjeX"  << std::endl;
    }

    /**
     * TODO Documentar
     */
    void Modelo::trasladarEnEjeY(bool sentido) {
        // TODO Implementar
        std::cout << "trasladarEnEjeY"  << std::endl;
    }

    /**
     * TODO Documentar
     */
    void Modelo::trasladarEnEjeZ(bool sentido) {
        // TODO Implementar
        std::cout << "trasladarEnEjeZ"  << std::endl;
    }

    /**
     * TODO Documentar
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
     * TODO Documentar
     * @param nodo
     * @param escena
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

        // Hacemos lo mismo para cada hijo del nodo TODO Revisar si hay que hacerlo
        for(unsigned int i = 0; i < nodo->mNumChildren; i++) {
            procesarNodo(nodo->mChildren[i], escena);
        }

    }

    /**
     * TODO Documentar
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
        // directory = path.substr(0, path.find_last_of('/')); TODO Borrar si no es necesario

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


        /*
         * Descomentar si los modelos incluyen color
         // Creamos el VBO de colores y lo enlazamos A partir de aquí, todas las órdenes se refieren él
         glGenBuffers ( 1, &idVBOcolores );
         glBindBuffer ( GL_ARRAY_BUFFER, idVBOcolores );
         // Entregamos la información para añadir al VBO el atributo color
         glBufferData ( GL_ARRAY_BUFFER, 9*sizeof(GLfloat), colores, GL_STATIC_DRAW );
        // Indicamos a OpenGL cómo están organizados los datos del atributo color de vértice en el VBO activo
        // Usamos el índice 1 para el color
        glVertexAttribPointer ( 1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), nullptr );
        // Activamos el atributo color almacenado en el VBO
        glEnableVertexAttribArray ( 1 );
        */

        // ------ IBO ------
        // Creamos el IBO y lo enlazamos. A partir de aquí, todas las órdenes se refieren al IBO
        glGenBuffers ( 1, &idIBO );
        glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, idIBO );

        // Entregamos los índices para llenar el IBO
        glBufferData ( GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(GLuint), &indices.front(), GL_STATIC_DRAW );
    }

    /**
     * TODO Documentar
     */
    void Modelo::refrescarModelo(glm::mat4  matrizCompuesta) {
        // Activamos nuestro shader program
        glUseProgram ( programShader->getIdOpenGL());

        // Pasamos la matriz como uniform al vertex shader
        std::string nombreUniform = "matrizModeladoVisionProyeccion"; // Mismo nombre que hemos puesto en el shader
        GLint posicion = glGetUniformLocation ( programShader->getIdOpenGL(), nombreUniform.c_str () );

        if ( posicion != -1 )
        {  // El uniform existe y se ha podido localizar correctamente
            glUniformMatrix4fv ( posicion, 1, GL_FALSE, &matrizCompuesta[0][0] );
        }

        // Activamos nuestro VAO
        glBindVertexArray (idVAO);

        // Indicamos el IBO a usar
        glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, idIBO );

        // Comenzamos el proceso de rendering
        glDrawElements ( GL_TRIANGLES, vertices.size(), GL_UNSIGNED_INT, nullptr );

    }
} // PAG