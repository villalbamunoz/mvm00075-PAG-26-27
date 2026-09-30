// #include <GL/gl.h>
#include "Renderer.h"

#include <iostream>
#include <ostream>


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
        if ( idVBO != 0 )
        { glDeleteBuffers ( 1, &idVBO );
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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glPolygonMode ( GL_FRONT_AND_BACK, GL_FILL );
        glUseProgram ( idSP );
        glBindVertexArray ( idVAO );
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
        std::string miVertexShader =
        "#version 410\n"
        "layout (location = 0) in vec3 posicion;\n"
        "void main ()\n"
        "{ gl_Position = vec4 ( posicion, 1 );\n"
        "}\n";

        std::string miFragmentShader = "#version 410\n"
        "out vec4 colorFragmento;\n"
        "void main ()\n"
        "{ colorFragmento = vec4 ( 1.0, .4, .2, 1.0 );\n"
        "}\n";

        idVS = glCreateShader ( GL_VERTEX_SHADER );
        const GLchar* fuenteVS = miVertexShader.c_str ();
        glShaderSource (idVS, 1, &fuenteVS, nullptr);
        glCompileShader (idVS);
        idFS = glCreateShader ( GL_FRAGMENT_SHADER );
        const GLchar* fuenteFS = miFragmentShader.c_str ();
        glShaderSource(idFS, 1, &fuenteFS, nullptr);
        glCompileShader (idFS);
        idSP = glCreateProgram ();
        glAttachShader ( idSP, idVS);
        glAttachShader ( idSP, idFS);
        glLinkProgram (idSP);
    }


    /**
    * Méto_do para crear el VAO para el modelo a renderizar
    * @note No se incluye ninguna comprobación de errores
    */
    void PAG::Renderer::creaModelo() {
        GLfloat vertices[] = { -.5, -.5, 0,
    .5, -.5, 0,
    .0, .5, 0 };
        GLuint indices[] = { 0, 1, 2 };
        glGenVertexArrays ( 1, &idVAO );
        glBindVertexArray ( idVAO );
        glGenBuffers ( 1, &idVBO );
        glBindBuffer ( GL_ARRAY_BUFFER, idVBO );
        glBufferData ( GL_ARRAY_BUFFER, 9*sizeof(GLfloat), vertices, GL_STATIC_DRAW );
        glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), nullptr );
        glEnableVertexAttribArray ( 0 );
        glGenBuffers ( 1, &idIBO );
        glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, idIBO );
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
