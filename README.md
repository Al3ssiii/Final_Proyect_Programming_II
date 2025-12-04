⚔️ RIVAL FRONTIERS 

Proyecto Final | Programación II (C++)

Somos el grupo detrás de RIVAL FRONTIERS (nuestro proyecto final), un juego de estrategia por turnos desarrollado enteramente en C++ y ejecutado en consola deployable. Este proyecto ha sido el trabajo final de nuestro curso de Programación II y representa nuestro esfuerzo por crear un juego funcional, modular y tactico.

En RIVAL FRONTIERS, te enfrentaras contra una IA programada por nosotros, en este juego por turnos en un mapa bidimensional (en simples palabras, una matriz).

🎯 Nuestro Objetivo

El objetivo principal de este proyecto era aplicar los principios de la Programación Orientada a Objetos (POO) para construir un motor de juego limpio y extensible. Buscábamos entregar:

  -  Una base funcional con todas las mecánicas esenciales de un juego táctico.

  -  Una arquitectura modular (separando UI, Lógica, Mapa y Controlador) para demostrar buenas prácticas de diseño de software.

  -  Una experiencia de juego completa, desde el reclutamiento hasta el combate.
    

🛠️ Mecánicas de Juego Principales

Hemos implementado un conjunto sólido de mecánicas para simular un conflicto estratégico:
🗺️ El Tablero y el Entorno

  -  El mapa es una matriz 2D compuesta por Tiles.

  -  Cada Tile tiene un tipo de Terreno (Llanura, Bosque, Montaña, Agua, Pantano) que afecta el movimiento.

  -  Una casilla puede contener un Terreno, una Unidad, y/o un Edificio.

🛡️ Unidades, Edificios y Recursos

  -  Unidades: Contamos con varios tipos (Soldado, Arquero, Caballero, Mago, Ingeniero), cada uno con sus propias estadísticas (Vida, Ataque, Defensa, Movimiento) y costo.

  -  Edificios: Cuartel (para reclutar), Torre (defensa), y Granja (producción de recursos).

  -  Recursos: El juego se basa en Comida, Metal y Energía. Se obtienen pasivamente al final de cada turno y se gastan en acciones.

  -  Puntos de Acción (PA): Tienes un límite de PA por turno para ejecutar acciones como mover, construir o reclutar.

💥 Movimiento y Combate

   -  Movimiento: Limitado a casillas adyacentes (no diagonales). El Agua es intransitable.

   -  Combate: Es determinista y simultáneo. El daño se calcula simplemente por la diferencia entre el Ataque de una unidad y la Defensa de la otra.

🤖 IA Enemiga 

Implementamos una IA básica para que el juego sea un desafío:

   -  Produce unidades en sus cuarteles.

  -  Intenta avanzar sobre el tablero.

  -  Ataca cualquier unidad del jugador que encuentre en su camino.

   -  La lógica de la IA y el movimiento automatico de sus unidades se gestionan en la fase de mundo del turno.

💻 Estructura del Codigo: Diseño Modular (POO)

Uno de nuestros mayores enfoques fue la separación de responsabilidades. La arquitectura del proyecto está organizada en los siguientes módulos clave para garantizar el mantenimiento y la extensibilidad:

  -  UX/Renderer: Este apartado esta expersado en 2 archivos, *ux.h* y *ux.cpp*. Esta es la encargada de entregar una mejor experiencia al usuario, con mejores interfaces, pantallas de carga y una barra de carga que entrega mensajes divertos. Ademas, entrega el texto ASCII de "RIVAL FRONTIERS".
  -  Map: Este esta dividido en "map.h" y "map.cpp". Define las Estructuras, Unidades, Edificios. Ademas de la generacion del mapa.
  -  Game Engine: Expresado en "game_engine.h" y "game_engine.cpp". Aqui es donde todas las funcionalidades de las clases se unen para llevar al cabo el juego, basicamente es el nucleo. Se mantiene la validan y ejecutan todas las acciones necesarias. Tambien se administra la logica de la IA.
  -  Controller: Divido en "controller.h"  y "controller.cpp". Traduce los inputs del usuario y los convierte en llamadas al Engine creado.

⚙️ COmpilamiento del juego
El requisito es tener make y un Sistema Operativo (por obvias razones). En nuestro codigo hemos flexibilizado el codigo y es compatible en su totalidad con Linux como en Windows. Con esto nos referimos mas que nada por la funcion de *Limpieza de Pantalla*, la cual tiene diferencias entre SO. En Windows es *"cls"*, mientras que en Linux o MAC es *"clear"*.

▶️ Ejecución
Abre tu terminal, ingresa al directorio general (Final_Proyect_Programming_II) y ejecuta los siguientes codigos:
# 1. Crea el directorio de construccion
mkdir build

# 2. Entra al directorio
cd build

# 3. Configurar el proyecto con CMake
cmake ..

# 4. Compilar el código fuente
make

# 5. Ejecuta el juego
./Final_Proyect_Programming_II

De esta manera podras ejecutar satisfactoriamente nuestro juego!.

💡 Limitaciones

 Recordar que la IA es sumamente rustica, no esta implementado el *Pathfinding*, por ejemplo.

🧑‍💻 Créditos

 Este juego llamado *RIVAL FRONTIERS* fue realizado por:
   -  Diego Godoy
   -  Bruno Zamora
   -  Alessio Tardillo
   -  Jose Carita

     

      
