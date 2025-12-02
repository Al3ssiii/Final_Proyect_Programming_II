#ifndef FINAL_PROYECT_PROGRAMMING_II_UX_H
#define FINAL_PROYECT_PROGRAMMING_II_UX_H

#include <string>
#include <vector>
#include <limits>

void LimpiarPantalla();
std::string center(const std::string &s, int width = 70);
void mostrarASCII();
int interfaz();

void loadingBarSalida();
void loadingBarEntrada();

void entrada();
void salida();

int runUXMenu();

#endif // FINAL_PROYECT_PROGRAMMING_II_UX_H
