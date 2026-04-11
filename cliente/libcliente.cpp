/* Nombre: Software cliente 
 * Detalles: Para conectarse a un servidor y hacer consultas
 * Desarrollo: suhijo
 * Licencia: estudiantil
 * Pendientes: 
 * 
 * - si se consulta y servidor devuelve error no logueado, reconectar.
 * - si se cae servidor y va enviar datos al socker validar este arrriba y conectar
 * - sacar funciones estandar y ponerlas en librerias
 */
#ifndef LIBCLIENTE_HPP_INCLUDED
#define LIBCLIENTE_HPP_INCLUDED

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <arpa/inet.h>
#include <unistd.h>
#include "libcliente.hpp"

using namespace std;

// Declarar variables
string initcliente::token = ""; 
string initcliente::host = "";
string initcliente::buffer(1024, '\0');
string initcliente::msg = "";

int initcliente::sock = 0;
int initcliente::port = 0;

bool initcliente::val_tel(const string& s) {	
    return !s.empty() && find_if(s.begin(),s.end(), [](unsigned char c) { return !isdigit(c); }) == s.end();
}

bool initcliente::validar_hmsg(string val_cmd, string val_msg) {
    size_t pos = val_msg.find(':');
    
    if (pos != string::npos) {
        string cabecera = val_msg.substr(0, pos);
        
        if (cabecera == val_cmd) {
            return true; 
        }
    }
    return false;
}

int initcliente::cargar_config() {

    string line;
    
    ifstream file(fileconf);

    if (!file.is_open()) {
        cerr << "Error: No se pudo abrir el archivo en " << fileconf << endl;

	exit(1);

    }

	while (getline(file, line)) {
        size_t delimiterPos = line.find("=");
	
		if ( line.find(' ') != string::npos ) {
			cerr << "Error, el archivo " << fileconf << " contiene espacios" << endl;

			file.close();
			cerrar(1);
		}

        if (delimiterPos != string::npos) {
            string key = line.substr(0, delimiterPos);
            string value = line.substr(delimiterPos + 1);
            
            //cout << "bsucando token host port :"<< key << endl;

            if ( key == "token" ) token = value;
            else if ( key == "servidor" )host = value;
            else if ( key == "port" )port = stoi(value);
        }
    }

    file.close();

    if ( token.empty() || host.empty() || (port == 0) )    {
	    cerr << "Falta un parametro: token " << token << "o  host: "  << host << endl;

	    cerrar(1);
    }
    
    cout << "Configuración Cargada.\nConectando a " << host << " puerto " << port<< endl;

    return 0;
}

int initcliente::conectar() {
	// valida conexion y crea socket
	
	struct sockaddr_in serv_addr;
	
	int con_read = 0;
	string con_rta;
	
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		cerr << "Error al crear socket\n";
		exit(1);
	}

		serv_addr.sin_family = AF_INET;
    	serv_addr.sin_port = htons(port);

    	if (inet_pton(AF_INET, host.c_str() , &serv_addr.sin_addr) <= 0) {
        	cout << "Direccion de servidor invalida\n";

			cerrar(1);
    	}

    	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        	cout << "Fallo conexion al servidor: " << host << endl;

		cerrar(1);
    	}

		msg = "INIT:" + token ;
    	send(sock, msg.c_str(), msg.size(), 0);

    	con_read = read(sock, buffer.data(), buffer.size());
		con_rta = validar_msg("INIT", buffer, con_read);
		
	if (con_rta == "OK") {
		cout << "Iniciando sesión" << endl;

	}
	else {
		cerr << "Autenticacion servidor no exitosa " << endl;
		cerrar(1);
	}

    return 0;
}

string initcliente::validar_msg(string val_cmd, string val_rmsg, int val_tmsg) {
	// Valida la respuesta CMD sea val_cmd y devuelve msg.
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

int initcliente::cerrar(int cer_val) {	
	cout << "Cerrando conexión con servidor" << endl; 

	close(sock);
	exit(cer_val);
	
	return 0;
}

void initcliente::limpiar_pantalla(int tout){
	sleep(tout);
	cout << "\033[2J\033[1;1H";

	}

int initcliente::enviar_msg(string env_msg) {

    int env_send = 0, env_rcv = 0; 
    env_msg = "CHECK:" + env_msg;
    
    env_send = send(sock, env_msg.c_str(), env_msg.size(), 0);
    
    if (env_send < 0) {
        cerr << "Error al enviar mensaje" << endl;
        
        return 1;
    }

    fill(buffer.begin(), buffer.end(), 0); 
    env_rcv = read(sock, buffer.data(), buffer.size());
    
    if (env_rcv < 0) {
        cerr << "Error al leer respuesta del servidor" << endl;
        
        return 1;
    }
    return env_rcv;
}


#endif
