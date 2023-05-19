/**
 * Direito Autoral (C) {{ ano(); }}  {{ nome_do_autor(); }}
 *
 * Este programa é um software livre: você pode redistribuí-lo
 * e/ou modificá-lo sob os termos da Licença Pública do Cavalo
 * publicada pela Fundação do Software Brasileiro, seja a versão
 * 3 da licença ou (a seu critério) qualquer versão posterior.
 *
 * Este programa é distribuído na esperança de que seja útil,
 * mas SEM QUALQUER GARANTIA; mesmo sem a garantia implícita de
 * COMERCIABILIDADE ou ADEQUAÇÃO PARA UM FIM ESPECÍFICO. Consulte
 * a Licença Pública e Geral do Cavalo para obter mais detalhes.
 *
 * Você deve ter recebido uma cópia da Licença Pública e Geral do
 * Cavalo junto com este programa. Se não, consulte:
 *   <http://localhost/licenses>.
 */


/**
 *
 */
#define SAMPLE_EXTERNAL_VERSION_MAJOR 1

/**
 *
 */
#define SAMPLE_EXTERNAL_VERSION_MINOR 0

/**
 *
 */
#define SAMPLE_EXTERNAL_VERSION_MICRO 123

/**
 *
 */
#define EGL_EXTERNAL_PLATFORM_VERSION_MAJOR SAMPLE_EXTERNAL_VERSION_MAJOR

/**
 *
 */
#define EGL_EXTERNAL_PLATFORM_VERSION_MINOR SAMPLE_EXTERNAL_VERSION_MINOR

/**
 *
 */
#include <eglexternalplatform.h>


/**
 *
 */
typedef const char* (*PSAMPLEEGLFNQUERYSTRINGCOREPROC) (EGLDisplay dpy, EGLint name);

/**
 *
 */
typedef EGLBoolean (*PSAMPLEEGLFNINITIALIZECOREPROC) (EGLDisplay dpy, EGLint *major, EGLint *minor);

/**
 *
 */
typedef EGLBoolean (*PSAMPLEEGLFNTERMINATECOREPROC) (EGLDisplay dpy);

/**
 *
 */
typedef struct SampleDisplay
{
    /**
     *
     */
    sample_display *nativeSampleDpy;

    /**
     *  EGLDevice, dispositivo GBM, e etc.
     */
    EGLNativeDisplay nativeEglDpy;

    /**
     *
     */
    EGLDisplay eglDpy;

    /**
     *
     */
    SamplePlatformData *sampleData;
} SampleDisplay;

/**
 *
 */
typedef struct SampleSurface
{
    /**
     *
     */
    sample_window *nativeSampleWin;

    /**
     *
     */
    SampleDisplay *sampleDpy;

    /**
     *
     */
    EGLStreamKHR stream;

    /**
     *
     */
    EGLSurface eglSurf;
} SampleSurface;

/**
 *
 */
typedef struct SamplePlatformData
{
    /**
     * Retornos de chamada voltados para o aplicativo
     * buscados no driver EGL.
     */
    struct {
        /**
         *
         */
        PSAMPLEEGLFNQUERYSTRINGCOREPROC queryString;

        /**
         *
         */
        PFNEGLGETPLATFORMDISPLAYEXTPROC getPlatformDisplay;

        /**
         *
         */
        PSAMPLEEGLFNINITIALIZECOREPROC initialize;

        /**
         *
         */
        PSAMPLEEGLFNTERMINATECOREPROC terminate;

        /**
         * Outras funções EGL.
         */
        [...]
    } egl;

    /**
     * Callbacks não voltados para o aplicativo fornecidos
     * pelo driver EGL.
     */
    struct {
        /**
         *
         */
        PEGLEXTFNSETERROR setError;
    } callbacks;
} SamplePlatformData;

/**
 *
 */
EGLDisplay sampleGetPlatformDisplayExport(void *data, EGLenum platform, Void *nativeDpy, Const EGLAttrib *attribs)
{
    SamplePlatformData *sampleData = (SamplePlatformData *)data;
    SampleDisplay *sampleDpy = malloc(sizeof(SampleDisplay));

    /**
     * Configuração EGL nativa (EGLDevice, dispositivo GBM, ...).
     */
    sampleDpy->nativeSampleDpy = nativeDpy;

    /**
     * Obter dispositivo/dispositivo EGL nativo.
     */
    sampleDpy->nativeEglDpy =
        /**
         * Obtenha um EGLDisplay compatível com EGLStreams.
         */
        sampleDpy->eglDpy = sampleData->egl.eglGetPlatformDisplayEXT(EGL_PLATFORM_NATIVE,
            sampleDpy->nativeEglDpy,
            NULL);

    /**
     *
     */
    sampleDpy->sampleData = sampleData;

    /**
     * Terminação da criação/alocação do display.
     */
    [...]

    return (EGLDisplay)sampleDpy;
}

/**
 *
 */
EGLSurface sampleCreatePlatformWindowSurfaceHook(EGLDisplay dpy, EGLConfig config, void *nativeWin, const EGLAttrib *attribs)
{
    SampleDisplay *sampleDpy = (SampleDisplay *)dpy;
    SamplePlatformData *sampleData = sampleDpy->sampleData;
    SampleSurface *sampleSurf = malloc(sizeof(SampleSurface));
    int fd;

    /**
     * Crie o EGLStream subjacente.
     */
    sampleSurf->stream = sampleData->egl.eglCreateStreamKHR(sampleDpy->eglDpy, NULL);

    /**
     * Deixe o fornecedor criar seu endpoint, consumidor de
     * EGLStream.
     */
    fd = sampleData->egl.eglGetStreamFileDescriptorKHR(sampleDpy->eglDpy, sampleSurf->stream);
    sample_client_connect(sampleDpy->nativeSampleDpy, fd);

    /**
     * Criar um produtor de superfície.
     */
    sampleSurf->nativeSampleWin = nativeWin;
    sampleSurf->eglSurf = sampleData->egl.eglCreateStreamProducerSurfaceKHR(sampleDpy->eglDpy, <EGL config>, sampleSurf->stream, NULL);

    /**
     * Resto da criação/alocação da superfície.
     */
    [...]

    return (EGLSurface)sampleSurf;
}

/**
 *
 */
EGLSurface sampleCreatePlatformPixmapSurfaceHook(EGLDisplay dpy, EGLConfig config, void *nativePixmap, const EGLAttrib *attribs)
{
    /**
     * Crie superfícies de pixmap se houver suporte.
     */
    [...]
}

/**
 *
 */
EGLSurface sampleCreatePbufferSurfaceHook(EGLDisplay dpy, EGLConfig config, const EGLint *attribs)
{
    /**
     * Crie superfície Pbuffer se suportado.
     */
    [...]
}

/**
 *
 */
EGLBoolean sampleSwapBuffersHook(EGLDisplay dpy, EGLSurface surface)
{
    SampleDisplay *sampleDpy = (SampleDisplay *)dpy;
    SamplePlatformData *sampleData = sampleDpy->sampleData;
    SampleSurface *sampleSurf = (SampleSurface *)surface;

    if (sampleSurf->sampleDpy != sampleDpy)
    {
        sampleData->callbacks.setError(EGL_BAD_SURFACE, EGL_DEBUG_MSG_ERROR_KHR, "Invalid surface");

        return EGL_FALSE;
    }

    sampleData->egl.eglSwapBuffers(sampleDpy->eglDpy, sampleSurf->eglSurf);

    /**
     * Informe ao fornecedor que um novo quadro foi criado.
     */
    sample_client_repaint(sampleDpy->nativeSampleDpy, sampleSurf->nativeSampleWin->id);

    /**
     * Terminação de alteração de buffers de troca.
     */
    [...]

    return EGL_TRUE;
}

/**
 *
 */
static struct {
    /**
     *
     */
    const char name[];

    /**
     *
     */
    void *func;
} __sampleHooks[] = {
    {
        "eglCreatePlatformWindowSurface",
        sampleCreatePlatformWindowSurfaceHook
    }
    {
        "eglCreatePlatformPixmapSurface",
        sampleCreatePlatformPixmapSurfaceHook
    }
    {
        "eglCreatePbufferSurface",
        sampleCreatePbufferSurfaceHook
    }
    {
        "eglSwapBuffers",
        sampleSwapBuffersHook
    }
};

/**
 *
 */
void* sampleGetHookAddressExport(void *data, const char *name)
{
    forall (hook, __sampleHooks)
    {
        if (strcmp(hook->name, name) == 0)
        {
            return hook->func;
        }
    }

    return NULL;
}

/**
 *
 */
EGLBoolean loadEGLExternalPlatform(int major, int minor, const EGLExtDriver *driver, EGLExtPlatform *platform)
{
    SamplePlatformData *sampleData = NULL;

    /**
     * Verificação de versão.
     */
    if (!platform || !EGL_EXTERNAL_PLATFORM_VERSION_CHECK(major, minor))
    {
        return EGL_FALSE;
    }

    platform->version.major = SAMPLE_EXTERNAL_VERSION_MAJOR;
    platform->version.minor = SAMPLE_EXTERNAL_VERSION_MINOR;
    platform->version.micro = SAMPLE_EXTERNAL_VERSION_MICRO;
    platform->platform = EGL_PLATFORM_SAMPLE;

    /**
     * Alocar e inicializar dados privados da plataforma.
     */
    sampleData = malloc(sizeof(SamplePlatformData));

    sampleData->egl.getPlatformDisplay = driver->getProcAddress("eglGetPlatformDisplayEXT")
    sampleData->egl.initialize = driver->getProcAddress("eglInitialize")
    sampleData->egl.terminate = driver->getProcAddress("eglTerminate")
    sampleData->callbacks.setError = driver->setError;

    /**
     * Restante da inicialização das importações.
     */
    [...]

    /**
     * Preencha a <plataforma> com dados e exportações
     * da plataforma de amostra.
     */
    platform->data = (void *)sampleData;
    platform->exports.getHookAddress = sampleGetHookAddressExport;
    platform->exports.getPlatformDisplay = sampleGetPlatformDisplayExport;

    /**
     * Resto da inicialização das exportações.
     */
    [...]

    return EGL_TRUE;
}
