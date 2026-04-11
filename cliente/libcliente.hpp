/* Nombre: Software cliente 
 * Detalles: Para conectarse a un servidor y hacer consultas
 * Desarrollo: suhijo
 * Licencia: estudiantil
 * Nombre archivo: libcliente.hpp
 * Pendientes: 
 */

#include <iostream>
#include <cstring>
#include <arpa/inet.h>

#ifndef LIBCLIENTE_HPP
#define LIBCLIENTE_HPP

#define fileconf "/etc/cliente_info.conf"

using namespace std;

class initcliente {
	private:

		static int sock;
		static int port;

		static string token; 
		static string host;
		static string msg;
		

	public:
	
		static string buffer;
	
		static bool validar_hmsg(string , string );
		static bool val_tel(const string& );

		static int cargar_config();
		static int conectar();
		static int cerrar(int);
		static int opciones();
		static int enviar_msg(string);
		
		static string validar_msg(string , string, int);
		
		void limpiar_pantalla(int tout);

};

#endif
