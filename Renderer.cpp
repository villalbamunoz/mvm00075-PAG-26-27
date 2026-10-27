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
        for (int i = 0; i < modelosEnEscena.size(); ++i) {
            modelosEnEscena[i].liberarRecursos();
        }
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
     * Añade modelos predeterminados a la escena usando el mismo méto_do al que acceder la GUI. Permite hacer pruebas
     * más rápido al tener ya modelos en pantalla como punto de partida.
     */
    void Renderer::anadirModelosPredeterminados() {
        anadirModelo("../modelos/Panther.obj", shaderPredeterminados);
        anadirModelo("../modelos/dado.obj", shaderPredeterminados);
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
    void Renderer::anadirModelo(std::string rutaModelo, std::string nombreShaders) {
        // Insertamos el modelo
        modelosEnEscena.push_back(Modelo(rutaModelo, nombreShaders));
        modelosEnEscena.back().creaShaderProgram();
        modelosEnEscena.back().cargarModelo();

        // Trasladamos el modelo en la escena de forma aleatoria
        modelosEnEscena.back().trasladarEnEjeX((rand()%2) == 0, rand()%2);
        modelosEnEscena.back().trasladarEnEjeY((rand()%2) == 0, rand()%2);
        modelosEnEscena.back().trasladarEnEjeZ((rand()%2) == 0, rand()%2);

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
                if (sentido) {
                    modelosEnEscena[indiceAfectado].escalar(sentido, cantidadEscalarPos);
                } else {
                    modelosEnEscena[indiceAfectado].escalar(sentido, cantidadEscalarNeg);

                }
                break;
            }
            case rotarSobreEjeX: {
                modelosEnEscena[indiceAfectado].rotarSobreEjeX(sentido, gradosRotacion);
                break;
            }
            case rotarSobreEjeY: {
                modelosEnEscena[indiceAfectado].rotarSobreEjeY(sentido, gradosRotacion);
                break;
            }
            case rotarSobreEjeZ: {
                modelosEnEscena[indiceAfectado].rotarSobreEjeZ(sentido, gradosRotacion);
                break;
            }
            case trasladarEnEjeX: {
                modelosEnEscena[indiceAfectado].trasladarEnEjeX(sentido, distTraslacion);
                break;
            }
            case trasladarEnEjeY: {
                modelosEnEscena[indiceAfectado].trasladarEnEjeY(sentido, distTraslacion);
                break;
            }
            case trasladarEnEjeZ: {
                modelosEnEscena[indiceAfectado].trasladarEnEjeZ(sentido, distTraslacion);
                break;
            }
            default: {
                break;
            }

        }

    }

    /**
     * Elimina de la estructura de datos de modelos al modelo con ruta especificada. Si hay varios modelos con esa
     * misma ruta, se borra al que se encuentre antes en el vector
     * @param ruta String con la ruta del modelo a borrar
     */
    void Renderer::borrarModelo(std::string ruta) {
        // Buscamos el elemento a eliminar
        std::vector<Modelo>::iterator it;
        for (it = modelosEnEscena.begin(); it != modelosEnEscena.end(); ++it) {
            if (it->getRuta() == ruta) {
                break;
            }
        }
        // Borramos el elemento en la posición indiceBuscado
        modelosEnEscena.erase(it);
    }
}
