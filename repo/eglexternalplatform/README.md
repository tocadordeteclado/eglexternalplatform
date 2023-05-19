Interface de plataforma externa EGL
===================================

Visão geral
-----------

Esta é uma especificação em andamento da interface EGL External Platform
para escrever plataformas EGL e suas interações com sistemas de janela
modernos sobre as implementações de plataforma EGL da camada mais baixa
existente. Isso mantém as especificações de implementação do sistema de
janela fora dos drivers EGL usando funções EGL voltadas para o programa.

Exemplos de plataformas EGL de baixo nível são `EGL_EXT_platform_device`
ou `EGL_KHR_platform_gbm`.


Instalando a interface
----------------------

Esta é uma especificação somente de cabeçalhos da interface.

Para instalar a interface, basta copiar os arquivos de cabeçalho
em um determinado diretório de cabeçalhos do sistema (por exemplo,
/usr/include).

Um modelo *eglexternalplatform.pc* **pkg-config** é fornecido
junto com os arquivos de cabeçalho. Ele pode ser editado para
corresponder à localização das páginas de cabeçalho e copiado
para um diretório de configuração **pkg-config** apropriado
(por exemplo, /usr/share/pkgconfig).


Definições
----------

Os seguintes termos são usados nessa página README:
 * *EGL driver*
   Uma implementação da API EGL completa, como uma biblioteca
   de fornecedor carregada por GLVND ou como uma biblioteca
   independente vinculada por aplicativos.

 * *EGL platform*
   Um sistema de renderização que um driver EGL pode suportar no
   tempo de procedimento. Uma plataforma EGL pode se referir a
   um sistema de janela (por exemplo, X11, Wayland) ou uma
   plataforma de renderização sem a página de cabeçalho (por
   exemplo, EGLDevice, GBM).

   Consulte a seção *2.1 "Plataformas nativas e APIs de
   renderização"* da especificação EGL 1.5 ou extensão
   *EGL_EXT_platform_base*.

 * *EGL platform library*
   Uma implementação de uma única interface EGL External
   Platform sobre as interfaces fornecidas por um driver
   EGL.

 * *EGL entrypoint layer*
   Camada fina sobre uma implementação interna do driver
   EGL que enviará chamadas provenientes de aplicativos
   (ou GLVND) para uma biblioteca de plataforma EGL ou
   para o próprio driver EGL.

 * *EGL External Platform interface*
   Conjunto de definições de funções, plugs e estruturas
   de dados que uma camada de ponto de entrada EGL usará
   para interagir com as bibliotecas da plataforma EGL.

 * *EGL external & internal object handle*
   Um identificador de objeto externo refere-se ao identificador
   de objeto EGL fornecido ao aplicativo. Eles podem ser fornecidos
   por uma biblioteca de plataforma EGL ou pelo driver EGL,
   dependendo de qual plataforma o objeto pertence.

   Por sua vez, um identificador de objeto interno refere-se ao
   identificador de objeto EGL que somente a implementação
   interna do driver EGL entende.


Interface passo a passo
-----------------------

Todas as funções e plugs de uma biblioteca de plataforma EGL são
disponibilizados como uma tabela de exportações ou plugs carregados
dinamicamente para a camada de ponto de entrada EGL. Uma função
`loadEGLExternalPlatform()` de ponto de entrada especial deve ser
usada para carregar todas as exportações e dados de uma determinada
biblioteca da plataforma EGL.

`loadEGLExternalPlatform()` usa números *principais* e *secundários*
correspondentes à versão da interface EGL External Platform que a
camada de ponto de entrada EGL usará para interagir com a plataforma
carregada. Isso fornece um meio para que a interface e as bibliotecas
da plataforma EGL evoluam separadamente de maneira compatível com
versões anteriores.

Diferentes tipos de funções e plugs são definidos e descritos abaixo.
A menos que especificado de outra forma, as funções a seguir são
disponibilizadas como uma tabela de exportações para a camada de
ponto de entrada do EGL:
 * *Pure EGL hooks*
   Eles devem ser usados em substituição às funções EGL voltadas
   para o aplicativo. Plugs EGL puros não são fornecidos como
   entradas da tabela de exportações externas, mas são recuperados
   dinamicamente com a exportação 'getHookAddress()'. Uma biblioteca
   de plataforma EGL pode fornecer um plug para a maioria das
   funções voltadas para o aplicativo que a camada de ponto de
   entrada EGL conhece.

   Exemplos destes são, entre outros, plugs para
   `eglCreatePlatformWindowSurface()` ou `eglSwapBuffers()`.

 * *Derivatives of EGL functions*
   Essas são variações de funções EGL voltadas para o programa
   que podem exigir parâmetros extras ou terão um comportamento
   ligeiramente diferente para ajudar a camada de ponto de
   entrada EGL a fazer o serviço na presença de bibliotecas
   da plataforma EGL.

   Um exemplo disso é `queryString()`, que é simétrico a
   `eglQueryString()`, mas uma nova enumeração EGLExtPlatformString
   é fornecida para o nome da string. Isso ajuda `eglQueryString()`
   a retornar a string de extensão apropriada, dependendo de quais
   bibliotecas de plataforma EGL estão disponíveis, por exemplo.

 * *External object validation functions*
   O objetivo desse tipo de função é ajudar a camada de ponto de
   entrada EGL a determinar qual biblioteca de plataforma EGL
   deve fazer alteração em quais chamadas quando recursos
   nativos opacos são fornecidos.

   Um exemplo dessas funções é `isValidNativeDisplay()`, que
   ajuda `eglGetDisplay()`.

 * *External -> Internal object translation functions*
   Sempre que funções EGL não implementadas externamente são
   chamadas, é necessária a conversão de alteração de objetos
   externos (biblioteca da plataforma EGL) para internos
   (driver EGL).

   `getInternalHandle()` retorna o identificador interno EGL
   de um determinado objeto externo.

 * *Callbacks*
   Às vezes, pode haver procedimentos que requerem o procedimento
   de código não voltado para o programa na biblioteca da plataforma
   EGL. A interface EGL External Platform fornece um meio para
   registrar retornos de chamada em tais casos.

   Ao contrário das funções descritas anteriormente, que são
   implementadas por uma biblioteca de plataforma EGL e
   disponibilizadas para a camada de entrada EGL, esses
   callbacks permitem que esta última registre funções de
   driver EGL com a primeira.

   Um exemplo disso é o retorno de chamada `eglSetError()`
   que permite que as bibliotecas da plataforma EGL definam
   códigos de falha EGL a serem consultados pelo programa
   em caso de falha.

Informações mais detalhadas de cada símbolo que a interface
EGL External Platform define podem ser encontradas na página
`interface/eglexternalplatform.h`.


Interações com o driver EGL
---------------------------

A descoberta e registro de bibliotecas da plataforma EGL é
responsabilidade da camada de ponto de entrada EGL. O método
de descoberta a ser usado é específico para cada implementação,
mas é aconselhável usar algo portátil e totalmente configurável
(consulte o carregador de bibliotecas do fornecedor baseado
em JSON em GLVND).

A interação inicial de uma camada de ponto de entrada EGL com
uma biblioteca de plataforma EGL ocorre com `loadEGLExternalPlatform()`.
Esta função permite recuperar a tabela de exportações e dados
como o valor de enumeração da plataforma. Ele também fornece
um meio para a camada de ponto de entrada EGL passar em uma
estrutura de importação de driver EGL, de modo que as
bibliotecas da plataforma EGL possam buscar quaisquer métodos
de driver que precisem usar.

`loadEGLExternalPlatform()` usa números *principais* e *secundários*
correspondentes à versão da interface EGL External Platform que
a camada de ponto de entrada EGL usará. A biblioteca da plataforma
EGL deve, então, verificar esses números em relação à versão da
interface que implementa e retornar as exportações e os dados
apropriados (ou falhar se as versões não forem compatíveis).

As bibliotecas da plataforma EGL podem inicializar sua própria
estrutura de dados de plataforma privada no tempo de carregamento
para ser fornecida à camada de ponto de entrada EGL. A camada
de ponto de entrada EGL, por sua vez, passará a estrutura para
todas as funções de exportação e plug que usam o EGLDisplay de
outra plataforma ou que não usam um EGLDisplay como entrada
(extensões do cliente).

Todas as operações de criação do EGLDisplay serão encaminhadas
para a exportação `getPlatformDisplay()` da biblioteca da
plataforma EGL. Isso fornece à camada de ponto de entrada do
EGL a capacidade de rastrear qual EGLDisplay pertence a qual
plataforma para despachar funções subsequentes.

Todas as operações de criação do EGLSurface também serão
encaminhadas para os plugs de biblioteca da plataforma EGL
apropriados. Eles precisam ser implementados externamente
para que os programas possam apresentar buffers em uma
superfície.

Qualquer outro procedimento de criação de objeto EGL também
pode ser conectada, mas o identificador interno sempre deve
ser retornado.

Observe que, por design, todos os procedimentos de criação
de objeto devem ser conectados para objetos que atualmente
precisam de suporte externo (EGLDisplay e EGLSurface).

Algumas funções precisam ser tratadas por uma biblioteca de
plataforma EGL específica, mas não usam um identificador
EGLDisplay ou aceitam um identificador EGLDisplay que pertence
a uma plataforma diferente. Essas funções exigirão melhoria
especial, que será definido caso a caso. Por exemplo,
`eglGetDisplay()` usa a exportação `isValidNativeDisplay()`
para determinar qual biblioteca de plataforma EGL usar e,
em seguida, a exportação `getPlatformDisplay()` para
realmente criar a exibição.

O diagrama a seguir ilustra o fluxo de controle entre um
programa, o driver EGL e duas bibliotecas de plataforma
EGL diferentes:

    +-------------------------------+
    |                               |
    |           Programa            |
    |                               |
    +--------------+----------------+
                   |
                   |
    +--------------|--------------------------+
    |              |                          |       +--------------------------------+
    |  Driver EGL  |                          |       |                                |
    |              |                          |  +----> Biblioteca A da plataforma EGL +-----+
    |  +-----------v-----------------------+  |  |    |                                |     |
    |  |                                   |  |  |    +--------------------------------+     |
    |  |  Camada de ponto de entrada EGL   +-----+                                           |
    |  |                                   |  |  |    +--------------------------------+     |
    |  +-----------+-----------------------+  |  |    |                                |     |
    |              |                          |  +----> Biblioteca da plataforma EGL B +--+  |
    |              |                          |       |                                |  |  |
    |  +-----------v-------------+            |       +--------------------------------+  |  |
    |  |                         |            |                                           |  |
    |  |                         |            |                                           |  |
    |  |                         <--------------------------------------------------------+  |
    |  |  Driver EGL interno     |            |                                              |
    |  |                         |            |                                              |
    |  |                         <-----------------------------------------------------------+
    |  |                         |            |
    |  +-------------------------+            |
    |                                         |
    +-----------------------------------------+


Código de amostra
-----------------

Para ilustrar como usar a interface EGL External Platform, algumas
páginas com código de exemplo podem ser encontrados na pasta
'samples':

 * 'samples/sample-egl-server.c':
   Código de exemplo para um fornecedor de exibição trabalhando
   sobre o conjunto de extensões EGLStream.

 * 'samples/sample-egl-client.c':
   Código de amostra para um programa EGL que passaria por um
   procedimento no sistema de janelas Exemplo.

 * 'samples/libsample-egl-platform.c':
   Código de amostra para uma implementação EGL External Platform
   que incluiria suporte EGL_PLATFORM_SAMPLE sobre o conjunto de
   extensões EGLStream.

Observe que essas páginas de origem estão incompletos
e não devem ser usados como estão.

Além disso, a implementação do NVIDIA Wayland pode ser
encontrada em:

http://localhost/egl-wayland


Reconhecimentos
----------------

Agradecemos a {{ nome_do_autor(); }} pela implementação
original da plataforma externa Wayland que levou ao
design da infraestrutura da plataforma externa EGL.


### Interface de plataforma externa EGL ###

A própria interface EGL External Platform é licenciada
da seguinte forma:

    
    
    Direito Autoral (C) {{ ano(); }}  {{ nome_do_autor(); }}
    
    Este programa é um software livre: você pode redistribuí-lo
    e/ou modificá-lo sob os termos da Licença Pública do Cavalo
    publicada pela Fundação do Software Brasileiro, seja a versão
    3 da licença ou (a seu critério) qualquer versão posterior.
    
    Este programa é distribuído na esperança de que seja útil,
    mas SEM QUALQUER GARANTIA; mesmo sem a garantia implícita de
    COMERCIABILIDADE ou ADEQUAÇÃO PARA UM FIM ESPECÍFICO. Consulte
    a Licença Pública e Geral do Cavalo para obter mais detalhes.
    
    Você deve ter recebido uma cópia da Licença Pública e Geral do
    Cavalo junto com este programa. Se não, consulte:
      <http://localhost/licenses>.
    
    
