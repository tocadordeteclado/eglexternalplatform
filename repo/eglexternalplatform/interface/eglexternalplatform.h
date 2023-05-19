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


#ifndef EGL_EXTERNAL_PLATFORM_H
#define EGL_EXTERNAL_PLATFORM_H

    #include <EGL/egl.h>
    #include <EGL/eglext.h>
    #include "eglexternalplatformversion.h"


    /**
     * Objetos de interface de plataforma externa EGL.
     */

    /**
     *
     */
    typedef struct EGLExtPlatformExports EGLExtPlatformExports;

    /**
     *
     */
    typedef struct EGLExtPlatform EGLExtPlatform;

    /**
     *
     */
    typedef struct EGLExtDriver EGLExtDriver;

    /**
     * EGLExtPlatformString enum.
     * Isso indica qual string consultar. queryString().
     */
    typedef enum {
        /**
         * Extensões do cliente da plataforma.
         * Retorna uma string de extensão incluindo as extensões
         * específicas do cliente da plataforma. Por exemplo.
         * EGL_EXT_platform_wayland em plataforma Wayland.
         *
         * O parâmetro <dpy> é EGL_NO_DISPLAY.
         */
        EGL_EXT_PLATFORM_PLATFORM_CLIENT_EXTENSIONS = 0,

        /**
         * Extensões de exibição.
         * Retorna uma string de extensão incluindo todas as
         * extensões de exibição suportadas na exibição fornecida
         * pela implementação da plataforma externa.
         */
        EGL_EXT_PLATFORM_DISPLAY_EXTENSIONS,

        /**
         *
         */
        EGL_EXT_PLATFORM_STRING_NAME_COUNT
    } EGLExtPlatformString;

    /**
     * loadEGLExternalPlatform().
     *
     * Carrega a plataforma externa EGL e retorna um objeto
     * EGLExtPlatform no qual os dados da plataforma externa
     * e as funções exportadas foram configurados corretamente
     * de acordo com os números de versão principal e
     * secundária fornecidos.
     *
     * Se uma plataforma externa compatível for localizada para
     * <major>.<minor>, EGL_TRUE será retornado e <platform>
     * será inicializado corretamente. Caso contrário,
     * EGL_FALSE é retornado e <platform> permanece inalterado.
     *
     * Uma referência à implementação EGL subjacente que deve
     * ser usada é transmitida em <driver>.
     */
    typedef EGLBoolean (*PEGLEXTFNLOADEGLEXTERNALPLATFORM) (int major, int minor, const EGLExtDriver *driver, EGLExtPlatform *platform);

    /**
     * unloadEGLExternalPlatform().
     *
     * Descarrega a plataforma externa EGL, liberando quaisquer
     * recursos associados à estrutura de dados da plataforma
     * fornecida que possam ter sido alocados e ainda não
     * liberados.
     *
     * Se todos os recursos forem liberados adequadamente, EGL_TRUE
     * será retornado e o ponteiro de dados da plataforma fornecido
     * se tornará não muito correto. Caso contrário, EGL_FALSE é
     * retornado.
     */
    typedef EGLBoolean (*PEGLEXTFNUNLOADEGLEXTERNALPLATFORM) (void *platformData);

    /**
     * getHookAddress().
     *
     * A interface da plataforma externa EGL define um
     * conjunto mínimo de funções que devem ser fornecidas
     * por qualquer plataforma externa EGL no tempo
     * loadEGLExternalPlatform().
     *
     * No entanto, a maioria das outras funções EGL podem ser
     * substituídas por uma plataforma externa EGL.
     *
     * A implementação do EGL chamará getHookAddress() para
     * recuperar qualquer função EGL adicional que a plataforma
     * externa possa implementar. Seu comportamento é comparável
     * a eglGetProcAddress().
     *
     * Retorna o endereço do plug se as funções fornecidas
     * forem implementadas; caso contrário, retorna NULL.
     */
    typedef void* (*PEGLEXTFNGETHOOKADDRESS) (void *platformData, const char *name);

    /**
     * isValidNativeDisplay().
     *
     * Função de verificação de validade para um display
     * nativo. Ele retornará EGL_TRUE se a exibição nativa
     * fornecida for válida e pertencer à implementação da
     * plataforma externa; caso contrário, retornará
     * EGL_FALSE.
     */
    typedef EGLBoolean (*PEGLEXTFNISVALIDNATIVEDISPLAY) (void *platformData, void *nativeDisplay);

    /**
     * getPlatformDisplay().
     *
     * Igual a eglGetPlatformDisplay().
     */
    typedef EGLDisplay (*PEGLEXTFNGETPLATFORMDISPLAY) (void *platformData, EGLenum platform, void *nativeDisplay, const EGLAttrib* attribs);

    /**
     * queryString().
     *
     * Semelhante a eglQueryString(), mas usa sua própria
     * enumeração como parâmetro de nome de string.
     *
     * Retorna a string de extensão apropriada que é suportada
     * pela plataforma externa. Consulte as descrições das
     * enumerações EGLExtPlatformString para obter mais detalhes.
     */
    typedef const char * (*PEGLEXTFNQUERYSTRING) (void *platformData, EGLDisplay dpy, EGLExtPlatformString name);

    /**
     * getInternalHandle().
     *
     * Função de conversão de um manipulador de objeto externo
     * EGL para seu identificador interno EGL correspondente. Ele
     * retornará o identificador de objeto EGL interno se o
     * identificador externo fornecido for válido e pertencer ao
     * EGLDisplay fornecido; caso contrário, retornará NULL.
     *
     * Observe que o tipo de identificador de objeto deve ser
     * fornecido pelo chamador. Seu valor deve ser um dos enums
     * de tipo de objeto conforme definido em EGL_KHR_debug.
     */
    typedef void* (*PEGLEXTFNGETINTERNALHANDLE) (EGLDisplay dpy, EGLenum type, void *handle);

    /**
     * getObjectLabel().
     *
     * Retorna um rótulo de objeto externo EGL anexado
     * anteriormente com eglLabelObjectKHR() de
     * EGL_KHR_debug.
     *
     * Observe que o tipo de identificador de objeto deve
     * ser fornecido pelo chamador. Seu valor deve ser um
     * dos enums de tipo de objeto conforme definido em
     * EGL_KHR_debug.
     */
    typedef void* (*PEGLEXTFNGETOBJECTLABEL) (EGLDisplay dpy, EGLenum type, void *handle);

    /**
     * Definição de EGLExtPlatformExports. Esta é a tabela de
     * exportações que uma plataforma externa deve preencher
     * e disponibilizar para a implementação do EGL usar.
     */
    struct EGLExtPlatformExports
    {
        /**
         *
         */
        PEGLEXTFNUNLOADEGLEXTERNALPLATFORM unloadEGLExternalPlatform;

        /**
         *
         */
        PEGLEXTFNGETHOOKADDRESS getHookAddress;

        /**
         *
         */
        PEGLEXTFNISVALIDNATIVEDISPLAY isValidNativeDisplay;

        /**
         *
         */
        PEGLEXTFNGETPLATFORMDISPLAY getPlatformDisplay;

        /**
         *
         */
        PEGLEXTFNQUERYSTRING queryString;

        /**
         *
         */
        PEGLEXTFNGETINTERNALHANDLE getInternalHandle;

        /**
         *
         */
        PEGLEXTFNGETOBJECTLABEL getObjectLabel;
    };

    /**
     * Definição de EGLExtPlatform. Isso é comum a
     * todas as plataformas externas.
     *
     * Campos:
     *     - major/minor: Número da versão principal/secundária
     *                    da plataforma externa. Especifique o
     *                    número da versão da interface da plataforma
     *                    externa EGL que a plataforma fornecida
     *                    implementa. Eles estão vinculados a mudanças
     *                    na interface da plataforma externa EGL.
     *
     *     - micro:       Número da micro versão da plataforma externa.
     *                    Semelhante aos números maiores/menores, mas
     *                    está vinculado a mudanças específicas na
     *                    implementação da plataforma externa.
     *
     *     - platform:    Enumeração da plataforma EGL que a plataforma
     *                    externa correspondente implementa.
     *
     *     - data:        Ponteiro opaco para dados específicos da plataforma.
     *                    No momento do carregamento da plataforma, a plataforma
     *                    externa pode inicializar sua própria estrutura de dados
     *                    para armazenar qualquer informação que possa ser
     *                    requerida por alguma função que não ocupe um EGLDisplay
     *                    ou o display pertença a outra plataforma.
     *
     *     - exports:     Tabela de exportações da plataforma externa.
     */
    struct EGLExtPlatform
    {
        /**
         *
         */
        struct {
            /**
             *
             */
            int major;

            /**
             *
             */
            int minor;

            /**
             *
             */
            int micro;
        } version;

        /**
         *
         */
        EGLenum platform;

        /**
         *
         */
        void *data;

        /**
         *
         */
        EGLExtPlatformExports exports;
    };

    /**
     * getProcAddress().
     *
     * Equivalente a eglGetProcAddress() para buscar métodos
     * EGL fornecidos por um driver EGL específico.
     */
    typedef void* (*PEGLEXTFNGETPROCADDRESS) (const char *name);

    /**
     * setError().
     *
     * Sets the last EGL error, which can be queried with eglGetError() later on. It
     * also calls the EGL_KHR_debug callback if such extension is supported by the
     * driver.
     *
     * Takes the EGL error code and both message type and string as defined in
     * EGL_KHR_debug for the debug callback function.
     */
    typedef void (*PEGLEXTFNSETERROR) (EGLint error, EGLint msgType, const char *msg);

    /**
     * debugMessage().
     *
     * Chama o retorno de chamada EGL_KHR_debug se tal
     * extensão for suportada pelo driver.
     *
     * Recebe o tipo de mensagem e a sequência conforme
     * definido em EGL_KHR_debug para a função de retorno
     * de chamada de depuração.
     */
    typedef void* (*PEGLEXTFNDEBUGMESSAGE) (EGLint msgType, const char *msg);

    /**
     * streamSwapInterval().
     *
     * Manipule swapinterval no lado do consumidor EGLStream.
     * Deve ser um noop para qualquer consumidor que não
     * apresente diretamente a um monitor.
     *
     * Toma o identificador de fluxo e um ponteiro para o
     * valor do intervalo como parâmetros.
     *
     * Retorna um dos seguintes códigos de falha do EGL:
     *     - EGL_SUCCESS:    O procedimento de configuração do
     *                       intervalo foi bem-sucedida (ou noop).
     *
     *     - EGL_BAD_MATCH:  Uma substituição de intervalo do lado do
     *                       fornecedor está em vigor. O valor de
     *                       substituição é retornado no parâmetro
     *                       <interval>.
     *
     *     - EGL_BAD_ACCESS: O procedimento de configuração do intervalo
     *                       falhou.
     */
    typedef EGLint (*PEGLEXTFNSTREAMSWAPINTERVAL) (EGLStreamKHR stream, int *interval);

    /**
     * Definição EGLExtDriver. O driver externo EGL é o
     * componente responsável por enviar chamadas EGL
     * para a implementação EGL subjacente.
     */
    struct EGLExtDriver
    {
        /**
         *
         */
        PEGLEXTFNGETPROCADDRESS getProcAddress;

        /**
         *
         */
        PEGLEXTFNSETERROR setError;

        /**
         *
         */
        PEGLEXTFNDEBUGMESSAGE debugMessage;

        /**
         *
         */
        PEGLEXTFNSTREAMSWAPINTERVAL streamSwapInterval;

        /**
         *
         */
        #if EGL_EXTERNAL_PLATFORM_HAS(DRIVER_VERSION)
            /**
             *
             */
            int major;

            /**
             *
             */
            int minor;
        #endif
    };

#endif
