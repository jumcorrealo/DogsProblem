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
#include <fstream>
#include <list>


int numPerros,tot,fd2;
list<struct dogType> *array;

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

void printDog(dogType dog){
	cout << "#" << dog.Pos;
	cout << " "<<dog.Nombre;
	cout << ", Edad: "<<dog.Edad;
	cout << ", Raza: " << dog.Raza;
	cout << ", Estatura: " << dog.Estatura;
	cout << ", Peso: " << dog.Peso;
	cout << ", Sexo: " << dog.Sexo<<endl;
}

void cargarPerros(){
	ifstream data;
	data.open("dataDogs.dat", ios::in | ios::out);
	delete[] array;
	array = new list<struct dogType> [1000];
	for(int i=0; i<tot; i++){
		struct dogType dog;
		data.read((char*)&dog,sizeof(struct dogType));
		array[dog.Key].push_back(dog);
	}
	data.close();
}

void cargarNumero(){
	FILE* numDogs,*totnum;
	numDogs=fopen("numDogs.dat","r");
	fread(&numPerros,sizeof(int),1,numDogs);
	fclose(numDogs);
	totnum = fopen("totDogs.dat","r");
	fread(&tot,sizeof(int),1,totnum);
	fclose(totnum);
}

struct dogType verRegistro(int search){
	struct dogType dog;
	struct dogType nullDog;
	nullDog.Key=-1;
	ifstream data;
	data.open("dataDogs.dat");
	data.seekg(sizeof(struct dogType)*(search-10));
	if(search > tot) return nullDog;
	do{
		data.read((char*)&dog,sizeof(struct dogType));
		if(dog.Pos>search) return nullDog;
	}while(dog.Pos!=search);
	data.close();
	return dog;
}

void agregarRegistro(dogType dog){
	FILE * data,*num,*totnum;
	data = fopen("dataDogs.dat","a+");
	tot=tot+1;
	dog.Pos=tot;
	fwrite(&dog,sizeof(struct dogType),1,data); //Escribe el perro al final del archivo
	//array[dog.Key].push_back(dog);
	fclose(data);
	numPerros = numPerros +1;
	num = fopen("numDogs.dat","w");
	fwrite(&numPerros,sizeof(int),1,num);
	fclose(num);
	totnum = fopen("totDogs.dat","w");
	fwrite(&tot,sizeof(int),1,totnum);
	fclose(totnum);
}

void borrarRegistro(int search){
	ifstream data;
	ofstream temp;
	struct dogType dog;
	FILE * num;
	data.open("dataDogs.dat");
	data.seekg(sizeof(struct dogType)*(search-1));
	data.read((char*)&dog,sizeof(struct dogType));
	// Crea un archivo temporal en donde se escriben todas las estructuras menos la que se quiere borrar
	// luego se borra el archivo original y se renombra el temporal 
	data.seekg(0);
	temp.open("temp.dat");
	for(int i=0; i<numPerros;i++){
		data.read((char*)&dog,sizeof(struct dogType));
		if(dog.Pos!=search){
			temp.write((char*)&dog,sizeof(struct dogType));
		}
	}
	data.clear();
	data.seekg(0, ios::beg);
	data.close();
	temp.close();
	remove("dataDogs.dat");
	rename("temp.dat","dataDogs.dat");
	//--------
	//Resta una unidad al numero de perros en el archivo
	numPerros=numPerros-1;
	num=fopen("numDogs.dat","w");
	fwrite(&numPerros,sizeof(int),1,num);
	fclose(num);
	cargarPerros();
}

void buscarRegistro(char search[32]){
	list<dogType>::iterator it;
	int value=0;
	int count=0;
	bool compare=false;
	for(int i=0; i<sizeof(search)/sizeof(char)-1;i++){
		value = value + toupper(search[i]);
	}
	value = (value*32)%999;
	it=array[value].begin();
	while(it != array[value].end()){
		compare = true;
		for(int j=0; j<sizeof(search)/sizeof(char);j++){
			if(toupper(search[j])!=toupper(it->Nombre[j])) compare = false;
		}
		if(compare==true) {
			//printDog(*it);
			count++;
		}
		it++;
	}
	send(fd2,(void *) &count,sizeof(int),0);
	it=array[value].begin();
	int cnt=0;
	struct dogType dogs [count];
	while(it != array[value].end()){
		compare = true;
		for(int j=0; j<sizeof(search)/sizeof(char);j++){
			if(toupper(search[j])!=toupper(it->Nombre[j])) compare = false;
		}
		if(compare==true) {
			dogs[cnt]=*it;
			cnt++;
		}
		it++;
	}
	for(int i=0;i<count;i++){
		send(fd2,&dogs[i],sizeof(struct dogType),0);
	}
}

int main(){
	int fd,r;	
	struct sockaddr_in server;
	struct sockaddr_in cliente;
	#define PORT 6969	
	#define BACKLOG 2
	
	socklen_t sin_size;

	fd=socket(AF_INET,SOCK_STREAM,0);
	//Validar errores
	if(fd==-1) {
		perror("Error creando el socket");
		return 0;
	}
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = INADDR_ANY;
	bzero(server.sin_zero,8);
	
	r=bind(fd,(struct sockaddr*)&server,sizeof(struct sockaddr));
	//Validar errores
	if(r==-1) {
		perror("Error configurando socket");
		return 0;
	}
	r=listen(fd,BACKLOG);
	//Validar errores
	if(r==-1) {
		perror("Error en listen");
		return 0;
	}
	//Si se conectan varios clientes while() y fork()		

	fd2 = accept(fd,(struct sockaddr*)&cliente,&sin_size);	
	//Validar errores
	if(fd2==-1) {
		perror("Error conectando con el cliente");
		return 0;
	}
	
	//////////////////////// PROGRAMA PRINCIPAL //////////////////////////////////////
	int opc,search;
	char nSearch[32];
	do{	
		cargarNumero();
		cargarPerros();
		r = send(fd2,&numPerros,sizeof(int),0);
		r = recv(fd2,&opc,sizeof(int),0);
		struct dogType dog;
		switch(opc){
			case 1:	r = recv(fd2,&dog,sizeof(struct dogType),0);
					agregarRegistro(dog);
					break;
			case 2: r = recv(fd2,&search,sizeof(int),0);
					dog = verRegistro(search);
					r = send(fd2,(void *)&dog,sizeof(struct dogType),0);
					break;
			case 3: r = recv(fd2,&search,sizeof(int),0);
					borrarRegistro(search);
					break;
			case 4: r =  recv(fd2,nSearch,32,0);
					buscarRegistro(nSearch);
					break;
			case 5: cout << "Cerrando server" << endl;
					break;
			default:cout << "default" << endl;
					break;
		}
	} while(opc!=5);
	close(fd2);
	close(fd);
return 0;
}


	// Definida en la libreria
	//struct sockaddr_in{
		//short sin_family;
		//unsigned short sin_port;
		//struct in_addr sin_addr;
		//char sin_zero[8]: //No sirve para una puta mierda
	//};

	//struct in_addr{
	//unsigned long s_addr
	//}