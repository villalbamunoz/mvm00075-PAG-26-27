#ifndef MVM00075_PAG_MATERIAL_H
#define MVM00075_PAG_MATERIAL_H
#include <glm/glm.hpp>

namespace PAG {
    class Material {
    private:
        // TODO Dar valores predeterminados adecuados
        glm::vec3 colorAmbiente = {1,0,0};
        glm::vec3 colorDifuso = {0,1,0};
        glm::vec3 colorEspecular = {0,0,1};
        glm::float32 exponenteEspecular = 1;

    public:
        // Getters y setters
        glm::vec3* getPunteroColorAmbiente() {return &colorAmbiente;}

        void set_color_ambiente(glm::vec3 color_ambiente) {colorAmbiente = color_ambiente;}

        glm::vec3* getPunteroColorDifuso() {return &colorDifuso;}

        void set_color_difuso(glm::vec3 color_difuso) {colorDifuso = color_difuso;}

        glm::vec3* getPunteroColorEspecular() {return &colorEspecular;}

        void set_color_especular(glm::vec3 color_especular) {colorEspecular = color_especular;}

        glm::float32* getPunteroExpEspecular() {return &exponenteEspecular;}

        void set_exponente_especular(glm::float32 exponente_especular) {exponenteEspecular = exponente_especular;}
    };
} // PAG

#endif //MVM00075_PAG_MATERIAL_H