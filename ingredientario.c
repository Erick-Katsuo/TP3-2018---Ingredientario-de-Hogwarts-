#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_CODIGO 50
#define MAX_NOMBRE 50
#define MAX_INGREDIENTES 20

#define COMANDO_RECHAZADO 0
#define CREAR_POCION 1
#define ACTUALIZAR 2
#define COMPRAR 3
#define AYUDA 4


const char COMANDO_CREAR_POCION[MAX_CODIGO] = "crear_pocion";
const char COMANDO_ACTUALIZAR[MAX_CODIGO] = "actualizar";
const char COMANDO_COMPRAR[MAX_CODIGO] = "comprar";
const char COMANDO_AYUDA[MAX_CODIGO] = "ayuda";

const int POSICION_PARAMETRO_COMANDO = 1;
const int POSICION_PARAMETRO_DOS = 2;
const int POSICION_PARAMETRO_TRES = 3;

const int ARGUMENTOS_MINIMO = 1;
const int ARGUMENTOS_COMANDO_CREAR_POCION = 3;
const int ARGUMENTOS_COMANDO_ACTUALIZAR = 2;
const int ARGUMENTOS_COMANDO_COMPRAR = 4;
const int ARGUMENTOS_COMANDO_AYUDA = 2;
const int ARGUMENTOS_MAXIMO =4;

typedef struct ingrediente {
char codigo[MAX_CODIGO];
int cantidad;
} ingrediente_t ;

typedef struct pocion {
int codigo;
char nombre[MAX_NOMBRE];
int cantidad_ingredientes;
ingrediente_t ingredientes[MAX_INGREDIENTES];
} pocion_t ;


/*
* Lee el archivo de ayuda caracter por caracter.
* Pre: -.
* Post: El contenido del archivo se mostrara por pantalla hasta que llegue al end of file del mismo.
*/
void leer_ayuda(FILE* aiuda){
	int leer;
	while((leer=fgetc(aiuda))!= EOF ){
		 fputc(leer, stdout );
	}
}

/*
* Abrira el archivo de ayuda y comenzara a leerlo.
* Pre: -.
* Post: Se mostrara por pantalla el contenido del archivo de ayuda.
*/
void comando_ayuda(){
	system("clear");
	system("clear");
	FILE* aiuda = fopen("aiuda.txt", "rt");
	if(!aiuda){
		perror("\n\n\nError al intentar abrir el archivo de ayuda");
		printf("\nSino tal vez Kwyjiboldemort no acepto bien el archivo de ayuda(?)\n\n\n");
	}else{
		leer_ayuda(aiuda);
		fclose(aiuda);
	}	
}

/*
* Revisara el stock del ingrediente actual evaluando si el stock que posee llega a la cantidad minima o no.
* Pre: -.
* Post: En caso de que el ingrediente evaluado no llegue al stock necesario se procedera a ser agregado al archivo de nombre pedido por consola.
*/
void revisar_stock_comando_comprar(int cantidad_minima, FILE* crear_lista_compras, ingrediente_t revisar_ingrediente){
	int cantidad_comprar=0;
	if(revisar_ingrediente.cantidad < cantidad_minima){
		cantidad_comprar = cantidad_minima - revisar_ingrediente.cantidad;
		fprintf(crear_lista_compras, "%s;%i\n",revisar_ingrediente.codigo, cantidad_comprar);
	}
}

/*
* Leera uno por uno los ingredientes en el archivo "ingredientario.txt"
* Pre: -.
* Post: Terminara de leer cuando se llegue al end of file del archivo "ingredientario.txt".
*/
void revisar_ingrediente_comando_comprar(int cantidad_minima, FILE* revisar_ingredientario, FILE* crear_lista_compras){
	ingrediente_t revisar_ingrediente;
	int leido=fscanf(revisar_ingredientario,"%[^;];%i\n", revisar_ingrediente.codigo, &revisar_ingrediente.cantidad);
	while(leido!=EOF){
		revisar_stock_comando_comprar(cantidad_minima, crear_lista_compras, revisar_ingrediente);
		leido=fscanf(revisar_ingredientario,"%[^;];%i\n", revisar_ingrediente.codigo, &revisar_ingrediente.cantidad);
	}
	printf("Revision de stock terminada\n");
}

/*
* Leera el archivo "ingredientario.txt" y creara el archivo de nombre pedido por consola junto a sus ingredientes y stock.
* Pre: -.
* Post: El archivo de nombre pedido por consola quedara creado y completo con los ingredientes que se necesitan aumentar en stock.
*/
void crear_archivo_compras(char archivo_crear[], int cantidad_minima){
	FILE* revisar_ingredientario;
	FILE* crear_lista_compras;
	revisar_ingredientario=fopen("ingredientario.txt", "rt");
	if(!revisar_ingredientario){
		perror("Error al intentar abrir ingredientario.txt");
	}else{
		crear_lista_compras=fopen(archivo_crear, "wt");
	}
	if(!crear_lista_compras && revisar_ingredientario){
		printf("Failure to create file: %s", archivo_crear);
		perror("Error");
		fclose(revisar_ingredientario);
	}else if(crear_lista_compras && revisar_ingredientario){
		revisar_ingrediente_comando_comprar(cantidad_minima, revisar_ingredientario, crear_lista_compras);
		fclose(revisar_ingredientario);
		fclose(crear_lista_compras);
		printf("\t\tArchivo '%s' creado\n", archivo_crear );
	}
}

/*
* Inicializara las operaciones necesarias para crear el archivo de nombre a ingresar por consola, junto a su informacion de ingredientes y stock necesario
* para el minimo del que se necesita en general(la cantidad minima tambien tiene que ser ingresada por consola).
* Pre: -.
* Post: Se creara el archivo pedido con la informacion de los ingredientes que no llegaron al stock minimo necesario (ingresado por consola).
*		En caso de presentar errores en el proceso se informaran los mismos.
*/
void comando_comprar(char* argv[]){
	system("clear");
	system("clear");
	char archivo_crear[MAX_NOMBRE];
	int cantidad_minima=atoi(argv[POSICION_PARAMETRO_DOS]);
	if(strlen(argv[POSICION_PARAMETRO_TRES])<=MAX_NOMBRE){
		strcpy(archivo_crear, argv[POSICION_PARAMETRO_TRES]);
		printf("\t\tProcediendo a crear el archivo '%s' ..\n",archivo_crear);
		printf("Cantidad minima requerida: %i\n", cantidad_minima);
		crear_archivo_compras(archivo_crear, cantidad_minima);
	}else{
		printf("Error: max characters [%i]\n", MAX_NOMBRE);
	}
}

/*
* Unicamente agrega el ingrediente pasado por referencia mediante el puntero "ingrediente_actualizado".
* Aclaracion: No necesariamente el ingrediente llego a ser actualizado, puede ser sin actualizar.
* Pre: La informacion del ingrediente pasado por referencia mediante "ingrediente_actualizado" debe ser correcta.
* Post: Queda agregado el ingrediente y su informacion al archivo "new_ingredientario.txt".
*/
void agregar_actualizado(ingrediente_t *ingrediente_actualizado, FILE* new_ingredientario){
	fprintf(new_ingredientario, "%s;%i\n",ingrediente_actualizado->codigo, ingrediente_actualizado->cantidad );
}

/*
* Al no ser necesario actualizar, simplemente copiara el ingrediente y su stock al archivo "new_ingredientario.txt".
* Pre:-.
* Post: Queda agregado el ingrediente sin ser actualizado, y pasa a leer el siguiente ingrediente del archivo "ingredientario.txt".
*/
void copiar_restante(ingrediente_t *ingrediente_actual, int *leido, FILE* leer_ingredientario, FILE* new_ingredientario){
	agregar_actualizado(ingrediente_actual, new_ingredientario);
	*leido=fscanf(leer_ingredientario, "%[^;];%i\n", ingrediente_actual->codigo, &(ingrediente_actual->cantidad));
}

/*
* Actualizara el stock del ingrediente.
* Pre:-.
* Post: EL stock del ingrediente evaluado quedara actualizado, y se procedera a ingresarlo al archivo "new_ingredientario.txt".
*		Pasara a leer el siguiente ingrediente de los dos archivos "ingredientario.txt" y "uso_diario.dat"
*/
void actualizar_stock(ingrediente_t *ingrediente_actual, ingrediente_t *ingrediente_usado, ingrediente_t *ingrediente_actualizado, int *leido, FILE* leer_ingredientario, FILE* leer_uso_diario, FILE* new_ingredientario){
	strcpy(ingrediente_actualizado->codigo, ingrediente_actual->codigo );
	ingrediente_actualizado->cantidad = (ingrediente_actual->cantidad) - (ingrediente_usado->cantidad);
	agregar_actualizado(ingrediente_actualizado, new_ingredientario);
	*leido=fscanf(leer_ingredientario, "%[^;];%i\n", ingrediente_actual->codigo, &(ingrediente_actual->cantidad));
	fread(ingrediente_usado, sizeof(ingrediente_t), 1, leer_uso_diario);
}

/*
* Buscara los ingredientes del archivo "ingredientario.txt" que necesiten ser actualizados.
* Pre:-.
* Post: De ser necesario actualizara el ingrediente del archivo "ingredientario.txt", respentando el orden ascendente, junto al stock 
*		correspondiente. Si no es necesario actualizar algun ingrediente entonces este sera agregado sin cambios al archivo "new_ingredientario.txt"
*/
void revision_stock(ingrediente_t *ingrediente_actual, ingrediente_t *ingrediente_usado, ingrediente_t *ingrediente_actualizado, int *leido, FILE* leer_ingredientario, FILE* leer_uso_diario, FILE* new_ingredientario){
	if(strcmp(ingrediente_actual->codigo, ingrediente_usado->codigo)==0){
		actualizar_stock(ingrediente_actual, ingrediente_usado, ingrediente_actualizado, leido, leer_ingredientario, leer_uso_diario, new_ingredientario);
	}else if(strcmp(ingrediente_actual->codigo, ingrediente_usado->codigo)<0){
		copiar_restante(ingrediente_actual, leido, leer_ingredientario, new_ingredientario);
	}
}

/*
* Actualizara el stock de los ingredientes del archivo "ingredientario.txt" con el stock de "uso_diario.dat".
* Pre:-.
* Post: Quedaran actualizados todos los ingredientes del archivo "ingredientario.txt", respentando el orden ascendente, junto al stock 
*		correspondiente de cada uno.
*/
void actualizar_ingredientario_comando_actualizar(FILE* leer_ingredientario, FILE* leer_uso_diario, FILE* new_ingredientario){
	ingrediente_t ingrediente_actual;
	ingrediente_t ingrediente_usado;
	ingrediente_t ingrediente_actualizado;
	int leido=fscanf(leer_ingredientario, "%[^;];%i\n", ingrediente_actual.codigo, &ingrediente_actual.cantidad);
	fread(&ingrediente_usado, sizeof(ingrediente_t), 1, leer_uso_diario);
	while((leido!=EOF) && (!feof(leer_uso_diario))){
		revision_stock(&ingrediente_actual, &ingrediente_usado, &ingrediente_actualizado, &leido, leer_ingredientario, leer_uso_diario, new_ingredientario);
	}
	while(leido!=EOF){
		copiar_restante(&ingrediente_actual, &leido, leer_ingredientario, new_ingredientario);
	}
}

/*
* Realizara los ultimos pasos para finalizar el comando "actualizar".
* Pre:-.
* Post: Elimina el archivo "uso_diario.dat" y "ingredientario.txt", cambia el nombre de "new_ingredientario.txt" por "ingredientario.txt".
*/
void ultimo_paso_actualizar(){
	int delet_uso_diario = remove("uso_diario.dat");
	int delet_ingredientario = remove("ingredientario.txt");
	int new_name;
	if((delet_uso_diario==0) &&(delet_ingredientario==0)){
		printf("uso_diario.dat eliminado (junto al anterior ingredientario.txt)\n");
		new_name = rename("new_ingredientario.txt","ingredientario.txt");
		if(new_name==0){
			printf("Archivo 'actualizado' correctamente\n");
		}
	}
}

/*
* Inicalizara las operacione necesarias para realizar el comando Actualizar.
* Pre:-.
* Post: El archivo "ingredientario.txt" quedara actualizado restando la cantidad a cada ingrediente correspondiente segun lo registrado en 
*		el archivo "uso_diario.dat". El archivo "uso_diario.dat" sera eliminado.
*/
void comando_actualizar(){
	system("clear");
	system("clear");
	printf("\t\tActualizando stock del ingredientario..\n");
	FILE* leer_ingredientario;
	FILE* leer_uso_diario;
	FILE* new_ingredientario;
	leer_ingredientario=fopen("ingredientario.txt","rt");
	if(!leer_ingredientario){
		perror("Error al intentar abrir el archivo ingredientario.txt");
	}else{
		leer_uso_diario=fopen("uso_diario.dat", "rb");
	}
	if(!leer_uso_diario && leer_ingredientario){
		perror("Error al intentar abrir el archivo uso_diario.dat");
		fclose(leer_ingredientario);
	}else if(leer_uso_diario && leer_ingredientario){
		new_ingredientario=fopen("new_ingredientario.txt","wt");
	}
	if(!new_ingredientario && leer_ingredientario && leer_uso_diario){
		perror("Error al intetar abrir el archivo new_ingredientario.txt");
		fclose(leer_ingredientario);
		fclose(leer_uso_diario);
	}else if(leer_ingredientario && leer_uso_diario && new_ingredientario){
		actualizar_ingredientario_comando_actualizar(leer_ingredientario, leer_uso_diario, new_ingredientario);
		fclose(leer_ingredientario);
		fclose(leer_uso_diario);
		fclose(new_ingredientario);
		ultimo_paso_actualizar();
	}
}

/*
* Agrega el ingredeinte correspondiente leido desde el stuct "pocion_requerida".
* Pre:-.
* Post: Quedara agregado el ingrediente actual del struct "pocion_requerida" respetando el orden ascendente.
*/
void desde_pocion_requerida(FILE* new_stock, ingrediente_t *new_ingrediente, pocion_t pocion_requerida, int i){
	strcpy(new_ingrediente->codigo,pocion_requerida.ingredientes[i].codigo);
	new_ingrediente->cantidad=pocion_requerida.ingredientes[i].cantidad;
	fwrite(new_ingrediente, sizeof(ingrediente_t),1,new_stock);
}

/*
* Agrega el ingredeinte correspondiente y pasa a leer el siguiente del archivo "uso_diario".
* Pre:-.
* Post: Quedara agregado el ingrediente actual leido desde el archivo "uso_diario" respetando el orden ascendente y pasara a leer el siguiente.
*/
void desde_archivo_uso_diario(FILE* old_stock, FILE* new_stock, ingrediente_t *old_ingrediente, ingrediente_t *new_ingrediente){
	strcpy(new_ingrediente->codigo,old_ingrediente->codigo);
	new_ingrediente->cantidad=old_ingrediente->cantidad;
	fwrite(new_ingrediente, sizeof(ingrediente_t),1,new_stock);
	fread(old_ingrediente, sizeof(ingrediente_t), 1, old_stock);
}

/*
* Ingresara los ingredientes faltantes del archivo "uso_diario" de tal forma que deben quedar ordenados ascendentemente al ser actualizado.
* Pre:-.
* Post: Quedaran agregados los ingredientes faltantes del archivo "uso_diario" respetando el orden.
*/
void agregar_ingrediente_faltante_archivo(FILE* old_stock, FILE* new_stock, ingrediente_t *new_ingrediente, ingrediente_t *old_ingrediente){
	while(!feof(old_stock)){
		desde_archivo_uso_diario(old_stock, new_stock, old_ingrediente, new_ingrediente);
	}
}

/*
* Ingresara los ingredientes faltantes del struct "pocion_requerida" desde el valor que tenga la variable "agregar_faltantes".
* Pre: El valor de "agregar_faltantes" tiene que ser correctamente definido con un valor previo al fin de la iteracion en la funcion 
*	   "actualizar_ingredientes".
* Post: Quedaran agregados los ingredientes faltantes del struct "pocion_requerida" respetando el orden.
*/
void agregar_ingrediente_faltante_pocion(FILE* new_stock, ingrediente_t *new_ingrediente, pocion_t pocion_requerida, int agregar_faltantes){
	for(int k=agregar_faltantes;k<pocion_requerida.cantidad_ingredientes;k++){
		desde_pocion_requerida(new_stock, new_ingrediente, pocion_requerida, k);
	}
}

/*
* Ingresara ingredientes evaluando con los encontrados en el archivo "uso_diario.dat" y en el struct "pocion_requerida".
* Pre: -.
* Post: Los ingredientes que se pudieron evaluar del struct se nombraran como "void".
		Quedaran actualizados/agregados los ingredientes. (Pueden llegar a quedar ingredientes del struct "pocion_requerida" sin agregarse)
*/
void agregar_sin_actualizar_ingrediente(FILE* old_stock, FILE* new_stock, ingrediente_t *old_ingrediente, ingrediente_t *new_ingrediente, pocion_t pocion_requerida, int *i){
		desde_archivo_uso_diario(old_stock, new_stock, old_ingrediente, new_ingrediente);
		(*i)--;
}

/*
* Ingresara ingredientes evaluando con los encontrados en el archivo "uso_diario.dat" y en el struct "pocion_requerida".
* Pre: -.
* Post: Quedaran agregados los ingredientes, junto a su cantidad, que se usaron para hacer la pocion y NO estan en el archivo "uso_diario.dat".
*/
void actualizar_ingrediente_nuevo(FILE* old_stock, FILE* new_stock, ingrediente_t *old_ingrediente, ingrediente_t *new_ingrediente, pocion_t pocion_requerida, int *i){
		desde_pocion_requerida(new_stock, new_ingrediente, pocion_requerida, *i);
}

/*
* Ingresara ingredientes evaluando con los encontrados en el archivo "uso_diario.dat" y en el struct "pocion_requerida" que tengan las mismas
* iniciales y sean de igual nombre.
* Pre: -.
* Post: Los ingredientes que se pudieron evaluar del struct se nombraran como "void".
		Quedaran actualizados/agregados los ingredientes. (Pueden llegar a quedar ingredientes del struct "pocion_requerida" sin agregarse)
*/
void actualizar_ingrediente_encontrado(FILE* old_stock, FILE* new_stock, ingrediente_t *old_ingrediente, ingrediente_t *new_ingrediente, pocion_t pocion_requerida, int *i){
	strcpy(new_ingrediente->codigo,old_ingrediente->codigo);
	new_ingrediente->cantidad=(old_ingrediente->cantidad)+(pocion_requerida.ingredientes[*i].cantidad);
	fwrite(new_ingrediente, sizeof(ingrediente_t),1,new_stock);
	fread(old_ingrediente, sizeof(ingrediente_t), 1, old_stock);
}

/*
* Comenzara a ingresar ingredientes evaluando con los encontrados en el archivo "uso_diario.dat" y en el struct "pocion_requerida".
* Pre: -.
* Post: Quedaran actualizados/agregados los ingredientes que llegaron a ser evaluados.
*/
void actualizar_ingredientes_ascendente(FILE* old_stock, FILE* new_stock, ingrediente_t *old_ingrediente, ingrediente_t *new_ingrediente, pocion_t pocion_requerida, int *i){
	if(strcmp(pocion_requerida.ingredientes[*i].codigo,old_ingrediente->codigo)==0){
		actualizar_ingrediente_encontrado(old_stock,new_stock,old_ingrediente,new_ingrediente,pocion_requerida,i);
	}else if(strcmp(pocion_requerida.ingredientes[*i].codigo,old_ingrediente->codigo)<0){
		actualizar_ingrediente_nuevo(old_stock,new_stock,old_ingrediente,new_ingrediente,pocion_requerida,i);
	}else if(strcmp(pocion_requerida.ingredientes[*i].codigo,old_ingrediente->codigo)>0){
		agregar_sin_actualizar_ingrediente(old_stock,new_stock,old_ingrediente,new_ingrediente,pocion_requerida,i);
	}
}


/*
* Dependiendo de las condiciones en las que este cada ingrediente se procedera a ser actualizado ascendentemente en
* el archivo "new_uso_diario.dat" de forma temporal.
* Pre: -.
* Post: Quedaran agregados/actualizados ascendentemente los ingredientes que pudieron ser evaluados con los ingredientes del 
*		archivo "uso_diario.dat".
*		Por ende puede llegar a sobrar ingredientes que no llegaron a ordenarse debido al corte de la iteracion al cumplir alguna de
*		las condiciones. En ambas condiciones se devuelve el valor de i.
*/
int actualizar_ingredientes(FILE* old_stock, FILE* new_stock, ingrediente_t *old_ingrediente, ingrediente_t *new_ingrediente, pocion_t pocion_requerida){
	int i=0;
	while((!feof(old_stock)) && (i!=pocion_requerida.cantidad_ingredientes)){
		actualizar_ingredientes_ascendente(old_stock,new_stock,old_ingrediente,new_ingrediente,pocion_requerida,&i);
		i++;		
	}
	return i;
}

/*
* Realizara el ultimo paso de crear_pocion que es eliminar el archivo "uso_diario.dat" y renombrar el archivo "uso_diario_new.dat" por el eliminado.
* Pre: Todas las condiciones del comando se cumplieron como se debe.
* Post: Quedara actualizado el archivo "uso_diario.dat" informando al usuario por consola las operaciones que se produjeron con exito.
*/
void ultimo_paso_crear_pocion(){
	int delet = remove("uso_diario.dat");
	int new_name;
	if(delet==0){
		printf("Archivo eliminado correctamente\n");
		new_name = rename("new_uso_diario.dat","uso_diario.dat");
		if(new_name==0){
			printf("Archivo renombrado correctamente\n");
		}
	}
}

/*
* Comenzara a operar para realizar la actualizacion en el archivo "uso_diario.dat".
* Pre: -.
* Post: En caso de no encontrar errores entonces el programa finalizara, dejando actualizado el archivo "uso_diario.dat"
*/
void actualizar_stock_diario(FILE* old_stock, FILE* new_stock, pocion_t pocion_requerida){
	int agregar_faltantes=0;
	ingrediente_t old_ingrediente;
	ingrediente_t new_ingrediente;
	fread(&old_ingrediente, sizeof(ingrediente_t), 1, old_stock);
	agregar_faltantes=actualizar_ingredientes(old_stock,new_stock,&old_ingrediente,&new_ingrediente,pocion_requerida);
	agregar_ingrediente_faltante_archivo(old_stock ,new_stock, &new_ingrediente, &old_ingrediente);
	agregar_ingrediente_faltante_pocion(new_stock,&new_ingrediente,pocion_requerida, agregar_faltantes);
}
	

/*
* Agregara los ingredientes y stock usados a "uso_diario.dat".
* Pre: -.
* Post: Terminara de agregar cuando se llegue al limite de ingredientes correspondiente a la pocion requqerida.
*/
void agregar_ingredientes_usados(pocion_t pocion_requerida, FILE* crear){
	int limite_de_ingredientes = 0;
	ingrediente_t agregar_nuevo;
	while(limite_de_ingredientes != pocion_requerida.cantidad_ingredientes){
		agregar_nuevo.cantidad=pocion_requerida.ingredientes[limite_de_ingredientes].cantidad;
		strcpy(agregar_nuevo.codigo, pocion_requerida.ingredientes[limite_de_ingredientes].codigo);
		fwrite(&agregar_nuevo,sizeof(ingrediente_t), 1, crear);
		limite_de_ingredientes++;
	}
}

/*
* En caso de que no se encuentre el archivo "uso_diario.dat" se procedera a crear con los ingredientes, y respectivo stock, que se usaron
* para crear la pocion.
* Pre: la informacion de "pocion_requerida" debe estar correctamente definido.
* Post: Se creara "uso_diario.dat".
*/
void crear_uso_diario(pocion_t pocion_requerida){
	FILE* crear = fopen("uso_diario.dat", "wb");
	if(!crear){
		perror("Error al crear archivo uso_diario.dat");
	}
	agregar_ingredientes_usados(pocion_requerida, crear);
	fclose(crear);
	printf("Finalizo la copia de ingredientes utilizados al archivo 'uso_diario.dat'\n");
}

/*
* Abrira los archivos necesarios "uso_diario.dat" y "new_uso_diario.dat".
* Pre: -.
* Post: Si no se pudo abrir alguno de los dos se informara por consola.
		En caso contrario, proseguira con la siguiente operacion para continuar el comando "crear_pocion".
*/
void preparar_archivos_comando_crear_pocion(pocion_t pocion_requerida){
	FILE* old_stock;
	FILE* new_stock;
	old_stock = fopen("uso_diario.dat", "rb");
	if(!old_stock){
		perror("Error al leer archivo uso_diario.dat");
		printf("Comenzando la creacion del archivo\n");
		crear_uso_diario(pocion_requerida);
	}else{
		new_stock = fopen("new_uso_diario.dat", "wb");
	}
	if(!new_stock && old_stock){
		perror("Error al leer archivo new_uso_diario.dat");
		fclose(old_stock);
	}else if (old_stock && new_stock){
		actualizar_stock_diario(old_stock, new_stock, pocion_requerida);	
		fclose(old_stock);
		fclose(new_stock);
		ultimo_paso_crear_pocion();	
	}
}


/*
* Verifica que se posea el stock necesario del ingrediente para crear la pocion pedida.
* Pre:Las variables deben estar correctamente inicializadas.
* Post: Si no se cuenta con el stock necesario devolvera "false" mediante el puntero "verificar_stock".
*		En caso contrario queda verificado el stock necesario y el puntero "verificar_stock" queda en "true".
*/
void verificar_stock_suficiente(pocion_t pocion_requerida, ingrediente_t *ingrediente_existente, bool *verificar_stock, int i){
	if(pocion_requerida.ingredientes[i].cantidad>ingrediente_existente->cantidad){
		(*verificar_stock)=false;
	}
}

/*
* Verifica la existencia de los ingredientes requeridos para crear la pocion pedida.
* Pre:Las variables deben estar correctamente inicializadas.
* Post: Si encuentra el ingrediente pedido procedera a verificar el stock necesario del mismo para crear la pocion.
*		En caso contrario no se podra verificar la existencia del ingrediente requerido. (lo cual impedira crear la pocion pedida)
*		En ambos casos la funcion devuelve un bool el cual controla la verificacion junto a otras condiciones en la funcion "controles_corte_crear_pocion"
*/
bool verificar_existencia_ingrediente(bool *verificar_stock, bool *verificar_ingrediente, pocion_t pocion_requerida, ingrediente_t *ingrediente_existente, int *leido, int *ingrediente_pocion, FILE* read_ingredientario){
	bool keep =false;
	if(strcmp(pocion_requerida.ingredientes[*ingrediente_pocion].codigo, ingrediente_existente->codigo)==0){
		(*verificar_ingrediente)=true;
		keep = true;
		verificar_stock_suficiente(pocion_requerida, ingrediente_existente, verificar_stock, *ingrediente_pocion);
		(*ingrediente_pocion)++;
		*leido = fscanf(read_ingredientario,"%[^;];%i\n",ingrediente_existente->codigo,&(ingrediente_existente->cantidad));
	}else if(strcmp(pocion_requerida.ingredientes[*ingrediente_pocion].codigo, ingrediente_existente->codigo)<0){
		(*verificar_ingrediente)=false;
	}else if(strcmp(pocion_requerida.ingredientes[*ingrediente_pocion].codigo, ingrediente_existente->codigo)>0){
		*leido = fscanf(read_ingredientario,"%[^;];%i\n",ingrediente_existente->codigo,&(ingrediente_existente->cantidad));
		keep = true;
	}
	return keep;
}

/*
* Controla las condiciones de corte necesarios. (excepcion: variable leido controlado por el end of file )
* Pre:Las variables deben estar correctamente inicializadas.
* Post: Dependiendo de que condicion se cumpla ciertas variables se modificaran para controlar las condiciones de corte;
*/
void controles_corte_crear_pocion(bool *verificar_ingrediente, int *limite_ingredientes_encontrados, bool*ingredientes_disponibles, bool keep, pocion_t pocion_requerida){
	if((*verificar_ingrediente==true) && (keep == true)){
		(*limite_ingredientes_encontrados)++;
		(*verificar_ingrediente)=false;
		printf("Ingrediente found[%i]\n", *limite_ingredientes_encontrados);
	}else if((*verificar_ingrediente==false) && (keep == false)){
		(*limite_ingredientes_encontrados)=(pocion_requerida.cantidad_ingredientes);
	}
	if(*limite_ingredientes_encontrados==(pocion_requerida.cantidad_ingredientes) && (keep==true)){
		*ingredientes_disponibles=true;
	}
}

/*
* Informa la situacion al terminar de revisar si se cuentan con los ingredientes y el stock necesario para crear la pocion pedida.
* Pre: Las variables deben estar correctamente inicializadas.
* Post: Dependiendo del valor de las variables se mostrara por consola un mensaje en especifico y en caso de que se verifiquen los ingredientes
*		con su stock respectivo entonces procedera a actualizar el stock diario.
*/
void informar_situacion_crear_pocion(bool ingredientes_disponibles, bool verificar_stock, pocion_t pocion_requerida){
	if(ingredientes_disponibles==true && (verificar_stock==true)){
		printf("\t\tSuccessful: Pocion creada correctamente\n");
		preparar_archivos_comando_crear_pocion(pocion_requerida);
	}else if(ingredientes_disponibles==false && (verificar_stock==true)){
		printf("\t\tFailure: Falta algun ingrediente\n");
	}
	if(verificar_stock==false){
		printf("\t\tFailure: Stock insuficiente\n");
	}
}


/*
* Leera el archivo "ingredientario.txt" para poder ir chequeando uno por uno y asi ver si se poseen todos los ignredientes necesarios para
* la pocion requerida(datos dentro del struct "pocion_requerida").
* Pre: El struct "ingrediente_existente" debe estar correctamente inicializado.
* Post: Se verificaran los ingredientes y el stock necesario correspondiente a la pocion requerida informando el resultado de la lectura
*		en "ingredientario.txt".
*/
void leer_ingredientario(FILE* read_ingredientario, pocion_t pocion_requerida){
	bool keep=true;
	bool ingredientes_disponibles=false;
	bool verificar_ingrediente = false;
	bool verificar_stock = true;
	int ingrediente_pocion=0;
	ingrediente_t ingrediente_existente;
	int limite_ingredientes_encontrados=0;
	int leido = fscanf(read_ingredientario,"%[^;];%i\n",ingrediente_existente.codigo,&ingrediente_existente.cantidad);
	printf("\t\tIngredeintes[%i]\n",pocion_requerida.cantidad_ingredientes );
	while((leido != EOF) && (limite_ingredientes_encontrados !=(pocion_requerida.cantidad_ingredientes )) && (verificar_stock!=false) && (keep!=false)){
		keep=verificar_existencia_ingrediente(&verificar_stock, &verificar_ingrediente, pocion_requerida, &ingrediente_existente, &leido, &ingrediente_pocion, read_ingredientario);
		controles_corte_crear_pocion(&verificar_ingrediente,&limite_ingredientes_encontrados,&ingredientes_disponibles,keep, pocion_requerida);
	}	
	informar_situacion_crear_pocion(ingredientes_disponibles, verificar_stock, pocion_requerida);
}


/*
* Verificara la disponibilidad de la pocion requerida leyendo el archivo "ingredientario.txt" para poder verificar asi mismo si existen
* los ingredientes necesarios junto a la cantidad de stock minima de cada uno.
* Pre: La direccion del archivo a operar tienen que estar correctamente definido, el struct "pocion_requerida" debe estar correctamente inicializado.
* Post: Si el archivo se puede abrir correctamente se leera y procedera a realizar las verificaciones.
*		En caso contrario se informara el error por el cual no se pudo realizar la verificacion de disponibilidad.
*/
void verificar_disponibilidad(pocion_t pocion_requerida){
	FILE* read_ingredientario = fopen("ingredientario.txt", "rt");
	if(!read_ingredientario){
		perror("Error al intentar abrir el archivo ingredientario.txt");
	}else{
		leer_ingredientario(read_ingredientario, pocion_requerida);
		fclose(read_ingredientario);
	}
}

/*
* Controlara la lectura del archivo "pociones.dat" teniendo en cuenta las condiciones para el mismo.
* Pre: Es necesario que el id ingresado en el argumento 2 por consola sea mayor a 0(cero) y que exista.
* Post: Si encuentra la pocion se proseguira con la realizacion del comando "crear_pociones"
*		En caso contrario se informara el error.
*/
void realizar_lectura_crear_pociones(FILE* read_pociones, pocion_t pocion_requerida, char* argv[], int id_detectado){
	if((!feof(read_pociones)) && (id_detectado>0)){
		printf("\t\tPreparando %s ..\n", pocion_requerida.nombre );
		verificar_disponibilidad(pocion_requerida); 
	}else{
		printf("ERROR: Potion not found\n");
	}
}


/*
* Preparara la lectura para el archivo "pociones.dat" de tal forma que se lea la pocion del id detectado.
* Pre: -.
* Post: Se posicionara en el id requerido y se guardara la informacion en el struct "pocion_requerida".
*		Aclaracion: En caso de que el id ingresado en el argumento 2 por consola, este solo sera verificado si existe en la funcion
*					"controlar_lectura_crear_pociones"
*/
void preparar_lectura_crear_pociones(FILE* read_pociones, pocion_t *pocion_requerida, char* argv[], int id_detectado){
	size_t id_corregido=(size_t)id_detectado -1 ;
	fseek(read_pociones,(long)(sizeof(pocion_t)*(size_t)id_corregido),SEEK_SET);
	fread(pocion_requerida,sizeof(pocion_t),1,read_pociones);
}

/*
* Inicializara las operaciones necesarias para poder realizar el comando "crear_pocion" teniendo en cuenta las posibles fallas al realizarse.
* Pre: La direccion del archivo a operar tienen que estar correctamente definido.
* Post: Si los parametros y los archivos pueden operarse de forma correcta entonces el comando "crear_pocion" debera realizarse.
*		En caso contrario se informara el error por el cual no se pudo realizar el comando elegido.
*/
void comando_crear_pocion(char* argv[]){
	system("clear");
	system("clear");
	FILE* read_pociones = fopen("pociones.dat", "rb");
	if(!read_pociones){
		perror("Error al intentar abrir el archivo pociones.dat");
	}else{
		pocion_t pocion_requerida;
		int id_detectado=atoi(argv[POSICION_PARAMETRO_DOS]);
		preparar_lectura_crear_pociones(read_pociones, &pocion_requerida, argv, id_detectado);
		realizar_lectura_crear_pociones(read_pociones, pocion_requerida, argv, id_detectado);
		fclose(read_pociones);
	}
}

/*
* Dependiendo del valor del "comando_detectado" se realizaran varios casos.
* Pre: La variable "comando_detectado" debe tener un valor definido en la funcion "detectar_comando".
* Post: Luego de detectar y realizar el comando detectado el programa finalizara.
*/
void inicializar_comando(int comando_detectado, int argc, char* argv[]){
	switch(comando_detectado){
		case CREAR_POCION:
			comando_crear_pocion(argv);
			break;
		case ACTUALIZAR:
			comando_actualizar();
			break;
		case COMPRAR:
			comando_comprar(argv);
			break;
		case AYUDA:
			comando_ayuda();
			break;
		case COMANDO_RECHAZADO:
			printf("Error\nComando rechazado\n");
			break;
	}
}


/*
* Detectara si se ingreso la cantidad requerida de argumentos para el comando ingresaddo.
* Pre:-.
* Post: En caso de que no se ingresen los argumentos necesarios entonces el puntero "comando_detectado" se asignara con el valor de "COMANDO_RECHAZADO".
*/
void limitar_argumentos_recibidos(int argc, int *comando_detectado){
	if((*comando_detectado==CREAR_POCION) && (argc != ARGUMENTOS_COMANDO_CREAR_POCION)){
		*comando_detectado=COMANDO_RECHAZADO;
	}else if((*comando_detectado==ACTUALIZAR) && (argc != ARGUMENTOS_COMANDO_ACTUALIZAR)){
		*comando_detectado=COMANDO_RECHAZADO;
	}else if((*comando_detectado==COMPRAR) && (argc != ARGUMENTOS_COMANDO_COMPRAR)){
		*comando_detectado=COMANDO_RECHAZADO;
	}else if((*comando_detectado==AYUDA) && (argc != ARGUMENTOS_COMANDO_AYUDA)){
		*comando_detectado=COMANDO_RECHAZADO;
	}
}

/*
* Detectara si se ingreso un comando aceptable.
* Pre: Los comandos deben estar bien definidos en este programa previamente para que se puedan detectar correctamente.
* Post: Si se ingresa algun comando aceptable entonces devuelve el correspondiente valor dado a ese comando en este programa. 
* 		En caso contrario devuelve el valor de "COMANDO_RECHAZADO" que es 0(cero).
*/
int detectar_comando(int argc, char* argv[]){
	int comando = COMANDO_RECHAZADO;
	if(argc > ARGUMENTOS_MINIMO){
		if(strcmp(argv[POSICION_PARAMETRO_COMANDO], COMANDO_CREAR_POCION)==0){
			comando=CREAR_POCION;
		}else if(strcmp(argv[POSICION_PARAMETRO_COMANDO], COMANDO_ACTUALIZAR)==0){
			comando=ACTUALIZAR;
		}else if(strcmp(argv[POSICION_PARAMETRO_COMANDO], COMANDO_COMPRAR)==0){
			comando=COMPRAR;
		}else if(strcmp(argv[POSICION_PARAMETRO_COMANDO], COMANDO_AYUDA)==0){
			comando=AYUDA;
		}
	}
	return comando;
}

int main(int argc, char* argv[]){
	int comando_detectado;
	comando_detectado=detectar_comando(argc, argv);
	limitar_argumentos_recibidos(argc, &comando_detectado);
	inicializar_comando(comando_detectado, argc, argv);
	return 0;
}