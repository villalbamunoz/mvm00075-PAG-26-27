#ifndef MVM00075_PAG_ENUMERADOS_H
#define MVM00075_PAG_ENUMERADOS_H
namespace PAG {
    enum MovimientoCamara {
    sinMovCamara, Pan, Tilt, Dolly, Crane, Orbit, Zoom
    };
    enum TransformacionModelado {
    sinTransModelado, escalar, rotarSobreEjeX, rotarSobreEjeY, rotarSobreEjeZ, trasladarEnEjeX, trasladarEnEjeY, trasladarEnEjeZ
    };
}


#endif //MVM00075_PAG_ENUMERADOS_H