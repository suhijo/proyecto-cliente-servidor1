/* Nombre: Software servidor
 * Detles: Iinicia servicio por puerto, recibe peticiones y responde consultas
 * Desarrollo: suhijo
 * Licencia: estudiantil
 * Nombre archivo: libservidor.cpp
 * Pendientes: ninguno
 * 
 */
#ifndef LIBSERVIDOR_HPP_INCLUDED
#define LIBSERVIDOR_HPP_INCLUDED

#include <iostream>
#include <cstring>
#include <thread>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>
#include <fstream>
#include <cstdlib>
#include <mariadb/conncpp.hpp>
#include "libservidor.hpp"

using namespace std;

int initserver::port = 0;
int initserver::servidor_fd = -1;
int initserver::pipefd[2];

string initserver::token = "";
string initserver::hostbd = ""; 
string initserver::portbd = ""; 
string initserver::namebd = ""; 
string initserver::userbd="" ; 
string initserver::passbd = "";

volatile sig_atomic_t initserver::s_up = 1;

int initserver::cargar_config() { //
		
	string line;
    
	ifstream file(fileconf);

	if (!file.is_open()) {
		cerr << "Error: No se pudo abrir el archivo en " << fileconf << endl;

		exit(EXIT_FAILURE);

	}

	while (getline(file, line)) {
		size_t delimiterPos = line.find("=");
	
		if ( line.find(' ') != string::npos ) {
			cerr << "Error, el archivo " << fileconf << " contiene espacios" << endl;

			file.close();
			exit(EXIT_FAILURE);
		}

		if (delimiterPos != string::npos) {
			string key = line.substr(0, delimiterPos);
			string value = line.substr(delimiterPos + 1);

			if ( key == "token" ) token = value;
			else if ( key == "port" )port = stoi(value);
			else if ( key == "hostbd" )hostbd = value;
			else if ( key == "portbd" )portbd = value;
			else if ( key == "namebd" )namebd = value;
			else if ( key == "userbd" )userbd = value;
			else if ( key == "passbd" )passbd = value;
		}
	}

	file.close();

	if ( token.empty() || ( port == 0 ) || hostbd.empty() || portbd.empty() || namebd.empty() || userbd.empty() || passbd.empty() )    {
		cerr << "Falta un parametro en archivo de configuracion" << endl;

		exit(EXIT_FAILURE);
	}
    
	cout << "Configuración Cargada. "<< endl;

	return 0;
}
	
void initserver::preparar() {
	// Crea pid al archivo
	ofstream pidfile(APID);
	pidfile << getpid();
	pidfile.close();
			
}

string initserver::consulta_bd(string cbd_tel) {
	
	string cdb_rta = "empty";
	
	try {
           
        sql::Driver* driver = sql::mariadb::get_driver_instance();
		sql::SQLString url("jdbc:mariadb://" + hostbd + ":" + portbd + "/" + namebd);
		sql::Properties properties({{"user", userbd}, {"password", passbd}});
		
		unique_ptr<sql::Connection> conn(driver->connect(url, properties));
		unique_ptr<sql::PreparedStatement> stmnt(conn->prepareStatement("select dir_nombre, dir_direccion, ciud_nombre  from personas inner join ciudad ON ciudad.ciud_id = personas.dir_ciud_id where dir_tel = '"+cbd_tel+"'"));
			
		cout << "Iniciando conexion a BD" << endl;
			
		sql::ResultSet *res = stmnt->executeQuery();

		if ( res->next() ) cdb_rta = res->getString(1) + "," + res->getString(2)+ "," + res->getString(3);
			
	}
    catch(sql::SQLException& e){
        cerr << "Error conectando a DB " << e.what() << std::endl;
        cdb_rta = "error";
        return cdb_rta;
	}

	return cdb_rta;
}

string initserver::validar_msg(string val_cmd, string val_rmsg, int val_tmsg) {
	// Valida la respuesta sea val_cmd y devuelve respuesta .
	// si no, blanco.
	
	string comando, val_rta = "", val_msg(val_rmsg.c_str(),val_tmsg);
	size_t pos = val_msg.find(':');

	if (pos != string::npos) {
		comando = val_msg.substr(0, pos);
        
	}
   
	if (comando == val_cmd) {
		val_rta = val_msg.substr(pos +1 );
   	}

	return val_rta;
}

bool initserver::validar_hmsg(string val_cmd, string val_msg) {
	// valida comando val_cmd este en val_msg 
	size_t pos = val_msg.find(':');
    
	if (pos != string::npos) {
		string cabecera = val_msg.substr(0, pos);
        
		if (cabecera == val_cmd) {
			return true; 
		}
	}

	return false;
}

void initserver::iniciar (){
	
	signal(SIGTERM, m_senal);
    signal(SIGINT, m_senal);
    
    int opt = 1;
    struct sockaddr_in address{};
    int addrlen = sizeof(address);

    servidor_fd = socket(AF_INET, SOCK_STREAM, 0);

    setsockopt(servidor_fd,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    bind(servidor_fd, (struct sockaddr*)&address, sizeof(address));
    listen(servidor_fd, 10);

    std::cout << "Servidor iniciado en puerto " << port << std::endl;

    while (s_up) {
        int cliente_fd = accept(servidor_fd,
                               (struct sockaddr*)&address,
                               (socklen_t*)&addrlen);

        if (cliente_fd < 0) {
            if (!s_up) break; // señal recibida
            continue;
        }

        thread(cada_cliente, cliente_fd).detach();
    }
}

void initserver::salir() {
    if (servidor_fd != -1) close(servidor_fd);
    std::cout << "Servidor detenido correctamente\n";

}

void initserver::m_senal(int sig) {
		cout << "Deteniendo servicio" << endl;
		
		close(servidor_fd);
		exit(1);
}

void initserver::cada_cliente(int cliente_fd) {
    char buffer[1024] = {0};
    string resp = "", cli_chtel ="" ;
    
    cout << "Ciente conectado" << endl;
    while (true) {
		memset(buffer, 0, sizeof(buffer));
		int bytes = read(cliente_fd, buffer, sizeof(buffer));
		resp=""; 
		cli_chtel =""; ;

		
		if (bytes == 0) {
            cout << "Cliente desconectado." << endl;
            break;
        } 
        else if (bytes < 0) {
            perror("Error leyendo socket finalizando cliente");
            break;
        }

		if (bytes > 0) {
			// FIXME: validar si esta autenticado antes.
        
			if ( validar_hmsg("INIT", buffer) ) { // si CMD INIT validar inicia sesion
				
				if (validar_msg("INIT", buffer, bytes) == token) {
					resp = "INIT:OK";
			
					}
					else {
						resp = "INIT:fail";
					
					}
			}		
			else if ( validar_hmsg("CHECK", buffer) ) {
				cli_chtel = validar_msg("CHECK", buffer, bytes); 
				resp = "CHECK:" + consulta_bd(cli_chtel);			
		
				}
				else{
					resp = "INIT:error";
					}	
			send(cliente_fd, resp.c_str(), resp.size(), 0);
			
		}
	}
	
	close(cliente_fd);
}

#endif




