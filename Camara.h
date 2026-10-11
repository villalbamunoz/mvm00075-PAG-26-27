#ifndef MVM00075_PAG_CAMARA_H
#define MVM00075_PAG_CAMARA_H

namespace PAG {
    class Camara {



        public:

        // Métodos para realizar movimientos
        void pan(bool izquierda);
        void tilt(bool arriba);
        void dolly(bool delante, bool izquierda);
        void crane(bool arriba);
        void orbit(bool izquierda);
        void zoom(bool abrir_angulo);


    };
} // PAG

#endif //MVM00075_PAG_CAMARA_H