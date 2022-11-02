#include <windows.h>
#include <iostream>
#include <list>
#include <fstream>
#include <thread>

HANDLE stopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

BOOL HandlerRoutine(DWORD fdwCtrlType){
    switch( fdwCtrlType ){
        case CTRL_LOGOFF_EVENT:
            SetEvent(stopEvent);
            Beep(700,1000);
            return TRUE;

        case CTRL_SHUTDOWN_EVENT:
            SetEvent(stopEvent);
            Beep(700,1000);
            return TRUE;

        default:
            return FALSE;
    }
}

bool checkAlreadyExists(LPCSTR value){

    HKEY checkKey;
    LONG result = RegGetValueA(HKEY_CURRENT_USER, R"(SOFTWARE\Microsoft\Windows\CurrentVersion\Run)", value, RRF_RT_REG_SZ, nullptr, nullptr, nullptr);
    RegCloseKey(checkKey);

    if(result == ERROR_SUCCESS)
        return true;

    return false;
}

void addToRegistry(LPCSTR value, TCHAR *filePath){

    HKEY newKey;

    RegOpenKey(HKEY_CURRENT_USER, R"(Software\Microsoft\Windows\CurrentVersion\Run)", &newKey);
    LONG result = RegSetValueEx(newKey, value, 0, REG_SZ, (LPBYTE)filePath, lstrlen(filePath));
    RegCloseKey(newKey);

    if(result != ERROR_SUCCESS){
        MessageBox(nullptr, "Could not add to startup", "CReminders Error 0x00", MB_ICONERROR);
        exit(1);
    }
}

void deactivate(LPCSTR value) {

    HKEY newKey;

    RegOpenKey(HKEY_CURRENT_USER, R"(Software\Microsoft\Windows\CurrentVersion\Run)", &newKey);
    LONG result = RegDeleteValue(newKey, value);
    RegCloseKey(newKey);

    if (result != ERROR_SUCCESS) {
        MessageBox(nullptr, "Could not delete from startup", "CReminders Error 0x01", MB_ICONERROR);
        exit(1);
    }

    system("taskkill /f /im CReminders.exe");
    MessageBox(nullptr, "CReminders has successfully been deactivated\nSe ha desactivado CReminders exitosamente",
               "CReminders", MB_ICONINFORMATION);
}

std::string intDayToStringDayEn(int intDay){

    if(intDay == 1)
        return "monday";

    else if(intDay == 2)
        return "tuesday";

    else if(intDay == 3)
        return "wednesday";

    else if(intDay == 4)
        return "thursday";

    else if(intDay == 5)
        return "friday";

    else if(intDay == 6)
        return "saturday";

    else if(intDay == 0)
        return "sunday";

    return "";
}

int getCurrentTime(const std::string &arg, time_t now, tm *ltm){

    now = time(nullptr);
    localtime(&now);

    if(arg == "sec")
        return ltm -> tm_sec;

    else if(arg == "min")
        return ltm -> tm_min;

    else if(arg == "hour")
        return ltm -> tm_hour;

    else if(arg == "day")
        return ltm -> tm_mday;

    else if(arg == "wday")
        return ltm -> tm_wday;

    else if(arg == "month")
        return ltm -> tm_mon;

    else if(arg == "year")
        return ltm -> tm_year;

    return -1;
}

bool isToday(int i, time_t now, tm *ltm, std::list<std::string> *days_list){

    auto days_i = days_list -> begin();
    advance(days_i, i);

    const std::string days = *days_i;

    if(days == "everyday" || days.find(intDayToStringDayEn(getCurrentTime("wday", now, ltm))) != std::string::npos)
        return true;

    return false;
}

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

std::string patchSpaces(const std::string &line, const std::string &r){

    std::string newLine;

    for(char c : line){
        if(c == ' ')
            newLine += r;
        else
            newLine += c;
    }

    return newLine;
}

bool findChars(const std::string &line, const std::string &chars){

    for(char c : chars)
        if (line.find(c) != std::string::npos)
            return true;

    return false;
}

void clearLists(std::list <std::string> *notificationContent_list, std::list <std::string> *days_list, std::list <int> *hour_list, std::list <int> *minute_list){

    notificationContent_list->clear();
    days_list->clear();
    hour_list->clear();
    minute_list->clear();

}

std::string getDays(std::string daysLine){

    std::string daysL = "";
    transform(daysLine.begin(), daysLine.end(), daysLine.begin(), ::tolower);

    if(daysLine.find("everyday") != std::string::npos || daysLine.find("diario") != std::string::npos)
        daysL = "everyday";

    else{
        if (daysLine.find("lunes") != std::string::npos || daysLine.find("monday") != std::string::npos)
            daysL += "monday ";

        if (daysLine.find("martes") != std::string::npos || daysLine.find("tuesday") != std::string::npos)
            daysL += "tuesday ";

        if (daysLine.find("miercoles") != std::string::npos || daysLine.find("wednesday") != std::string::npos)
            daysL += "wednesday ";

        if (daysLine.find("jueves") != std::string::npos || daysLine.find("thursday") != std::string::npos)
            daysL += "thursday ";

        if (daysLine.find("viernes") != std::string::npos || daysLine.find("friday") != std::string::npos)
            daysL += "friday ";

        if (daysLine.find("sabado") != std::string::npos || daysLine.find("saturday") != std::string::npos)
            daysL += "saturday ";

        if (daysLine.find("domingo") != std::string::npos || daysLine.find("sunday") != std::string::npos)
            daysL += "sunday ";
    }

    return daysL;
}

std::string getDuration(std::string durationLine){

    std::string durationL;

    transform(durationLine.begin(), durationLine.end(), durationLine.begin(), ::tolower);

    if(durationL.find("short") != std::string::npos || durationL.find("corto") != std::string::npos || durationL.find("corta") != std::string::npos)
        durationL = "short";

    else
        durationL = "long";

    return durationL;

}

std::string getLineInformation(const std::string &line){

    std::string newLine;
    int i = line.find(':');

    if(i == std::string::npos){
        MessageBox(nullptr, "info.txt is incorrectly formatted", "CReminders Error 0x02", MB_ICONERROR);
        exit(1);
    }

    i++;

    for(; i < line.length(); i++)
        if(line[i] != ' ')
            break;

    newLine = patchSpaces(line.substr(i, line.length() - 1), "_SPC_");

    if(empty(newLine))
        return "_NULL~_";

    return newLine;
}

void getTimeInformation(const std::string &timeString, std::list <int> *hour_list, std::list <int> *minute_list){

    const int i = timeString.find(':');

    if(i == std::string::npos){
        MessageBox(nullptr, "info.txt is incorrectly formatted", "CReminders Error 0x03", MB_ICONERROR);
        exit(1);
    }

    try {
        hour_list->emplace_back(stoi(timeString.substr(0, i)));
        minute_list->emplace_back(stoi(timeString.substr(i + 1, timeString.length())));

    } catch (std::invalid_argument &e){
        MessageBox(nullptr, "info.txt is incorrectly formatted", "CReminders Error 0x04", MB_ICONERROR);
        exit(1);
    }

}

void getInformation(const std::string &infoPath, std::list<std::string> *notificationContent_list, std::list<std::string> *days_list, std::list <int> *hour_list, std::list <int> *minute_list, bool demo){

    std::ifstream info;
    std::string line;

    int i, nBlocks;
    const int linesPerBlock = 10;

    clearLists(notificationContent_list, days_list, hour_list, minute_list);

    info.open(infoPath);
    info.clear();
    info.seekg(0);

    if(info.fail()){
        info.close();
        MessageBox(nullptr, "info.txt did not open", "CReminders Error 0x05", MB_ICONERROR);
        exit(1);
    }

    for(i = 0; getline(info, line); i++);

    info.clear();
    info.seekg(0);

    i--;

    if(i % (linesPerBlock + 1) != 0){
        MessageBox(nullptr, "info.txt is not correctly formatted", "CReminders Error 0x06", MB_ICONERROR);
        exit(1);
    }

    nBlocks = i/(linesPerBlock + 1);
    std::string lines[nBlocks * linesPerBlock];

    for(int j = 0; j < nBlocks * linesPerBlock; j++){
        getline(info, line);

        if(line.find("----------") != std::string::npos)
            j--;

        else
            lines[j] = getLineInformation(line);
    }

    info.close();

    for(int  j = 0; j < nBlocks; j++){
        line = "";

        if(findChars(lines[j * linesPerBlock], "yYsS")){
            line += lines[1 + j * linesPerBlock] + ' ' + lines[2 + j * linesPerBlock] + ' ' + lines[3 + j * linesPerBlock];

            if (!demo)
                line += ' ' + getDuration(lines[4 + j * linesPerBlock]);
            else
                line += " short";

            if(findChars(lines[5 + j * linesPerBlock], "yYsS"))
                line += ' ' + lines[6 + j * linesPerBlock] + ' ' + lines[7 + j * linesPerBlock];
            else
                line += " _NULL~_ _NULL~_";

            notificationContent_list->emplace_back(line);
            getTimeInformation(lines[8 + j * linesPerBlock], hour_list, minute_list);

            if(getDays(lines[9 + j * linesPerBlock]).length())
                days_list->emplace_back(getDays(lines[9 + j * linesPerBlock]));
            else {
                std::cout << getDays(lines[9 + j * linesPerBlock]) << std::endl;
                MessageBox(nullptr, "info.txt is incorrectly formatted", "CReminders Error 0x07", MB_ICONERROR);
                exit(1);
            }
        }
    }
}

void notifyLastWritten(LPCSTR directoryPath, const std::string &infoPath, std::list<std::string> *notificationContent_list, std::list<std::string> *days_list, std::list <int> *hour_list, std::list <int> *minute_list){

    HANDLE handles[2];

    while(WaitForSingleObject(stopEvent, 0) == WAIT_TIMEOUT){

        handles[0] = FindFirstChangeNotification(directoryPath, FALSE, FILE_NOTIFY_CHANGE_LAST_WRITE);
        handles[1] = stopEvent;

        DWORD wait = WaitForMultipleObjects(2, handles, FALSE, INFINITE);
        if (wait == WAIT_OBJECT_0){
            getInformation(infoPath, notificationContent_list, days_list, hour_list, minute_list, false);
        }
    }
}

void showReminders(std::list <std::string> *notificationContent_list, const std::string &notificationPath){

    auto notificationContent_i = notificationContent_list->begin();
    std::string notificationContent;

    for (int i = 0; i < notificationContent_list->size(); i++) {
        notificationContent = *notificationContent_i;
        ShellExecuteA(nullptr, "open", notificationPath.c_str(), notificationContent.c_str(), nullptr, 0);
        Sleep(5200);

        std::advance(notificationContent_i, 1);
    }
}

void reset_txt(const std::string &infoPath, std::list <std::string> *notificationContent_list, std::list <std::string> *days_list, std::list <int> *hour_list, std::list <int> *minute_list, bool english){

    clearLists(notificationContent_list, days_list, hour_list, minute_list);

    std::ofstream newFile;
    newFile.open(infoPath, std::ios::out | std::ios::trunc);
    newFile.close();

    newFile.open(infoPath);

    if(english) {
        newFile << "-----------------------------------------------------------------------------------------------------------" << std::endl;
        newFile << "Active:         " << std::endl;
        newFile << "Title:          " << std::endl;
        newFile << "Message:        " << std::endl;
        newFile << "Icon:           " << std::endl;
        newFile << "Duration:       " << std::endl;
        newFile << "Button:         " << std::endl;
        newFile << "ButtonText:     " << std::endl;
        newFile << "ButtonLaunch:   " << std::endl;
        newFile << "Time:           " << std::endl;
        newFile << "Days:           " << std::endl;
        newFile << "-----------------------------------------------------------------------------------------------------------" << std::endl;
    }

    else{
        newFile << "-----------------------------------------------------------------------------------------------------------" << std::endl;
        newFile << "Activo:         " << std::endl;
        newFile << "Titulo:         " << std::endl;
        newFile << "Mensaje:        " << std::endl;
        newFile << "Icono:          " << std::endl;
        newFile << "Duracion:       " << std::endl;
        newFile << "Boton:          " << std::endl;
        newFile << "BotonTexto:     " << std::endl;
        newFile << "BotonEjecutar:  " << std::endl;
        newFile << "Hora:           " << std::endl;
        newFile << "Dias:           " << std::endl;
        newFile << "-----------------------------------------------------------------------------------------------------------" << std::endl;
    }
    newFile.close();
}

int main(int argc, char *argv[]){

    const LPCSTR value = "CReminders";
    std::string directoryPath, infoPath, notificationPath;

    std::list <std::string>     notificationContent_list;
    std::list <std::string>     days_list;
    std::list <int>             hour_list;
    std::list <int>             minute_list;

    std::string                 notificationContent;
    int                         hour;
    int                         minute;

    TCHAR filePath[MAX_PATH];
    GetModuleFileName(nullptr, filePath, MAX_PATH);

    if(!checkAlreadyExists(value))
        addToRegistry(value, filePath);

    time_t now = time(nullptr);
    tm *ltm = localtime(&now);

    directoryPath = getDirectoryPath(filePath, 1);
    infoPath = directoryPath + "\\\\info.txt";
    notificationPath = directoryPath + R"(\ToastNotification\dist\toastNotification.exe)";

    getInformation(infoPath, &notificationContent_list, &days_list, &hour_list, &minute_list, false);


    for(const auto& l: notificationContent_list)
        std::cout << l.c_str() << std::endl;
    /*
    for(const auto& l: hour_list)
        std::cout << l << std::endl;

    for(const auto& l: minute_list)
        std::cout << l << std::endl;

    for(const auto& l: days_list)
        std::cout << l << std::endl;
    */

    if(argc == 1) {
        SetConsoleCtrlHandler((PHANDLER_ROUTINE)HandlerRoutine, TRUE );
        std::thread threadLastWritten(notifyLastWritten, directoryPath.c_str(), infoPath, &notificationContent_list, &days_list, &hour_list, &minute_list);

        while (true) {

            auto notificationContent_i = notificationContent_list.begin();
            auto hour_i = hour_list.begin();
            auto minute_i = minute_list.begin();

            for (int i = 0; i < notificationContent_list.size(); i++) {
                notificationContent = *notificationContent_i;
                hour = *hour_i;
                minute = *minute_i;

                if (minute == getCurrentTime("min", now, ltm) && hour == getCurrentTime("hour", now, ltm) &&
                    isToday(i, now, ltm, &days_list)) {
                    ShellExecuteA(nullptr, "open", notificationPath.c_str(), notificationContent.c_str(), nullptr, 0);
                    Sleep(20000);
                }

                std::advance(notificationContent_i, 1);
                std::advance(hour_i, 1);
                std::advance(minute_i, 1);
            }
            Sleep(50000);
        }
    }

    else if(argv[1] == std::string("show") || argv[1] == std::string("mostrar")){
        getInformation(infoPath, &notificationContent_list, &days_list, &hour_list, &minute_list, true);
        showReminders(&notificationContent_list, notificationPath);
    }

    else if(argv[1] == std::string("deactivate") || argv[1] == std::string("desactivar"))
        deactivate(value);

    else if(argv[1] == std::string("reset_en"))
        reset_txt(infoPath, &notificationContent_list, &days_list, &hour_list, &minute_list, true);

    else if(argv[1] == std::string("reset_es"))
        reset_txt(infoPath, &notificationContent_list, &days_list, &hour_list, &minute_list, false);

    return 0;
}