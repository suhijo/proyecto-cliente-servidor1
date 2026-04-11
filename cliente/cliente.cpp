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

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <arpa/inet.h>
#include <unistd.h>
#include "libcliente.hpp"

using namespace std;

initcliente micliente;

int opciones();

int opciones() {
	string op_usr, op_msg, op_rtat, op_nombre, op_dir; 
	int op_tambuf;
	size_t pos;
	
	micliente.limpiar_pantalla(1);
	while(true) {
		
		op_usr = "", op_msg = "", op_rtat = "", op_nombre = "", op_dir = "";
		
		cout << "########################################################" << endl;
		cout << "# Digite numero de telefono(q para salir): ";
		cin >> op_usr;
		cout << "\n";
		
		if ( micliente.val_tel(op_usr) ) {
				op_tambuf= micliente.enviar_msg(op_usr);
				
				if ( micliente.validar_hmsg("CHECK", micliente.buffer) ) { 
					op_msg = micliente.validar_msg("CHECK", micliente.buffer, op_tambuf);
					
					if ( op_msg == "empty" ) {
						cout << "Persona dueña de ese número telefónico no existe " << endl;
						micliente.limpiar_pantalla(3);
						
					}
					else if ( op_msg == "error" ) {
						cout << "Error interno del servidor " << endl;
						micliente.limpiar_pantalla(3);
						
					}
					else {
						// numero encontrado
						pos = op_msg.find(',');
						
						if (pos != string::npos) {
							op_nombre = op_msg.substr(0, pos);
							op_rtat = op_msg.substr(pos+1);
							
							}
			
						op_msg = op_rtat;
						pos = op_msg.find(',');   
        
						if (pos != string::npos) {
							op_dir = op_msg.substr(0, pos);
							op_rtat = op_msg.substr(pos+1);
			
						}
						
						cout << "Numero encontrado. Los datos del usuario son:" << endl;
						cout << "\tNombre: " << op_nombre << endl;
						cout << "\tDireccion: " << op_dir << endl;
						cout << "\tCiudad: " << op_rtat << endl << endl;
						
						cout << "Presione cualquier tecla para continuar" << endl;
						system("read -n 1 char"); 	
						
						micliente.limpiar_pantalla(0);
						
					}
				}
				else if (micliente.validar_hmsg("INIT", micliente.buffer) ) {
					op_msg = micliente.validar_msg("INIT", micliente.buffer, op_tambuf);
					
					if (op_msg == "fail"){
						cout << "Reconectando..." << endl;
						micliente.conectar();
						micliente.limpiar_pantalla(3);
						
						}
						else{ 
							cout << "Respuesta no valida del servidor" << endl;
							micliente.limpiar_pantalla(3);
							
							}
						
				}
				else  {
					cout << "Respuesta no valida del servidor" << endl;
					micliente.limpiar_pantalla(3);
					
					}
		} 
		else if ( ( op_usr == "q" ) || (op_usr == "Q" )){
				micliente.cerrar(0);
				
			} else {
				cout << "Digite un telefono valido" << endl;
				micliente.limpiar_pantalla(3);
				
				}

	}	
	
	cout << "finalizando sesion " << endl;
	
	return 0;
}

int main () {
	
	micliente.cargar_config();

	micliente.conectar();

	opciones();

	micliente.cerrar(0);

return 0;
}	
