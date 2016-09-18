#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#define _espera 1
#define _semaforo 2
#define _jug1 3
#define _jug2 4
#define _tiempo 5
#define _invasor 6
#define _key 7
#define _disparo 8


#include <pthread.h> // Ejecución con varios hilos.
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h> // Para key_t
#include <time.h> // Proporciona time() que nos devuelve la hora.


/* Enteros para asignar memoria compartida */
#define _espera 1


#ifdef __cplusplus
extern "C" {
#endif

    //Estructuras
    typedef struct {
        int pos_x; // cooredenad inicial en x
        int pos_y; // coordenada inicial en y
        int puntos; // puntos obtenidos
        int vida;
    } Jugador; /* Representa a un jugador */

    typedef struct {
        int pos_x; // cooredenad inicial en x
        int pos_y; // coordenada inicial en y
        int tipo; // puntos obtenidos
        bool vivo; // nos muestra si es ta vivo
    }Invasor; /* Representa a un invasor */

    typedef struct {
        Invasor lista[20]; // lista de pelotas disponibles para un juego
    } List_invasores; /* Representa una lista de invasores */

    typedef struct {
        int pos_x; // posicion de disparo en x
        int pos_y; // posicion de disparo en y
    }Disparo; /* Representa un disparo */

    typedef struct {
        Disparo lista[1]; // lista de disparos
    } List_disparos; /* Representa una lista de disparos */


    //Memoria Compartida

    // Bandera espera
    key_t llave_espera; // Llave para la memoria compartida de la bandera de espera
    int id_espera; // Identificador para la memoria de la bandera de espera
    int *espera = NULL; //Apuntador a la zona de memoria de la bandera de espera

    // Semaforo
    key_t llave_semaforo; // Llave para el semaforo
    int id_semaforo; // Identificador para el array de semaforos

    //judafor 1
    key_t llave_jug1; // Llave para la memoria compartida del jugador 1
    int id_jug1; // Identificador para la memoria del jugador 1
    Jugador *jug1; // Apuntador a la zona de memoria del jugador 1

    //judafor 2
    key_t llave_jug2; // Llave para la memoria compartida del jugador 2
    int id_jug2; // Identificador para la memoria del jugador 2
    Jugador *jug2; // Apuntador a la zona de memoria del jugador 2

    // Tiempo del juego
    key_t llave_tiempo; // Llave para la memoria compartida del tiempo
    int id_tiempo; // Identificador para la memoria del tiempo
    int *tiempo; // Apuntador a la zona de memoria del tiempo

    //invasores
    key_t llave_invasor; // Llave para la memoria compartida de la(s) pelota(s)
    int id_invasor; // Identificador para la memoria de la(s) pelota(s)
    List_invasores *invasores; // Apuntador a la zona de memoria de la(s) pelota(s)

    //disparos
    key_t llave_disparo; // Llave para la memoria compartida de la(s) pelota(s)
    int id_disparo; // Identificador para la memoria de la(s) pelota(s)
    List_disparos *disparos; // Apuntador a la zona de memoria de la(s) pelota(s)

    // Key del teclado
    key_t llave_key; //Llave para la memoria compartida de key
    int id_key; // Identificador para la memoria de key
    char *key; // Apuntador a la zona de memoria de key

    //hilo de juego
    pthread_t id_hilo_juego; // Identificador del hilo_juego
    //hilo de tiempo
    pthread_t id_hilo_tiempo; // Identificador del hilo del hilo_tiempo
    //hilo de invasores
    pthread_t id_hilo_invasores; // Identificador del hilo del hilo_tiempo


#ifdef __cplusplus
}
#endif



#endif // UTIL_H_INCLUDED
