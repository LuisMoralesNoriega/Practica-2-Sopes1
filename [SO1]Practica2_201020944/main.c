#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

WINDOW *bienvenida,*j1,*j2;
int jugador;


void Bienvenida();
void IniciarPantalla();
void SeleccionarBando();

int main()
{
    jugador = 0;

    IniciarPantalla();

    SeleccionarBando();

    return 0;
}

void IniciarPantalla(){
    int pulso = 0;
    initscr();
    cbreak();
    keypad(stdscr,TRUE);
    Bienvenida();
    while(pulso=getch() != '\n'){
        Bienvenida();
    }
    refresh();
    clear();
    endwin();
}


void Bienvenida(){
    bienvenida = newwin(15,50,1,1);
    start_color(); //Esta función inicia los colores
    init_pair(1,COLOR_WHITE,COLOR_BLUE); //Texto(Blanco) | Fondo(Azul)
    init_pair(2,COLOR_BLACK,COLOR_RED);
    init_pair(3,COLOR_RED,COLOR_GREEN);
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
    waddstr(subj1,"Jugador 1");

    wbkgd(j2,COLOR_PAIR(3));
    wbkgd(subj2,COLOR_PAIR(3));
    box(j2,ACS_VLINE,ACS_HLINE);
    waddstr(subj2,"Jugador 2");

    mvprintw(15,20,"Presione 1 para elejir Jugador 1");
    mvprintw(16,20,"Presione 2 para elejir Jugador 2");

    mvprintw(18,20,"El juego iniciara hasta que esten los 2 jugadores");
    mvprintw(19,20,"");

    int elejido = 0;
    elejido = getch();
    if(elejido == '1'){
        jugador = 1;
    }else if(elejido == '2'){
        jugador = 2;
    }else{
        SeleccionarBando();
    }


}
