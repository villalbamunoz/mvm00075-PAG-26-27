#ifndef MVM00075_PAG_RENDERER_H
#define MVM00075_PAG_RENDERER_H
#include <string>

namespace PAG {
    class Renderer {
    private:
        static  Renderer* instancia;
        float color_borrado_frame_buffer[3] = {0.5f, 0.5f, 0.5f};

        Renderer();


    public:
        static Renderer& getInstancia();
        virtual ~Renderer();
        void refrescar_escena();
        void callback_resize(int width, int height);
        void habilitar_profundidad();

        void set_color_borrado_frame_buffer(float r, float g, float b, float a);

        std::string get_propiedades_del_contexto();

        float* get_color_borrado_frame_buffer();



    };
} // PAG

#endif //MVM00075_PAG_RENDERER_H