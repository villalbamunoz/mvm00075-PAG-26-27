#ifndef MVM00075_PAG_RENDERER_H
#define MVM00075_PAG_RENDERER_H

namespace PAG {
    class Renderer {
    private:
        static  Renderer* instancia;

        Renderer();

    public:
        static Renderer& getInstancia();
        virtual ~Renderer();
        void refrescar();

    };
} // PAG

#endif //MVM00075_PAG_RENDERER_H