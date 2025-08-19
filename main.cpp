#ifdef _WIN32
#include <windows.h>
#endif
#include "tank.hpp"
#include <iostream>
#include <algorithm>

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif



    while (true) {
        runTankCalculation();

        std::string svar;
        std::cout << "\nVill du köra igen? (j/n): ";
        std::getline(std::cin, svar);
        std::transform(svar.begin(), svar.end(), svar.begin(), ::tolower);
        svar.erase(std::remove_if(svar.begin(), svar.end(), ::isspace), svar.end());

        if (svar != "j" && svar != "ja") break;
        std::cout << "\n🔁 Ny beräkning...\n\n";
    }

    pause();
    return 0;
}