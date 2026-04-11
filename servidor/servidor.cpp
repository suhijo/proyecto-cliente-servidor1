/* Nombre: Software servidor
 * Detles: Iinicia servicio por puerto, recibe peticiones y responde consultas
 * Desarrollo: suhijo
 * Licencia: estudiantil
 * Nombre archivo: servidor.cpp
 * Pendientes: ninguno
 * 
 */
 
#include <iostream>
#include <cstring>
#include <thread>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>
#include <fstream>
#include <cstdlib>
#include "libservidor.hpp"

using namespace std;

int main() {
	
	initserver servidor;
	
	servidor.preparar();
	
    servidor.cargar_config();
    
	servidor.iniciar() ;

	servidor.salir();
    
    return 0;
}




