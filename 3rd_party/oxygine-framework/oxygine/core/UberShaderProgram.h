#pragma once
#include "Object.h"
#include "Restorable.h"
#include <vector>

namespace oxygine
{
    class UberShaderProgramBase: public Restorable
    {
    public:
        struct shader
        {
            shader(): program(0), flags(0) {}
            ShaderProgram* program;
            int flags;
        };

        UberShaderProgramBase();
        ~UberShaderProgramBase();

        void init(const QString& fracShader, const QString& vertexShader, const QString& fracTableShader);

        void release();

        typedef Closure<void (IVideoDriver*, ShaderProgram*)> ShaderUniformsCallback;

        ShaderUniformsCallback  getShaderUniformsCallback() const { return _cb; }
        void setShaderUniformsCallback(ShaderUniformsCallback cb) {_cb = cb;}


        virtual ShaderProgram*         getShaderProgram(int flags) = 0;


    protected:
        void* _getRestorableObject() {return this;}
        void _restore(Restorable*, void*);

        QString _fracShader;
        QString _vertexShader;
        QString _fracTableShader;
        ShaderUniformsCallback _cb;

        virtual void releaseShaders() {}
    };

    class UberShaderProgram : public UberShaderProgramBase
    {
    public:
        UberShaderProgram();
        ~UberShaderProgram();

        enum
        {
            ALPHA_PREMULTIPLY = 1,
            SEPARATE_ALPHA = 1 << 1,
            MASK_R_CHANNEL = 1 << 2,
            MASK = 1 << 3,
            ADD_COLOR = 1 << 4,
            COLOR_TABLE = 1 << 5,
            _SIZE = 1 << 6
        };

        enum
        {
            SAMPLER_BASE,
            SAMPLER_TABLE,
            SAMPLER_ALPHA,
            SAMPLER_MASK,
            SAMPLER_NUM,
        };

        ShaderProgram*  getShaderProgram(int flags) override;

        void apply(IVideoDriver* driver, spNativeTexture base, spNativeTexture alpha);

    protected:
        void releaseShaders() override;
        shader _shaders[_SIZE];
    };
}
