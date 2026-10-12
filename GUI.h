#ifndef MVM00075_PAG_GUI_H
#define MVM00075_PAG_GUI_H

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <string>

namespace PAG {

    enum MovimientoCamara {
        Ninguno, Pan, Tilt, Dolly, Crane, Orbit, Zoom
      }; 

    class GUI {
    private:
        static GUI* instancia;
        GUI();
        std::string mensajes;

    public:
        static GUI& getInstancia();
        virtual ~GUI();
        void liberar_recursos_gui();

        void inicializarGUI_GLFW_OpenGL(GLFWwindow *ventana);

        void crear_frame();
        void renderizar_frame();

        // Dibujado de ventanas
        void dibujar_ventana_mensajes();
        void dibujar_ventana_seleccion_color(float* color);
        void dibujar_ventana_entrada_texto(std::string* texto, bool *cambio);
        void dibujar_ventana_movimientos_camara(MovimientoCamara* movimiento_seleccionado);

        void anadir_mensaje(std::string mensaje);

        void evento_boton_raton(int boton, bool pulsado);
        void evento_movimiento_raton(double x, double y);



    };
} // PAG

#endif //MVM00075_PAG_GUI_H