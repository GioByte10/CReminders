#include <string.h>
#include <iostream>


//unPatchSpaces this function changes the "_SPC_" string to a space
// e.g. "Hello_SPC_World" -> "Hello World"
void unPatchSpaces(std::string &str){
    while(str.find("_SPC_") != std::string::npos){
        str.replace(str.find("_SPC_"), 5, " ");
    }
}


int main(int argc, char *argv[]) {

    if(argc == 3) {
        if(sizeof(argv[2]) == 1){

            std::string errorMessage[12];

            errorMessage[0] = "the info.txt file is missing a ':' in one of its rows";
            errorMessage[1] = "the time for one of the reminders is missing the ':' character";
            errorMessage[2] = "the time for one of the reminders is invalid";
            errorMessage[3] = "the info.txt file did not open";
            errorMessage[4] = "info.txt is incorrectly formatted. It has extra lines";
            errorMessage[5] = "the days for one of the reminders are either missing or wrong";

            errorMessage[6] = "al archivo info.txt le falta un ':' en una de sus filas";
            errorMessage[7] = "la hora de uno de los recordatorios no tiene el caracter ':'";
            errorMessage[8] = "la hora de uno de los recordatorios es invalida";
            errorMessage[9] = "el archivo info.txt no se pudo abrir";
            errorMessage[10] = "info.txt esta no configurado correctamente. Tiene lineas extras";
            errorMessage[11] = "los dias de uno de los recordatorios estan mal o faltan";

            std::string str = "Hello_SPC_World_SPC_this_SPC_is_SPC_a_SPC_test";
            unPatchSpaces(str);
            std::cout << str << std::endl;
        }else
            std::cout << "Error: invalid argument" << std::endl;
    }
    return 0;
}