#include "ux.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <random>
#include <string>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <limits>
#include <algorithm>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::this_thread::sleep_for;

const int WIDTH = 80;

void LimpiarPantalla()
{
#if defined(_WIN32)
    system("cls");
#else
    system("clear");
#endif
}

string center(const string &s, int width)
{
    int len = s.length();
    if (len >= width)
        return s;

    int left = (width - len) / 2;
    int right = width - len - left;

    return string(left, ' ') + s + string(right, ' ');
}

void mostrarASCII()
{
    const char *G = "\033[38;5;46m";
    const char *R = "\033[0m";

    cout << G << center("██████  ██ ██    ██  █████  ██         ") << R << "\n";
    cout << G << center("██   ██ ██ ██    ██ ██   ██ ██       ") << R << "\n";
    cout << G << center("██████  ██ ██    ██ ███████ ██       ") << R << "\n";
    cout << G << center("██   ██ ██  ██  ██  ██   ██ ██        ") << R << "\n";
    cout << G << center("██   ██ ██   ████   ██   ██ ███████      ") << R << "\n\n";

    cout << G << center("███████ ██████   ██████  ███    ██ ████████ ██ ███████ ██████  ███████ ") << R << "\n";
    cout << G << center("██      ██   ██ ██    ██ ████   ██    ██    ██ ██      ██   ██ ██      ") << R << "\n";
    cout << G << center("█████   ██████  ██    ██ ██ ██  ██    ██    ██ █████   ██████  ███████ ") << R << "\n";
    cout << G << center("██      ██   ██ ██    ██ ██  ██ ██    ██    ██ ██      ██   ██      ██ ") << R << "\n";
    cout << G << center("██      ██   ██  ██████  ██   ████    ██    ██ ███████ ██   ██ ███████ ") << R << "\n\n";
}

int interfaz()
{
    int elec = 0;

    do
    {
        LimpiarPantalla();
        mostrarASCII();

        cout << "\033[35m";
        cout << center("============================================") << "\n";
        cout << center("                 MENU") << "\n";
        cout << center("============================================") << "\n\n";

        cout << "\033[0m";

        cout << center("[1] Jugar") << "\n";
        cout << center("[2] Bromitas?") << "\n";
        cout << center("[3] Salir") << "\n\n";

        cout << center("⚠ Seleccione una opción: ");

        if (!(cin >> elec))
        {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            elec = 0;
        }

    } while (elec < 1 || elec > 3);

    return elec;
}

template <typename T>

string mensajeSeguro(const vector<T> &v, int progreso)
{
    int idx = std::min((int)(progreso / 10), (int)v.size() - 1);
    return v[idx];
}

void loadingBar(const vector<string> &mensajes)
{
    int progreso = 0;

    while (progreso <= 100)
    {
        LimpiarPantalla();
        mostrarASCII();

        cout << "\n"
             << center(mensajeSeguro(mensajes, progreso)) << "\n\n";

        string bar = "[";
        for (int i = 0; i < 49; i++)
        {
            if (i < progreso / 2)
                bar += "\033[32m#\033[0m";
            else
                bar += " ";
        }
        bar += "]";

        cout << center(bar) << "\n";
        cout << center(std::to_string(progreso) + "%") << "\n";

        sleep_for(milliseconds(300 + (rand() % 200)));

        progreso += 5;
    }

    LimpiarPantalla();
}

void loadingBarSalida()
{
    vector<string> mensajes{
        "Guardando tus avances... (si es que hiciste alguno)",
        "Desconectando la IA principal... ¡AY NO, ME ESTOY APAG-",
        "Cerrando procesos ocultos... (no preguntes cuales)",
        "Liberando memoria RAM... (esa que te falto todo el juego)",
        "Apagando servidores... adios a los trabajadores explotados",
        "Desactivando modo gamer... vuelve a ser civil ahora",
        "Calculando si realmente te quieres ir... confirmado",
        "Cerrando shaders... estaban ocupados haciendo nada",
        "Desinstalando bugs del sistema... (no se puede)",
        "Cerrando sesion... ojala no vuelvas por un crash"};

    loadingBar(mensajes);
}

void loadingBarEntrada()
{
    vector<string> mensajes{
        "Despertando al servidor...",
        "Cargando texturas HD...",
        "Compilando shaders...",
        "Buscando bugs...",
        "Optimizando el juego...",
        "Aumentando FPS artificialmente...",
        "Poniendo musica epica...",
        "Cargando fisicas realistas...",
        "Cargando UI...",
        "A punto de terminar..."};

    loadingBar(mensajes);
}

void entrada()
{
    LimpiarPantalla();
    loadingBarEntrada();
}

void salida()
{
    LimpiarPantalla();
    cout << "\n\n";
    cout << "\033[1;32m" << center("Gracias por jugar.") << "\033[0m\n\n";
    sleep_for(milliseconds(150));
    loadingBarSalida();
}

int runUXMenu()
{
    int elec = interfaz();

    switch (elec)
    {
    case 1:
        entrada();
        break;

    case 2:
        break;

    case 3:
        salida();
        break;
    }

    return elec;
}
