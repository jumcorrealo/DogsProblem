#include <stdio.h>
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <cstdlib>
#include <time.h>
using namespace std;

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

void cargarNombres(string array[1729]){
	ifstream nombres("nombresMascotas.txt");
	if(nombres.is_open()){
		for(int i=0; i<1729;i++){
			nombres >> array[i];
		}
	nombres.close();
	}
}

void cargarRazas(string array[50]){
	ifstream razas("nombresRazas.txt");
	if(razas.is_open()){
		for(int i=0; i<50;i++){
			razas >> array[i];
		}
	razas.close();
	}
}

int main(){
	FILE* datos; 
	string arrayNombre[1729];
	string arrayRaza[50];
	char genero[2];
	genero[0]='M';
	genero[1]='H';
	srand ( time(NULL) ); //Con esto se pone una seed para generar numeros aleatorios, utiliza el tiempo que siempre cambia para que el numero siempre sea distinto
	cargarNombres(arrayNombre); // Crea un arreglo con todos los nombres del archivo
	cargarRazas(arrayRaza); //Crea un arreglo con todas las razas del archivo
	datos=fopen("dataDogs.dat","w");
	for(int i=0;i<1000000;i++){
		char nom[32]=""; // Este es para la key
		int name = rand() % 1729; // el rand() % 1729 genera un numero aleatorio entre 0 y 1728
		int age = rand() % 13; // lo mismo entre 0 y 12
		int race = rand() % 50; // entre 0 y 4 (Que esto hay que cambiar cuando se metan mas razas
		int height = rand() % 150; // entre 0 y 149 para la altura
		float weight = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/10)); //Este genera un numero aleatorio float entre 0 y 10 si no me equivoco
		int genre = rand() % 2; // y este uno entre 0 y 1
		strncpy(nom, arrayNombre[name].c_str(), 32); //con esto se convierte el string que se lee del arreglo en un array de char
		
		int value=0;
		for(int j=0; j<sizeof(nom)/sizeof(char);j++){
			value = value + toupper(nom[j]);
		} // con esto se suma los valores ascii de la cadena transformada a solo mayusculas, para la busqueda
		value = (value*sizeof(nom)/sizeof(char))%999; //y con esto se genera una key entre 0 y 999
		
		struct dogType dog = {"",age,"",height, weight, genero[genre],value,i+1}; //se crea el perro con los valores que conseguimos antes menos el nombre y la raza
		strncpy(dog.Nombre, arrayNombre[name].c_str(), 32); //con esto se copia como un char[] el string que esta en la posicion del numero random que generamos
		strncpy(dog.Raza, arrayRaza[race].c_str(), 16); // y este lo mismo pero con razas
		fwrite(&dog,sizeof(struct dogType),1,datos); // y con esto escribimos la estructura en un archivo
	}
	fclose(datos);
	FILE* num = fopen("numDogs.dat","w");
	int n = 1000000;
	fwrite(&n,sizeof(int),1,num);
	fclose(num);
	FILE* totnum = fopen("totDogs.dat","w");
	fwrite(&n,sizeof(int),1,totnum);
	fclose(totnum);
}