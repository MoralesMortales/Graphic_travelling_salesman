#include "miniwin.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <limits>
#include <sstream>

#define winAltura 400
#define winAmplit 600

class problema {
private:
    int n;
    float coordenada[2][101];

public:
    problema() {}
    problema(int nn, float coordenadass[2][101]);
    int getn();
    float distancia(int i, int j);
    void showcoordenadas();
    float getx(int i) { return coordenada[0][i]; }
    float gety(int i) { return coordenada[1][i]; }
};

class tour {
private:
    problema p;
    int configuracion[2][101];
    int actual;

public:
    tour() {}
    tour(problema pp, int config[101]);

    void explorar();
    float costo(int actual);
    void avanzar();
    void showtour();
    bool valida();

    int getactual();
    int getciudad(int parada) { return configuracion[getactual()][parada]; }
};

using namespace miniwin;

int floaTopixel_x(float x1) {
    return (x1 * winAmplit) / 10;
}

int floaTopixel_y(float y1) {
    return (winAltura * (10 - y1) / 10);
}

int main() {
    srand(time(nullptr));

    float coord[2][101] = { {0, 1, 4, 2, 5, 6, 3, 8, 7, 6, 3, 4, 5, 2, 2, 5, 2, 4, 1, 5},
                            {0, 9, 8, 6, 7, 9, 4, 2, 5, 4, 3, 6, 1, 8, 5, 5, 2, 4, 1, 1} };

    int config[101] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};

    problema pp(20, coord);  // Ajustado a 20 para incluir todos los puntos
    tour tt(pp, config);

    float mejorCosto = std::numeric_limits<float>::max();
    int mejorConfiguracion[101];
    int sinMejoras = 0;
    const int maxSinMejoras = 1000;
    int iteraciones = 0;

    color(AZUL);
    vredimensiona(winAmplit, winAltura);

    while (sinMejoras < maxSinMejoras) {
        borra();
        for (int i = 1; i <= pp.getn(); i++) {
            circulo_lleno(floaTopixel_x(pp.getx(i)), floaTopixel_y(pp.gety(i)), 10);
        }
        for (int i = 1; i < pp.getn(); i++) {
            linea(floaTopixel_x(pp.getx(tt.getciudad(i))), floaTopixel_y(pp.gety(tt.getciudad(i))),
                  floaTopixel_x(pp.getx(tt.getciudad(i + 1))), floaTopixel_y(pp.gety(tt.getciudad(i + 1))));
        }
        linea(floaTopixel_x(pp.getx(tt.getciudad(pp.getn()))), floaTopixel_y(pp.gety(tt.getciudad(pp.getn()))),
              floaTopixel_x(pp.getx(tt.getciudad(1))), floaTopixel_y(pp.gety(tt.getciudad(1))));

        // Mostrar el contador de iteraciones
        std::ostringstream oss;
        oss << "Iteraciones: " << iteraciones;
        texto(winAmplit - 150, 20, oss.str());

        refresca();

        tt.explorar();
        float nuevoCosto = tt.costo(!tt.getactual());
        if (nuevoCosto < mejorCosto && tt.valida()) {
            mejorCosto = nuevoCosto;
            for (int i = 0; i <= pp.getn(); ++i) {
                mejorConfiguracion[i] = tt.getciudad(i);
            }
            tt.avanzar();
            sinMejoras = 0;
        } else {
            sinMejoras++;
        }

        iteraciones++;
        espera(5);
    }

    std::cout << "Mejor costo encontrado: " << mejorCosto << std::endl;
    std::cout << "Mejor configuración: ";
    for (int i = 0; i < pp.getn(); ++i) {
        std::cout << mejorConfiguracion[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}

problema::problema(int nn, float coordenadass[2][101]) {
    n = nn;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < nn; ++j) {
            coordenada[i][j] = coordenadass[i][j];
        }
    }
}

int problema::getn() {
    return n;
}

float problema::distancia(int i, int j) {
    return sqrt(pow(coordenada[0][i] - coordenada[0][j], 2) + pow(coordenada[1][i] - coordenada[1][j], 2));
}

void problema::showcoordenadas() {
    std::cout << "|x|";
    for (int i = 1; i < n; i++) {
        std::cout << coordenada[0][i] << "|";
    }
    std::cout << std::endl << "|y|";
    for (int i = 1; i < n; i++) {
        std::cout << coordenada[1][i] << "|";
    }
}

tour::tour(problema pp, int config[101]) : p(pp), actual(0) {
    for (int i = 0; i <= pp.getn(); ++i) {
        configuracion[0][i] = config[i];
    }
}

void tour::explorar() {
    int x = 1;
    int y = 1;
    int temp;

    while (x == y) {
        x = rand() % p.getn() + 1;
        y = rand() % p.getn() + 1;
    }

    for (int i = 1; i <= p.getn(); i++) {
        configuracion[!actual][i] = configuracion[actual][i];
    }

    temp = configuracion[!actual][x];
    configuracion[!actual][x] = configuracion[!actual][y];
    configuracion[!actual][y] = temp;
}

float tour::costo(int actual) {
    float total = 0;
    for (int i = 1; i < p.getn(); i++) {
        total += p.distancia(configuracion[actual][i], configuracion[actual][i + 1]);
    }
    total += p.distancia(configuracion[actual][p.getn()], configuracion[actual][1]);
    return total;
}

void tour::avanzar() {
    actual = !actual;
}

int tour::getactual() {
    return actual;
}

bool tour::valida() {
    std::vector<int> conexiones(p.getn(), 0);
    for (int i = 1; i < p.getn(); i++) {
        conexiones[configuracion[actual][i]]++;
        conexiones[configuracion[actual][i + 1]]++;
    }
    conexiones[configuracion[actual][p.getn()]]++;
    conexiones[configuracion[actual][1]]++;

    for (int i = 0; i < p.getn(); i++) {
        if (conexiones[i] > 2) {
            return false;
        }
    }
    return true;
}

void tour::showtour() {
    for (int i = 1; i <= p.getn(); i++) {
        std::cout << getciudad(i) << " ";
    }
    std::cout << std::endl;
}
