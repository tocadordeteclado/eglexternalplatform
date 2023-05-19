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


#ifndef EGL_EXTERNAL_PLATFORM_VERSION_H
#define EGL_EXTERNAL_PLATFORM_VERSION_H

    /**
     * <EGL_EXTERNAL_PLATFORM_VERSION_MAJOR>.<EGL_EXTERNAL_PLATFORM_VERSION_MINOR>
     * faz a definição da versão de interface de plataforma externa do EGL.
     *
     * O includer deste arquivo pode substituir EGL_EXTERNAL_PLATFORM_VERSION_MAJOR
     * ou EGL_EXTERNAL_PLATFORM_VERSION_MINOR para construir em uma determinada
     * versão de interface de plataforma externa EGL.
     *
     * Observação: que, se apenas EGL_EXTERNAL_PLATFORM_VERSION_MAJOR
     * for substituído, o menor valor possível para
     * EGL_EXTERNAL_PLATFORM_VERSION_MINOR é levado.
     *
     * Como atualizar esses números de versão:
     *     - Se uma alteração compatível com versões anteriores
     *       for feita na interface, aumente EGL_EXTERNAL_PLATFORM_VERSION_MINOR
     *       por 1.
     *
     *     - Se a compatibilidade com versões anteriores for removido
     *       por uma alteração, aumente EGL_EXTERNAL_PLATFORM_VERSION_MAJOR
     *       por 1 e definir EGL_EXTERNAL_PLATFORM_VERSION_MINOR para 0.
     *       (mantenha este tipo de alterações no mínimo).
     */
    #if !defined(EGL_EXTERNAL_PLATFORM_VERSION_MAJOR)
        /**
         *
         */
        #define EGL_EXTERNAL_PLATFORM_VERSION_MAJOR 1

        /**
         *
         */
        #if !defined(EGL_EXTERNAL_PLATFORM_VERSION_MINOR)
            /**
             *
             */
            #define EGL_EXTERNAL_PLATFORM_VERSION_MINOR 1
        #endif
    #elif !defined(EGL_EXTERNAL_PLATFORM_VERSION_MINOR)
        /**
         *
         */
        #define EGL_EXTERNAL_PLATFORM_VERSION_MINOR 0
    #endif

    /**
     * EGL_EXTERNAL_PLATFORM_VERSION_CMP.
     *
     * Macro auxiliar para comparar dois números de versão
     * diferentes. Avalia como verdadeiro se <_MAJOR1_>.<_MINOR1_>
     * é compatível com <_MAJOR2_>.<_MINOR2_>.
     */
    #define EGL_EXTERNAL_PLATFORM_VERSION_CMP(_MAJOR1_, _MINOR1_, _MAJOR2_, _MINOR2_) \
        (((_MAJOR1_) == (_MAJOR2_)) && ((_MINOR1_) >= (_MINOR2_)))

    /**
     * EGL_EXTERNAL_PLATFORM_VERSION_CHECK.
     *
     * Macro auxiliar para verificar se a versão atual da
     * interface da plataforma externa EGL é compatível com
     * o número de versão fornecido <_MAJOR_>.<_MINOR_>.
     */
    #define EGL_EXTERNAL_PLATFORM_VERSION_CHECK(_MAJOR_, _MINOR_) \
        EGL_EXTERNAL_PLATFORM_VERSION_CMP(EGL_EXTERNAL_PLATFORM_VERSION_MAJOR, \
            EGL_EXTERNAL_PLATFORM_VERSION_MINOR, \
            _MAJOR_, \
            _MINOR_)

    /**
     * EGL_EXTERNAL_PLATFORM_HAS.
     *
     * Macro auxiliar para verificar se a versão atual da
     * interface da plataforma externa EGL implementa o
     * recurso fornecido <_FEATURE_>.
     */
    #define EGL_EXTERNAL_PLATFORM_HAS(_FEATURE_) \
        EGL_EXTERNAL_PLATFORM_VERSION_CHECK(EGL_EXTERNAL_PLATFORM_ ## _FEATURE_ ## _SINCE_MAJOR, \
            EGL_EXTERNAL_PLATFORM_ ## _FEATURE_ ## _SINCE_MINOR)

    /**
     * EGL_EXTERNAL_PLATFORM_SUPPORTS.
     *
     * Macro auxiliar para verificar se o número de versão
     * da interface de plataforma externa EGL fornecido
     * <_MAJOR_>.<_MINOR_> suporta o recurso fornecido
     * <_FEATURE_>.
     */
    #define EGL_EXTERNAL_PLATFORM_SUPPORTS(_MAJOR_, _MINOR_, _FEATURE_) \
        EGL_EXTERNAL_PLATFORM_VERSION_CMP(_MAJOR_, _MINOR_, \
            EGL_EXTERNAL_PLATFORM_ ## _FEATURE_ ## _SINCE_MAJOR, \
            EGL_EXTERNAL_PLATFORM_ ## _FEATURE_ ## _SINCE_MINOR)

    /**
     * Lista de recursos suportados.
     *
     * Sempre que um novo recurso/função é adicionado à
     * interface da plataforma externa EGL, junto com o
     * aumento do número da versão correspondente, um par
     * de números
     * <EGL_EXTERNAL_PLATFORM_"FEATURE NAME"_SINCE_MAJOR>.<EGL_EXTERNAL_PLATFORM_"FEATURE NAME"_SINCE_MINOR>
     * deve ser adicionado.
     *
     * Todos os novos símbolos e usos do novo recurso/função
     * devem ser protegidos com EGL_EXTERNAL_PLATFORM_HAS(<nome-do-recurso>).
     *
     * Além disso, qualquer implementação de plataforma
     * externa que suporte o novo recurso/função também
     * deve proteger a inicialização de exportação
     * correspondente na função 'loadEGLExternalPlatform()'
     * com EGL_EXTERNAL_PLATFORM_SUPPORTS(<major>, <minor>, <feature-name>)
     * usando o número de versão fornecido.
     *
     * Exemplo:
     *     Em eglexternalplatformversion.h:
     *         #define EGL_EXTERNAL_PLATFORM_EXEMPLO_SINCE_MAJOR 1
     *         #define EGL_EXTERNAL_PLATFORM_EXEMPLO_SINCE_MINOR 0
     *
     *     Em eglexternalplatform.h:
     *         #if EGL_EXTERNAL_PLATFORM_HAS(EXEMPLO)
     *             typedef void* (*PEGLEXTFNEXAMPLE)(void *exampleAttr);
     *         #endif
     *
     *         struct EGLExtPlatformExports
     *         {
     *             [...]
     *
     *             #if EGL_EXTERNAL_PLATFORM_HAS(EXEMPLO)
     *                 PEGLEXTFNEXAMPLE exemplo;
     *             #endif
     *         };
     *
     *     Na implementação loadEGLExternalPlatform() da plataforma:
     *         EGLBoolean loadEGLExternalPlatform(int major, int minor, const EGLExtDriver *driver, EGLExtPlatform *platform)
     *         {
     *             if (!EGL_EXTERNAL_PLATFORM_VERSION_CHECK(major, minor))
     *             {
     *                 return EGL_FALSE;
     *             }
     *
     *             [...]
     *
     *             #if EGL_EXTERNAL_PLATFORM_HAS(EXAMPLE)
     *                 if (EGL_EXTERNAL_PLATFORM_SUPPORTS(major, minor, EXAMPLE))
     *                 {
     *                     platform->exports.example = exampleImpl;
     *                 }
     *             #endif
     *
     *             [...]
     *         }
     */

    /**
     * DRIVER_VERSION.
     *
     * Campos <major> e <minor> adicionados ao EGLExtDriver
     * para drivers para permitir que a plataforma externa
     * saiba a versão EGL suportada.
     */
    #define EGL_EXTERNAL_PLATFORM_DRIVER_VERSION_SINCE_MAJOR 1
    #define EGL_EXTERNAL_PLATFORM_DRIVER_VERSION_SINCE_MINOR 1

#endif
