#include "UberShaderProgram.h"
#include "NativeTexture.h"
#include "vertex.h"
#include "gl/ShaderProgramGL.h"
#include "gl/VideoDriverGLES20.h"

namespace oxygine
{
    UberShaderProgramBase::UberShaderProgramBase()
    {

    }

    void UberShaderProgramBase::init(const QString& fracShader, const QString& vertexShader, const QString& fracTableShader)
    {
        _fracShader = fracShader;
        _vertexShader = vertexShader;
        _fracTableShader = fracTableShader;
        reg(CLOSURE(this, &UberShaderProgramBase::_restore), 0);
    }

    UberShaderProgramBase::~UberShaderProgramBase()
    {
        releaseShaders();
    }

    void UberShaderProgramBase::_restore(Restorable*, void*)
    {

    }

    void UberShaderProgramBase::release()
    {
        releaseShaders();
        unreg();
    }

    ShaderProgram* UberShaderProgram::getShaderProgram(int flags)
    {
        shader& s = _shaders[flags];

        if (!s.program)
        {
            bvertex_format bformat = vertexPCT2::FORMAT;
            s.flags = flags;
            QString prepend;
            if (flags & ALPHA_PREMULTIPLY)
            {
                prepend += "#define ALPHA_PREMULTIPLY\n";
            }
            if (flags & SEPARATE_ALPHA)
            {
                prepend += "#define SEPARATE_ALPHA\n";
            }
            if (flags & MASK_R_CHANNEL)
            {
                prepend += "#define MASK_R_CHANNEL\n";
            }
            if (flags & MASK)
            {
                prepend += "#define MASK\n";
            }
            if (flags & ADD_COLOR)
            {
                prepend += "#define ADD_COLOR\n";
            }
            QString fs = prepend;
            if (flags & COLOR_TABLE)
            {
                fs += _fracTableShader;
            }
            else
            {
                fs  += _fracShader;
            }
            QString vs = prepend + _vertexShader;
            VideoDriverGLES20* driver = ((VideoDriverGLES20*)IVideoDriver::instance);
            const VertexDeclarationGL* decl = driver->getVertexDeclaration(bformat);
            ShaderProgramGL* pgl = new ShaderProgramGL(vs, fs, decl);
            driver->setShaderProgram(pgl);
            driver->setUniformInt("base_texture", UberShaderProgram::SAMPLER_BASE);
            driver->setUniformInt("colorTable", UberShaderProgram::SAMPLER_TABLE);
            driver->setUniformInt("alpha_texture", UberShaderProgram::SAMPLER_ALPHA);
            driver->setUniformInt("mask_texture", UberShaderProgram::SAMPLER_MASK);
            s.program = pgl;
        }

        return s.program;
    }

    UberShaderProgram::UberShaderProgram()
    {

    }

    UberShaderProgram::~UberShaderProgram()
    {
        releaseShaders();
    }

    void UberShaderProgram::releaseShaders()
    {
        for (int i = 0; i < _SIZE; ++i)
        {
            shader& s = _shaders[i];
            delete s.program;
            s.program = 0;
        }
    }

    void UberShaderProgram::apply(IVideoDriver* driver, spNativeTexture base, spNativeTexture alpha)
    {
        driver->setTexture(UberShaderProgram::SAMPLER_BASE, base);

        if (alpha)
        {
            driver->setTexture(UberShaderProgram::SAMPLER_ALPHA, alpha);
        }
    }
}
