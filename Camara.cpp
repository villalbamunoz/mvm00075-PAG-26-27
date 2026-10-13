#include "Camara.h"

#include <iostream>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/epsilon.hpp>
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
     * Ejecuta un movimiento de Pan
     * @param izquierda Booleano que indica hacia donde rotamos. Si es True, rotamos de forma positiva en el eje
     * v de la cámara, como si torciésemos la cabeza a la izquierda. En caso contrario, rotamos de forma negativa
     * en el eje v de la cámara, como si la torciésemos a la derecha.
     */
    void Camara::pan(bool izquierda) {
        /*Si queremos rotar un modelo tomando como centro de rotación un punto p arbitrario en el espacio,
         *deberemos en primer lugar trasladar el modelo utilizando como vector de traslación (O - p), siendo O el
         *origen de coordenadas. Una vez hecha esta traslación, aplicaremos la rotación, y finalmente, volveremos a
         *trasladar el modelo utilizando como vector de traslación (p - O).*/
        // Matriz de traslación (O - p): Movemos el LA al origen
        glm::mat4 mTraslacion1 = glm::translate( glm::vec3(0,0,0) - puntoPosCamara);

        // Matriz de rotación
        glm::float32 radianesPan = glm::radians(gradosPan);
        if (!izquierda) {
            radianesPan = -radianesPan;
        }
        glm::mat4 mRotacion = glm::rotate(radianesPan, get_vector_v());

        // Matriz de traslación (p - 0): Devolvemos el LA a su posición inicial
        glm::mat4 mTraslacion2 = glm::translate( puntoPosCamara - glm::vec3(0,0,0));

        // Multiplicamos las matrices en el orden inverso al que se van a aplicar
        glm::mat4 mFinal = mTraslacion2 * mRotacion * mTraslacion1;

        // Aplicamos la matriz al punto LA
        puntoLA = mFinal * glm::vec4(puntoLA, 1.0f);

    }

    /**
     * Ejecuta un movimiento de Tilt
     * @param arriba Booleano que indica hacia donde rotamos. Si es True, rotamos de forma positiva en el eje
     * u de la cámara, como si alzásemos la cabeza hacia arriba. En caso contrario, rotamos de forma negativa
     * en el eje u de la cámara, como si la agachásemos hacia abajo.
     */
    void Camara::tilt(bool arriba) {
        /*Si queremos rotar un modelo tomando como centro de rotación un punto p arbitrario en el espacio,
         *deberemos en primer lugar trasladar el modelo utilizando como vector de traslación (O - p), siendo O el
         *origen de coordenadas. Una vez hecha esta traslación, aplicaremos la rotación, y finalmente, volveremos a
         *trasladar el modelo utilizando como vector de traslación (p - O).*/
        // Matriz de traslación (O - p): Movemos el LA al origen
        glm::mat4 mTraslacion1 = glm::translate( glm::vec3(0,0,0) - puntoPosCamara);

        // Matriz de rotación
        glm::float32 radianesTilt = glm::radians(gradosTilt);
        if (!arriba) {
            radianesTilt = -radianesTilt;
        }
        glm::mat4 matrizRotacion = glm::rotate(radianesTilt, get_vector_u());

        // Matriz de traslación (p - 0): Devolvemos el LA a su posición inicial
        glm::mat4 matrizTraslacion2 = glm::translate( puntoPosCamara - glm::vec3(0,0,0));

        // Multiplicamos las matrices en el orden inverso al que se van a aplicar
        glm::mat4 matrizFinal = matrizTraslacion2 * matrizRotacion * mTraslacion1;

        // Aplicamos la matriz al punto LA
        puntoLA = matrizFinal * glm::vec4(puntoLA, 1.0f);
    }

    /**
     * Ejecuta un movimiento Dolly
     * @param direccion Opción del enumerado OpcionesDolly que indica cuál de los cuatro movimientos haremos: positivo
     * en X, negativo en X, positivo en Z o negativo en Z.
     */
    void Camara::dolly(OpcionesDolly direccion) {
        // Calculamos la matriz de traslación
        glm::mat4 matrizTraslacion;

        switch (direccion){
            case xPos:
                matrizTraslacion = glm::translate(glm::vec3(distDolly,0,0));
                break;
            case xNeg:
                matrizTraslacion = glm::translate(glm::vec3(-distDolly,0,0));
                break;
            case zPos:
                matrizTraslacion = glm::translate(glm::vec3(0,0,distDolly));
                break;
            case zNeg:
                matrizTraslacion = glm::translate(glm::vec3(0,0,-distDolly));
                break;

        }

        // Aplicamos la matriz de traslación a los puntos LA y posición de la cámara
        puntoLA = matrizTraslacion * glm::vec4(puntoLA, 1.0f);
        puntoPosCamara = matrizTraslacion * glm::vec4(puntoPosCamara, 1.0f);
    }

    /**
     * Ejecuta un movimiento Crane
     * @param arriba: Booleano que indica si nos movemos positivamente en el eje Y (subir) o negativamente (bajar)
     */
    void Camara::crane(bool arriba) {
        // Calculamos la matriz de traslación
        glm::mat4 matrizTraslacion;
        if (arriba) {
            matrizTraslacion = glm::translate(glm::vec3(0,distCrane,0));
        } else {
            matrizTraslacion = glm::translate(glm::vec3(0,-distCrane,0));
        }

        // Aplicamos la matriz de traslación a los puntos LA y posición de la cámara
        puntoLA = matrizTraslacion * glm::vec4(puntoLA, 1.0f);
        puntoPosCamara = matrizTraslacion * glm::vec4(puntoPosCamara, 1.0f);
    }

    /**
     * Ejecuta un movimiento Orbit
     * @param opcion: Opción del enumerado OpcionesOrbit que indica qué tipo de órbita realizamos: positiva/negativa
     * en el eje u (latitud), positiva/negativa en el eje v (longitud)
     */
    void Camara::orbit(OpcionesOrbit opcion) {
        /*Si queremos rotar un modelo tomando como centro de rotación un punto p arbitrario en el espacio,
         *deberemos en primer lugar trasladar el modelo utilizando como vector de traslación (O - p), siendo O el
         *origen de coordenadas. Una vez hecha esta traslación, aplicaremos la rotación, y finalmente, volveremos a
         *trasladar el modelo utilizando como vector de traslación (p - O).*/
        // Matriz de traslación (O - p): Movemos la cámara al origen
        glm::mat4 mTraslacion1 = glm::translate( glm::vec3(0,0,0) - puntoLA);

        // Matriz de rotación
        glm::float32 radianesOrbit = glm::radians(gradosOrbit);
        glm::mat4 matrizRotacion;
        switch (opcion) {
            case latPos:
                matrizRotacion = glm::rotate(radianesOrbit, get_vector_u());
                break;

            case latNeg:
                matrizRotacion = glm::rotate(-radianesOrbit,  get_vector_u());
                break;

            case lonPos:
                matrizRotacion = glm::rotate(radianesOrbit, get_vector_v());
                break;

            case lonNeg:
                matrizRotacion = glm::rotate(-radianesOrbit, get_vector_v());
                break;
        }


        // Matriz de traslación (p - 0): Devolvemos la cámara a su posición inicial
        glm::mat4 matrizTraslacion2 = glm::translate(puntoLA - glm::vec3(0,0,0) );

        // Multiplicamos las matrices en el orden inverso al que se van a aplicar
        glm::mat4 matrizFinal = matrizTraslacion2 * matrizRotacion * mTraslacion1;

        // Aplicamos la matriz a la posición de la cámara
        puntoPosCamara= matrizFinal * glm::vec4(puntoPosCamara, 1.0f);

    }


    /**
     * Ejecuta zoom.
     * @param abrir_angulo: Booleano que indica si aumentamos o disminuimos el ángulo de la cámara
     */
    void Camara::zoom(bool abrir_angulo) {
        if (abrir_angulo) {
            fovYGrados -= gradosZoom;
        } else {
            fovYGrados += gradosZoom;
        }
    }


    /**
     * Calcula y devuelve el vector n del sistema de coordenadas de la cámara
     */
    glm::vec3 Camara::get_vector_n() {
        /*El vector n se obtiene restando la posición de la cámara del punto la y cambiando el signo del vector resultante.
         *Por último, se normaliza el vector.*/
        return glm::normalize( -(puntoLA - puntoPosCamara));
    }

    /**
     * Calcula y devuelve el vector u del sistema de coordenadas de la cámara
     */
    glm::vec3 Camara::get_vector_u() {
        // El vector u se obtiene haciendo el producto vectorial de Y por n, y normalizando el resultado.
        /* Cuando los vectores n e Y son colineales (por ejemplo: el vector Y vale (0,1,0), y orientamos la cámara
         * para que mire completamente hacia arriba o hacia abajo), su producto vectorial es (0,0,0), y no se pueden
         * calcular el resto de vectores del sistema de coordenadas. En nuestro código tendremos que añadir
         * comprobaciones para esta situación, y resolverlas de la manera adecuada. Una forma de hacerlo es
         * sustituyendo Y por Z (0,0,1) en el producto vectorial*/

        glm::bvec3 vector_bool = glm::epsilonEqual({0,1,0}, get_vector_n(), 0.005f);

        if (glm::all(vector_bool)) {
            // Ambos vectores son iguales componente a componente, usamos Z en vez de Y para el producto vectorial
            return glm::normalize( glm::cross({0,0,1}, get_vector_n()));
        }
        // Si no son iguales, devolvemos el cálculo normal
        return glm::normalize( glm::cross({0,1,0}, get_vector_n()));
    }

    /**
     * Calcula y devuelve el vector v del sistema de coordenadas de la cámara
     */
    glm::vec3 Camara::get_vector_v() {
        // Una vez tenemos calculados n y u, el vector v se obtiene como el producto vectorial de estos dos vectores
        return glm::cross(get_vector_n(), get_vector_u());
    }
}