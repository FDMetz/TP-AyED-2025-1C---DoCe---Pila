#include "DoCe.h"

/** Si quieren que la mano del jugador y de la ia sean listas de 3 cartas, haganlo (?)*/

/*
*----------------TO-DO-LIST---------------*

partida()
|
|-> mezclarMazo()
|-> repartirCartas() -> desapilar 1 para cada 1 hasta que ambos tengan 3
|-> loop
     |-> turnoJugador()
     |		|-> mostrarCartas()
     |		|-> usarCarta()
     |		|-> jugador.puntaje == 12 ? Si sí, gana y sale del loop. Si no, sigue.
     |		|-> El MAZO está VACÍO ? Si sí, mezclarDescarteYusarDeMazo(). Si no, sigue normal.
     |		|-> robarDelMazo() -> desapilar de pila
     |
     |-> turnoIA()---> un switch de tres salidas con cada camino segun la dificultad
            |-> (jugar una carta según nivel de dificultad)
          	|-> IA.puntaje == 12? Si sí, gana y sale del loop. Si no, sigue.
          	|-> El MAZO está VACÍO ? Si sí, mezclarDescarteYusarDeMazo(). Si no, sigue normal.
            |-> robarDelMazo() -> desapilar de pila
            |-> vuelve a empezar el loop

verRanking ---> API

guardarResultado ---> API

*/

char menu(){

    int opc='Z';

    do{
    printf("DoCe\n\n"
           "[A] Jugar\n"
           "[B] Ver ranking\n"
           "[C] Salir\n\n"
           "Seleccione una opcion: ");
    opc = getchar();
    while( getchar()!='\n'); //Igual, si le mandas EOF se rompe...
    opc = toupper(opc);
    system("cls");
    }while( opc<'A' || opc>'C' );

    return opc;
}

void jugar(){

    unsigned dificultad;
    char nombreJugador[26];

    printf("\nIngrese el nombre del jugador: ");
    scanf("%s",nombreJugador);
    puts("\nSeleccione el nivel de dificultad contra la maquina: \n\n"
         "1) Facil\n"
         "2) Medio\n"
         "3) Dificil\n"
         );

    do{
    scanf("%d",&dificultad);
    }while(dificultad<1 || dificultad>3);

    partida(dificultad, nombreJugador);
}

void partida(unsigned dificultad, const char* nombreJugador){

    char fin = '0';
    tJugador jugador;
    tIA ia;
    tPilaEstatica mazo;
    tPilaEstatica descarte;

    ia.dificultad = dificultad;
    strcpy(jugador.nombre,nombreJugador);
    jugador.puntaje = 0;

    ///Preparativos antes de comenzar el juego
    crearPila(&mazo);
    mezclarMazo(&mazo);

////Se usa para probar mezclar mazo
//    for(int i=0;i<TAM_MAZO;i++){
//        int aux;
//        desapilar(&mazo,&aux,sizeof(unsigned));
//        printf("%d ",aux);
//    }
//    system("pause");


    repartirCartas(&mazo, &jugador, &ia);


    ///loop
    while(fin != '1'){
       if(turnoJugador(&jugador, &mazo, &descarte) == GANO)
            fin = '0';
       else
        if(turnoIa(&ia,&mazo,&descarte) == GANO)
            fin = '0';
    }

    //guardarResultado()
}

void mezclarMazo(tPilaEstatica* mazo){

    //IDs de cartas:      cant:
    //MAS_DOS         0     6
    //MAS_UNO         1     10
    //MENOS_UNO       2     8
    //MENOS_DOS       3     6
    //REPETIR_TURNO   4     6
    //ESPEJO          5     4

    //A cada pos del vector le corresponde una ID de carta
    unsigned cartas[] = {CANT_MAS_DOS,CANT_MAS_UNO,CANT_MENOS_UNO,CANT_MENOS_DOS,CANT_REPETIR_TURNO,CANT_ESPEJO};
    unsigned cartasApiladas = 0, idCarta;

    srand(time(0));

    while(cartasApiladas < 40){

        do{
            idCarta = rand() % 6;
        }while(cartas[idCarta] == 0);

        apilar(mazo,&idCarta,sizeof(unsigned));
        cartas[idCarta]--;
        cartasApiladas++;
    }
}

void MezclarDescarte(tPilaEstatica* descarte, tPilaEstatica* mazo){

    unsigned descartadas[TAM_MAZO], i=0, cant=0, j, aux;

    srand(time(0));

    while( pilaVacia(descarte) != PILA_VACIA ){
        desapilar(descarte,&descartadas[i],sizeof(unsigned));
        i++;
        cant++;
    }

    for(i=cant;i>0;i--){
        j = rand() % i;
        aux = descartadas[j];
        descartadas[i] = descartadas[j];
        descartadas[j] = aux;
    }

    for(i=0;i<cant;i++)
        apilar(mazo,&descartadas[i],sizeof(unsigned));
}

void repartirCartas(tPilaEstatica* mazo, tJugador* jugador, tIA* ia){
    unsigned i;
    for(i = 0 ; i < 3 ; i++){
        desapilar(mazo,&jugador->mano[i],sizeof(unsigned));
        desapilar(mazo,&ia->mano[i],sizeof(unsigned));
    }
}

void mostrarCartasJugador(tJugador jugador){
    unsigned i;
    printf("Mano de %s: \n\n", jugador.nombre);
    for(i = 0 ; i < 3 ; i++){
        switch(jugador.mano[i]){
        case MAS_DOS:
            printf("%u- Mas dos puntos\n",i+1);
            break;
        case MAS_UNO:
            printf("%u- Mas un punto\n",i+1);
            break;
        case MENOS_UNO:
            printf("%u- Menos un punto al oponente\n",i+1);
            break;
        case MENOS_DOS:
            printf("%u- Menos dos puntos al oponente\n",i+1);
            break;
        case REPETIR_TURNO:
            printf("%u- Repetir turno\n",i+1);
            break;
        case ESPEJO:
            printf("%u- Espejo\n",i+1);
            break;
        }
    }
    printf("Puntaje: %d",jugador.puntaje);
}

///Completar - Nico
int turnoJugador(tJugador* jugador, tPilaEstatica* mazo, tPilaEstatica* descarte){
    mostrarCartasJugador(*jugador);

    fflush(stdin);
    getchar(); //estas dos lineas eran para ver algo nomas, quitalas cuando hagas esta parte(?)

    //jugador.puntaje == 12 ? Ganó, salir por GANO : salir por SIGUE
}

///Completar - Matías
int turnoIa(tIA* ia, tPilaEstatica* mazo, tPilaEstatica* descarte){
}

///completar - Federico: guardarResultado() y verRanking() (la parte de conexión con la API)
