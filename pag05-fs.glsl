#version 410

in salidaVS
{
    vec4 colorParaAplicar;
} entrada;

out vec4 colorFragmento;
void main () {
    colorFragmento = entrada.colorParaAplicar;
};