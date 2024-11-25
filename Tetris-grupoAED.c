#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define ANCHO 10
#define ALTO 20

int tablero[ALTO][ANCHO] = {0};
int puntaje = 0;

int tetrominos[7][4][2] = {
    // I
    { {0,0}, {1,0}, {2,0}, {3,0} },
    // O
    { {0,0}, {1,0}, {0,1}, {1,1} },
    // T
    { {1,0}, {0,1}, {1,1}, {2,1} },
    // S
    { {1,0}, {2,0}, {0,1}, {1,1} },
    // Z
    { {0,0}, {1,0}, {1,1}, {2,1} },
    // J
    { {0,0}, {0,1}, {1,1}, {2,1} },
    // L
    { {2,0}, {0,1}, {1,1}, {2,1} },
};

int pieza_actual[4][2];
int tipo_pieza_actual;
int posicion_x_actual, posicion_y_actual;

void mover_cursor(int x, int y) {
    COORD coord = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void dibujar_tablero() {
    system("cls");
    printf("Puntaje: %d\n", puntaje);
    for (int y = 0; y < ALTO; y++) {
        for (int x = 0; x < ANCHO; x++) {
            int ocupado = tablero[y][x];
            for (int i = 0; i < 4; i++) {
                if (x == posicion_x_actual + pieza_actual[i][0] && y == posicion_y_actual + pieza_actual[i][1]) {
                    ocupado = 1;
                    break;
                }
            }
            if (ocupado) {
                printf("#");
            } else {
                printf(".");
            }
        }
        printf("\n");
    }
}

void nueva_pieza() {
    tipo_pieza_actual = rand() % 7;
    for (int i = 0; i < 4; i++) {
        pieza_actual[i][0] = tetrominos[tipo_pieza_actual][i][0];
        pieza_actual[i][1] = tetrominos[tipo_pieza_actual][i][1];
    }
    posicion_x_actual = ANCHO / 2 - 2;
    posicion_y_actual = 0;
}
int verificar_colision(int dx, int dy) {
    for (int i = 0; i < 4; i++) {
        int nuevo_x = posicion_x_actual + pieza_actual[i][0] + dx;
        int nuevo_y = posicion_y_actual + pieza_actual[i][1] + dy;
        if (nuevo_x < 0 || nuevo_x >= ANCHO || nuevo_y >= ALTO) {
            return 1;
        }
        if (nuevo_y >= 0 && tablero[nuevo_y][nuevo_x]) {
            return 1;
        }
    }
    return 0;
}

void colocar_pieza() {
    for (int i = 0; i < 4; i++) {
        int x = posicion_x_actual + pieza_actual[i][0];
        int y = posicion_y_actual + pieza_actual[i][1];
        if (y >= 0 && y < ALTO && x >= 0 && x < ANCHO) {
            tablero[y][x] = 1;
        }
    }
}

void eliminar_lineas() {
    for (int y = 0; y < ALTO; y++) {
        int completa = 1;
        for (int x = 0; x < ANCHO; x++) {
            if (!tablero[y][x]) {
                completa = 0;
                break;
            }
        }
        if (completa) {
            for (int yy = y; yy > 0; yy--) {
                for (int x = 0; x < ANCHO; x++) {
                    tablero[yy][x] = tablero[yy - 1][x];
                }
            }
            for (int x = 0; x < ANCHO; x++) {
                tablero[0][x] = 0;
            }
            puntaje += 100;
        }
    }
}
int main() {
    char entrada;
    DWORD tiempo_anterior = 0;
    int retraso = 500;
    srand(time(NULL));
    nueva_pieza();
    while (1) {
        dibujar_tablero();
        // Control del jugador
        if (_kbhit()) {
            entrada = _getch();
            if (entrada == 'a' && !verificar_colision(-1, 0)) posicion_x_actual--;
            if (entrada == 'd' && !verificar_colision(1, 0)) posicion_x_actual++;
            if (entrada == 's' && !verificar_colision(0, 1)) posicion_y_actual++;
            if (entrada == 'w') {
                // Rotar la pieza
                int pieza_temporal[4][2];
                for (int i = 0; i < 4; i++) {
                    pieza_temporal[i][0] = -pieza_actual[i][1];
                    pieza_temporal[i][1] = pieza_actual[i][0];
                }
                int colision = 0;
                for (int i = 0; i < 4; i++) {
                    int nuevo_x = posicion_x_actual + pieza_temporal[i][0];
                    int nuevo_y = posicion_y_actual + pieza_temporal[i][1];
                    if (nuevo_x < 0 || nuevo_x >= ANCHO || nuevo_y >= ALTO || (nuevo_y >= 0 && tablero[nuevo_y][nuevo_x])) {
                        colision = 1;
                        break;
                    }
                }
                if (!colision) {
                    for (int i = 0; i < 4; i++) {
                        pieza_actual[i][0] = pieza_temporal[i][0];
                        pieza_actual[i][1] = pieza_temporal[i][1];
                    }
                }
            }
            if (entrada == 'q') break;
        }
        DWORD tiempo_actual = GetTickCount();
        if (tiempo_actual - tiempo_anterior > retraso) {
            if (!verificar_colision(0, 1)) {
                posicion_y_actual++;
            } else {
                colocar_pieza();
                eliminar_lineas();
                nueva_pieza();
                if (verificar_colision(0, 0)) {
                    dibujar_tablero();
                    printf("Juego Terminado\n");
                    system("pause");
                    break;
                }
            }
            tiempo_anterior = tiempo_actual;
        }
        Sleep(10);
    }
    printf("Puntaje final: %d\n", puntaje);
    system("pause");
    return 0;
}
