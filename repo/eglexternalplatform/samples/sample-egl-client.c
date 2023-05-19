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
int main()
{
    /**
     * Configuração do sistema de janelas.
     */
    __sampleDpy = sample_display_connect("sample-display");
    __sampleSurf = sample_create_surface(__sampleDpy);

    /**
     * Terminação da configuração do sistema de janelas.
     */
    [...]

    /**
     * Configurações EGL.
     */
    __eglDpy  = eglGetPlatformDisplayEXT(EGL_PLATFORM_SAMPLE, __sampleDpy, NULL);
    __eglSurf = eglCreatePlatformWindowSurfaceEXT(__eglDpy, <EGL config>, __sampleSurf, NULL);
    __eglCtx  = eglCreateContext(__eglDpy, <EGL config>, EGL_NO_CONTEXT, NULL);

    eglMakeCurrent(__eglDpy, __eglSurf, __eglSurf, __eglCtx);

    /**
     * Terminação da configuração de EGL e OpenGL.
     */
    [...]

    while (1)
    {
        sample_dispatch_events(__sampleDpy);

        /**
         * Renderize.
         */
        [...]

        eglSwapBuffers(__eglDpy, __eglSurf);
    }

    /**
     * Limpar.
     */
    [...]
}
