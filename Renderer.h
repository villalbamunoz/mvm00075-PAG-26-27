#ifndef MVM00075_PAG_RENDERER_H
#define MVM00075_PAG_RENDERER_H
#include <string>

namespace PAG {
    class Renderer {
    private:
        static  Renderer* instancia;

        Renderer();


    public:
        static Renderer& getInstancia();
        virtual ~Renderer();
        void refrescar_escena();
        void callback_resize(int width, int height);
        void habilitar_profundidad();

        void set_color_borrado_frame_buffer(float R, float G, float B, float A);

        std::string get_propiedades_del_contexto();



    };
} // PAG

#endif //MVM00075_PAG_RENDERER_H