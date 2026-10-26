#ifndef MVM00075_PAG_CAMARA_H
#define MVM00075_PAG_CAMARA_H

#include <glm/glm.hpp>

namespace PAG {
    enum OpcionesDolly {
        xPos, xNeg, zPos, zNeg
      };

    enum OpcionesOrbit {
        latPos, latNeg, lonPos, lonNeg
    };

    class Camara {
    private:

        // Necesidades del singleton
        static Camara* instancia;
        Camara() {};

        // Definición de la cámara
        glm::vec3 puntoPosCamara = {2.0f,2.0f,2.0f}; // Posicion de la camara
        glm::vec3 puntoLA = {.0f,.0f,.0f}; // Punto donde mira la cámara (LookAt)
        glm::float32 fovYGrados = 45; // Ángulo de visión en vertical
        glm::float32 aspectRatio = 1; // Relación de aspecto
        glm::float32 distPlanoNear = 0.1f;// Distancia plano near
        glm::float32 distPlanoFar = 5.0f; // Distancia plano far

        // Parámetros que definen los movimientos
        glm::float32 gradosPan = 1.0f; // Cuánto rotamos el punto LA respecto de la posición de la cámara al hacer Pan
        glm::float32 gradosTilt = 1.0f; // Cuánto rotamos el punto LA respecto de la posición de la cámara al hacer Tilt
        glm::float32 distDolly = 0.01f; // Cuánto nos movemos en cada pulsación que hacemos Dolly
        glm::float32 distCrane = 0.01f; // Cuánto nos movemos en cada pulsación que hacemos Crane
        glm::float32 gradosOrbit = 0.5f; // Cuánto rotamos el punto de posición de la cámara respecto al LA al hacer Orbit
        glm::float32 gradosZoom = 1.0f; // Cuánto abrimos el zoom por pulsación de tecla



    public:
        static Camara& getInstancia();

        // Métodos para realizar movimientos
        void pan(bool izquierda);
        void tilt(bool arriba);
        void dolly(OpcionesDolly direccion);
        void crane(bool arriba);
        void orbit(OpcionesOrbit opcion);
        void zoom(bool abrir_angulo);

        // Getters de parámetros guardado explícitamente
        glm::vec3 get_punto_pos_camara() const {
            return puntoPosCamara;
        }

        glm::vec3 get_punto_la() const {
            return puntoLA;
        }

        glm::float32 get_fov_y_grados() const {
            return fovYGrados;
        }

        glm::float32 get_aspect_ratio() const {
            return aspectRatio;
        }

        glm::float32 get_dist_plano_near() const {
            return distPlanoNear;
        }

        glm::float32 get_dist_plano_far() const {
            return distPlanoFar;
        }

        // Getters de parámetros calculables
        glm::vec3 get_vector_n();
        glm::vec3 get_vector_u();
        glm::vec3 get_vector_v();

    };
} // PAG

#endif //MVM00075_PAG_CAMARA_H