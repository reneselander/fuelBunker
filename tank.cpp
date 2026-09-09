#include "tank.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <windows.h>

double readDouble(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        std::string line;
        if (!std::getline(std::cin, line)) continue;
        for (auto &c : line) if (c == ',') c = '.';
        if (std::count(line.begin(), line.end(), '.') > 1) {
            std::cerr << "För många decimalpunkter. Försök igen.\n";
            continue;
        }
        std::stringstream ss(line);
        double v;
        if (ss >> v) return v;
        std::cerr << "Ogiltigt tal. Försök igen.\n";
    }
}

double readDoubleCm(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        std::string line;
        if (!std::getline(std::cin, line)) continue;
        for (auto &c : line) if (c == ',') c = '.';
        if (std::count(line.begin(), line.end(), '.') > 1) {
            std::cerr << "För många decimalpunkter. Försök igen.\n";
            continue;
        }
        auto dotPos = line.find('.');
        if (dotPos != std::string::npos && (line.size() - dotPos - 1) > 2) {
            std::cerr << "Max två decimaler i cm. Försök igen.\n";
            continue;
        }
        std::stringstream ss(line);
        double v_cm;
        if (ss >> v_cm) return v_cm / 100.0;
        std::cerr << "Ogiltigt tal. Försök igen.\n";
    }
}

void pause() {
    std::cout << "Tryck Enter för att avsluta...";
    std::cin.get();
}

void runTankCalculation() {
    const double wall = 0.004;
    double outerL = readDouble("Ange tankens yttre längd  (m): ");
    double outerW = readDouble("Ange tankens yttre bredd (m): ");    
    double outerH = readDouble("Ange tankens yttre höjd   (m): ");
    double distTop = readDoubleCm("Avstånd från toppen till vätskeytan (cm, ex 12.34): ");

    double innerW = outerW - 2 * wall;
    double innerL = outerL - 2 * wall;
    double innerH = outerH - wall;
    double level  = innerH - distTop;

    double fuel_m3  = innerW * innerL * level;
    double total_m3 = innerW * innerL * innerH;
    double fuel_L   = fuel_m3 * 1000.0;

    double pct = std::round((fuel_m3 / total_m3) * 1000.0) / 10.0;

    std::cout << std::fixed << std::setprecision(3)
              << "\nTankens inre dimensioner:\n"
              << "  Längd:  " << innerL << " m\n"
              << "  Bredd: " << innerW << " m\n"
              << "  Höjd:  " << innerH << " m\n";

    std::cout << std::fixed << std::setprecision(2)
              << "\nBränslenivå: " << level << " m\n"
              << "Volym:       " << fuel_m3 << " m³ (" << fuel_L << " L)\n";

    std::cout << "Fyllnadsgrad: " << std::fixed << std::setprecision(1) << pct << "%\n\n";

    const int BAR_W = 30;
    int filled = int(std::round(pct / 100.0 * BAR_W));

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Färgkodning
    if (pct < 20.0)
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY); // Röd
    else if (pct < 50.0)
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); // Gul
    else
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY); // Grön

    std::cout << "Tankstatus: [";
    for (int i = 0; i < BAR_W; ++i)
        std::cout << (i < filled ? '#' : ' ');
    std::cout << "] " << std::fixed << std::setprecision(1) << pct << "%\n\n";

    // Återställ färg
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    if (pct < 20.0) std::cout << "⚠️ Varning: låg bränslenivå!\n";
    if (pct < 10.0) {
        double target_m3 = total_m3 * 0.99;
        double need_L = (target_m3 - fuel_m3) * 1000.0;
        int order100 = int(std::round(need_L / 100.0) * 100);
        std::cout << "📦 Beställ minst " << order100 << " L för att nå 99% fyllnad.\n";
    } else if (pct < 20.0) {
        std::cout << "📦 Planera bränslebeställning snart.\n";
    } else {
        std::cout << "📦 Ingen beställning behövs just nu.\n";
    }
}