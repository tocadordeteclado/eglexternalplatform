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
void handleClientConn(void *data)
{
    int fd = (int)data;
    EGLStreamKHR stream;
    GLuint texId;

    stream = eglCreateStreamFromFileDescriptorKHR(__eglDpy, fd);

    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, texId);
    eglStreamConsumerGLTextureExternalKHR(__eglDpy, stream);
    sampleSurfaceListAppend(__surfaceList, stream, texId);

    /**
     * Terminação da manipulação da conexão do cliente.
     */
    [...]
}

/**
 *
 */
void handleClientRepaint(void *data)
{
    int id = (int)data;

    forall (surf, __surfaceList)
    {
        if (surf->id == id)
        {
            eglStreamConsumerAcquireKHR(__eglDpy, surf->stream);
            break;
        }
    }

    /**
     * Terminação do tratamento de nova pintura do cliente.
     */
    [...]
}

/**
 *
 */
int main()
{
    /**
     * Configuração do sistema de janelas.
     */
    __sampleDpy = sample_create_display("sample-display");
    sample_listen_to_client_connections(__sampleDpy, handleClientConn);
    sample_listen_to_client_repaints(__sampleDpy, handleClientRepaint);

    /**
     * Terminação da configuração do sistema de janelas.
     */
    [...]

    /**
     * Configuração EGL nativa (EGLDevice, dispositivo GBM, ...).
     * Obter dispositivo/dispositivo EGL nativo.
     */
    __nativeDpy =
        /**
         * Obtenha um EGLDisplay com suporte a EGLStreams
         * que permita a criação de EGLSurfaces de
         * escaneamento.
         */
        __eglDpy = eglGetPlatformDisplayEXT(EGL_PLATFORM_NATIVE, __nativeDpy, NULL);

    /**
     *
     */
    eglInitialize(__eglDpy);

    /**
     * Crie um scanout EGLSurface em __eglDpy.
     */
    __eglSurf = ;

    /**
     *
     */
    __eglCtx = eglCreateContext(__eglDpy, <EGL config>, EGL_NO_CONTEXT, NULL);

    /**
     *
     */
    eglMakeCurrent(__eglDpy, __eglSurf, __eglSurf, __eglCtx);

    /**
     * Terminação da configuração de EGL e OpenGL.
     */
    [...]

    /**
     *
     */
    while (1)
    {
        sample_dispatch_events(__sampleDpy);

        /**
         * Compor, repintar, etc ?
         */
        [...]

        /**
         *
         */
        forall (surf, __surfaceList)
        {
            /**
             * Quadrado de superfície de desenho.
             */
            glBindTexture(GL_TEXTURE_EXTERNAL_OES, surf->texId);
        }

        /**
         * Terminação do tratamento de repintura.
         */
        [...]

        /**
         *
         */
        eglSwapBuffers(__eglDpy, eglSrf);
    }

    /**
     * Limpar.
     */
    [...]
}
