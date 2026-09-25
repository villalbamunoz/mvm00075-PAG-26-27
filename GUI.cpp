#include "GUI.h"

namespace PAG {
    // Inicialización perezosa del singleton (la colocamos al principio por convención).
    GUI* GUI::instancia = nullptr;
    
    
    /**
    * Constructor por defecto
    */
    GUI::GUI() {
        // Inicialización de ImGUI (solo se hace una vez)
        IMGUI_CHECKVERSION();
        ImGui::CreateContext ();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    }

    /**
    * Destructor
    */
    GUI::~GUI() {
        // Liberamos recursos de ImGUI
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext ();
    }

    /**
    * Consulta del objeto único de la clase
    * @return La dirección de memoria del objeto
    */
    GUI& GUI::getInstancia() {
        // Lazy initialization: si aún no existe, lo crea
        if ( !instancia ) {
            instancia = new GUI();
        }
        return *instancia;
    }

    void GUI::inicializarGUI_GLFW_OpenGL(GLFWwindow *ventana) {
        // Inicialización de ImGUI para GLFW y OpenGL
        ImGui_ImplGlfw_InitForOpenGL ( ventana, true );
        ImGui_ImplOpenGL3_Init ();
    }

    void GUI::crearFrame() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void GUI::renderizarFrame() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData ( ImGui::GetDrawData() );
    }
} // PAG