#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"
#include "list.h"

int de_Void_a_Int(void *); 
char* quitarSalto(char* linea);
void *get_csv_field (char *, int ); 


typedef struct{
  char id[30];
  char ciudad[30];
  char direccion[30];
  char tipo[30];
  int capacidad;
  char valor[30];
}Propiedad;

int main(void) 
{
  int opcion;
  char id[30], ciudad[30],direccion[30],tipo[30],valor[30];
  int capacidad;
  int capacidadMinima;
  int num;
  
  
  Propiedad *p;
  FILE* file;
  Pair *aux, *auxCiudad, *auxTipo;
  
  void* auxi;
  HashMap *mapaID = createMap(100);
  HashMap *mapaCiudad = createMap(100);
  HashMap *mapaTipo = createMap(100);
  HashMap *mapaFavoritos = createMap(100);
  
  char **listaPropiedades = (char **) malloc (sizeof(char*)*100);
  int contPropiedades = 0;
  int contFavoritos = 0;
  
  void **listaFavoritos = (void **) malloc (sizeof(void*)*100);
  
  List *ciudades, *copiaCiudades, *tipos, *copiaTipos;
  
  while(opcion!=0)
  {
    do{
      /*Menú que muestra en pantalla las opciones que podemos 
      realizar*/
      printf("\n Introduzca una opción (1-9)");
      printf("\n 1.Importar propiedades desde un archivo CSV");
      printf("\n 2.Agregar Propiedad");
      printf("\n 3.Mostrar todas las propiedades");
      printf("\n 4.Mostrar propiedades de cierta ciudad");
      printf("\n 5.Mostrar propiedades según su tipo");
      printf("\n 6.Mostrar propiedades con capacidad mínima");
      printf("\n 7.Agregar favoritos");
      printf("\n 8.Mostrar favoritos");
      printf("\n 9.Exportar base de datos");
      printf("\n 0.Salir");
      printf("\n");

      fflush(stdin);
      scanf("%i",&opcion);
      getchar(); 
      printf("\n");
    }while (opcion>10 && opcion<1);

    switch(opcion)
    {
      case 1:
        // Se abre el archivo de mundos csv en modo lectura "r"
        file = fopen ("archivoProp.csv", "r");
        // Cadena para guardar la linea completa del archivo csv
        char linea[1024];
        int v, contador;
        contador = 0;
        
        fgets(linea, 1023, file);
        
       
        int k=0;
        
        
        while(fgets(linea, 1023, file) != NULL)
        { // Se lee la linea
          
          //printf("ola");
          
          for(v=0;v<6;v++)
          {
            auxi = get_csv_field(linea, v); // Se obtiene el nombre
            switch (v)
            {
              case 0: 
                strcpy(id, auxi);
                contador++;
              break;
              case 1: 
                strcpy(ciudad, auxi);
                contador++;
              break;
              case 2:
                strcpy(direccion,auxi);
                contador++;
              break;
              case 3:
                strcpy(tipo, auxi);
                contador++;
              break;
              case 4:
                num = *(int *)auxi;
                num -= 48;
                capacidad = num;
                contador++;
              break;
              case 5:
                strcpy(valor, auxi);
                contador++;
              break;
            }
            if(contador == 6)
            {
              p = (Propiedad *) malloc (sizeof(Propiedad));

              strcpy(p->id, id);
              strcpy(p->ciudad, ciudad);
              strcpy(p->direccion, direccion);
              strcpy(p->tipo, tipo);
              p->capacidad = capacidad;
              strcpy(p->valor, valor);

              
              
              contador = 0;
              //Realizamos el procedimiento de agregar la propiedad al igual que el caso 2
              aux = searchMap(mapaID, p->id);
        
              if(aux == NULL)
              {
                //insertamos en el mapaID cada propiedad. /key = id, value = tipoPropiedad
                insertMap(mapaID, p->id, p);
                
                //agregamos en la lista Propiedades cada id de las propiedades.
                listaPropiedades[contPropiedades] = p->id;
                contPropiedades++;
                //printf("2ola");
                //agregamos la ciudad al mapaCiudad. /key = ciudad, value = listaCiudades (contiene los id de cada propiedad de la ciudad)
                if(searchMap(mapaCiudad, p->ciudad) == NULL)
                {
                  ciudades = createList();
                  pushFront(ciudades, p->id);
                  insertMap(mapaCiudad, p->ciudad, ciudades);
                }
                //añadimos el id de la propiedad ingresada a la lista de la ciudad.
                else
                {
                  auxCiudad = searchMap(mapaCiudad, p->ciudad);
                  copiaCiudades = auxCiudad->value;
                  pushFront(copiaCiudades, p->id);    
                  auxCiudad->value = copiaCiudades;
                }
                if(searchMap(mapaTipo, p->tipo) == NULL)
                {
                  tipos = createList();
                  pushFront(tipos, p->id);
                  insertMap(mapaTipo, p->tipo, tipos);
                }
                else
                {
                  auxTipo = searchMap(mapaTipo, p->tipo);
                  copiaTipos = auxTipo->value;
                  pushFront(copiaTipos, p->id);
                  auxTipo->value = copiaTipos;
                }
              }
            }
          }
        }
        printf("~Archivo importado correctamente~\n");
      break;
      case 2:
        
        p = (Propiedad *) malloc (sizeof(Propiedad));
        
        printf("Ingrese id: ");
        fgets(p->id,30,stdin);
        strcpy(p->id, quitarSalto(p->id));
        
        
        printf("Ingrese ciudad: ");
        fgets(p->ciudad,30,stdin);
        strcpy(p->ciudad, quitarSalto(p->ciudad));
        
        printf("Ingrese dirección: ");
        fgets(p->direccion,30,stdin);
        strcpy(p->direccion, quitarSalto(p->direccion));
        
        printf("Ingrese tipo: ");
        fgets(p->tipo, 30, stdin);
        strcpy(p->tipo, quitarSalto(p->tipo));
        
        printf("Ingrese valor: ");
        fgets(p->valor, 30, stdin);
        strcpy(p->valor, quitarSalto(p->valor));
        
        printf("Ingrese capacidad: ");
        scanf("%i",&p->capacidad);
        
        
        aux = searchMap(mapaID, p->id);
        
        if(aux == NULL)
        {
          
          //insertamos en el mapaID cada propiedad. /key = id, value = tipoPropiedad
          insertMap(mapaID, p->id, p);
          
          //agregamos en la lista Propiedades cada id de las propiedades.
          listaPropiedades[contPropiedades] = p->id;
          contPropiedades++;

          //agregamos la ciudad al mapaCiudad. /key = ciudad, value = listaCiudades (contiene los id de cada propiedad de la ciudad)
          if(searchMap(mapaCiudad, p->ciudad) == NULL)
          {
            ciudades = createList();
            pushFront(ciudades, p->id);
            insertMap(mapaCiudad, p->ciudad, ciudades);
          }
          //añadimos el id de la propiedad ingresada a la lista de la ciudad.
          else
          {
            auxCiudad = searchMap(mapaCiudad, p->ciudad);
            copiaCiudades = auxCiudad->value;
            pushFront(copiaCiudades, p->id);    
            auxCiudad->value = copiaCiudades;
          }
          if(searchMap(mapaTipo, p->tipo) == NULL)
          {
            tipos = createList();
            pushFront(tipos, p->id);
            insertMap(mapaTipo, p->tipo, tipos);
          }
          else
          {
            auxTipo = searchMap(mapaTipo, p->tipo);
            copiaTipos = auxTipo->value;
            pushFront(copiaTipos, p->id);
            auxTipo->value = copiaTipos;
          }
          printf("\n~Propiedad Agregada~\n");
        }
        else
        {
          printf("No es posible agregar la propiedad.\n");
        }
      break;
      case 3:
        //recorremos la lista de Propiedades en la posicion i, encontrándonos con las id de cada propiedad agregada, y buscandola en el mapaID para poder mostrarla
        //printf("contador: %i", contPropiedades);
        for(int i=0; i<contPropiedades; i++)
        {
          aux = searchMap(mapaID, listaPropiedades[i]);
          if(aux!=NULL)
          {
            p = aux->value;
            printf(" %s, %s, %s, %s, %i, %s \n", (char *)p->id, (char *)p->ciudad,  (char *)p->direccion, (char *)p->tipo, p->capacidad, (char *)p->valor);
          }
        }
      break;
      case 4:
        printf("Ingrese ciudad: ");
        fgets(ciudad, 30, stdin);
        strcpy(ciudad, quitarSalto(ciudad));
        
        //buscamos la key ingresada en el mapaCiudad, copiamos la lista almacenada en el valor del mapa, y la recorremos hasta que sea nula, para buscar los id (data de la lista) en el mapa ID
        auxCiudad = searchMap(mapaCiudad, ciudad);
        if(auxCiudad != NULL)
        {
          copiaCiudades = auxCiudad->value;
          for(char *j = firstList(copiaCiudades); j != NULL; j = nextList(copiaCiudades))
          {
            aux = searchMap(mapaID, j);

            if(aux != NULL)
            {
              p = aux->value;
              printf(" %s, %s, %s, %s, %i, %s \n", (char *)p->id, (char *)p->ciudad,  (char *)p->direccion, (char *)p->tipo, p->capacidad, (char *)p->valor);
            }
          }
        }
        else
        {
          printf("~No hay propiedades en la ciudad buscada~\n");
        }
      break;
      case 5:
        printf("Ingrese el tipo: ");
        fgets(tipo, 30, stdin);
        strcpy(tipo, quitarSalto(tipo));

        auxTipo = searchMap(mapaTipo, tipo);
        if(auxTipo != NULL)
        {
          copiaTipos = auxTipo->value;
          for(char *j = firstList(copiaTipos); j != NULL; j = nextList(copiaTipos))
          {
            aux = searchMap(mapaID, j);

            if(aux != NULL)
            {
              p = aux->value;
              printf(" %s, %s, %s, %s, %i, %s \n", (char *)p->id, (char *)p->ciudad,  (char *)p->direccion, (char *)p->tipo, p->capacidad, (char *)p->valor);
            }
          }
        }
        else
        {
          printf("~No hay propiedades de este tipo~\n");
        }
      break;
      case 6:

        printf("Ingrese capacidad minima: ");
        scanf("%i", &capacidadMinima);
        
        getchar(); 
        fflush(stdin);
        
        for(int i=0; i<contPropiedades; i++)
        {
          aux = searchMap(mapaID, listaPropiedades[i]);
          if(aux!=NULL)
          {
            p = aux->value;
            if(p->capacidad >= capacidadMinima)
            {
              printf(" %s, %s, %s, %s, %i, %s \n", (char *)p->id, (char *)p->ciudad,  (char *)p->direccion, (char *)p->tipo, p->capacidad, (char *)p->valor);
            }
            
          }
        }
        
      break;
      case 7:
        printf("Ingrese id: ");
        fgets(id, 30, stdin);
        strcpy(id, quitarSalto(id));
        
        if(searchMap(mapaID, id) != NULL)
        {
          listaFavoritos[contFavoritos] = id;
          contFavoritos++;
          printf("~Propiedad Agregada a Favoritos~\n");
        }
        else
        {
          printf("~El id ingresado no exite~\n");
        }
      break;
    
      case 8:
        if(contFavoritos == 0)
        {
          printf("No hay propiedades favoritas\n");
        }
        else
        {
          for(int i=0; i<contFavoritos; i++)
          {
            aux = searchMap(mapaID, listaFavoritos[i]);
            if(aux != NULL)
            {
              p = aux->value;
              printf(" %s, %s, %s, %s, %i, %s \n", (char *)p->id, (char *)p->ciudad,  (char *)p->direccion, (char *)p->tipo, p->capacidad, (char *)p->valor);
            }
          }
        
        }
      break;
      case 9:
        
        remove("Propiedades.csv");
        
        
        // Abrimos el archivo ".csv"
        file = fopen("Propiedades.csv", "w");
        
        if (!file) exit(EXIT_FAILURE);
        //Se crea un ciclo que comienza al inicio de la lista y termina cuando no hay mas elementos
        fprintf(file, "Id,Ciudad,Dirección,Tipo,Capacidad,Valor\n");
        for(int i = 0 ; i < contPropiedades; i++)
        {
          aux = searchMap(mapaID, listaPropiedades[i]);
          p = aux->value;
          if(aux != NULL)
          {  
            fprintf(file, "%s,", p->id);
            fprintf(file, "%s,", p->ciudad);
            fprintf(file, "%s,", p->direccion);
            fprintf(file, "%s,", p->tipo);
            fprintf(file, "%i,", p->capacidad);
            fprintf(file, "%s\n", p->valor);
          }
        }
        
        //Se cierra el archivo y mostramos un mensaje de que funcionó correctamente
        fclose(file);
        printf("~Documento .csv exportado~\n");
      break;
    }
    
  }
  return 0;
}

    
char* quitarSalto(char* linea)
{
  if ((strlen(linea) > 0) && (linea[strlen(linea) - 1] == '\n'))
  {
    linea[strlen(linea) - 1] = '\0';
  }
  return(linea);
}
    
/*Funcion utilizada para leer los datos de un archivo .csv línea por línea*/
void *get_csv_field (char * tmp, int k) 
{
  int open_mark = 0;
  char* ret=(char*) malloc (100*sizeof(char));
  int ini_i=0, i=0;
  int j=0;
  while(tmp[i+1]!='\0')
  {
    if(tmp[i]== '\"'){
      open_mark = 1-open_mark;
      if(open_mark) ini_i = i+1;
      i++;
      continue;
    }
    if(open_mark || tmp[i]!= ','){
      if(k==j) ret[i-ini_i] = tmp[i];
      i++;
      continue;
    }
    if(tmp[i]== ','){
      if(k==j){
        ret[i-ini_i] = 0;
        return ret;
      }
      j++; ini_i = i+1;
    }
    i++;
  }
  if(k==j) {
    ret[i-ini_i] = 0;
    return ret;
  }
  return NULL;
}
int de_Void_a_Int(void *numero) 
{
  int ip = *(int *) numero;
  int *a = &ip;
  return(*a);
}