#ifndef MVM00075_PAG_CAMARA_H
#define MVM00075_PAG_CAMARA_H

#include <glm/glm.hpp>

namespace PAG {
    enum OpcionesDolly {
        Delante, Detras, Izquierda, Derecha
      };

    class Camara {
    private:

        // Necesidades del singleton
        static Camara* instancia;
        Camara() {};

        // Definición de la cámara
        glm::vec3 puntoPosCamara = {1.0f,1.0f,1.0f}; // Posicion de la camara
        glm::vec3 puntoLA = {.0f,.0f,.0f}; // Punto donde mira la cámara (LookAt)
        glm::float32 fovYGrados = 45; // Ángulo de visión en vertical
        glm::float32 aspectRatio = 1; // Relación de aspecto
        glm::float32 distPlanoNear = 0.1f;// Distancia plano near
        glm::float32 distPlanoFar = 5.0f; // Distancia plano far


    public:
        static Camara& getInstancia();

        // Métodos para realizar movimientos
        void pan(bool izquierda);
        void tilt(bool arriba);
        void dolly(OpcionesDolly direccion);
        void crane(bool arriba);
        void orbit(bool izquierda);
        void zoom(bool abrir_angulo);

        // Getters
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
    };
} // PAG

#endif //MVM00075_PAG_CAMARA_H