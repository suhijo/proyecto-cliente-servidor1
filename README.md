# Proyecto Cliente-Servidor C++

Este proyecto implementa una arquitectura de comunicación básica entre un cliente y un servidor utilizando sockets en C++.
El servicio servidor  recibe un numero de telefono del cliente y consulta en la BD definidia en el archivo /etc/cliente_info.conf  
En cliente solicita un numero de telefono y se coencta al servidor definido en  /etc/server_info.conf

##  Estructura del Proyecto

El repositorio está organizado de la siguiente manera:

* cliente/: Contiene el código fuente de la aplicación cliente.
* servidor/: Contiene la lógica del servidor y la gestión de conexiones.

##  Requisitos

Para compilar y ejecutar este proyecto, necesitarás:
* Un compilador de C++ (GCC/G++ o Clang).
* Make (En desarollo).
* Sistema operativo basado en Unix/Linux (recomendado para sockets).
* Libreria libmariadbcpp

##  Compilación

Desde la terminal, puedes compilar los módulos de la siguiente manera:

**Para el servidor:**
cd servidor && build.sh

**Para el cliente:**
cd cliente && build.sh
