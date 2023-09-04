#include <stdlib.h>
#include "pokemon.h"
#include "tipo.h"
#include <stdio.h>
#include "ataque.h"
#include <string.h>
#include <stdbool.h>

#define MAX_NOMBRE 200
#define MAX_TIPO 10
#define MAX_ATAQUES 4
const int ERROR = -1;
#define FORMATO_LECTURA_POKEMON "%[^;];%c"
#define FORMATO_LECTURA_INFO_POKEMON "%[^;];%c;%i"
const char TIPO_NORMAL ='N';
const char TIPO_FUEGO='F';
const char TIPO_AGUA = 'A';
const char TIPO_PLANTA = 'P';
const char TIPO_ELECTRICO = 'E';
const char TIPO_ROCA = 'R';

struct pokemon {
	char nombre[MAX_NOMBRE];
	enum TIPO tipo_pokemon;
	struct ataque ataques[MAX_ATAQUES];

};

struct info_pokemon {
	pokemon_t** pokemones;
	int cantidad;
};

/*
 *pre: recibe el texto y la posicion actual en el texto. 
 * post: devuelve el tipo segun el caracter 
 */
enum TIPO tipo(char tipo)
{
    enum TIPO tipo_buscado;
	if(tipo == TIPO_ELECTRICO){
        tipo_buscado = ELECTRICO;
	}else if(tipo == TIPO_ROCA){
		tipo_buscado = ROCA;
	}else if(tipo == TIPO_AGUA){
		tipo_buscado = AGUA;
	}else if(tipo == TIPO_FUEGO){
		tipo_buscado = FUEGO;
	}else if(tipo == TIPO_NORMAL){
		tipo_buscado = NORMAL;
	}else{
		tipo_buscado = PLANTA;
	}

	return tipo_buscado;
}

bool existe_un_nombre(const char* nombre){
	
    return strlen(nombre) > 0;
}

bool existe_un_tipo(char tipo){
	return (tipo ==  TIPO_AGUA || tipo ==  TIPO_ELECTRICO || tipo ==  TIPO_FUEGO || tipo == TIPO_NORMAL || tipo ==  TIPO_PLANTA || tipo ==  TIPO_ROCA);
}

bool existe_un_poder(int poder){
	return (poder > 0);
}



void leer_pokemon(const char* linea, pokemon_t* pokemon, bool* correcto){
	
	char tipo_poke=' ';
	int leidos = sscanf(linea, FORMATO_LECTURA_POKEMON, pokemon->nombre, &tipo_poke);


	if(leidos != 2 || !existe_un_nombre(pokemon->nombre) || !existe_un_tipo(tipo_poke)){
	
		printf("falta para nombre\n");
		*correcto = false;
		return;

	}
	
	pokemon->tipo_pokemon = tipo(tipo_poke);
	//printf(" %s\n",pokemon->nombre);
	

}

void leer_ataques(const char* linea, pokemon_t* pokemon, struct ataque* ataque ,bool* correcto){
	char tipo_poke=' ';
	int poder=0;
	
	int leidos = sscanf(linea, FORMATO_LECTURA_INFO_POKEMON,ataque->nombre,&tipo_poke, &poder);

	if(leidos != 3 || !existe_un_nombre(ataque->nombre) || !existe_un_tipo(tipo_poke) || !existe_un_poder(poder)){
		printf("falta para ataque\n");
		*correcto = false;
		return;
	}
	ataque->tipo = tipo(tipo_poke);
	ataque->poder = (unsigned int)poder;

}

void completar_pokemon(const char* linea_poke, pokemon_t* poke, bool* poke_listo,int contador, bool* correcto){


	if(contador== 1){
		leer_pokemon(linea_poke,poke,correcto);

	}else if(contador == 2){
        leer_ataques(linea_poke,poke, &(poke->ataques[0]),correcto);

	}else if(contador == 3){
        leer_ataques(linea_poke,poke, &(poke->ataques[1]),correcto);

	}else if(contador == 4){
        leer_ataques(linea_poke,poke, &(poke->ataques[2]),correcto);
        *poke_listo = true;
	}
	

}

informacion_pokemon_t *pokemon_cargar_archivo(const char *path)
{
	if(path == NULL){
		return NULL;
	}
	informacion_pokemon_t* info = malloc(sizeof(informacion_pokemon_t));
	if(info == NULL){
		return NULL;
	}
	info->pokemones= NULL;
	info->cantidad=0;
    
	FILE*  archivo = fopen(path,"r");
	if(archivo == NULL){
		perror("el archivo no pudo abrirse\n");
		free(info);
		return  NULL;
	}
    char linea_poke[500];
	bool poke_listo= false;
	int contador=0;
	bool leido_correctamente= true;


	pokemon_t poke;
    while(fgets(linea_poke,500,archivo) != NULL && leido_correctamente){
        
		if(contador < 4){
			contador++;
			completar_pokemon(linea_poke,&poke,&poke_listo,contador,&leido_correctamente);
		}
		if( poke_listo && leido_correctamente){
		

			pokemon_t* pokemon_listo = malloc(sizeof(pokemon_t));
			if(pokemon_listo == NULL){
				free(info);
				return NULL;
			}
            *pokemon_listo = poke;
			info->pokemones = realloc(info->pokemones, (long unsigned int)(1+info->cantidad)*(sizeof(pokemon_t*)));
			
			if(info->pokemones != NULL){
				info->pokemones[info->cantidad] = pokemon_listo;
        	    info->cantidad++;
			}else{
			
				free(pokemon_listo);
			}
			contador=0;
			poke_listo=false;
		}

	}
	if(info->cantidad==0){
		free(info);
		return NULL;
	}
	
	fclose(archivo);
	return info;
}



pokemon_t *pokemon_buscar(informacion_pokemon_t *ip, const char *nombre)
{   
	if(ip == NULL || nombre == NULL){
		return NULL;
	}
	bool encontrado=false;

	int i=0;
	
	pokemon_t* poke = malloc(sizeof(pokemon_t));
	if(poke == NULL){
		return NULL;
	
	}
	while(i < ip->cantidad && !encontrado){
		*poke = *(ip->pokemones[i]);
		if(strcmp(poke->nombre,nombre) == 0){
            encontrado = true;
		}
		i++;
	}
	if(!encontrado){
		free(poke);
		return NULL;
	}else{
		return poke;
	}
}

int pokemon_cantidad(informacion_pokemon_t *ip)
{
	if(ip == NULL){
		return 0;
	}
	return ip->cantidad;
}

const char *pokemon_nombre(pokemon_t *pokemon)
{
	if(pokemon == NULL){
		return NULL;
	}
	return pokemon->nombre;
}

bool buscar_ataque(const char* nombre, struct ataque ataque){
	return (strcmp(ataque.nombre,nombre) ==0 );
}

enum TIPO pokemon_tipo(pokemon_t *pokemon)
{
	if(pokemon == NULL){
		return 0;
	}
	return pokemon->tipo_pokemon;
}

const struct ataque *pokemon_buscar_ataque(pokemon_t *pokemon, const char *nombre)
{
	if(pokemon== NULL || nombre == NULL){
		return NULL;
	}
	struct ataque* ataque = malloc(sizeof(struct ataque));
	if(ataque == NULL){
		return NULL;
	}
	bool ataque_encontrado= false;
	for (int i = 0; i < 3; i++){
		if(buscar_ataque(nombre,pokemon->ataques[i])){
			*ataque = pokemon->ataques[i];
			ataque_encontrado=true;

		}
	}
	if(!ataque_encontrado){
		free(ataque);
		return NULL;
	}
	return ataque;
}

void ordenar_pokes(informacion_pokemon_t* info){

    pokemon_t* poke_aux;
	for (int i = 1; i < info->cantidad; i++){
        for (int j = 0; j < info->cantidad - i; j++){
            const char* primer = pokemon_nombre(info->pokemones[j]);
            const char* segundo = pokemon_nombre(info->pokemones[j+1]);
            if(strcmp(primer,segundo) > 0){
                poke_aux = info->pokemones[j];
                info->pokemones[j] = info->pokemones[j+1];
                info->pokemones[j+1] = poke_aux;
            }
        }   
    }
}
int con_cada_pokemon(informacion_pokemon_t *ip, void (*f)(pokemon_t *, void *),void *aux)
{
	
	if(ip == NULL || aux == NULL || f == NULL){
		return 0;
	}
	ordenar_pokes(ip);
	int contador=0;
	for (int i = 0; i < ip->cantidad; i++){
		f(ip->pokemones[i],aux);
		contador++;
	}
	
	return contador;
}

int con_cada_ataque(pokemon_t *pokemon, void (*f)(const struct ataque *, void *), void *aux)
{
	if(pokemon == NULL || f == NULL || aux== NULL){
		return 0;

	}
	int contador=0;
	for (int i = 0; i < 3; i++){
		f(&(pokemon->ataques[i]),aux);
		contador++;
	}
	
	return contador;
}

void pokemon_destruir_todo(informacion_pokemon_t *ip)
{
	if(ip  != NULL){
		for (int i = ip->cantidad; i > 0; i--){
			free((ip->pokemones[i-1]));
		}
		free(ip->pokemones);
	}else{
		return;
	}
	free(ip);

	
}