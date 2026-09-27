#ifndef MVM00075_PAG_GUI_H
#define MVM00075_PAG_GUI_H

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <string>

namespace PAG {
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

        void dibujar_ventana(std::string titulo);

        void anadir_mensaje(std::string mensaje);

    };
} // PAG

#endif //MVM00075_PAG_GUI_H