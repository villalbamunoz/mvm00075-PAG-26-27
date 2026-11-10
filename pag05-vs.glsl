#version 410
uniform mat4 matrizModeladoVision; /* Para aplicar a los vectores normales y tangentes */
uniform mat4 matrizModeladoVisionProyeccion; /* Para aplicar a los vértices */
layout (location = 0) in vec3 posicion;
layout (location = 1) in vec3 color;
out vec4 colorParaAplicar;
void main () {
    colorParaAplicar = vec4 ( color, 1.0 );
    gl_Position = matrizModeladoVisionProyeccion * vec4 ( posicion, 1 );
}