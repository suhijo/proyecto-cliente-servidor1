/* Nombre: Software servidor
 * Detles: Iinicia servicio por puerto, recibe peticiones y responde consultas
 * Desarrollo: suhijo
 * Licencia: estudiantil
 * Nombre archivo: libservidor.hpp
 * Pendientes: ninguno
 * 
 */
#ifndef LIBSERVIDOR_HPP
#define LIBSERVIDOR_HPP

#include <iostream>
#include <cstring>
#include <signal.h>

#define APID "/tmp/server.pid"
#define fileconf "/etc/server_info.conf"

using namespace std;

class initserver {
	private:
		static int servidor_fd;
		static int pipefd[2];
		static int port;
		
		static string hostbd; 
		static string portbd; 
		static string namebd; 
		static string userbd; 
		static string passbd;
		static string token;
		
		static volatile sig_atomic_t s_up;

	public:

		static int cargar_config(); 
		static void preparar();
		static string consulta_bd(string cbd_tel);
		static string validar_msg(string val_cmd, string val_rmsg, int val_tmsg);
		static bool validar_hmsg(string val_cmd, string val_msg);
		static void iniciar();
		static void salir();
		static void m_senal(int sig);
		static void cada_cliente(int cliente_fd);
};

#endif




