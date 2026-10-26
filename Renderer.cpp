#include "Renderer.h"

#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>


#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp> // Para poder usar glm::lookAt
#include <glm/ext/matrix_clip_space.hpp> // Para poder usar glm::perspective

namespace PAG {
    // Inicialización perezosa del singleton (la colocamos al principio por convención).
    Renderer* Renderer::instancia = nullptr;

    /**
    * Constructor por defecto
    */
    Renderer::Renderer() {}

    /**
    * Destructor
    */
    Renderer::~Renderer() {
        if ( idVBOcoordenadas != 0 )
        { glDeleteBuffers ( 1, &idVBOcoordenadas );
        }
        if ( idVBOcolores != 0 )
        { glDeleteBuffers ( 1, &idVBOcolores );
        }
        if ( idIBO != 0 )
        { glDeleteBuffers ( 1, &idIBO );
        }
        if ( idVAO != 0 )
        { glDeleteVertexArrays ( 1, &idVAO );
        }


        // TODO Borrar correctamente la ED de modelos en escena (si es que es necesario hacer algo en especial)
    }

    /**
    * Consulta del objeto único de la clase
    * @return La dirección de memoria del objeto
    */
    Renderer& Renderer::getInstancia() {
        // Lazy initialization: si aún no existe, lo crea
        if ( !instancia ) {
            instancia = new Renderer();
        }
        return *instancia;
    }

    /**
    * Méto_do para hacer el refresco de la escena.
    * Toma como parámetros los necesarios para calcular las matrices de visión y proyección en perspectiva, la cual
    * proporciona a cada modelo para que las aplique.
    */
    void Renderer::refrescar_escena (glm::vec3 puntoPosCamara, glm::vec3 puntoLA, glm::vec3 upVector, glm::float32 fovYGrados,
            glm::float32 aspectRatio, glm::float32 distZNear, glm::float32 distZFar)
    {
        // Limpiamos los buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Dibujamos los modelos como malla de alambre
        glPolygonMode ( GL_FRONT_AND_BACK, GL_LINE );

        // Calculamos las matrices de visión y proyección
        glm::mat4 matrizVision = glm::lookAt(puntoPosCamara, puntoLA, {0,1,0});
        glm::mat4 matrizProyeccion = glm::perspective(glm::radians(fovYGrados), aspectRatio, distZNear, distZFar);

        // Creamos la matriz compuesta por la de visión y proyección
        glm::mat4 matrizCompuesta = matrizProyeccion * matrizVision;

        // Recorremos los modelos y refrescamos cada uno, pasando la matriz compuesta como parámetro
        for (int i = 0; i < modelosEnEscena.size(); ++i) {
            modelosEnEscena[i].refrescarModelo(matrizCompuesta);
        }


    }

    /**
    * Función para introducir en el callback de resize cada vez que se cambie el tamaño del área de dibujo OpenGL.
     * @param width Nuevo ancho tras el resize
     * @param height Nuevo alto tras el resize
     */
    void Renderer::callback_resize(int width, int height) {
        glViewport ( 0, 0, width, height );
    }


    /**
     * Función para cambiar el color de borrado del frame buffer. Guarda los datos en Renderer
     * @param r Nuevo R para el color de borrado del frame buffer
     * @param g Nuevo G para el color de borrado del frame buffer
     * @param b Nuevo B para el color de borrado del frame buffer
     * @param a Nuevo A (alfa) para el color de borrado del frame buffer. Normalmente será 1.
     */
    void Renderer::set_color_borrado_frame_buffer(float r, float g, float b, float a) {
        colorBorradoFrameBuffer[0] = r;
        colorBorradoFrameBuffer[1] = g;
        colorBorradoFrameBuffer[2] = b;

        glClearColor (r,g,b,a);
    }

    /**
     * @return Un vector de 3 flotantes: las RGB del color con el que estamos limpiando el frame buffer
     */
    float *Renderer::get_color_borrado_frame_buffer() {
        return colorBorradoFrameBuffer;
    }

    /**
     *
     * @return Un string conteniendo un resumen de las propiedades del contexto de OpenGL
     */
    std::string Renderer::get_propiedades_del_contexto() {
        std::string resultado; // (const char*)
        resultado.append((const char*)(glGetString ( GL_RENDERER )));
        resultado.append("\n");

        resultado.append((const char*)(glGetString ( GL_VENDOR )));
        resultado.append("\n");

        resultado.append((const char*)(glGetString ( GL_VERSION )));
        resultado.append("\n");

        resultado.append((const char*)(glGetString ( GL_SHADING_LANGUAGE_VERSION )));
        resultado.append("\n");

        return resultado;
    }


    void Renderer::habilitar_profundidad() {
        glEnable ( GL_DEPTH_TEST );

    }


    /**
    * TODO Documentar
    */
    void Renderer::creaShadersPrograms() {
        // Recorremos los modelos y que cada uno cree su shader program
        for (int i = 0; i < modelosEnEscena.size(); ++i) {
            modelosEnEscena[i].creaShaderProgram();
        }
    }


    /**
    * Méto_do para crear el VAO predeterminado de los modelos
    * @note No se incluye ninguna comprobación de errores
    */
    void Renderer::cargarModelosPredeterminados() {
        for (int i = 0; i < modelosEnEscena.size(); ++i) {
            modelosEnEscena[i].cargarModelo();
        }
    }

    /**
     * TODO Documentar
     */
    void Renderer::anadirModelosPredeterminados() {
        // TODO Incluir varios y esparcirlos de forma aleatoria por la escena
        Modelo modeloParaInsertar1("../modelos/Panther.obj");
        modelosEnEscena.push_back(modeloParaInsertar1);

        Modelo modeloParaInsertar2("../modelos/dado.obj");
        modelosEnEscena.push_back(modeloParaInsertar2);
    }


    /**
    * Méto_do para inicializar los parámetros globales de OpenGL
    */
    void Renderer::inicializaOpenGL ( )
    { glClearColor ( colorBorradoFrameBuffer[0],
        colorBorradoFrameBuffer[1],
        colorBorradoFrameBuffer[2],
        1 );
        glEnable ( GL_DEPTH_TEST );
        glEnable ( GL_MULTISAMPLE );
    }


    /**
     * Añade un modelo a la escena a partir de la ruta a su fichero .obj.
     */
    void Renderer::anadirModelo(std::string ruta) {
        // TODO Comprobar si hay que cambiar el tratamiento de la ruta, según nos lo de la librería para cargar ficheros
        Modelo modeloParaInsertar(ruta);
        modelosEnEscena.push_back(modeloParaInsertar);
        modelosEnEscena.back().creaShaderProgram();
        modelosEnEscena.back().cargarModelo();
        std::cout << "modelosEnEscena.size(): "<<  modelosEnEscena.size();
    }

    /**
     * Devuelve un vector de strings con las rutas a los modelos cargados actualmente
     */
    std::vector<std::string> Renderer::getRutasModelos() {
        std::vector<std::string> rutas;
        for (int i = 0; i < modelosEnEscena.size(); ++i) {
            rutas.push_back(modelosEnEscena[i].getRuta());
        }
        return rutas;
    }

    void Renderer::aplicarTransformacion(std::string rutaModelo, TransformacionModelado transformacion, bool sentido) {
        // Obtenemos el índice del modelo seleccionado
        int indiceAfectado = -1;
        for (int i = 0; i < modelosEnEscena.size(); ++i) {
            if (modelosEnEscena[i].getRuta() == rutaModelo) {
                indiceAfectado = i;
            }
        }

        // Comprobación de posible error
        if (indiceAfectado == -1) {
            throw std::runtime_error("[Renderer::aplicarTransformacion] No se ha encontrado el modelo a transformar");
        }

        // Llamamos a la transformación pedida sobre el modelo pedido
        switch (transformacion) {
            case escalar: {
                modelosEnEscena[indiceAfectado].escalar(sentido);
                break;
            }
            case   rotarSobreEjeX: {
                modelosEnEscena[indiceAfectado].rotarSobreEjeX(sentido);
                break;
            }
            case rotarSobreEjeY: {
                modelosEnEscena[indiceAfectado].rotarSobreEjeY(sentido);
                break;
            }
            case rotarSobreEjeZ: {
                modelosEnEscena[indiceAfectado].rotarSobreEjeZ(sentido);
                break;
            }
            case trasladarEnEjeX: {
                modelosEnEscena[indiceAfectado].trasladarEnEjeX(sentido);
                break;
            }
            case trasladarEnEjeY: {
                modelosEnEscena[indiceAfectado].trasladarEnEjeY(sentido);
                break;
            }
            case trasladarEnEjeZ: {
                modelosEnEscena[indiceAfectado].trasladarEnEjeZ(sentido);
                break;
            }
            default: {
                break;
            }

        }

    }
}
