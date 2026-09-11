# mvm00075-PAG-26-27
Proyecto de prácticas de la asignatura Programación de Aplicaciones Gráficas de Manuel Villalba Muñoz para el curso 2026-27

## Sesión 1: Ejercicio de reflexión
Una posible solución al problema es definir `PAG::Renderer` como una clase que siga el patrón de diseño _singleton_. Tiene sentido, pues no se va a declarar más de una instancia de ella, al encargarse de una tarea tan global como gestionar el contexto gráfico de OpenGL de nuestra aplicación. Por ejemplo, si tuviéramos 2 instancias diferentes de `PAG::Renderer` no tendría diferencia el llamar al método `refrescarVentana()` de una u otra, pues ambos harían lo mismo.

En el contexto de la aplicación actual, podríamos declarar `PAG:Renderer` como una variable global en el fichero main.cpp, pues no hay otro fichero donde hacerlo. En cualquier caso, debe ser un lugar accesible para todos los módulos de la aplicación, pues contendría funcionalidad básica necesaria para casi cualquier acción que queramos realizar en esta. De esta forma encontramos una vía para evitar el problema de no poder registrar métodos de clases en los _callbacks_. Podemos colocar funciones sencillas de C como las que tenemos actualmente y sustituir su contenido por una llamada al método oportuno de la variable global `PAG::Renderer`. La limitación solo existe en lo que podemos o no registrar como _callback_, no en lo que estas funciones contengan. 

En cuanto a qué tan acoplada está la solución, podemos cambiar el código que se ejecutará cuando ocurran los _callbacks_ sin tener que tocar directamente las funciones registradas como tal, pues entendemos que definiremos el _singleton_ en sus respectivos ficheros separados, que serán los que editemos para cambiar el funcionamiento real de los _callbacks_.

Adjunto en "img/sesion_1_diagrama_UML.png" un diseño básico de como se vería `PAG:Renderer` en un diagrama UML. Muestra lo justo y necesario para indicar que es un _singleton_: atributo privado de la única instancia de sí misma, método público para acceder a esta y constructor privado. 
