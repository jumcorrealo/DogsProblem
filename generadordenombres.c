#include <stdlib.h>
#include <stdio.h>

//Se declara la estructura

struct dogType{

	char nombre [32];
	char tipo [32];
	int edad;
	char raza[16];
	int estatura;
	float peso;
	char sexo;
	
};

//Metodo de generación

void generar(struct dogType *ap){

	FILE *data;
	data=fopen("dataDogs.dat","r+b");
	//Abre archivo, si no existe; lo crea
	if(!data){
		data=fopen("dataDogs.dat","w+b");
	} else {
        printf("arachivo dataDogs open\n");
    }
    
    /**********************************nombres********************************/
    FILE *fnew = fopen("nombres.txt", "r");
	if(fnew==NULL){
		printf("Archivo de nombres no encontrado\n");
		return;
	}else {
        printf("arachivo nombres open\n");
    }
    char nombres[1716][32];
	int ascendente = 0;
	while(!feof(fnew)){
		fscanf(fnew, "%s", nombres[ascendente]);
		ascendente++;
	}
    
    /***************************Tipo***************************************/
    FILE *fnew2 = fopen("tipo.txt", "r");
	if(fnew2==NULL){
		printf("Archivo de tipo no encontrado\n");
		return;
	} else {
        printf("arachivo tipo open\n");
    }
	//Carga de Datos a memoria
	char tipo[20][32];
	int up = 0;
	while(!feof(fnew2)){
		fscanf(fnew2, "%s", tipo[up]);
		up++;
	}
	

    /***********************Edad********************************/
    FILE *fnew3 = fopen("edad.txt", "r");
	if(fnew3==NULL){
		printf("Archivo de edad no encontrado\n");
		return;
	}else {
        printf("arachivo edad open\n");
    }
    int edad[20];
	int up2 = 0;
	while(!feof(fnew3)){
		fscanf(fnew3, "%d", &edad[up2]);
		up2++;
	}
    
    /**********************raza*******************************/
    FILE *fnew4 = fopen("raza.txt", "r");
	if(fnew4==NULL){
		printf("Archivo de raza no encontrado\n");
		return;
	} else {
		printf("arachivo raza open\n");
    }
	
	char raza[20][32];
	int up3 = 0;
	while(!feof(fnew4)){
		fscanf(fnew4, "%s", raza[up3]);
		up3++;
    }
	

    do{
	   	printf("Genero del Animal(M/H)\n");
			scanf(" %c",&ap->sexo);
	   	if(ap->sexo!='M'&& ap->sexo!='H'&& ap->sexo!='m'&& ap->sexo!='h'){
	   		printf("Genero Inválido\n");
			continue;
	   	}
	   	break;
	}while(1);
	

	for(int i=0;i<=10000000;i++){
		//Se genera un numero aleatorio entre todos los nombres en el archivo
		int aleatorio = rand()%1000;
        int aleatorio2 = rand()%20;
		//Se copia un nombre aleatorio de memoria al struct
		strcpy(ap->nombre,nombres[aleatorio]);
        strcpy(ap->tipo,tipo[aleatorio2]);
        ap->edad = edad[aleatorio2];
        strcpy(ap->raza,raza[aleatorio2]);
        ap->estatura = aleatorio2;
        ap->peso = aleatorio2;
		//Se carga la estructura a el archivo
        fwrite(ap,sizeof(struct dogType),1,data); //Inserta la estructura en el Archivo.
    	
	}
	fclose(data);
    free(ap);
	printf("Archivo Generado\n");
}

int main(){

	struct dogType * ap;
	ap = malloc(sizeof(struct dogType));

	//Abre archivo, si no existe; lo crea

	FILE *data;
		data=fopen("dataDogs.dat","r+b");
		if(!data){
        	data=fopen("dataDogs.dat","w+b");
		}

	int imput;
	int activo = 1;
		printf("\n===============================================\n");
		printf("|	SISTEMA DE GENERACIÓN DE ANIMALES      |\n");
		printf("===============================================\n\n");
		printf("Escoja una de la siguientes opciones:\n\n");
		printf("\t1. Generar Archivo\n");
		printf("\t2. Salir\n");

	while(activo == 1)
	{


	int imput;
		printf("\nIngrese opción:");
		scanf("%d", &imput);

		switch(imput)
		{

		case 1:
		printf("Generar Archivo:\n");
                generar(ap);
                printf("\n");

          	case 2:
		printf("\n\n------Salir------\n\n");
		activo = 0;
		break;

		default:
      		printf("Opción no valida, intente de nuevo\n");
		break;

		};
};
fclose(data);
free(ap);
};