#include <iostream>
#include <string>
#include <cstdio>
#include <windows.h>

std::string getDirectoryPath(TCHAR *filePath, int steps){

    int i = lstrlen(filePath);
    std::string directoryPath;

    for(int step = 0; step < steps; step++) {
        for (; i >= 0; i--)
            if (filePath[i] == '\\')
                break;
        i--;
    }

    i++;

    for(int j = 0; j < i; j++){
        directoryPath += filePath[j];

        if(filePath[j] == '\\')
            directoryPath += '\\';
    }

    return directoryPath;
}

void patchSpaces(std::string &str){
    for(int i = 0; i < str.length(); i++)
        if(str[i] == ' ')
            str[i] = '_';
}

int main(){

    TCHAR filePath[MAX_PATH];
    GetModuleFileName(nullptr, filePath, MAX_PATH);

    std::string valid[] = {"show", "mostrar", "deactivate", "desactivar", "reset_en", "reset_es"};

    std::string executablePath = getDirectoryPath(filePath, 1) + "\\\\CReminders.exe";
    std::string input;

    printf("Enter one of the options:\n\n");
    printf("show\n");
    printf("Quickly gives a preview of all reminders\n\n");
    printf("deactivate\n");
    printf("Deactivates the program. To reactivate it you must run CReminders.exe again\n\n");
    printf("reset en\n");
    printf("Resets the info.txt file to its original state, in english. Note that this will delete all the reminders that you created\n\n\n");

    printf("Introduzca una de las opciones:\n\n");
    printf("mostrar\n");
    printf("Muestra rapidamente como se verian los recordatorios\n\n");
    printf("desactivar\n");
    printf("Desactiva el programa. Para volver a activarlo se debe ejecutar CReminders.exe de nuevo\n\n");
    printf("reset es\n");
    printf("Resetea el archivo info.txt a su estado inicial, en espanol. Note que esto borrara todos los recordatorios que haya creado\n\n");

    std::getline(std::cin, input);
    patchSpaces(input);

    while(!(input == "exit" || input == "salir")) {

        if(std::none_of(std::begin(valid), std::end(valid), [input](std::string str){return input == str;}))
            printf("Invalid input\n\n");

        else
            ShellExecuteA(nullptr, "open", executablePath.c_str(), input.c_str(), nullptr, SW_SHOW);


        std::getline(std::cin, input);
        patchSpaces(input);

    }

    return 0;
}