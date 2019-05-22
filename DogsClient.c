#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <iostream>

#define PORT 6969
struct sockaddr_in server;
int r,fd,opc,numPerros;

struct dogType{
	char Nombre[32];
	int Edad;
	char Raza [16];
	int Estatura;
	float Peso;
	char Sexo;
	int Key;
	int Pos;
};

void doPause(){
	cout << "\tPresione ENTER para continuar";
	cin.clear();
	cin.ignore();
	cin.ignore();
	system("clear");
}

void printDog(dogType dog){
	cout << "#" << dog.Pos;
	cout << " "<<dog.Nombre;
	cout << ", Edad: "<<dog.Edad;
	cout << ", Raza: " << dog.Raza;
	cout << ", Estatura: " << dog.Estatura;
	cout << ", Peso: " << dog.Peso;
	cout << ", Sexo: " << dog.Sexo<<endl;
}

int sendCliente(int op, void* data){
	send(fd,&op,sizeof(int),0);
	struct dogType dog,doge;
	int num;
	switch(op){
		case 1:	return send(fd,(struct dogType* )data,sizeof(struct dogType),0);
				break;
		case 2: r = send(fd,(int *) data,sizeof(int),0);
				//errores
				r = recv(fd,&dog,sizeof(struct dogType),0);
				if(dog.Key!=-1) printDog(dog);
				else cout << "Perro no encontrado" <<endl;
				return r;
				break;
		case 3: return send(fd,(int *) data,sizeof(int),0);
				break;
		case 4: r = send(fd,(char *)data, 32,0);
				r = recv(fd,&num,sizeof(int),0);
				for(int i=0;i<num;i++){
					recv(fd,&doge,sizeof(struct dogType),0);
					printDog(doge);
				}
				return r;
				break;
		default: return 0;
				break;
	}
}

struct dogType nuevoRegistro(){
	int edad,estatura,value;
	float peso;
	char sexo,nom[32];
	string nombre="";
	string raza="";
	cout << "Ingrese el nombre"<<endl;
	cin >> nom;
	cout <<endl<< "Ingrese la edad"<<endl;
	cin >> edad;
	cout << endl<<"Ingrese la raza" <<endl;
	cin >> raza;
	cout << endl<<"Ingrese la estatura" << endl;
	cin >> estatura;
	cout << endl << "Ingrese el peso" << endl;
	cin >> peso;
	cout << endl << "Ingrese el sexo (H o M)" <<endl;
	cin >> sexo;
	value=0;
	for(int i=0; i<sizeof(nom)/sizeof(char)-1;i++){
		value = value + toupper(nom[i]);
	}
	nombre=nom;
	value = (value*32)%999;
	struct dogType dog={"",edad,"",estatura,peso,sexo,value}; //Crea el perro con los valores dados
	strncpy(dog.Nombre, nombre.c_str(), 32); //Asigna el nombre al perro
	strncpy(dog.Raza, raza.c_str(), 16); //Asigna la raza al perro
	return dog;
}

int main(){
	int opcion,num,search;
	char nSearch[32];
	struct dogType dog;
	fd=socket(AF_INET,SOCK_STREAM,0);
	//validar errores
	if(fd==-1) {
		perror("Error creando el socket");
		return 0;
	}
	server.sin_family=AF_INET;
	server.sin_port=htons(PORT);
	server.sin_addr.s_addr=inet_addr("127.0.0.1");
	bzero(server.sin_zero,8);

	r=connect(fd,(struct sockaddr*)&server,sizeof(struct sockaddr));
	//validar errores
	if(r==-1) {
		perror("Error de conexion con el servidor");
		return 0;
	}
	do{
		system("clear");
		cout << "\t\t¡Bienvenido al sistema de la veterinaria!" <<endl<<endl;
		cout << "\t1.Ingresar Registro" << endl;
		cout << "\t2.Ver Registro" <<endl;
		cout << "\t3.Borrar Registro" <<endl;
		cout << "\t4.Buscar Registro"<<endl;
		cout << "\t5.Salir" << endl<<endl;
		cout << "\tSeleccione una opcion"<<endl<<"\t";
		//Recibe la opcion y realiza la operacion seleccionada
		cin >> opcion;
		r = recv(fd,&numPerros,sizeof(int),0);
		num =1 ; //Cambiar por algun identificador quizas
		switch(opcion){
			case 1: system("clear");
					dog = nuevoRegistro();
					r = sendCliente(1,(void *)&dog);
					if(r==-1) {
						perror("Error enviando los datos");
						return 0;
					}
					cout << "El registro se inserto correctamente" << endl;
					doPause();
					break;
			case 2: system("clear");
					cout << "Actualmente hay "<<numPerros<<" perros"<<endl;
					cout << "Ingrese un numero" << endl;
					cin >> search;
					r = sendCliente(2,(void *)&search);
					doPause();
					break;
			case 3: system("clear");
					cout << "Ingrese un numero" << endl;
					cin >> search;
					r = sendCliente(3,(void *)&search);
					cout << "El registro se borro con exito" << endl;
					doPause();
					break;
			case 4: system("clear");
					cout << "Ingrese un nombre" << endl;
					cin.clear();
					cin >> nSearch;
					r = sendCliente(4,(void *) nSearch);
					doPause();
					break;
			case 5: r = sendCliente(5,(void *)&num);
					system("clear");
					cout << "\t\tGracias por utilizar el programa"<<endl<<endl<<"\t";
					doPause();
					close(fd);
					break;
			default: cout << "\tInserte un valor valido"<<endl;
					doPause();
					break;
		}
	}while(opcion!=5);
	/*do{
		r=recv(fd,buffer,12,0);
	}while (r<12);
	buffer[r]='\0';
	
	printf("%s",buffer);
	printf("%s","\n");
	*/
return 0;
}