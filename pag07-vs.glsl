#version 410
uniform mat4 matrizModeladoVision; /* Para aplicar a los vectores normales y tangentes */
uniform mat4 matrizModeladoVisionProyeccion; /* Para aplicar a los vértices */
uniform vec3 colorDifuso;

layout (location = 0) in vec3 posicion;
layout (location = 1) in vec3 color; /* TODO Quitar */

out salidaVS
{
    vec4 colorParaAplicar;
} salida;

void main () {
    salida.colorParaAplicar = vec4 ( colorDifuso, 1.0 );
    gl_Position = matrizModeladoVisionProyeccion * vec4 ( posicion, 1 );
}