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
        ImGui::SetNextWindowPos ( ImVec2 (10, 450), ImGuiCond_Once );


        if ( ImGui::Begin("Color")){
            ImGui::SetWindowSize(ImVec2 (300, 50));

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
    void GUI::dibujar_ventana_entrada_nombre_shaders(std::string* texto, bool *cambio) {
        // Indicamos la posición de la ventana que vamos a dibujar
        ImGui::SetNextWindowPos ( ImVec2 (10, 325), ImGuiCond_Once );


        if ( ImGui::Begin("Program Shader")){
            ImGui::SetWindowSize(ImVec2 (300, 100));

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

    /**
     * Dibuja la ventana para seleccionar el movimiento actual de la cámara.
     * @param movimiento_seleccionado: Opción del enumerado MovimientoCamara que indica cuál es el movimiento
     * elegido actualmente, para pintar sus instrucciones en la ventana.
     */
    void GUI::dibujar_ventana_movimientos_camara(MovimientoCamara* movimiento_seleccionado) {
        // Indicamos la posición de la ventana que vamos a dibujar
        ImGui::SetNextWindowPos ( ImVec2 (700, 10), ImGuiCond_Once );


        if ( ImGui::Begin("Movimientos de cámara")){
            ImGui::SetWindowSize(ImVec2 (300, 100));

            // La ventana está desplegada
            ImGui::SetWindowFontScale ( 1.0f ); // Escalamos el texto si fuera necesario

            bool _changedM;
            int indice_seleccionado;
            // "Traducimos" de enum a entero según el orden en el que los movimientos aparecen en el selector
            switch (*movimiento_seleccionado) {
                case sinMovCamara:
                    indice_seleccionado = 0;
                    break;
                case Pan:
                    indice_seleccionado = 1;
                    break;
                case Tilt:
                    indice_seleccionado = 2;
                    break;
                case Dolly:
                    indice_seleccionado = 3;
                    break;
                case Crane:
                    indice_seleccionado = 4;
                    break;
                case Orbit:
                    indice_seleccionado = 5;
                    break;
                case Zoom:
                    indice_seleccionado = 6;
                    break;
                default:
                    indice_seleccionado = 0;
                    break;

            }

            // Pintamos el selector de movimiento
            if ( _changedM = ImGui::Combo ( "##Movimiento", (int*)&indice_seleccionado,
                "Ninguno\0Pan\0Tilt\0Dolly\0Crane\0Orbit\0Zoom\0\0" ) ) {
            }

            // Pintamos las instrucciones y marcamos el movimiento en el puntero
            switch (indice_seleccionado) {
                case 1:
                    // Pan
                    ImGui::Text("Pan: teclas A y D");
                    *movimiento_seleccionado = Pan;
                    break;
                case 2:
                    // Tilt
                    ImGui::Text("Tilt: teclas W y S");
                    *movimiento_seleccionado = Tilt;
                    break;
                case 3:
                    // Dolly
                    ImGui::Text("Dolly: teclas W, A, S y D");
                    *movimiento_seleccionado = Dolly;
                    break;
                case 4:
                    // Crane
                    ImGui::Text("Crane: teclas W y S");
                    *movimiento_seleccionado = Crane;
                    break;
                case 5:
                    // Orbit
                    ImGui::Text("Orbit: teclas W, A, S y D");
                    *movimiento_seleccionado = Orbit;
                    break;

                case 6:
                    // Zoom
                    ImGui::Text("Zoom: teclas W y S");
                    *movimiento_seleccionado = Zoom;
                    break;
                default:
                    ImGui::Text("Aquí aparecerán los controles");
                    *movimiento_seleccionado = sinMovCamara;
                    break;
            }

        }
        // Si la ventana no está desplegada, Begin devuelve false
        ImGui::End ();


    }

    /**
     * TODO Documentar
     * @param rutasModelos
     * @param hayTransformacion
     * @param transformacion
     * @param sentido
     */
    void GUI::dibujar_ventana_transformacion_modelado(std::vector<std::string> rutasModelos, bool* hayTransformacion,
                                                      TransformacionModelado* transformacionSeleccionada, bool* sentido,
                                                      std::string* rutaModeloSeleccionado
                                                      ) {
        // Indicamos la posición de la ventana que vamos a dibujar
        ImGui::SetNextWindowPos ( ImVec2 (700, 50), ImGuiCond_Once );


        if ( ImGui::Begin("Transformación de modelado")){
            // La ventana está desplegada
            ImGui::SetWindowSize(ImVec2 (300, 200));
            ImGui::SetWindowFontScale ( 1.0f ); // Escalamos el texto si fuera necesario

            // --- SELECTOR DE MODELO ---
            // Variables necesarias para el selector de modelo
            bool changedMRutaModelo;
            int indiceRutaModeloSeleccionado = -1;

            // Obtenemos el índice del modelo
            for (int i = 0; i < rutasModelos.size(); ++i) {
                if (rutasModelos[i] == *rutaModeloSeleccionado) {
                    indiceRutaModeloSeleccionado = i;
                }
            }

            // Comprobación de posible error
            if (indiceRutaModeloSeleccionado == -1) {
                throw std::runtime_error("[GUI::dibujar_ventana_transformacion_modelado] No se ha encontrado el modelo seleccionado entre los cargados");
            }

            // Para tener opciones variables en un combo no nos sirve ImGui::Combo
            if (ImGui::BeginCombo("##Modelo", rutasModelos[indiceRutaModeloSeleccionado].c_str(), 0)) {
                for (int i = 0; i < rutasModelos.size(); ++i) {
                    const bool isSelected = (indiceRutaModeloSeleccionado == i);
                    if (ImGui::Selectable(rutasModelos[i].c_str(), isSelected)) {
                        indiceRutaModeloSeleccionado = i;
                    }

                    // Set the initial focus when opening the combo
                    // (scrolling + keyboard navigation focus)
                    if (isSelected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }

            // Actualizamos el puntero con la selección hecha
            *rutaModeloSeleccionado = rutasModelos[indiceRutaModeloSeleccionado];





            // --- SELECTOR DE TRANSFORMACIÓN ---
            // Variables necesarias para el selector de transformacion
            bool changedMTransformacion;
            int indiceTransformacionSeleccionado;
            // "Traducimos" de enum a entero según el orden en el que los movimientos aparecen en el selector
            switch (*transformacionSeleccionada) {
                case escalar:
                    indiceTransformacionSeleccionado = 1;
                    break;
                case rotarSobreEjeX:
                    indiceTransformacionSeleccionado = 2;
                    break;
                case rotarSobreEjeY:
                    indiceTransformacionSeleccionado = 3;
                    break;
                case rotarSobreEjeZ:
                    indiceTransformacionSeleccionado = 4;
                    break;
                case trasladarEnEjeX:
                    indiceTransformacionSeleccionado = 5;
                    break;
                case trasladarEnEjeY:
                    indiceTransformacionSeleccionado = 6;
                    break;
                case trasladarEnEjeZ:
                    indiceTransformacionSeleccionado = 7;
                    break;
                default:
                    indiceTransformacionSeleccionado = 0;
                    break;

            }

            // Pintamos el selector de transformación
            if ( changedMTransformacion = ImGui::Combo ( "##Transformación", (int*)&indiceTransformacionSeleccionado,
                                                         "escalar\0rotarSobreEjeX\0rotarSobreEjeY\0rotarSobreEjeZ\0trasladarEnEjeX\0trasladarEnEjeY\0trasladarEnEjeZ\0\0" ) ) {
            }

            // Marcamos la transformación en el puntero
            switch (indiceTransformacionSeleccionado) {
                case 1:
                    *transformacionSeleccionada = escalar;
                    break;
                case 2:
                    *transformacionSeleccionada = rotarSobreEjeX;
                    break;
                case 3:
                    *transformacionSeleccionada = rotarSobreEjeY;
                    break;
                case 4:
                    *transformacionSeleccionada = rotarSobreEjeZ;
                    break;
                case 5:
                    *transformacionSeleccionada = trasladarEnEjeX;
                    break;
                case 6:
                    *transformacionSeleccionada = trasladarEnEjeY;
                    break;
                case 7:
                    *transformacionSeleccionada = trasladarEnEjeZ;
                    break;
                default:
                    *transformacionSeleccionada = sinTransModelado;
                    break;
            }



            // --- BOTONES PARA APLICAR TRANSFORMACIÓN ---
            // Pintamos los botones
            // Botón para ir en sentido positivo
            static int clickedPos = 0;
            if (ImGui::Button("+"))
                clickedPos++;
            if (clickedPos & 1)
            {
                // Indicamos que hay que volver a cargar los shaders
                *hayTransformacion = true;
                *sentido = true;

                // Con estas líneas podríamos incluir feedback visual por pantalla
                // ImGui::SameLine();
                // ImGui::Text("Texto feedback");
            }

            // Botón para ir en sentido negativo
            ImGui::SameLine(); // Lo colocamos en la misma línea que el anterior botón
            static int clickedNeg = 0;
            if (ImGui::Button("-"))
                clickedNeg++;
            if (clickedNeg & 1)
            {
                // Indicamos que hay que volver a cargar los shaders
                *hayTransformacion = true;
                *sentido = false;

                // Con estas líneas podríamos incluir feedback visual por pantalla
                // ImGui::SameLine();
                // ImGui::Text("Texto feedback");
            }


        }
        // Si la ventana no está desplegada, Begin devuelve false
        ImGui::End ();

    }


    /*
     * Añade una cadena de texto a los mensajes de la ventana dedicada a ello
     * @param mensaje: cadena a añadir
     */
    void GUI::anadir_mensaje(std::string mensaje) {
        mensajes.append(mensaje);
    }

    /*
     * Comunica a ImGUI los eventos de pulsado de botones del ratón
     */
    void GUI::evento_boton_raton(int boton, bool pulsado) {
        ImGuiIO& io = ImGui::GetIO ();
        io.AddMouseButtonEvent ( boton, pulsado );
    }

    /*
     * Comunica a ImGUI los eventos de movimiento del ratón
     *
     */
    void GUI::evento_movimiento_raton(double x, double y) {
        ImGuiIO& io = ImGui::GetIO ();
        io.AddMousePosEvent(x, y);
    }
} // PAG