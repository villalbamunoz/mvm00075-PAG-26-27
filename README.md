# mvm00075-PAG-26-27
Proyecto de prácticas de la asignatura Programación de Aplicaciones Gráficas de Manuel Villalba Muñoz para el curso 2026-27

## Sesión 1: Ejercicio de reflexión
Una posible solución al problema es definir `PAG::Renderer` como una clase que siga el patrón de diseño _singleton_. Tiene sentido, pues no se va a declarar más de una instancia de ella, al encargarse de una tarea tan global como gestionar el contexto gráfico de OpenGL de nuestra aplicación. Por ejemplo, si tuviéramos 2 instancias diferentes de `PAG::Renderer` no tendría diferencia el llamar al método `refrescarVentana()` de una u otra, pues ambos harían lo mismo.

En el contexto de la aplicación actual, podríamos declarar `PAG:Renderer` como una variable global en el fichero main.cpp, pues no hay otro fichero donde hacerlo. En cualquier caso, debe ser un lugar accesible para todos los módulos de la aplicación, pues contendría funcionalidad básica necesaria para casi cualquier acción que queramos realizar en esta. De esta forma encontramos una vía para evitar el problema de no poder registrar métodos de clases en los _callbacks_. Podemos colocar funciones sencillas de C como las que tenemos actualmente y sustituir su contenido por una llamada al método oportuno de la variable global `PAG::Renderer`. La limitación solo existe en lo que podemos o no registrar como _callback_, no en lo que estas funciones contengan. 

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

Con la constante `GL_COMPILE_STATUS` comprobamos si ha compilado correctamente. En caso negativo, queremos consultar más detalles. Para ello debemos primeramente obtener el tamaño del mensaje de error, usando la constante `GL_INFO_LOG_LENGTH`. En caso de tener algún mensaje (longitud mayor a 0), lo obtenemos con el método `glGetShaderInfoLog`. Lanzamos una excepción de tipo `runtime_error`, que será capturada en el main y mostrada por la GUI añadiendo el texto a la estructura de datos de mensajes del singleton `GUI`.  

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