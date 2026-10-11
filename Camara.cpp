#include "Camara.h"

#include <iostream>

namespace PAG {
    // Inicialización perezosa del singleton (la colocamos al principio por convención).
    Camara* Camara::instancia = nullptr;

    Camara &Camara::getInstancia() {
        // Lazy initialization: si aún no existe, lo crea
        if ( !instancia ) {
            instancia = new Camara();
        }
        return *instancia;
    }

    /**
     * TODO Documentar
     */
    void Camara::pan(bool izquierda) {
        // TODO Hacer movimiento
        std::cout << "pan" << std::endl;
    }

    /**
     * TODO Documentar
     */
    void Camara::tilt(bool arriba) {
        // TODO Hacer movimiento
        std::cout << "tilt" << std::endl;
    }

    /**
     * TODO Documentar
     */
    void Camara::dolly(OpcionesDolly direccion) {
        // TODO Hacer movimiento
        std::cout << "dolly" << std::endl;
    }

    /**
     * TODO Documentar
     */
    void Camara::crane(bool arriba) {
        // TODO Hacer movimiento
        std::cout << "crane" << std::endl;
    }

    /**
     * TODO Documentar
     */
    void Camara::orbit(bool izquierda) {
        // TODO Hacer movimiento
        std::cout << "orbit" << std::endl;
    }


    /**
     * TODO Documentar
     */
    void Camara::zoom(bool abrir_angulo) {
        // TODO Hacer movimiento
        std::cout << "zoom" << std::endl;
    }






} // PAG