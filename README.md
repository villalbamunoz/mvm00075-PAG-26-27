# mvm00075-PAG-26-27
Proyecto de prácticas de la asignatura Programación de Aplicaciones Gráficas de Manuel Villalba Muñoz para el curso 2026-27

## Sesión 1: Ejercicio de reflexión
Una posible solución al problema es definir `PAG::Renderer` como una clase que siga el patrón de diseño _singleton_. Tiene sentido, pues no se va a declarar más de una instancia de ella, al encargarse de una tarea tan global como gestionar el contexto gráfico de OpenGL de nuestra aplicación. Por ejemplo, si tuviéramos 2 instancias diferentes de `PAG::Renderer` no tendría diferencia el llamar al método `refrescarVentana()` de una u otra, pues ambos harían lo mismo.

En el contexto de la aplicación actual, podríamos declarar `PAG:Renderer` como una variable global en el fichero `main.cpp`, pues no hay otro fichero donde hacerlo. En cualquier caso, debe ser un lugar accesible para todos los módulos de la aplicación, pues contendría funcionalidad básica necesaria para casi cualquier acción que queramos realizar en esta. De esta forma encontramos una vía para evitar el problema de no poder registrar métodos de clases en los _callbacks_. Podemos colocar funciones sencillas de C como las que tenemos actualmente y sustituir su contenido por una llamada al método oportuno de la variable global `PAG::Renderer`. La limitación solo existe en lo que podemos o no registrar como _callback_, no en lo que estas funciones contengan. 

En cuanto a qué tan acoplada está la solución, podemos cambiar el código que se ejecutará cuando ocurran los _callbacks_ sin tener que tocar directamente las funciones registradas como tal, pues entendemos que definiremos el _singleton_ en sus respectivos ficheros separados, que serán los que editemos para cambiar el funcionamiento real de los _callbacks_.

Adjunto en "img/sesion_1_diagrama_UML.png" un diseño básico de como se vería `PAG:Renderer` en un diagrama UML. Muestra lo justo y necesario para indicar que es un _singleton_: atributo privado de la única instancia de sí misma, método público para acceder a esta y constructor privado. 

## Sesión 2


## Sesión 3
### Comprobación de errores de compilado y enlazado en shaders
Se ha introducido estas comprobaciones en el método `creaShaderProgram()` de `Renderer`. 

Empezamos con las comprobaciones de compilado. Nos basamos en el método `glGetShaderiv` para obtener diferentes datos sobre esta tarea. Toma 3 parámetros:
- ID del shader del cual queremos informarnos
- Constante que indica la información pedida
- Variable donde se guardará el resultado de la consulta. 

Con la constante `GL_COMPILE_STATUS` comprobamos si ha compilado correctamente. En caso negativo, queremos consultar más detalles. Para ello debemos primeramente obtener el tamaño del mensaje de error, usando la constante `GL_INFO_LOG_LENGTH`. En caso de tener algún mensaje (longitud mayor a 0), lo obtenemos con el método `glGetShaderInfoLog`. Lanzamos una excepción de tipo `runtime_error`, que será capturada en el `main` y mostrada por la GUI añadiendo el texto a la estructura de datos de mensajes del singleton `GUI`.  

El proceso es casi idéntico para los errores de enlazado, solo que usando métodos para el program shader: `glGetProgramiv` y `glGetProgramInfoLog` 

### Adición de colores a los vértices
He seguido un enfoque no entrelazado para añadir un nuevo VBO de colores el VAO, por considerarlo más sencillo.

La creación de ambos VBOs (coordenadas y colores) es idéntica, solo que las llamadas a los métodos se hacen sobre el identificador de cada uno que definimos como atributos de `Renderer`. Los creamso en `creaModelo()` de la siguiente forma:
- Definimos vectores estáticos para los datos. Para coordenadas, 3 flotantes (XYZ) y para colores también 3 flotantes (RGB)
- Creamos (`glGenBuffers`) y enlazamos (`glBindBuffer`) el VBO. Después de esto, todas las instrucciones sobre el VBO activo se aplicarán sobre este.
- Con `glBufferData` entregamos a OpenGL la información sobre el VBO: tipo de estructura, tamaño, dirección de memoria y forma de uso
- Con `glVertexAttribPointer` indicamos como están organizados los vectores de VBO que hemos enviado: su índice (0 para coordenadas, 1 para colores), tipo de dato, tamaño y que no se deben normalizar.
- Por último, activamos el atributo  con `glEnableVertexAttribArray` indicando su índice

Tras ello debemos hacer cambios en los shaders. En el vertex shader añadimos una variable de entrada `color`, la cual se transformará en una de salida, `colorParaAplicar`, introduciéndole un cuarto valor, el alfa. En el fragment shader tomamos ese valor de entrada y lo mandamos a la salida, `colorFragmento`. Con este proceso, el fragment shader interpolará los colores de cada vértice generando un degradado en el triángulo.

### Causa de la deformación del triángulo al redimensionar la ventana
Asumo que OpenGL no trabaja con dimensiones absolutas de la pantalla del dispositivo que estamos usando, sino que emplea coordenadas relativas a la ventana de interfaz gráfica donde se muestra la aplicación. Por ello, al deformar la ventana, el triańgulo pierde su proporción "absoluta" desde nuestra perspectiva, pero sigue manteniendo las mismas proporciones respecto a las dimensiones de la ventana. Es decir, pongamos que el triángulo tiene un vértice que dista un 20% de la ventana del borde izquierdo y un 60% del borde superior. Esas distancias en píxeles serán mayores o menores según el ratio de aspecto de la ventana, pero siempre mantendrán la proporción. Estos cambios provocan que el triángulo se deforme al hacerlo la ventana.

## Sesión 4
### Desacoplado de gestión de shaders a clases separadas
A fin de intentar facilitar un futuro escalado, he separado la gestión de los shaders en 2 clases: 
- `Shader`: Para contener información básica del shader. Por ahora solo el ID de objeto en OpenGL. Tiene un funcionamiento genérico, con el propósito de que en el futuro pueda contener más tipos de shaders 
- `ShaderProgram`: Para compilar y enlazar los vertex y fragment shader y crear así el shader program. Gestiona los vertex y fragment shader usados actualmente en memoria dinámica, en una relación de composición. He tomado esta decisión porque, en el estado actual de la aplicación, los program shaders solo surgen de enlazar vertex y fragment shaders, y estos últimos solo existen para combinarse en un program shader.

La creación de todos los objetos shader de OpenGL se ha llevado a `ShaderProgram`, que le pasa la información de ID a `Shader` para almacenarla. La destrucción del vertex y el fragment se ha llevado a `Shader`, para poder hacerlos con su destructor, y la del program shader a su clase homónima por la misma razón.  

`Renderer` tiene a su vez una relación de composición con `ProgramShader`, gestionando su creación y su destrucción. 

### Ventana GUI para elegir shaders
Se ha seguido el sistema sugerido en la sesión anterior, donde los shaders hechos para usarse juntos comienzan con el mismo prefijo. Esta información sobre los shader cargados actualmente la tiene guardada `Renderer`. El método de `GUI` para crear la ventana de cambio de shaders contiene dos parámetros: 
- `texto`: Un punto a string donde se guardará el texto introducido en la ventana por el usuario.
- `cambio`: Puntero a bool que la función marcará a True si se ha intentado cargar nuevo nombre de shaders.  

En el main, a través de `cambio`, obtenemos la señal para volver a crear el shader program. Solo lo hacemos cuando el usuario lo ha solicitado. A fin de probar la ventana, en el proyecto hay ahora mismo dos parejas de shaders: "pag03" que recibe correctamente los datos de color y renderiza el triángulo con degradado y "rosa", que no usa dicha información y colorea el triángulo de un único tono rosa.


## Sesión 5
### Controles de la cámara
Podemos elegir el movimiento actual usando el menú de selección de la ventana GUI dedicada a la cámara. Se mostrarán las instrucciones para usar el movimiento actual. Además de poder controlarse con el ratón, cada movimiento puede controlarse también con las teclas:  
- _Pan_: A y D
- _Tilt_: W y S
- _Dolly_: W, A, S y D
- _Crane_: W y S
- _Orbit_: W, A, S y D
- _Zoom_: W y S

Se han elegido estas combinaciones por convención, al ser las más usadas en videojuegos y existir la asociación W-delante-arriba, S-detrás-abajo, A-izquierda y D-Derecha.

Para controlar los movimientos con el ratón, se ha seguido la estrategia propuesta en el guion. En el `main` hay 3 variables globales:
- `boton_izq_raton_pulsado`: Booleano que se pone a verdadero al pulsar el botón izquierdo del ratón y falso al soltarlo
- `pos_x_raton_ultima_pulsacion_tecla_izq`: Al pulsar el botón izquierdo del ratón, guardamos aquí su coordenada en X
- `pos_y_raton_ultima_pulsacion_tecla_izq`: Al pulsar el botón izquierdo del ratón, guardamos aquí su coordenada en Y  

He colocado un nuevo callback para el movimiento del ratón, que llama al movimiento actual de la cámara si se está manteniendo pulsado el botón izquierdo mientras se mueve el ratón. Se elige la dirección del movimiento dependiendo de si estamos aumentando o disminuyendo la X/Y y según el tipo de movimiento. Por ejemplo, para _pan_ da igual los movimientos en Y que hagamos, pero para _dolly_ se tiene en cuenta tanto los cambios en X como los de Y.


### Adición de la cámara virtual
En primer lugar, he incluido los parámetros que definen la cámara en una clase homónima `PAG::Camara` dedicada para ello. Se trata de un singleton que contiene la posición, el punto al que mira, su ángulo de visión, ratio de aspecto y distancias a los planos near y far. Contiene además los parámetros que definen la cantidad de movimiento en esta sesión, es decir, cuántos grados rotamos al hacer _tilt_, cuánto nos desplazamos al hacer _dolly_, etc. En cuanto a los métodos, tiene uno por cada tipo de movimiento (_pan_, _tilt_, _dolly_, _crane_, _orbit_ y _zoom_), cada uno con un parámetro booleano o enumerado específico para concretar lo necesario en cada caso. Por ejemplo, para el _pan_ solo nos hace falta especificar el sentido en el que rotamos, pero para el _dolly_ tenemos 4 opciones, por eso he usado un enumerado. La clase cuenta además con métodos para calcular parámetros de la cámara no definidos explícitamente en la clase, los 3 vectores de su sistema de coordenadas unv.  

He añadido 2 nuevos parámetros uniform para los shaders de esta sesión (pag05):
- `matrizModeladoVision`: Para aplicar a los vectores normales y tangentes. No se ha usado aún en esta sesión.
- `matrizModeladoVisionProyeccion`: Para aplicar a los vértices. Sí se ha usado en esta sesión, multiplica a las coordenadas de posición del vértice para calcular la posición.

En `Renderer::refrescar_escena()` calculamos las matrices de visión y proyección usando los parámetros de la cámara extraídos de su clase. He decidido que en `main` se extraigan estos datos y se le pasen como parámetros a la función `PAG::refrescar_escena()` y que por ahora Renderer no interactúe con la cámara por intentar mantener el desacoplamiento. Componemos las matrices multiplicándolas y las pasamos al vertex shader. Al encontrarse este procedimiento dentro de `Renderer::refrescar_escena()`, ahora podemos aplicar cambios en los parámetros de la cámara desde cualquier punto de la aplicación y se verán reflejados en cuanto se renderice la imagen de nuevo.  

#### Resolución del problema al calcular el vector u
Para el cálculo del vector u cuando n e Y son colineales y su producto vectorial resulta (0,0,0), se ha optado por la solución propuesta en el temario de teoría, sustituir Y por Z al calcular el producto vectorial en ese caso. Para ello he usado `glm::epsilonEqual`, introduciendo manualmente el umbral 0.005 porque con la constante `epsilon` de GLM nunca se llegaban a igualar los dos vectores. 


## Sesión 6
### Ventanas actuales en la GUI
La interfaz cuenta con las siguientes ventanas:
- **Mensajes**: Muestra texto sobre eventos de la aplicación.
- **Color**: Permite cambiar el color del fondo.
- **Movimientos de cámara**: Permite seleccionar un movimiento de cámara. Este pasa a controlarse con las teclas que se indican en la propia ventana y haciendo clic con el ratón sobre cualquier punto de la aplicación.
- **Transformación y borrado**: Permite seleccionar uno de los modelos cargados actualmente. Podremos borrar este con un botón o aplicarle la transformación seleccionada con los botones "+" (rotar y trasladar en positivo, agrandar al escalar) y "-" (rotar y trasladar en negativo, achicar al escalar).
- **Carga de modelos**: Permite abrir el explorador de archivos para escoger un modelo en formato OBJ para cargar a la escena. Este se carga usando los shaders cuyo nombre especificamos en la caja de texto de esta ventana. Debemos especificar esto antes de cargar el modelo.

### Múltiples modelos simultáneos
Para empezar, se ha definido una nueva clase Modelo. Todo el proceso que antes gestionaba `Renderer` sobre el triángulo sencillo con el que acabamos la última sesión ahora lo gestiona la clase `Modelo`. Por ello se le han transferido los mismos métodos (`creaShaderProgram`, `cargarModelo` `refrescarModelo`) que trabajan sobre los mismos datos (`programShader`, `nombreShaders`, `vertices`, `indices`, IDs de objetos de OpenGL). Ahora `Renderer` tiene un vector objetos `Modelo` que gestiona y los métodos que se han transferido ahora recorren dicha estructura de datos ejecutando sus equivalentes en cada elemento. He considerado lo más acertado establecer una relación de composición entre ambas clases, pues `Renderer` gestiona la creación y destrucción de todos los objetos `Modelo`, ninguno de ellos existe de forma independiente. 

Además de esta transferencia de responsabilidades, los modelos deben poder recibir transformaciones de modelado. Por ello, la clase cuenta con el atributo `matrizTransformacionModelado`, que se combinará con la de proyección y visión recibida del `Renderer` en cada ciclo de eventos antes de pasarla a los shaders. De esta forma la clase cuenta con métodos para aplicar transformaciones que se aplican sobre este atributo y sobre otro atributo auxiliar `puntoPosModelo`, que se tiene como dato para calcular las rotaciones sobre sí mismo. De esta forma cada modelo puede recibir sus propias transformaciones de modelado, pero todos comparten las de visión y proyección para existir de forma coherente en la escena.

La clase `Modelo` cuenta además con un método `liberarRecursos()` ajeno al destructor. Esto es así para evitar asignaciones repetidas o erróneas de índices de objetos de OpenGL al crear modelos antes de introducirlos en el vector de `Renderer`. Esta última clase se encarga en su destructor de llamar manualmente a este método para cada modelo cargado, de forma que los recursos de OpenGL quedan liberados antes de la destrucción de la clase que los gestiona. 

### Carga de modelos OBJ
Se ha usado Assimp tal y como se indica en los tutoriales aportados, pero simplificando el proceso para cargar solo modelos con una única malla. Los vértices e índices se guardan en vectores STL atributo en cada modelo. De esta forma podemos pasarlos a OpenGL indicando la dirección de memoria donde empiezan los datos contiguos con `&indices.front()` y `&vertices.front()`.   

### Otros cambios
Se han movido todos los enumerados a un fichero separado `enumerados.h`, para que sean accesibles desde cualquier clase sin conflictos.

Cada vez que se añade un modelo, este sufre una traslación aleatoria tanto en cantidad como en sentido en cada eje, de forma que los modelos quedan esparcidos por la escena, aproximadamente dentro del campo visual. Esto también se aplica a los dos modelos instanciados por defecto, los `dado.obj` y `Panther.obj` dados como ejemplo en Platea. Por ello, cada vez que se ejecuta la aplicación aparecen en lugares diferentes.

Los shaders de ejemplo de la anterior sesión `pag03` y `rosa` se mantienen igual. Procesan los datos de entrada, pero no reciben el uniform de matriz de transformación, de modo que muestran el modelo siempre de frente, sin importar las transformaciones que mandemos ni los movimientos de cámara que hagamos. La diferencia entre ellos es que `pag03` recibe colores y `rosa` renderiza el modelo de dicho color en todos los casos.


