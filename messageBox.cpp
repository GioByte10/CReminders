#include <string.h>
#include <iostream>
#include <windows.h>


//unPatchSpaces this function changes the "_SPC_" string to a space
// e.g. "Hello_SPC_World" -> "Hello World"
std::string unPatchSpaces(std::string newStr){

    while(newStr.find("_SPC_") != std::string::npos) {
        newStr.replace(newStr.find("_SPC_"), 5, " ");
    }

    return newStr;
}


int main(int argc, char *argv[]) {

    if(argc == 3) {
        std::string title = unPatchSpaces(argv[1]);

        if (strlen(argv[2]) <= 3 ) {

            std::string helpQ[2];
            helpQ[0] = " Do you need help with this error?";
            helpQ[1] = " Necesita ayuda con este error?";

            std::string errorMessage[12];
            errorMessage[0] = "The info.txt file is missing a ':' in one of its rows.";
            errorMessage[1] = "The time for one of the reminders is missing the ':' character.";
            errorMessage[2] = "The time for one of the reminders is invalid.";
            errorMessage[3] = "The info.txt file did not open.";
            errorMessage[4] = "info.txt is incorrectly formatted. It has extra or missing lines.";
            errorMessage[5] = "The days for one of the reminders are either missing or wrong.";

            for (int i = 0; i < 6; i++)
                errorMessage[i] += helpQ[0];

            errorMessage[6] = "Al archivo info.txt le falta un ':' en una de sus filas.";
            errorMessage[7] = "La hora de una de las notificaciones no tiene el caracter ':'.";
            errorMessage[8] = "La hora de una de las notificaciones es invalida.";
            errorMessage[9] = "El archivo info.txt no se pudo abrir.";
            errorMessage[10] = "info.txt esta no configurado correctamente. Tiene lineas de mas o de menos.";
            errorMessage[11] = "Los dias de una de las notificaciones estan mal o faltan.";

            for (int i = 6; i < 12; i++)
                errorMessage[i] += helpQ[1];

            int n = atoi(argv[2]);
            int ans = MessageBoxA(nullptr, errorMessage[n].c_str(), title.c_str(), MB_YESNO | MB_ICONERROR);

            if(ans == IDYES)
                ShellExecuteA(nullptr, "open", "https://github.com/GioByte10/CReminders/wiki/Help", nullptr, nullptr, SW_SHOWMAXIMIZED);

        } else {
            std::string text = unPatchSpaces(argv[2]);
            MessageBoxA(nullptr, text.c_str(), title.c_str(), MB_OK | MB_ICONINFORMATION);
        }
    }
    return 0;
}