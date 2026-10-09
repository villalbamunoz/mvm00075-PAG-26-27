#include "ProgramShader.h"
#include <fstream>
#include <ostream>
#include <sstream>

namespace PAG {
    /**
     * Carga y compila el código fuente del vertex shader
     * @param ruta Ruta donde se encuentra el fichero de código fuente del shader. Debe tomar como referencia de
     * directorio actual a la carpeta cmake-build-debug
     */
    void ProgramShader::cargarVertexShader(std::string ruta) {
        // Cargamos desde fichero dde código fuente del vertex shader
        std::ifstream archivoVS;
        archivoVS.open ( ruta );

        if ( !archivoVS.is_open () )
        {  /* Error abriendo el archivo.
              Habrá que procesarlo convenientemente */
            throw std::runtime_error("Error al cargar el fichero de vertex shader");

        }

        /* Carga del código fuente */
        std::stringstream streamVS;
        streamVS << archivoVS.rdbuf ();
        std::string miVertexShader = streamVS.str ();

        /* Cerramos el archivo */
        archivoVS.close ();


        // Creamos y compilamos el vertex shader
        vertexShader = new Shader();
        vertexShader->setIdOpenGL(glCreateShader ( GL_VERTEX_SHADER ));
        const GLchar* fuenteVS = miVertexShader.c_str ();
        glShaderSource (vertexShader->getIdOpenGL(), 1, &fuenteVS, nullptr);
        glCompileShader (vertexShader->getIdOpenGL());

        // Comprobamos errores en el compilado del vertex shader
        GLint resultadoCompilacionVS;
        glGetShaderiv ( vertexShader->getIdOpenGL(), GL_COMPILE_STATUS, &resultadoCompilacionVS );

        if ( resultadoCompilacionVS == GL_FALSE ) {
            /* Ha habido un error en la compilación.
            Para saber qué ha pasado, tenemos que recuperar el mensaje de error de OpenGL */
            GLint tamMsj = 0;
            std::string mensaje = "Error al compilar el vertex shader: ";
            glGetShaderiv ( vertexShader->getIdOpenGL(), GL_INFO_LOG_LENGTH, &tamMsj );

            if ( tamMsj > 0 ) {
                GLchar* mensajeFormatoC = new GLchar[tamMsj];
                GLint datosEscritos = 0;
                glGetShaderInfoLog ( vertexShader->getIdOpenGL(), tamMsj, &datosEscritos
                                     , mensajeFormatoC );
                mensaje.append ( mensajeFormatoC );
                mensaje.append ( "\n" );
                delete[] mensajeFormatoC;
                mensajeFormatoC = nullptr;

                // En "mensaje" tenemos la información del error, la comunicamos a través de la excepción
                throw std::runtime_error(mensaje);
            }
        }
    }



    /**
     * Carga y compila el código fuente del fragment shader
     * @param ruta Ruta donde se encuentra el fichero de código fuente del shader. Debe tomar como referencia de
     * directorio actual a la carpeta cmake-build-debug
     */
    void ProgramShader::cargarFragmentShader(std::string ruta) {
         // Cargamos desde fichero dde código fuente del fragment shader
        std::ifstream archivoFS;
        archivoFS.open ( ruta );

        if ( !archivoFS.is_open () )
        {  /* Error abriendo el archivo.
              Habrá que procesarlo convenientemente */
            throw std::runtime_error("Error al cargar el fichero de fragment shader");

        }

        /* Carga del código fuente */
        std::stringstream streamFS;
        streamFS << archivoFS.rdbuf ();
        std::string miFragmentShader = streamFS.str ();

        /* Cerramos el archivo */
        archivoFS.close ();

        // Creamos y compilamos el fragment shader
        fragmentShader = new Shader();
        fragmentShader->setIdOpenGL( glCreateShader ( GL_FRAGMENT_SHADER ));
        const GLchar* fuenteFS = miFragmentShader.c_str ();
        glShaderSource(fragmentShader->getIdOpenGL(), 1, &fuenteFS, nullptr);
        glCompileShader (fragmentShader->getIdOpenGL());

        // Comprobamos errores en el compilado del fragment shader
        GLint resultadoCompilacionFS;
        glGetShaderiv ( fragmentShader->getIdOpenGL(), GL_COMPILE_STATUS, &resultadoCompilacionFS );

        if ( resultadoCompilacionFS == GL_FALSE ) {
            /* Ha habido un error en la compilación.
            Para saber qué ha pasado, tenemos que recuperar el mensaje de error de OpenGL */
            GLint tamMsj = 0;
            std::string mensaje = "Error al compilar el fragment shader: ";
            glGetShaderiv ( fragmentShader->getIdOpenGL(), GL_INFO_LOG_LENGTH, &tamMsj );

            if ( tamMsj > 0 ) {
                GLchar* mensajeFormatoC = new GLchar[tamMsj];
                GLint datosEscritos = 0;
                glGetShaderInfoLog ( fragmentShader->getIdOpenGL(), tamMsj, &datosEscritos
                                     , mensajeFormatoC );
                mensaje.append ( mensajeFormatoC );
                delete[] mensajeFormatoC;
                mensajeFormatoC = nullptr;

                // En "mensaje" tenemos la información del error, la comunicamos a través de la excepción
                throw std::runtime_error(mensaje);
            }
        }
    }

    /**
     * Enlaza el vertex shader y el fragment shader para crear el program shader
     */
    void ProgramShader::crearProgramShader() {
        // Enlazamos vertex shader y fragment shader para crear el program shader
        idOpenGL = glCreateProgram ();
        glAttachShader ( idOpenGL, vertexShader->getIdOpenGL());
        glAttachShader ( idOpenGL, fragmentShader->getIdOpenGL());
        glLinkProgram (idOpenGL);

        // Comprobación de errores de enlazado
        /*
         * A veces los drives de las tarjetas gráficas no dan toda la información que deberían sobre los errores,
         * de modo que pueden no saltar las excepciones si hay errores de enlazado.
         */
        GLint resultadoEnlazado = 0;
        glGetProgramiv ( idOpenGL, GL_LINK_STATUS, &resultadoEnlazado );

        if ( resultadoEnlazado == GL_FALSE )
        {  /* Ha habido un error en la compilación.
              Para saber qué ha pasado, tenemos que recuperar el mensaje de error de
              OpenGL */
            GLint tamMsj = 0;
            std::string mensaje = "Error en el enlazado de shaders: ";
            glGetProgramiv ( idOpenGL, GL_INFO_LOG_LENGTH, &tamMsj );

            if ( tamMsj > 0 )
            {  GLchar* mensajeFormatoC = new GLchar[tamMsj];
                GLint datosEscritos = 0;
                glGetProgramInfoLog ( idOpenGL, tamMsj, &datosEscritos
                                      , mensajeFormatoC );
                mensaje.append ( mensajeFormatoC );
                mensaje.append ( "\n" );
                delete[] mensajeFormatoC;
                mensajeFormatoC = nullptr;

                // En "mensaje" tenemos la información del error, la comunicamos a través de la excepción
                throw std::runtime_error(mensaje);
            }
        }
    }


    ProgramShader::~ProgramShader() {
        if ( vertexShader != nullptr ) {
            delete vertexShader;
            vertexShader = nullptr;
        }
        if ( fragmentShader != nullptr ) {
            delete fragmentShader;
            fragmentShader = nullptr;
        }
        if ( idOpenGL != 0 ) {
            glDeleteProgram ( idOpenGL );
        }

    }

} // PAG