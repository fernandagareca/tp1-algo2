<div align="right">
<img width="32px" src="img/algo2.svg">
</div>

# TP1

## Repositorio de marianela fernanda gareca janko - 109606 - fernanda.gareca321@gmail.com

- Para compilar:

```bash
make pruebas_chanutron
```

- Para ejecutar:

```bash
./pruebas_chanutron
```

- Para ejecutar con valgrind:
```bash
make valgrind-chanutron
```
---
##  Funcionamiento

Explicación de cómo funcionan las estructuras desarrolladas en el TP y el funcionamiento general del mismo.

Aclarar en esta parte todas las decisiones que se tomaron al realizar el TP, cosas que no se aclaren en el enunciado, fragmentos de código que necesiten explicación extra, etc.

Incluír **EN TODOS LOS TPS** los diagramas relevantes al problema (mayormente diagramas de memoria para explicar las estructuras, pero se pueden utilizar otros diagramas si es necesario).

### Por ejemplo: diagrma de flujo de la lectura del archivo

En la funcion principal pokemon cargar archivo ,primero abro el archivo si no se puede abrir retorno null ,casoi contrario que se abrio correctamente reservo memoria para la informacion de los pokemones y verifico haber podido reservar memoria si no pude cierro el archivo y  retorno null, caso contrario leo la linea si hubo algun problema para leer y no habia ningun pokemon cargado anterioremente en el vector cierro el archivo y libero la memoria reservada pero si habian pokemones previamente cargados cierro el archivo y retorno la informacion.
Pero si se pudo leer la linea y aun no lei 4 lineas necesarias para un pokemon,completo los campos segun los datos de la linea ,en caso de error en el formao de la linea cierro el archivo y retorno la informacion recopilada correctamente ,caso contrario que ya lei 4 lineas cargo el pokemon al vector dinamio y aumento su tamaño ,vuelvo a repetir estos pasos hasta que no se pueda leer mas lineas del archivo.

<div align="center">
<img width="70%" src="img/diagrama de flujo.jpeg">
</div>



En el archivo `pokemon.c` la función `pokemon_cargar_archivo` reservo memoria para la informacion de pokemones ,verifico que se pueda reservar caso contrario cierro el archivo y retorno `NULL`.
En la funcion `guardar_pokemon` reservo memoria a un `pokemon_t*` un puntero que apunta a un `pokemon_t`  en el heap y le asigno a `pokemon_listo` los datos de `poke` que seria un pokemon con todos los campos validos y creo una variable auxiliar `pokemon_** pokemon` a la cual le asigno el resultado de `realloc` ,verifico que se pueda agrandar el vector ,en ese caso le asigno a `info->pokemones` la variable auxiliar y agrego el pokemon al vector ,aumento la cantidad de pokemones para agrandar el vector dinamico en la siguiente iteracion. 


```c
   /*en la funcion pokemon cargar archivo */
informacion_pokemon_t *info = calloc(1,sizeof(informacion_pokemon_t));
if (info == NULL) {
    fclose(archivo);
    return NULL;
}
     /* en la funcion guardar pokemon */
pokemon_t *pokemon_listo = calloc(1,sizeof(pokemon_t));
if (pokemon_listo == NULL) {
    *problema_memoria= true;
}
*pokemon_listo = poke;
pokemon_t** pokemon = realloc(info->pokemones,(long unsigned int)(1 + info->cantidad) * (sizeof(pokemon_t *)));

if (pokemon != NULL) {
    info->pokemones = pokemon;
    info->pokemones[info->cantidad] = pokemon_listo;
    info->cantidad++;
} else {
    free(pokemon_listo);
    *problema_memoria = true;
}
```


<div align="center">
<img width="70%" src="img/diagrama de memoria.jpeg">
</div>

---

## Respuestas a las preguntas teóricas

    ```c
                      /*ordenaniento por burbujeo*/
     pokemon_t *poke_aux;                                                          -> 1
	for (int i = 1; i < info->cantidad; i++) {                                    -> n 
		for (int j = 0; j < info->cantidad - i; j++) {                            -> n -i
			const char *primer_poke = pokemon_nombre(info->pokemones[j]);         -> 1
			const char *segundo_poke = pokemon_nombre(info->pokemones[j + 1]);    -> 1
			if (strcmp(primer_poke, segundo_poke) > 0) {
				poke_aux = info->pokemones[j];                                    -> 1
				info->pokemones[j] = info->pokemones[j + 1];                      -> 1
				info->pokemones[j + 1] = poke_aux;                                -> 1
			}
		}
	}
    ```

        COMPLEJIDAD ALGORITMICA
        ========================
    n = info->cantidad

    i vas de 1 a n y j va de 0 a n-i lo cual seria de la siguiente forma:
    (ciclos)      (comparaciones)
    i = 1  ==>      j hace  n
    i = 2  ==>      j hace  n-1
    i = 3  ==>      j hace  n-2
    i = 4  ==>      j hace  n-3
        ......
    i = n-3 ==>    j hace 3
    i = n-2 ==>    j hace 2
    i = n-1 ==>    j hace 1
    i = n   ==>    j hace 0

    seria una sumatoria --> 1 +2+3+4+...+(n-4)+(n-3)+(n-2)+(n-1)+n = n(n-1)/2 =n²/2 -n/2 
    que en el peor de los casos que debe hacer el maximo de intercambios en cada ciclo seria O(n²)
    en el mejor de los casos que el vector este ordenado no haria intercambios y tendria una complejidad
    algoritmica de O(n)
