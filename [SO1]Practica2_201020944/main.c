#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include "util.h"

WINDOW *bienvenida; //pantallas del juego
WINDOW *j1;
WINDOW *j2;
int jugador;// variable para ver que jugador soy

//----------- MEMORIA COMPARTIDA ---------------------------------------

void MemoriaComparida(); //metodo de memoria compartida
void LiberarMemoria();

//----------- SEMAFORO -------------------------------------------------

void semaforos();
int crea_sem(int);
int abre_sem();
void sem_P();
void sem_V();

//---------- PANTALLAS -------------------------------------------------

void IniciarPantalla();//metodo para iniciar pantalla
void Bienvenida();//metodo para pantalla de bienvenida
void SeleccionarBando();//metodo para pantalla de seleccion de jugador
void EsperarRival();//metodo para esperar al rival


//----------- JUEGO ----------------------------------------------------
void IniciarJugadores();


int main()
{
    MemoriaComparida();

    semaforos();

    jugador = 0;//iniciamos judador con 0 1 = jugador1 | 2 = jugador2

    IniciarPantalla(); //iniciamos pantalla

    SeleccionarBando(); //pantalla para escojer que jugador soy

    PantallaJuego();// pantalla de juego

    LiberarMemoria();

    return 0;
}

//----------------------------------------------- AREA MEMORIA COMPARTIDA --------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------

void MemoriaComparida(){
    // Conseguimos una clave para la memoria compartida de espera seleccionar jugador.
    llave_espera = ftok("/bin/ls", _espera);
    // Creamos la memoria con la clave recién conseguida.
    id_espera = shmget(llave_espera, sizeof (int) * 4, 0777 | IPC_CREAT);
    // Hacemos que uno de nuestros punteros apunte a la zona de memoria recién creada.
    espera = (int *) shmat(id_espera, (char *) 0, 0);
    /* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/

    if (espera[0] != 1) {
        espera[0] = 1; // indica que es el primero
    } else if (espera[0] == 1) {
        espera[0] = 2; // indica que es e segundo
    }


     // Conseguimos una clave para la memoria compartida.
    llave_jug1 = ftok("/bin/ls", _jug1);
    // Creamos la memoria con la clave recién conseguida.
    id_jug1 = shmget(llave_jug1, sizeof (Jugador), 0777 | IPC_CREAT);
    // Hacemos que uno de nuestros punteros apunte a la zona de memoria recién creada.
    jug1 = (Jugador *) shmat(id_jug1, NULL, 0);
    /* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/

    // Conseguimos una clave para la memoria compartida.
    llave_jug2 = ftok("/bin/ls", _jug2);
    // Creamos la memoria con la clave recién conseguida.
    id_jug2 = shmget(llave_jug2, sizeof (Jugador), 0777 | IPC_CREAT);
    // Hacemos que uno de nuestros punteros apunte a la zona de memoria recién creada.
    jug2 = (Jugador *) shmat(id_jug2, NULL, 0);
    /* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/

    // Conseguimos una clave para la memoria compartida.
    llave_tiempo = ftok("/bin/ls", _tiempo);
    // Creamos la memoria con la clave recién conseguida.
    id_tiempo = shmget(llave_tiempo, sizeof (int), 0777 | IPC_CREAT);
    // Hacemos que uno de nuestros punteros apunte a la zona de memoria recién creada.
    tiempo = (int *) shmat(id_tiempo, (char *) 0, 0);
    /* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/

}

void LiberarMemoria(){
    // Desasociamos el puntero de la memoria compartida.
    shmdt((char *) espera);
    // Liberamos la memoria compartida.
    shmctl(id_espera, IPC_RMID, (struct shmid_ds *) NULL);
    /* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/

    // Desasociamos el puntero de la memoria compartida.
    shmdt((char *) jug1);
    // Liberamos la memoria compartida.
    shmctl(id_jug1, IPC_RMID, (struct shmid_ds *) NULL);
    /* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/

    // Desasociamos el puntero de la memoria compartida.
    shmdt((char *) jug2);
    // Liberamos la memoria compartida.
    shmctl(id_jug2, IPC_RMID, (struct shmid_ds *) NULL);
    /* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/

    // Desasociamos el puntero de la memoria compartida.
    shmdt((char *) tiempo);
    // Liberamos la memoria compartida.
    shmctl(id_tiempo, IPC_RMID, (struct shmid_ds *) NULL);
    /* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/
}

//----------------------------------------------- AREA PANTALLAS -----------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------
void IniciarPantalla(){
    int pulso = 0; // variable para ver que tecla apacho
    initscr(); //inicia ncurses
    cbreak();
    keypad(stdscr,TRUE);
    Bienvenida();//inicia pantalla de bienvenida
    while(pulso=getch() != '\n'){
        Bienvenida();
    }
    refresh();
    clear();
    endwin();//termina pantalla de bienvenida
}

void Bienvenida(){
    bienvenida = newwin(15,50,1,1); //inicializa la pantalla de bienvenida
    start_color(); //Esta función inicia los colores
    init_pair(1,COLOR_WHITE,COLOR_BLUE); //Texto(Blanco) | Fondo(Azul)
    init_pair(2,COLOR_BLACK,COLOR_RED);
    init_pair(3,COLOR_RED,COLOR_GREEN);
    init_pair(4,COLOR_YELLOW,COLOR_BLACK);
    init_pair(5,COLOR_RED,COLOR_WHITE);
    bkgd(COLOR_PAIR(1));  //Aqui define el color de fondo del programa
    mvprintw(20,1,"Luis Aroldo Morales Noriega");
    mvprintw(20,70,"201020944");
    attrset(A_BOLD);
    mvprintw(7,31,"SPACE INVADERS");
    attrset(A_PROTECT);
    mvprintw(5,32,"Bienvenidos");
    mvprintw(10,25,"Presiona Enter para Continuar");
    refresh();
}

void SeleccionarBando(){
    clear();
    j1 = subwin(stdscr,3,13,5,30);
    j2 = subwin(stdscr,3,13,10,30);

    WINDOW * subj1 = subwin(j1,1,10,6,32);
    WINDOW * subj2 = subwin(j2,1,10,11,32);

    wbkgd(j1,COLOR_PAIR(2));
    wbkgd(subj1,COLOR_PAIR(2));
    box(j1,ACS_VLINE,ACS_HLINE);


    wbkgd(j2,COLOR_PAIR(3));
    wbkgd(subj2,COLOR_PAIR(3));
    box(j2,ACS_VLINE,ACS_HLINE);


    mvprintw(15,20,"Presione 1 para elejir Jugador 1");
    mvprintw(16,20,"Presione 2 para elejir Jugador 2");

    mvprintw(18,20,"El juego iniciara hasta que esten los 2 jugadores");
    mvprintw(19,20,"");

    // habilitamos o deshabilitamos opciones de seleccion
    if (espera[1] == 1) {
        waddstr(subj2,"Jugador 2");
        wbkgd(subj1,COLOR_PAIR(1));
        wbkgd(j1,COLOR_PAIR(1));
    } else if (espera[2] == 1) {
        waddstr(subj1,"Jugador 1");
        wbkgd(subj2,COLOR_PAIR(1));
        wbkgd(j2,COLOR_PAIR(1));
    } else {
        waddstr(subj1,"Jugador 1");
        waddstr(subj2,"Jugador 2");
    }

    refresh();

    int elejido = 0;
    elejido = getch();
    if(elejido == '1' && espera[1] != 1){
        espera[1] = 1; // indica que el rojo ya no esta disponible
        jugador = 1;
        EsperarRival();
    }else if(elejido == '2' && espera[2] != 1){
        espera[2] = 1; // indica que el azul ya no esta disponible
        jugador = 2;
        EsperarRival();
    }else{
        SeleccionarBando();
    }

    PantallaJuego();
}

void EsperarRival(){
    clear();
    bienvenida = newwin(15,50,1,1); //inicializa la pantalla de bienvenida
    bkgd(COLOR_PAIR(1));  //Aqui define el color de fondo del programa
    attrset(A_BOLD);
    mvprintw(7,31,"SPACE INVADERS");
    attrset(A_PROTECT);
    mvprintw(10,25,"Esperando Rival .........");
    refresh();

    // Utilizacion del semaforo para e  sperar rival
    if(espera[0] == 1) {
        sem_P(); // pone el semaforo en rojo
    }else if (espera[0] == 2){
        sem_V(); // pone el semaforo en verde
    }

    wclear(bienvenida); // limpiamos stdscr
    refresh(); // refrescamos stdscr

}

void PantallaJuego(){

    IniciarJugadores();

    clear();
    WINDOW *juego = newwin(15,50,1,1);
    bkgd(COLOR_PAIR(4));
    mvprintw(7,31,"Jugador: %d",jugador);

    getch();
}


void IniciarJugadores(){
    //jugador 1
    jug1->pos_x = 23;
    jug1->pos_y = 1;
    jug1->puntos = 0;
    jug1->vida = 5;

    //jugador 2
    jug2->pos_x = 23;
    jug2->pos_y = 22;
    jug2->puntos = 0;
    jug2->vida = 5;

    // Tiempo
    tiempo[0] = 0;
}


//----------------------------------------------- AREA SEMAFORO ------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------
void semaforos() {
    /* Se obtien una clave para el semaforo de jugador 1. */
    llave_semaforo = ftok("/bin/ls", _semaforo);
    if (llave_semaforo == (key_t) - 1) {
        printf("No puedo conseguir clave de semáforo 1");
        exit(0);
    }

    // Creamos el semaforo si somos los primeros, si no solo abrimos
    if (espera[0] == 1) {
        id_semaforo = crea_sem(0);
    } else if (espera[0] == 2) {
        id_semaforo = abre_sem();
    }
}

int crea_sem(int valor_inicial){
    int semaid = semget(llave_semaforo,1, 0600 | IPC_CREAT);
    if(semaid == -1) return -1;
    semctl(semaid,0,SETVAL,valor_inicial);
    return semaid;
}

int abre_sem(){
    return semget(llave_semaforo,1,0600);
}

void sem_P() {
    struct sembuf op_P [] = {
        0, -1, 0 /* Decrementa semval o bloquea si cero */
    };

    semop(id_semaforo, op_P, 1);
}

void sem_V() {
    struct sembuf op_V [] = {
        0, 1, 0 /* Incrementa en 1 el semáforo */
    };
    semop(id_semaforo, op_V, 1);
}





