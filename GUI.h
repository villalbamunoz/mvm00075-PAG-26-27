#ifndef MVM00075_PAG_GUI_H
#define MVM00075_PAG_GUI_H

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace PAG {
    class GUI {
    private:
        static GUI* instancia;
        GUI();

    public:
        static GUI& getInstancia();
        virtual ~GUI();

        void inicializarGUI_GLFW_OpenGL(GLFWwindow *ventana);

        void crearFrame();
        void renderizarFrame();
    };
} // PAG

#endif //MVM00075_PAG_GUI_H