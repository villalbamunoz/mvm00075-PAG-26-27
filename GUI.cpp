#include "GUI.h"

#include <iostream>
#include <ostream>
#include <imgui_stdlib.h>
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
    GUI::~GUI() {}

    /**
     * Función para liberar los recursos de ImGUI en el momento que deseemos.
     */
    void GUI::liberar_recursos_gui() {

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

    /**
     * Inicializa los nexos entre ImGUI para GLFW y OpenGL
     * @param ventana Ventana actual de GLFW
     */
    void GUI::inicializarGUI_GLFW_OpenGL(GLFWwindow *ventana) {
        // Inicialización de ImGUI para GLFW y OpenGL
        ImGui_ImplGlfw_InitForOpenGL ( ventana, true );
        ImGui_ImplOpenGL3_Init ();
    }

    /**
     * Crea un frame de ImGUI. Debe ir al principio de la parte de interfaz del ciclo de eventos
     */
    void GUI::crear_frame() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    /**
     * Renderiza un frame de ImGUI. Debe ir al final de la parte de interfaz del ciclo de eventos
     *
     */
    void GUI::renderizar_frame() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData ( ImGui::GetDrawData() );
    }

    /**
     * Dibuja la ventana de mensajes que sustituye a la salida estándar
     *
     */
    void GUI::dibujar_ventana_mensajes() {
        // Indicamos la posición de la ventana que vamos a dibujar
        ImGui::SetNextWindowPos ( ImVec2 (10, 10), ImGuiCond_Once );


        if ( ImGui::Begin("Mensajes")){
            ImGui::SetWindowSize(ImVec2 (300, 300));

            // La ventana está desplegada
            ImGui::SetWindowFontScale ( 1.0f ); // Escalamos el texto si fuera necesario

            // Pintamos los controles
            ImGui::Text(mensajes.c_str());
        }
        // Si la ventana no está desplegada, Begin devuelve false
        ImGui::End ();
    }

    /**
     * Dibuja la ventana que contiene el widget de selección de color y asigna su input al color de
     * borrado de frame buffer
     *
     */
    void GUI::dibujar_ventana_seleccion_color(float* color) {
        // Indicamos la posición de la ventana que vamos a dibujar
        ImGui::SetNextWindowPos ( ImVec2 (500, 10), ImGuiCond_Once );


        if ( ImGui::Begin("Color")){
            ImGui::SetWindowSize(ImVec2 (300, 300));

            // La ventana está desplegada
            ImGui::SetWindowFontScale ( 1.0f ); // Escalamos el texto si fuera necesario

            // Pintamos los controles
            {
                ImGui::ColorEdit3("Color ventana", color);
            }

        }
        // Si la ventana no está desplegada, Begin devuelve false
        ImGui::End ();


    }


    /**
     * Dibuja una ventana en la que introducir el prefijo de los shaders a usar
     * @param texto Puntero a string que contendrá el texto introducido en la ventana por el usuario
     * @param cambio Puntero a bool que la función marcará a True si se ha intentado cargar nuevo nombre de shaders
     */
    void GUI::dibujar_ventana_entrada_texto(std::string* texto, bool *cambio) {
        // Indicamos la posición de la ventana que vamos a dibujar
        ImGui::SetNextWindowPos ( ImVec2 (250, 300), ImGuiCond_Once );


        if ( ImGui::Begin("Program Shader")){
            ImGui::SetWindowSize(ImVec2 (300, 300));

            // La ventana está desplegada
            ImGui::SetWindowFontScale ( 1.0f ); // Escalamos el texto si fuera necesario

            // Pintamos la caja de texto
            ImGui::InputText ( "##", texto, ImGuiInputTextFlags_AutoSelectAll );

            // Pintamos el botón
            static int clicked = 0;
            if (ImGui::Button("Cargar"))
                clicked++;
            if (clicked & 1)
            {
                // Indicamos que hay que volver a cargar los shaders
                *cambio = true;

                // Feedback visual por pantalla
                ImGui::SameLine();
                ImGui::Text("Shader cargado");
            }


        }
        // Si la ventana no está desplegada, Begin devuelve false
        ImGui::End ();

        // Si no se ha desplegado la ventana, no ha habido cambios en el color seleccionado,
        // devolvemos una señal de ello.
    }

    /*
     * Añade una cadena de texto a los mensajes de la ventana dedicada a ello
     * @param mensaje: cadena a añadir
     */
    void GUI::anadir_mensaje(std::string mensaje) {
        mensajes.append(mensaje);
    }

    /*
     * Comunica a ImGUI los eventos de ratón
     *
     */
    void GUI::evento_raton(int boton, bool pulsado) {
        ImGuiIO& io = ImGui::GetIO ();
        io.AddMouseButtonEvent ( boton, pulsado );
    }
} // PAG