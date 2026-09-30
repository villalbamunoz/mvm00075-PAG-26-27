// TODO Explicar la sesión 3 en el README.md
// TODO Arreglar lo de que la ventana de mensajes no sean redimensionables (seguramente sea pasar el evento de ratón a ImGUI)
#include <iostream>
#include "Renderer.h"
#include "GUI.h"

#include <bits/stdc++.h> // Necesario para generar enteros aleatorios


// IMPORTANTE: El include de GLAD debe estar siempre ANTES de el de GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>



void error_callback ( int errno, const char* desc )
{ std::string aux (desc);
    PAG::GUI::getInstancia().anadir_mensaje("Error de GLFW número ");
    PAG::GUI::getInstancia().anadir_mensaje((const char*)(errno));
    PAG::GUI::getInstancia().anadir_mensaje(": ");
    PAG::GUI::getInstancia().anadir_mensaje(aux);
    PAG::GUI::getInstancia().anadir_mensaje("\n");
}

// - Esta función callback será llamada cada vez que el área de dibujo
// OpenGL deba ser redibujada.
void window_refresh_callback ( GLFWwindow *window )
{
    PAG::Renderer::getInstancia().refrescar_escena();

    // - GLFW usa un doble buffer para que no haya parpadeo. Esta orden
    // intercambia el buffer back (que se ha estado dibujando) por el
    // que se mostraba hasta ahora front. Debe ser la última orden de
    // este callback
    glfwSwapBuffers ( window );
    }

// - Esta función callback será llamada cada vez que se cambie el tamaño
// del área de dibujo OpenGL.
void framebuffer_size_callback ( GLFWwindow *window, int width, int height ) {
    PAG::Renderer::getInstancia().callback_resize(width, height);
    PAG::GUI::getInstancia().anadir_mensaje("Callback de resize llamado \n");
}

// - Esta función callback será llamada cada vez que se pulse una tecla
// dirigida al área de dibujo OpenGL.
void key_callback ( GLFWwindow *window, int key, int scancode, int action, int mods ){
    if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS ) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    PAG::GUI::getInstancia().anadir_mensaje("Callback de tecla llamado \n");
    // TODO Aquí se añadirá la comunicación del evento de tecla con la GUI en caso de que vayamos a usarlo
}


// - Esta función callback será llamada cada vez que se pulse algún botón
// del ratón sobre el área de dibujo OpenGL.
void mouse_button_callback ( GLFWwindow *window, int button, int action, int mods ) {
    if ( action == GLFW_PRESS ){

        PAG::GUI::getInstancia().anadir_mensaje("Pulsado el botón: ");
        PAG::GUI::getInstancia().anadir_mensaje((std::to_string(button)));
        PAG::GUI::getInstancia().anadir_mensaje("\n");

        // Comunicamos el evento a la interfaz
        PAG::GUI::getInstancia().evento_raton(button, true);
    }
    else if ( action == GLFW_RELEASE )
    {
        PAG::GUI::getInstancia().anadir_mensaje("Soltado el botón: ");
        PAG::GUI::getInstancia().anadir_mensaje(std::to_string(button));
        PAG::GUI::getInstancia().anadir_mensaje("\n");

        // Comunicamos el evento a la interfaz
        PAG::GUI::getInstancia().evento_raton(button, false);
    }
}

// - Esta función callback será llamada cada vez que se mueva la rueda
// del ratón sobre el área de dibujo OpenGL.
void scroll_callback( GLFWwindow *window, double xoffset, double yoffset )
{

    PAG::GUI::getInstancia().anadir_mensaje("Movida la rueda del ratón");
    PAG::GUI::getInstancia().anadir_mensaje(std::to_string(xoffset));
    PAG::GUI::getInstancia().anadir_mensaje(" Unidades en horizontal y ");
    PAG::GUI::getInstancia().anadir_mensaje(std::to_string(yoffset));
    PAG::GUI::getInstancia().anadir_mensaje(" unidades en vertical");
    PAG::GUI::getInstancia().anadir_mensaje("\n");

    /*
 *
 * Elegimos 3 valores aleatorios entre 0 y 1 para los 3 parámetros que definen el color de fondo. Para ello:
 * rand() devuelve un entero aleatorio entre 0 y RAND_MAX. El valor de RAND_MAX depende del compilador,
 * pero cualquier número entre 0 y RAND_MAX dividido entre RAND_MAX devuelve un valor entre 0 y 1.
 * Para que el resultado sea entero, convertimos dividendo y divisor previamente a flotantes con un cast.
 * Repetimos esta operación 3 veces, una para cada parámetro RGB. Dejamos la transparencia siempre a 1 (opaco)
 * Así, cada vez que se mueva la rueda del ratón, el fondo cambia a un color aleatorio.
 */
    PAG::Renderer::getInstancia().set_color_borrado_frame_buffer( (float)(rand()) / (float)(RAND_MAX),
        (float)(rand()) / (float)(RAND_MAX),
        (float)(rand()) / (float)(RAND_MAX),
        1.0 );

    // Llamamos aquí al callback de refrescar ventana, pues acabamos de provocar un cambio.
    window_refresh_callback(window);


}






int main() {
    PAG::GUI::getInstancia().anadir_mensaje("Starting Application PAG - Prueba 01 \n");

    // Semilla para la generación de números aleatorios
    srand(time(0));

    // (GLFWerrorfun) es un cast que GLFW necesita
    glfwSetErrorCallback ( (GLFWerrorfun) error_callback );


    // - Inicializa GLFW. Es un proceso que sólo debe realizarse una vez en la aplicación
    if ( glfwInit () != GLFW_TRUE ) {
        PAG::GUI::getInstancia().anadir_mensaje("Failed to initialize GLFW \n");

    return -1;
    }
    // - Definimos las características que queremos que tenga el contexto gráfico
    // OpenGL de la ventana que vamos a crear. Por ejemplo, el número de muestras o el
    // modo Core Profile.
    glfwWindowHint ( GLFW_SAMPLES, 4 ); // - Activa antialiasing con 4 muestras.
    glfwWindowHint ( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE ); // - Esta y las 2
    glfwWindowHint ( GLFW_CONTEXT_VERSION_MAJOR, 4 ); // siguientes activan un contexto
    glfwWindowHint ( GLFW_CONTEXT_VERSION_MINOR, 3 ); // OpenGL Core Profile 4.3.
    // - Definimos el puntero para guardar la dirección de la ventana de la aplicación y
    // la creamos
    GLFWwindow *window;
    // - Tamaño, título de la ventana, en ventana y no en pantalla completa,
    // sin compartir recursos con otras ventanas.
    window = glfwCreateWindow ( 1024, 576, "PAG Introduction", nullptr, nullptr );
    // - Comprobamos si la creación de la ventana ha tenido éxito.
    if ( window == nullptr )
    {
        PAG::GUI::getInstancia().anadir_mensaje("Failed to open GLFW window \n");
    glfwTerminate (); // - Liberamos los recursos que ocupaba GLFW.
    return -2;
    }
    // - Hace que el contexto OpenGL asociado a la ventana que acabamos de crear pase a
    // ser el contexto actual de OpenGL para las siguientes llamadas a la biblioteca
    glfwMakeContextCurrent ( window );
    // - Ahora inicializamos GLAD.
    if ( !gladLoadGLLoader ( (GLADloadproc) glfwGetProcAddress ) ) {
        PAG::GUI::getInstancia().anadir_mensaje("GLAD initialization failed \n");
        glfwDestroyWindow ( window ); // - Liberamos los recursos que ocupaba GLFW.
        window = nullptr;
        glfwTerminate ();
        return -3;
        }

    PAG::GUI::getInstancia().inicializarGUI_GLFW_OpenGL(window);

    // - Interrogamos a OpenGL para que nos informe de las propiedades del contexto
    // 3D construido.
    PAG::GUI::getInstancia().anadir_mensaje(PAG::Renderer::getInstancia().get_propiedades_del_contexto());
    PAG::GUI::getInstancia().anadir_mensaje("\n");

    // - Registramos los callbacks que responderán a los eventos principales
    glfwSetWindowRefreshCallback ( window, window_refresh_callback );
    glfwSetFramebufferSizeCallback ( window, framebuffer_size_callback );
    glfwSetKeyCallback ( window, key_callback );
    glfwSetMouseButtonCallback ( window, mouse_button_callback );
    glfwSetScrollCallback ( window, scroll_callback );

    // - Establecemos un gris medio como color con el que se borrará el frame buffer.
    // No tiene por qué ejecutarse en cada paso por el ciclo de eventos.
    PAG::Renderer::getInstancia().set_color_borrado_frame_buffer(0.6, 0.6, 0.6, 1.0 );

    // - Le decimos a OpenGL que tenga en cuenta la profundidad a la hora de dibujar.
    // No tiene por qué ejecutarse en cada paso por el ciclo de eventos.
    PAG::Renderer::getInstancia().habilitar_profundidad();



    // Antes del ciclo de eventos creamos el modelo del triángulo, capturando las posibles excepciones
    try {
        PAG::Renderer::getInstancia().creaShaderProgram ();
        PAG::Renderer::getInstancia().creaModelo ();

        PAG::Renderer::getInstancia().inicializaOpenGL();
    } catch (std::exception &e) {
        PAG::GUI::getInstancia().anadir_mensaje(e.what());
    }


    // - CICLO DE EVENTOS DE LA APLICACIÓN.
    // La condición de parada es que la
    // ventana principal deba cerrarse. Por ejemplo, si el usuario pulsa el
    // botón de cerrar la ventana (la X).
    while ( !glfwWindowShouldClose ( window ) )
    {
        // - Obtiene y organiza los eventos pendientes, tales como pulsaciones de
        // teclas o de ratón, etc. Siempre al final de cada iteración del ciclo
        // de eventos y después de glfwSwapBuffers(window);
        glfwPollEvents ();

        PAG::Renderer::getInstancia().refrescar_escena();

        PAG::GUI::getInstancia().crear_frame();
        PAG::GUI::getInstancia().dibujar_ventana_mensajes();

        float* color_borrado_frame_buffer = PAG::Renderer::getInstancia().get_color_borrado_frame_buffer();

        PAG::GUI::getInstancia().dibujar_ventana_seleccion_color(color_borrado_frame_buffer);
        PAG::Renderer::getInstancia().set_color_borrado_frame_buffer(
                       color_borrado_frame_buffer[0],
                       color_borrado_frame_buffer[1],
                       color_borrado_frame_buffer[2],
                       1);


        // Último paso de renderización del frame
        PAG::GUI::getInstancia().renderizar_frame();
        glfwSwapBuffers(window);
    }

    // - Una vez terminado el ciclo de eventos, liberar recursos, etc.
    PAG::GUI::getInstancia().anadir_mensaje("Finishing application pag prueba \n");

    PAG::GUI::getInstancia().liberar_recursos_gui();
    glfwDestroyWindow ( window ); // - Cerramos y destruimos la ventana de la aplicación.
    window = nullptr;
    glfwTerminate (); // - Liberamos los recursos que ocupaba GLFW.

}