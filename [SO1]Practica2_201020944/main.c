#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "util.h"


WINDOW *bienvenida; //pantallas del juego
WINDOW *j1;
WINDOW *j2;
WINDOW *datos;
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
void PantallaJuego();
void FinJuego();

//----------- JUEGO ----------------------------------------------------
void IniciarJugadores();
void IniciarInvasores();
void CrearTablero();
void DibujarInvasores();
void Disparar();

//----------- HILOS ----------------------------------------------------
void *hilo_juego();
void *hilo_tiempo();
void *hilo_invasores();



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

    // Conseguimos una clave para la memoria compartida.
    llave_key = ftok("/bin/ls", _key);
    // Creamos la memoria con la clave recién conseguida.
    id_key = shmget(llave_key, sizeof (char), 0777 | IPC_CREAT);
    // Hacemos que uno de nuestros punteros apunte a la zona de memoria recién creada.
    key = (char *) shmat(id_key, (char *) 0, 0);
    /* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/

    // Conseguimos una clave para la memoria compartida.
    llave_invasor = ftok("/bin/ls", _invasor);
    // Creamos la memoria con la clave recién conseguida.
    id_invasor = shmget(llave_invasor, sizeof (List_invasores), 0777 | IPC_CREAT);
    // Hacemos que uno de nuestros punteros apunte a la zona de memoria recién creada.
    invasores = (List_invasores*) shmat(id_invasor, NULL, 0);

    // Conseguimos una clave para la memoria compartida.
    llave_disparo = ftok("/bin/ls", _disparo);
    // Creamos la memoria con la clave recién conseguida.
    id_disparo = shmget(llave_disparo, sizeof (List_disparos), 0777 | IPC_CREAT);
    // Hacemos que uno de nuestros punteros apunte a la zona de memoria recién creada.
    disparos = (List_disparos*) shmat(id_disparo, NULL, 0);

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
    shmdt((char *) key);
    // Liberamos la memoria compartida.
    shmctl(id_key, IPC_RMID, (struct shmid_ds *) NULL);
    /* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/

    // Desasociamos el puntero de la memoria compartida.
    shmdt((char *) invasores);
    // Liberamos la memoria compartida.
    shmctl(id_invasor, IPC_RMID, (struct shmid_ds *) NULL);
    /* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/

    // Desasociamos el puntero de la memoria compartida.
    shmdt((char *) disparos);
    // Liberamos la memoria compartida.
    shmctl(id_disparo, IPC_RMID, (struct shmid_ds *) NULL);
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
    init_pair(4,COLOR_BLUE,COLOR_WHITE);
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

    espera[3]=0;
    espera[4]=0;
    IniciarJugadores();
    IniciarInvasores();

    clear();
    datos = newwin(15,50,1,1);
    bkgd(COLOR_PAIR(4));

    // Creamos el hilo del juego
    pthread_create(&id_hilo_juego, NULL, hilo_juego, NULL);
    // Creamos el hilo del reloj
    pthread_create(&id_hilo_tiempo, NULL, hilo_tiempo, NULL);
    // Creamos el hilo de los invasores
    pthread_create(&id_hilo_invasores, NULL, hilo_invasores, NULL);


    while (key[0] != 't'){
        key[0] = getch();
        //mover jugadores
        if(jugador == 1){
            // Izquierda jugador 1
            if ((key[0] == 'a') && jug1->pos_x > 10) {
                jug1->pos_x = jug1->pos_x - 1;
            }
            // Derecha jugador 1
            if ((key[0] == 'd') && jug1->pos_x < 45) {
                jug1->pos_x = jug1->pos_x + 1;
            }

            if ((key[0] == 'w') && jug1->pos_x < 45) {
                espera[4] = 1;
            }

        }else if(jugador == 2){
            // Izquierda jugador 1
            if ((key[0] == 'a') && jug2->pos_x > 10) {
                jug2->pos_x = jug2->pos_x - 1;
            }
            // Derecha jugador 1
            if ((key[0] == 'd') && jug2->pos_x < 45) {
                jug2->pos_x = jug2->pos_x + 1;
            }

            if ((key[0] == 'w') && jug2->pos_x < 45) {
                espera[4] = 1;
            }

        }

        if (key[0] == 'l') {
            espera[3] = 1;
        }
    }

    // matamos el hilo del juego
    pthread_kill(&id_hilo_juego);
    // matamos el hilo del reloj
    pthread_kill(&id_hilo_tiempo);
    // matamos el hilo de invasores
    pthread_kill(&id_hilo_invasores);


//    getch();
}

void FinJuego() {
    int pulso;

    clear();

    //fin = newwin(15,50,1,1); //inicializa la pantalla de bienvenida
    bkgd(COLOR_PAIR(1));  //Aqui define el color de fondo del programa
    attrset(A_BOLD);
    mvprintw(7,32,"SPACE INVADERS");
    attrset(A_BOLD);
    mvprintw(10,33,"Fin del Juego");

    attrset(A_PROTECT);
    int pts = 0;
    if (jug1->puntos > jug2->puntos) {
        mvprintw(12, 30, "Ganador:  Jugador 1");
        pts = jug1->puntos;
    } else {
        mvprintw(12, 30, "Ganador:  Jugador 2");
        pts = jug2->puntos;
    }
    mvprintw(14, 30, "Puntaje:  %d", pts);
    mvprintw(16, 30, "Tiempo:   %d:%d min",tiempo[1], tiempo[0] / 1000);
    mvprintw(20, 22, "Presiona la tecla \"T\" para finializar.");
    /*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

    /* Refrescamos para ver los cambios en cada ventana . . . . . . . . . . . */
    refresh();
    /*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

    while ((pulso = getch()) != 'e');

    /*Importante para poder ver las dos opciones de nuevo */
    espera[1] = 0; // habilitamos al rojo
    espera[2] = 0; // habilitamos al azul
    getch();
}

void IniciarJugadores(){
    //jugador 1
    jug1->pos_x = 25;
    jug1->pos_y = 2;
    jug1->puntos = 0;
    jug1->vida = 5;

    //jugador 2
    jug2->pos_x = 25;
    jug2->pos_y = 26;
    jug2->puntos = 0;
    jug2->vida = 5;

    // Tiempo
    tiempo[0] = 0;
    tiempo[1] = 0;
}

void IniciarInvasores(){
    int x = 5;
    int y = 8;
    int t = 1;
    int i;
    int cnt = 1;
    for(i = 0; i < 20; i++){
        cnt++;
        if(cnt == 5){
            invasores->lista[i].pos_x = x;
            invasores->lista[i].pos_y = y;
            invasores->lista[i].tipo = 2;
            invasores->lista[i].vivo = true;
            x = 5;
            y = y + 3;
            cnt = 1;
        }else{
            invasores->lista[i].pos_x = x;
            invasores->lista[i].pos_y = y;
            invasores->lista[i].tipo = 1;
            invasores->lista[i].vivo = true;
            x = x + 10;
        }
    }
}

void MostrarDatos(){
    if(jugador == 1){
        mvprintw(5,60,"Jugador: %d",jugador);
        mvprintw(6,62,"Puntos: %d",jug1->puntos);
        mvprintw(7,62,"Vidas: %d",jug1->vida);
        mvprintw(24,60,"Jugador: 2");
        mvprintw(25,62,"Puntos: %d",jug2->puntos);
        mvprintw(26,62,"Vidas: %d",jug2->vida);
    }else if(jugador == 2){
        mvprintw(5,60,"Jugador: %d",jugador);
        mvprintw(6,62,"Puntos: %d",jug2->puntos);
        mvprintw(7,62,"Vidas: %d",jug2->vida);
        mvprintw(24,60,"Jugador: 1");
        mvprintw(25,62,"Puntos: %d",jug1->puntos);
        mvprintw(26,62,"Vidas: %d",jug1->vida);
    }

    mvprintw(15,60,"Tiempo:");
    mvprintw(16,62,"%d:%d",tiempo[1],tiempo[0]/1400);

}

void CrearTablero(){
    int i;
    char c;
    //lineas horizontales
    c = 95;
    for(i = 1; i <= 74; i++){
        move(0,i);
        printw("%c",c);
        move(27,i);
        printw("%c",c);
    }

    for(i = 59; i <= 74; i++){
        move(10,i);
        printw("%c",c);
        move(20,i);
        printw("%c",c);
    }

    //lineas verticales
    c = 124;
    for(i = 1; i <= 27; i++){
        move(i,1);
        printw("%c",c);
        move(i,75);
        printw("%c",c);
    }

    for(i = 1; i <= 27; i++){
        move(i,58);
        printw("%c",c);
    }

}

void DibujarJugadores(){
    int x;
    int y;
    int i;

    x = jug1->pos_x;
    y = jug1->pos_y;

    // Dibuja jugador 1
    move(y,x-1);
    printw("<");
    for (i = 0; i < 5; i++) {
        move(y,x+i);
        printw("-");
    }
    move(y,x+5);
    printw(">");

    x = jug2->pos_x;
    y = jug2->pos_y;

    // Dibuja jugador 2
    move(y,x-1);
    printw("<");
    for (i = 0; i < 5; i++) {
        move(y,x+i);
        printw("-");
    }
    move(y,x+5);
    printw(">");

}

void DibujarInvasores(){
    int x = 0;
    int y = 0;
    int t = 0;
    int i;
    for(i = 0; i < 20; i++){
        if(invasores->lista[i].vivo == true){
            x = invasores->lista[i].pos_x;
            y = invasores->lista[i].pos_y;
            t = invasores->lista[i].tipo;
            move(y,x+5);
            if(t == 1){
                printw("\\-.-/");
            }else if(t == 2){
                printw("/-.-\\");
            }
        }
    }
}

void Disparar(){
    int x;
    int y;
    if(jugador == 1){
        x = disparos->lista[0].pos_x = jug1->pos_x + 3;
        y = disparos->lista[0].pos_y = jug1->pos_y + 1;
    }else if(jugador == 2){
        x = disparos->lista[1].pos_x = jug2->pos_x + 3;
        y = disparos->lista[1].pos_y = jug2->pos_y - 1;
    }


    while(1){
        usleep(3 * 1000);
        move(y,x);
        printw("*");
        refresh();
        if(jugador == 1){
            y = y + 1;
            disparos->lista[0].pos_y = y;
        }else if(jugador == 2){
            y = y - 1;
            disparos->lista[1].pos_y = y;
        }

        int i;
        bool yamato = false;
        for(i = 0; i < 20; i++){
            if(yamato == false){
                if(invasores->lista[i].vivo == true){
                    int pos_disx = x;
                    int pos_disy = y;
                    int pos_invx = invasores->lista[i].pos_x;
                    int pos_invy = invasores->lista[i].pos_y;
                    int cnt;
                    for(cnt = 0; cnt < 5; cnt++){
                        if(pos_disx == (pos_invx + cnt) && pos_disy == pos_invy){
                            invasores->lista[i].vivo = false;
                            yamato = true;
                            if(jugador == 1){
                                if(invasores->lista[i].tipo == 1){
                                    jug1->puntos = jug1->puntos + 10;
                                    if(jug1->puntos > 100){
                                        espera[3] = 1;
                                    }
                                }else if(invasores->lista[i].tipo == 2){
                                    jug1->puntos = jug1->puntos + 15;
                                    if(jug1->puntos > 100){
                                        espera[3] = 1;
                                    }
                                }
                            }else if(jugador == 2){
                                if(invasores->lista[i].tipo == 1){
                                    jug2->puntos = jug1->puntos + 10;
                                    if(jug2->puntos > 100){
                                        espera[3] = 1;
                                    }
                                }else if(invasores->lista[i].tipo == 2){
                                    jug2->puntos = jug1->puntos + 15;
                                    if(jug2->puntos > 100){
                                        espera[3] = 1;
                                    }
                                }
                            }
                            break;
                        }

                        if(jugador == 1){
                            if(pos_disx == jug2->pos_x && pos_disy == jug2->pos_y){
                                jug2->vida = jug2->vida - 1;
                                if(jug2->vida == 0){
                                    espera[3] = 1;
                                }
                            }
                        }else if(jugador == 2){
                            if(pos_disx == jug1->pos_x && pos_disy == jug1->pos_y){
                                jug1->vida = jug1->vida - 1;
                                if(jug1->vida == 0){
                                    espera[3] = 1;
                                }
                            }
                        }


                    }
                }
            }
        }

        if(y == 1 || y == 27){
            break;
        }
    }
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


//----------------------------------------------- AREA HILOS ------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------

void* hilo_juego() {
    //for(;;){
    while(1){
        if(espera[3]==0){
            clear();
            CrearTablero();
            DibujarJugadores();
            DibujarInvasores();
            MostrarDatos();
            if(espera[4] == 1){
                Disparar();
                espera[4] = 0;
            }
            refresh();
        }else if(espera[3]==1){
            FinJuego();
            refresh();
        }
    }
    //}
}

void* hilo_tiempo() {
    while (1) {
        usleep(1 * 1000); // 1 ms
        tiempo[0] = tiempo[0] + 1;
        if(tiempo[0]/1400 == 60){
            tiempo[0] = 0;
            tiempo[1] = tiempo[1] + 1;
        }
    }
}

void* hilo_invasores(){
    int cnt = 0;
    while(1){
        usleep(2 * 1000);
        if(cnt == 0){
            int i;
            for(i = 0; i < 20; i++){
                invasores->lista[i].pos_x = invasores->lista[i].pos_x + 3;
            }
            cnt = 1;
        }else if(cnt == 1){
            int i;
            for(i = 0; i < 20; i++){
                invasores->lista[i].pos_x = invasores->lista[i].pos_x - 3;
            }
            cnt = 0;
        }
    }
}

//----------------------------------------------- AREA DEKKER ------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------



