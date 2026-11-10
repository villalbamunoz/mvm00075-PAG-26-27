#version 410

/* Parámetros de entrada y salida */
in salidaVS
{
    vec4 colorDifusoParaAplicar;
} entrada;

out vec4 colorFragmento;

/* Subrutinas */
subroutine vec4 fCalcularColor();
subroutine uniform fCalcularColor uMetodoColorElegido;
subroutine ( fCalcularColor )
vec4 usarColorDifuso ()
{
    return entrada.colorDifusoParaAplicar;
}

subroutine ( fCalcularColor )
vec4 usarColorPredeterminado ()
{
    return vec4 ( 0, 0, 0, 1 ); /* Colocamos el color negro */
}


void main () {
    colorFragmento = uMetodoColorElegido ();
};