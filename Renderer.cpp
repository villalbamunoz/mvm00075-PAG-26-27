// #include <GL/gl.h>
#include "Renderer.h"

#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>


#include "glad/glad.h"


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
        if ( idVS != 0 )
        { glDeleteShader ( idVS );
        }
        if ( idFS != 0 )
        { glDeleteShader ( idFS );
        }
        if ( idSP != 0 )
        { glDeleteProgram ( idSP );
        }
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
    * Méto_do para hacer el refresco de la escena
    */
    void Renderer::refrescar_escena ()
    {
        // Limpiamos los buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glPolygonMode ( GL_FRONT_AND_BACK, GL_FILL );
        glUseProgram ( idSP ); // Activamos nuestro shader program
        glBindVertexArray ( idVAO ); // Activamos nuestro VAO
        glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, idIBO );
        glDrawElements ( GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr );
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
        color_borrado_frame_buffer[0] = r;
        color_borrado_frame_buffer[1] = g;
        color_borrado_frame_buffer[2] = b;

        glClearColor (r,g,b,a);
    }

    /**
     * @return Un vector de 3 flotantes: las RGB del color con el que estamos limpiando el frame buffer
     */
    float *Renderer::get_color_borrado_frame_buffer() {
        return color_borrado_frame_buffer;
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
    * Méto_do para crear, compilar y enlazar el shader program
    * @note No se incluye ninguna comprobación de errores
    */
    void Renderer::creaShaderProgram() {
        // Cargamos desde ficheros las definiciones del vertex y fragment shader
        std::ifstream archivoVS;
        archivoVS.open ( "../pag03-vs.glsl" );

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


        std::ifstream archivoFS;
        archivoFS.open ( "../pag03-fs.glsl" );

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


        // Creamos y compilamos el vertex shader
        idVS = glCreateShader ( GL_VERTEX_SHADER );
        const GLchar* fuenteVS = miVertexShader.c_str ();
        glShaderSource (idVS, 1, &fuenteVS, nullptr);
        glCompileShader (idVS);

        // Comprobamos errores en el compilado del vertex shader
        GLint resultadoCompilacionVS;
        glGetShaderiv ( idVS, GL_COMPILE_STATUS, &resultadoCompilacionVS );

        if ( resultadoCompilacionVS == GL_FALSE ) {
            /* Ha habido un error en la compilación.
            Para saber qué ha pasado, tenemos que recuperar el mensaje de error de OpenGL */
            GLint tamMsj = 0;
            std::string mensaje = "Error al compilar el vertex shader: ";
            glGetShaderiv ( idVS, GL_INFO_LOG_LENGTH, &tamMsj );

            if ( tamMsj > 0 ) {
                GLchar* mensajeFormatoC = new GLchar[tamMsj];
                GLint datosEscritos = 0;
                glGetShaderInfoLog ( idVS, tamMsj, &datosEscritos
                                     , mensajeFormatoC );
                mensaje.append ( mensajeFormatoC );
                delete[] mensajeFormatoC;
                mensajeFormatoC = nullptr;

                // En "mensaje" tenemos la información del error, la comunicamos a través de la excepción
                throw std::runtime_error(mensaje);
            }
        }


        // Creamos y compilamos el fragment shader
        idFS = glCreateShader ( GL_FRAGMENT_SHADER );
        const GLchar* fuenteFS = miFragmentShader.c_str ();
        glShaderSource(idFS, 1, &fuenteFS, nullptr);
        glCompileShader (idFS);

        // Comprobamos errores en el compilado del fragment shader
        GLint resultadoCompilacionFS;
        glGetShaderiv ( idFS, GL_COMPILE_STATUS, &resultadoCompilacionFS );

        if ( resultadoCompilacionFS == GL_FALSE ) {
            /* Ha habido un error en la compilación.
            Para saber qué ha pasado, tenemos que recuperar el mensaje de error de OpenGL */
            GLint tamMsj = 0;
            std::string mensaje = "Error al compilar el fragment shader: ";
            glGetShaderiv ( idFS, GL_INFO_LOG_LENGTH, &tamMsj );

            if ( tamMsj > 0 ) {
                GLchar* mensajeFormatoC = new GLchar[tamMsj];
                GLint datosEscritos = 0;
                glGetShaderInfoLog ( idFS, tamMsj, &datosEscritos
                                     , mensajeFormatoC );
                mensaje.append ( mensajeFormatoC );
                delete[] mensajeFormatoC;
                mensajeFormatoC = nullptr;

                // En "mensaje" tenemos la información del error, la comunicamos a través de la excepción
                throw std::runtime_error(mensaje);
            }
        }




        // Enlazamos vertex shader y fragment shader para crear el program shader
        idSP = glCreateProgram ();
        glAttachShader ( idSP, idVS);
        glAttachShader ( idSP, idFS);
        glLinkProgram (idSP);

        // Comprobación de errores de enlazado
        /*
         * A veces los drives de las tarjetas gráficas no dan toda la información que deberían sobre los errores,
         * de modo que pueden no saltar las excepciones si hay errores de enlazado.
         */
        GLint resultadoEnlazado = 0;
        glGetProgramiv ( idSP, GL_LINK_STATUS, &resultadoEnlazado );

        if ( resultadoEnlazado == GL_FALSE )
        {  /* Ha habido un error en la compilación.
              Para saber qué ha pasado, tenemos que recuperar el mensaje de error de
              OpenGL */
            GLint tamMsj = 0;
            std::string mensaje = "Error en el enlazado de shaders: ";
            glGetProgramiv ( idSP, GL_INFO_LOG_LENGTH, &tamMsj );

            if ( tamMsj > 0 )
            {  GLchar* mensajeFormatoC = new GLchar[tamMsj];
                GLint datosEscritos = 0;
                glGetProgramInfoLog ( idSP, tamMsj, &datosEscritos
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
    * Méto_do para crear el VAO para el modelo a renderizar
    * @note No se incluye ninguna comprobación de errores
    */
    void PAG::Renderer::creaModelo() {
        GLfloat vertices[] = { -.5, -.5, 0,
                                .5, -.5, 0,
                                .0, .5, 0 };
        GLfloat colores[] = { 1, 0, 0,
                                0, 1, 0,
                                0, 0, 1 };
        GLuint indices[] = { 0, 1, 2 };


        // ------ VAO ------
        // Creamos el VAO y lo enlazamos. A partir de aquí, todas las órdenes se referirían al VAO
        glGenVertexArrays ( 1, &idVAO );
        glBindVertexArray ( idVAO );




        // ------ VBOs ------
        // Creamos el VBO y lo enlazamos A partir de aquí, todas las órdenes se refieren él
        glGenBuffers ( 1, &idVBOcoordenadas );
        glBindBuffer ( GL_ARRAY_BUFFER, idVBOcoordenadas );

        // Entregamos la información para añadir al VBO el atributo coordenadas
        glBufferData ( GL_ARRAY_BUFFER, 9*sizeof(GLfloat), vertices, GL_STATIC_DRAW );

        // Indicamos a OpenGL cómo están organizados los datos del atributo coordenadas de vértice en el VBO activo
        // Usamos el índice 0 para las coordenadas
        glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), nullptr );

        // Activamos el atributo coordenadas almacenado en el VBO
        glEnableVertexAttribArray ( 0 );


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



        // ------ IBO ------
        // Creamos el IBO y lo enlazamos. A partir de aquí, todas las órdenes se refieren al IBO
        glGenBuffers ( 1, &idIBO );
        glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, idIBO );

        // Entregamos los vértices para llenar el IBO
        glBufferData ( GL_ELEMENT_ARRAY_BUFFER, 3*sizeof(GLuint), indices, GL_STATIC_DRAW );
    }


    /**
    * Méto_do para inicializar los parámetros globales de OpenGL
    */
    void PAG::Renderer::inicializaOpenGL ( )
    { glClearColor ( color_borrado_frame_buffer[0],
        color_borrado_frame_buffer[1],
        color_borrado_frame_buffer[2],
        1 );
        glEnable ( GL_DEPTH_TEST );
        glEnable ( GL_MULTISAMPLE );
    }




}
