#version 410
in vec4 colorParaAplicar;
layout ( location = 0 ) out vec4 colorFragmento;
void main () {
    colorFragmento = colorParaAplicar;
};