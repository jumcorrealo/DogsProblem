#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h> //Permite hacer el número aleatorio.
#include <termios.h>//libreria agregada e instalada para la funcion getch()
#include <ctype.h>
#include <math.h>
/*Constantes definidas*/
#define SIZENOM 32 //Entero para el tamaño de bytes los nombres
#define SIZERAZ 32 //ENtero para el tamaño de bytes las razas
#define ARRAY_SIZE 10013 //Entero que define el tamaño del archivo

/*********************************GETCH********************************/
static struct termios old, new;

/* Initialize new terminal i/o settings */
void initTermios(int echo) {
  tcgetattr(0, &old); //grab old terminal i/o settings
  new = old; //make new settings same as old settings
  new.c_lflag &= ~ICANON; //disable buffered i/o
  new.c_lflag &= echo ? ECHO : ~ECHO; //set echo mode
  tcsetattr(0, TCSANOW, &new); //apply terminal io settings
}

/* Restore old terminal i/o settings */
void resetTermios(void){
  tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo){
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}

/*
Read 1 character without echo
getch() function definition.
*/
char getch(void){
  return getch_(0);
}

/*
Read 1 character with echo
getche() function definition.
*/
char getche(void)
{
  return getch_(1);
}



////////////////////////////////////////////////////////////////////////////////
////////////////////// STRUCT/////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
struct dogType{ //datos de la estructura
    char nombre[SIZENOM]; //Nombre: Cadena Máxima de 32 Caracteres
    char tipo[32]; //Tipo: Cadena Máxima de 32 Caracteres
    int edad; //Edad: Entero de 32 bits
    char raza[16]; //Raza: Cadena de máximo 36 caracteres
    int estatura; // Estatura: Entero de 32 bits
    float peso; // Peso: Real de 32 bits;
    char sexo; //Sexo: 1 Caracter
    char fichero; //fichero de las historia clinica
};

const int Size= sizeof(struct dogType); //Constante que almacena el tamaño de bites de la estructura


////////////////////////////////////////////////////////////////////////////////
////////////////////// LINKED LIST & HASH//////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

//Creamos una estructura nodo que nos guardará la estructura y su posición
struct node{
    int index;                      //Lista
    struct dogType elem;            //Respectiva estructura
    struct node *next;              //El nodo enlazado
};

struct linkedList{
    struct node *head;              //Se crea un nodo cabecera para la linkedList general
    int size;
} *mainList, *ptr;                  //un arreglo de 10013 posiciones para implementar una tabla hash.
                                    //1009 posiciones con el fin de que hayan menos colisiones al ser un número primo


int hashFunction(char *str); //FUnción que toma el nombre de cada perro y lo asigna a su estructura. Retornandoce el indice de esta
void addElem(struct linkedList *list, struct dogType dog);//Función que añade un elemento a la hash
struct dogType* getElem(struct linkedList *list, int index);//Función que retorna un puntero a la estructura en la hash
void addElemWithCurrentIndex(struct linkedList *list, struct node *theNode);//Se añade un nodo a la lista sin modificar el index
void updateIndex(struct linkedList *list, int start); //Corrige los indices en la hash cuando se actualizan los registros.
void removeElem(struct linkedList *list, int index); //Elimina un nodo.
void removeElemByFileIndex(struct linkedList *list, int index); //ELimina un nodo basado en la posición especifica
struct linkedList* newLinkedList(); //Retorna un puntero a la nueva lista enlazada

////////////////////////////////////////////////////////////////////////////////
//////////////////////FUNCIONES DE CARGA DE DATOS//////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void loadMainList();//Se cargan las estructuras presentes en la lista. Nos verifica desde principio si esta disponible
void loadHashTable( ); //Cargará la Hash Table en orden
void printList(struct linkedList *list, char *str); //Imprime todas las listas asignadas en busqueda
void crear_historia();// Crea la historia clinica
void generar(struct dogType *ap); //genera los datos


/////////////////////////////////////////////////////////////////////////////
////////////////////////////FUNCIONES PRINCIPALES////////////////////////////////
////////////////////////////////////////////////////////////////////////////


FILE *fp; //Archivo para guardar las estructuras.
struct dogType *dog; //Estructura dog general

void escribir(); //Función que nos permitirá escribir un nuevo registro.
void ver(); //Función que nos permitirá ver un registro en especificio
void buscar(); //Función que nos permitirá ver los registros de un perro en especificio
void borrar(); //Función que nos permitirá eliminar un registro en especifico


int TamanioArchivo(FILE *fp); //Función que nos calculará el tamaño actual del archivo o en mismas palabras, la cantidad de Estructuras

int CantidadStruct; //Cuenta la cantidad de Estructuras que habrá en el archivo.



////////////////////////Main//////////////////////////////////////////////////
int main(){
    loadMainList( );
    loadHashTable( );
    
    char x,y;
    char m; //Variable de rebote del getch. Evita que confunda el getch como valor de x
    int salir=0;
    while(salir==0){
        CantidadStruct=0;
        printf("Digite la opcion a realizar: \n");
        printf("1- Ingresar Registro \n");
        printf("2- Ver Registro \n");
        printf("3- Borrar Registro \n");
        printf("4- Buscar Registro \n");
        printf("5- Salir \n");
        printf("\nOpcion: ");
        scanf("%s", &x);
        switch(x){
            case '1':
                printf("\nProceso de inscripcion, orpima una tecla para continuar\n\n");
                m=getch();
                getch();
                system("clear");
                escribir(); //Abre la opción de inscribir un perro
               	printf("\nInscripcion terminada, imprima una tecla para continuar");
               	m=getch();
               	getch();
                system("clear");
                break;
            case '2':
                printf("\nProceso de ver, orpima una tecla para continuar\n\n");
                m=getch();
               	m=getch();
               	system("clear");
                ver();// Abre la opción de ver un registro de perro.
               	printf("\nVista terminada, imprima una tecla para continuar\n\n");
               	getch();
               	getch();
                crear_historia();
               	system("clear");
		break;
            case '3':
                printf("\nProceso de borrado, orpima una tecla para continuar\n\n");
                m=getch();
               	m=getch();
               	system("clear");
                borrar(); //Abre la opción de borrar el registro de un perro
               	printf("\nBorrado completado, imprima una tecla para continuar\n\n");
               	m=getch();
               	m=getch();
               	system("clear");
                break;
            case '4':
                printf("\nProceso de Busqueda, orpima una tecla para continuar\n\n");
                m=getch();
               	m=getch();
               	system("clear");
                buscar();
                //buscar(); //Abre la opción de busqueda del registro de uno o más perros
                printf("\nBuscar completado, imprima una tecla para continuar\n\n");
               	m=getch();
               	m=getch();
               	system("clear");
                //printf("\nQue hace aqui?, esto aun no esta listo.\n");
                break;
            case '5':
                exit(1);
                break;
            
            default:
                break;
        }
    }
    return 0;
}


//////////////////Tamaño del Archivo-Numero de Estructuras////////////////

int TamanioArchivo(FILE *fp){
    fseek(fp, 0, SEEK_END);
    int Total=ftell(fp)/Size;
    return Total;
}

/*
*
*
*Funciones correspondientes a la tabla HASH
*
*
*/

int hashFunction(char *str){
    int pos=0, k=0;
    char f = ' ';
    while(f!='\0'){
        f=str[pos++];
        k += f;
        k *=3;
    }
    return k % ARRAY_SIZE;
    
}

void addElem(struct linkedList *list, struct dogType dog){
    struct node *newNode, *current;
    newNode = malloc(sizeof(struct node));//Se añade abre un espacio en memoria para añadir la estructura a una lista.
    if (newNode==NULL){
        perror("Error creando nodos para la tabla Hash");
        exit(-1);
    }

    //Asignamos al nodo la estructura correspondiente y elementos enlazados
    newNode->elem= dog;
    newNode->next= NULL;
    newNode->index= list->size;

    //Función de rutina para enlazar las estructuras
    if(list->head==NULL){
        list->head=newNode;
        list->size++;
    }else{
        newNode -> next = list -> head;
        list -> head = newNode;
        list -> size++;
    }
}

struct dogType* getElem(struct linkedList *list, int index){
    struct node *current = list-> head;
    struct dogType *dogNode = malloc(Size);
    if (dogNode==NULL){
        perror("Error retornando el puntero de estructura");
        exit(-1);
    }

    while (index-- > 0){
        current = current -> next;
    }
    dogNode=&current->elem;
    return dogNode;
}

void addElemWithCurrentIndex(struct linkedList *list, struct node *theNode){
    struct node *newNode = malloc(sizeof(struct node));
    if (newNode == NULL){
        perror( "Error creando un nodo" );
        exit(-1);
    }
    newNode->elem=theNode -> elem;
    newNode->next=NULL;
    newNode->index=theNode -> index;
    if(list->head==NULL){
        list->head=newNode;
        list->size++;
    }else{
        newNode->next= list -> head;
        list->head= newNode;
        list->size++;
    }


}

void updateIndex(struct linkedList *list, int start){
    if ( list == NULL ){printf("No hay registros");return;}

    struct node *current = list->head; //EL nuevo nodo cabecera
    while(current != NULL){
        if (current->index > start){
            current->index--;
        }
        current = current->next;
    }
}

void removeElem(struct linkedList *list, int index){
    if (list->size == 0){printf("No hay registros");return;}
    if (index < 0 || index > list->size-1){printf("No permite borrar registros");return;}
    struct node *current, *toRemove;
    current = list -> head;
    toRemove = list -> head;

    if (index == 0){
        list-> head = list -> head -> next;
        free(toRemove);
        list->size--;
    }else{
        toRemove = toRemove -> next;
        while ( index-- > 1 ){
            current = current -> next;
            toRemove = toRemove -> next;
        }
        current -> next = toRemove -> next;
        free( toRemove );
        list -> size--;
    }
}

void removeElemByFileIndex(struct linkedList *list, int index){
    if (list->size==0){printf("No hay registros");return;}
    struct node *current, *toRemove;
    current = list -> head;
    toRemove = list -> head;

    if ( toRemove -> index == index ){
        list -> head = list -> head -> next;
        free( toRemove );
        list -> size--;
        return;
    }
    toRemove = toRemove -> next;
    while (toRemove!=NULL){
        if (toRemove->index == index){
            current->next = toRemove -> next;
            free( toRemove );
            list -> size--;
            return;
        }
        current = toRemove;
        toRemove = toRemove -> next;
    }
}

struct linkedList* newLinkedList(){
    struct linkedList *list = malloc( sizeof( struct linkedList ) );
    if ( list == NULL ){
        perror( "Hubo un error con la recreacion de la lista" );
        exit(-1);
    }
    struct node *mainHead = NULL; //Creamos una nueva cabecera
    list -> head = mainHead;
    list -> size = 0;
    return list;
}

/*
*
*
* FUNCIONES DE CARGA DE ESTRUCTURAS Y HASH TABLE
*
*
*/


void loadMainList(){
    int r;
    mainList = newLinkedList();
    dog = malloc(Size);
    //Errores
    if (dog == NULL){
        perror( "Error cargando la lista principal." );
        exit( -1 );
    }
    fp=fopen( "dataDogs.dat", "r" );
    //Errores
    if (fp==NULL){perror("No se encontro el archivo dataDogs.dat"); return;}

    int i;
    int estrucnum=TamanioArchivo(fp);

    for(i=0;i<estrucnum;i++){ //Leemos cada estructura a ver si todo está en orden
        fseek(fp, (i)*Size, SEEK_SET);
        r=fread(dog,Size,1,fp);
        if(r!=1){
            perror( "Error al leer estructura" );
            exit( -1 );
        }
        addElem(mainList,*dog);
    }
    free(dog);
    fclose(fp);
}

void loadHashTable( ){
    fp=fopen("dataDogs.dat","a+");//Añadirá la nueva estructura al final del archivo.

    if (ptr == NULL){
        printf("ingreso al primer if \n");
        //ptr = malloc( sizeof( struct linkedList ) * ARRAY_SIZE ); //Se crea una lista del tamaño asignado arriba
        ptr = malloc (107374);
        //ptr = malloc(sizeof(fp));
        printf("petición del malloc done");
        if ( ptr == NULL ){
            perror( "Error, no existe la hash PTR" );
            exit( -1 );
        }
    }
    struct node *current = mainList -> head;
    int index;
    while ( current != NULL ){
        index = hashFunction( current -> elem.nombre ); //Se le asigna a cada estructura una posición en ptr
        addElemWithCurrentIndex( &ptr[ index ], current );
        current = current -> next;
    }
}

void printList(struct linkedList *list, char *str){
    struct node *current;
    if (list->head == NULL){ //Busqueda vacía
        printf( "\nMascotas:\n\n" );
        printf( "No se pudieron encontrar registros" );
    }else{
        int regs = 0;
        char sig;
        current = list -> head;
        printf( "\nMascotas:\n\n" );
        while(current != NULL){
            if (!strcmp(str, current -> elem.nombre)){
                printf( "Nombre: %s\n", current -> elem.nombre );
                printf( "Edad: %i\n", current -> elem.edad );
                printf( "Raza: %s\n", current -> elem.raza );
                printf( "Estatura: %i cm\n", current -> elem.estatura);
                printf( "Peso: %3.2lf Kg\n", current -> elem.peso );
                printf( "Sexo: %c\n\n", current -> elem.sexo );
                regs++;
                
            }
            
            current = current -> next;
            /* if(sig=='salir'){
                break;
            } */
        }
        printf( "Se encontraron %i registros", regs );
    }

}



/*
*
*
* FUNCIONES DE REGISTRO DE ESTRUCTURAS
*
*
*/

/////////////////////////Insertar Estructura///////////////////////////
void escribir(){
    fp=fopen("dataDogs.dat","a+");//Añadirá la nueva estructura al final del archivo.
    if (fp==NULL) {perror("No se encontro el archivo dataDogs.dat"); return;}
	rewind(fp); //REvobinamos en caso de que haya error


	dog=malloc(Size); //Abrimos un tamaño en memoria para guardar la estructura escrita
	/*
	*
	*Acá escribiremos los datos del perro
	*
	*/
	printf("\n Escriba el nombre del animal: ");
		scanf("%s", dog->nombre);
        dog -> nombre[ 0 ] = toupper( dog -> nombre[ 0 ] );
        int i;								//Solicitamos todos los respectivos datos.
        for( i = 1; dog -> nombre[ i ] != '\0'; i++ )
        dog -> nombre[ i ] = tolower( dog -> nombre[ i ] );

        printf("\n Escriba el tipo de animal (perro, gato, tortuga ... etc ): ");
        scanf("%s", dog->tipo);
    	printf("\n Escriba su edad en anios: ");
		scanf("%i", &dog->edad);
   	    printf("\n Escriba la raza: ");
		scanf("%s", dog->raza);
    	printf("\n Escriba su estatura en centimetros: ");
		scanf("%i", &dog->estatura);
 	    printf("\n Escriba su peso en Kg: ");
		scanf("%f", &dog->peso);
    	printf("\n Escriba su sexo[H/M]: ");
        char Gen;
        int val;
        scanf("%s", &Gen);
	//Esto nos servirá para distinguir perros Machos y Hembras
        while(Gen!='H'&&Gen!='M'&&Gen!='h'&&Gen!='m'){
            printf("\n Caracter no valido.\n Por favor, intente de nuevo: ");
            scanf("%s", &Gen);
        }
		dog->sexo=Gen;

    	fwrite(dog, Size, 1, fp); //Inserta la estructura en el Archivo.
    	CantidadStruct= TamanioArchivo(fp); //Nos dará el entero para tener el registro del perro.
        addElem( mainList, *dog ); //Agregamos la nueva estructura a mainlist
    	int indice = hashFunction(dog -> nombre);			//Aplicamos la función hash al nuevo nombre de la estructura.
        addElemWithCurrentIndex( &ptr[ indice ], mainList -> head ); //Adicionamos en ptr la nueva estructura.
    	printf("\nEl registro de %s quedo efectivamente guardado.\nNo de registro: %i \nKey del registro: %i",dog->nombre,CantidadStruct,indice);
    	fclose(fp);
    	free (dog); //Cerramos el archivo y liberamos memoria
}

/////////////////////////Ver Estructura Especifica///////////////////////////
void ver(){

    	fp = fopen("dataDogs.dat","r"); //Abrimos el archivo únicamente para lectura
    	if (fp==NULL) {perror("No se encontro el archivo dataDogs.dat");return;}
    	dog=malloc(Size);
    	CantidadStruct= TamanioArchivo(fp);
    //Me verificará si hay perros registrados
    	if (CantidadStruct==0){
        	printf("No hay registros");
        	return;
    	}
    //Si hay estructuras, imprimirá la cantidad que hay y luego me pedirá que inserte un numero en el rango establecido
    	printf("Hay una cantidad de %i perros registrados.\n\nPor favor, inserte el numero de registro que desea ver: ", CantidadStruct);
    	int reg;
    	scanf("%d", &reg);
    	while(reg>CantidadStruct){
        	printf("El registro no existe. Por favor, intente otra vez: ");
        	scanf("%d", &reg);
    	}
    	rewind(fp);
        
    	fseek(fp,(reg-1)*Size,SEEK_SET); //se mueve el apuntador para saber desde donde copiar
    	fread(dog, Size,1,fp);

        printf("\nNombre: %s\n",dog->nombre);
        printf("\nTipo: %s\n", dog->tipo);
        printf("Edad: %i anios\n", dog->edad);
        printf("Raza: %s\n", dog->raza);
        printf("Estatura: %icm\n", dog->estatura);
        printf("Peso: %3.2f Kg\n", dog->peso);
        printf("Sexo: %c\n", dog->sexo);
    	fclose(fp);
    	free(dog);
    	printf("\nOprima cualquier tecla para continuar\n");
        /* int temp = hashFunction(dog->nombre);
        crear_historia(temp); */
}

///////////////////////////////Borrar Estructura Especifica///////////////////////////

void borrar(){
	FILE *tmp; //Creamos un archivo temporal.
	int i; //Variable para el ciclo
	int aEliminar; //Número del perro a eliminar
	dog=malloc(Size);
	fp=fopen("dataDogs.dat","r");
	char nom[SIZENOM];

	//Errores
	if (fp==NULL) {perror("No se encontro el archivo dataDogs.dat"); return;}
	CantidadStruct= TamanioArchivo(fp);

	if (CantidadStruct==0){printf("No hay registros");return;}

	printf("Hay una cantidad de %i registros. Cual desea elinimar?: ", CantidadStruct);

	scanf("%d",&aEliminar); //Pedimos que inserte el número entero en el que el perro fue guardado y que esté en el rango del archivo.
	while(aEliminar>CantidadStruct){
        	printf("El registro no existe. Por favor, intente otra vez: ");
        	scanf("%d", &aEliminar);
    	}

	tmp=fopen("dataDogs.tmp", "a+");
    printf("\nBorrando, espere un momento por favor\n");
	/*
	*
	*Lo que hace este For será hacer una duplica de todos los archivos en uno temporal, menos el perro indicado
	*
	*/
	for(i=0;i<CantidadStruct;i++){
		fseek(fp, (i)*Size,SEEK_SET);
		if(i!=(aEliminar-1)){
			fread(dog, Size, 1, fp);
			//strcpy(nom,dog->nombre);
			fwrite(dog, Size, 1,tmp);
		}else{
			fread(dog, Size, 1, fp);


		}
	}
	fclose(fp);
	fclose(tmp);
	system("rm dataDogs.dat"); //Borramos el archivo original
	fp=fopen("dataDogs.dat","a+");
	tmp=fopen("dataDogs.tmp","r+");
	/*
	*
	*Hacemos el proceso a la inversa, copiamos los elementos de temporal al archivo original.
	*
	*/
	for(i=0;i<(CantidadStruct-1);i++){
		fseek(fp, (i)*Size,SEEK_SET);
		fread(dog, Size, 1, tmp);
		fwrite(dog, Size, 1,fp);
	}
    
    struct dogType *doggy = getElem( mainList, mainList -> size - aEliminar);
    printf("struct dogtype modificada \n");
    struct dogType myDog = *doggy;
    printf("struct 2 modificada \n");

    doggy = getElem( mainList, mainList -> size - aEliminar );//Buscamos la estructura con el número de registro correspondiente.
    printf("doggy done\n");
    removeElem( mainList, mainList -> size - aEliminar );
    printf("remove eleent done\n");
    int indice = hashFunction(myDog.nombre);			         //Removemos la estructura de ptr.
    removeElemByFileIndex( &ptr[ indice ], aEliminar - 1 );

    for ( i = 0; i < ARRAY_SIZE; i++ )
        updateIndex( &ptr[ i ], aEliminar - 1 );
                                                    //Corregimos los indices de las estructuras.
    printf( "Hash table actualizada.\n" );


    remove("dataDogs.tmp");
	//free(dog);
	fclose(fp);
	fclose(tmp);
    printf("\nEl registro ha sido exitosamente eliminado.\n");
}


///////////////////////////////Mostrar Estructuras Por Nombre///////////////////////////


void buscar( ){
    char NombreBusqueda[SIZENOM]; //Insertamos un nombre de tamaño 32 para buscar y comparar
    int i;
    printf( "Ingrese el nombre de la mascota a buscar: " );
    scanf( " %s", NombreBusqueda );
    NombreBusqueda[ 0 ]=toupper(NombreBusqueda[0]);
    for( i = 1; NombreBusqueda[ i ] != '\0'; i++ )		//Corregimos las mayúsculas y minúsculas.
    NombreBusqueda[ i ] = tolower( NombreBusqueda[ i ] );

    int index = hashFunction(NombreBusqueda);
    printList(&ptr[index],NombreBusqueda);//Imprimimos las estructuras por separado
}

void crear_historia(){

    FILE *fp;

    char nombre[32];
    char sys[32] = "gedit ";
 	char caracter;
    char entrada[9];

    printf("\nIntroduce 'Número del registro' para buscar la historia clínica: \n");	

    scanf("%s",nombre);

    fp = fopen(nombre, "r");
    if(!fp){
        printf("El archivo no existe\n");
        strcat(nombre,".txt");
        fp = fopen(nombre, "w+b");
        printf("Archivo creado\n");
        strcat(sys, nombre);
        system(sys);
    } else {
        printf("El archivo existe: \n");
        system(sys);
    }

 	fclose ( fp );
}

