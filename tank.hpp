#pragma once
#include <string>

// Funktioner för inmatning
double readDouble(const std::string& prompt);
double readDoubleCm(const std::string& prompt);

// Funktion för att pausa
void pause();

// Funktion för att beräkna och visa tankstatus
void runTankCalculation();