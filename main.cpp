/*  https://github.com/GioByte10/CReminders */

#include <windows.h>
#include <iostream>
#include <list>
#include <fstream>
#include <thread>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

static HANDLE stopEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
static int lang = 0;        // 0 = English, 1 = Spanish

//  arg hierarchy/order: value, directoryPath, filePath, infoPath, notificationPath, messageBoxPath, notificationContent_list, days_list, hour_list, minute_list, everythingElse

BOOL HandlerRoutine(DWORD fdwCtrlType) {
    switch( fdwCtrlType )
    {
        case CTRL_LOGOFF_EVENT:
            SetEvent(stopEvent);
            return TRUE;

        case CTRL_SHUTDOWN_EVENT:
            SetEvent(stopEvent);
            return TRUE;

        default:
            return FALSE;
    }
}

bool checkAlreadyExists(const LPCSTR value){

    HKEY checkKey;
    LONG result = RegGetValueA(HKEY_CURRENT_USER, R"(SOFTWARE\Microsoft\Windows\CurrentVersion\Run)", value, RRF_RT_REG_SZ, nullptr, nullptr, nullptr);
    RegCloseKey(checkKey);

    if(result == ERROR_SUCCESS)
        return true;

    return false;
}

void addToRegistry(const LPCSTR value, const TCHAR filePath[]){

    HKEY newKey;

    RegOpenKey(HKEY_CURRENT_USER, R"(Software\Microsoft\Windows\CurrentVersion\Run)", &newKey);
    LONG result = RegSetValueEx(newKey, value, 0, REG_SZ, (LPBYTE)filePath, lstrlen(filePath));
    RegCloseKey(newKey);

    if(result != ERROR_SUCCESS){
        SetEvent(stopEvent);
        ShellExecuteA(nullptr, "open", "https://github.com/GioByte10/CReminders/wiki", nullptr, nullptr, SW_SHOWMAXIMIZED);
        MessageBox(nullptr, "Could not add to startup", "CReminders Error 0x00", MB_ICONERROR);
        exit(1);
    }
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

void myMessageBox(const std::string &messageBoxPath, std::string title, std::string text){

    title = patchSpaces(title, "_SPC_");
    text = patchSpaces(text, "_SPC_");

    std::string content = title + ' ' + text;

    ShellExecuteA(nullptr, "open", messageBoxPath.c_str(), content.c_str(), nullptr, SW_SHOW);

}

void deactivate(const LPCSTR value, const std::string &messageBoxPath) {

    HKEY newKey;

    RegOpenKey(HKEY_CURRENT_USER, R"(Software\Microsoft\Windows\CurrentVersion\Run)", &newKey);
    LONG result = RegDeleteValue(newKey, value);
    RegCloseKey(newKey);

    if (result != ERROR_SUCCESS) {
        SetEvent(stopEvent);
        ShellExecuteA(nullptr, "open", "https://github.com/GioByte10/CReminders/wiki", nullptr, nullptr, SW_SHOWMAXIMIZED);
        myMessageBox(messageBoxPath, "CReminders Error 0x01", "Could not remove from startup");
        exit(1);
    }

    if(!lang)
        myMessageBox(messageBoxPath, "CReminders", "CReminders has been successfully deactivated");
    else
        myMessageBox(messageBoxPath, "CReminders", "CReminders se ha desactivado correctamente");

    system("taskkill /f /im CReminders.exe");
    exit(1);
}

void errorExit(const TCHAR filePath[], const std::string &messageBoxPath, std::string errorTitle, std::string errorText){

    myMessageBox(messageBoxPath, errorTitle, errorText);
    SetEvent(stopEvent);
    Sleep(2000);
    ShellExecuteA(nullptr, "open", filePath, "error", nullptr, SW_SHOW);
    exit(1);
}

std::string intDayToStringDayEn(const int intDay){

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

bool isToday(const std::list<std::string> &days_list, const int i, time_t now, tm *ltm){

    auto days_i = days_list.begin();
    advance(days_i, i);

    const std::string days = *days_i;

    if(days == "everyday" || days.find(intDayToStringDayEn(getCurrentTime("wday", now, ltm))) != std::string::npos)
        return true;

    return false;
}

std::string getDirectoryPath(const TCHAR filePath[], const int steps){

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

bool findChars(const std::string &line, const std::string &chars){

    for(char c : chars)
        if (line.find(c) != std::string::npos)
            return true;

    return false;
}

void clearLists(std::list <std::string> &notificationContent_list, std::list <std::string> &days_list, std::list <int> &hour_list, std::list <int> &minute_list){

    notificationContent_list.clear();
    days_list.clear();
    hour_list.clear();
    minute_list.clear();

}

std::string getDays(std::string &daysLine){

    std::string daysL;
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

std::string getDuration(std::string &durationLine){

    std::string durationL;

    transform(durationLine.begin(), durationLine.end(), durationLine.begin(), ::tolower);

    if(durationL.find("short") != std::string::npos || durationL.find("corto") != std::string::npos || durationL.find("corta") != std::string::npos)
        durationL = "short";

    else
        durationL = "long";

    return durationL;

}

std::string getLineInformation(const TCHAR filePath[], const std::string &messageBoxPath, const std::string &line){

    std::string newLine;
    int i = (int) line.find(':');

    if(i == std::string::npos)
        errorExit(filePath, messageBoxPath, "CReminders Error 0x02", std::to_string((2 - 2) + 6 * lang));

    i++;

    for(; i < line.length(); i++)
        if(line[i] != ' ')
            break;

    newLine = patchSpaces(line.substr(i, line.length() - 1), "_SPC_");

    if(empty(newLine))
        return "_NULL~_";

    return newLine;
}

void getTimeInformation(const TCHAR filePath[], const std::string messageBoxPath, std::list <int> &hour_list, std::list <int> &minute_list, const std::string &timeString){

    const int i = (int) timeString.find(':');

    if(i == std::string::npos)
        errorExit(filePath, messageBoxPath, "CReminders Error 0x03", std::to_string((3 - 2) + 6 * lang));

    try {
        hour_list.emplace_back(stoi(timeString.substr(0, i)));
        minute_list.emplace_back(stoi(timeString.substr(i + 1, timeString.length())));

    } catch (std::invalid_argument &e) {
        errorExit(filePath, messageBoxPath, "CReminders Error 0x04", std::to_string((4 - 2) + 6 * lang));
    }
}

void getInformation(const TCHAR filePath[], const std::string &infoPath, const std::string &messageBoxPath, std::list<std::string> &notificationContent_list, std::list<std::string> &days_list, std::list <int> &hour_list, std::list <int> &minute_list, bool demo){

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
        errorExit(filePath, messageBoxPath, "CReminders Error 0x05", std::to_string((5 - 2) + 6 * lang));
    }

    while(getline(info, line)) {
        if (line.find("-------") != std::string::npos) {
            getline(info, line);

            if (line.find("Activo") != std::string::npos)
                lang = 1;

            break;
        }
    }

    info.clear();
    info.seekg(0);

    for(i = 0; getline(info, line);){
        if(line.length() > 0)
            i++;
    }

    info.clear();
    info.seekg(0);

    i--;

    if(i % (linesPerBlock + 1) != 0) {
        info.close();
        errorExit(filePath, messageBoxPath, "CReminders Error 0x06", std::to_string((6 - 2) + 6 * lang));
    }

    nBlocks = i/(linesPerBlock + 1);
    std::string lines[nBlocks * linesPerBlock];

    for(int j = 0; j < nBlocks * linesPerBlock; j++){
        getline(info, line);

        if(line.find("----------") != std::string::npos)
            j--;

        else
            lines[j] = getLineInformation(filePath, messageBoxPath, line);
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

            notificationContent_list.emplace_back(line);
            getTimeInformation(filePath, messageBoxPath, hour_list, minute_list, lines[8 + j * linesPerBlock]);

            if(getDays(lines[9 + j * linesPerBlock]).length())
                days_list.emplace_back(getDays(lines[9 + j * linesPerBlock]));

            else
                errorExit(filePath, messageBoxPath, "CReminders Error 0x07", std::to_string((7 - 2) + 6 * lang));
        }
    }

}

void notifyLastWritten(const LPCSTR directoryPath, const TCHAR filePath[], const std::string &infoPath, const std::string &messageBoxPath, std::list<std::string> &notificationContent_list, std::list<std::string> &days_list, std::list <int> &hour_list, std::list <int> &minute_list){

    HANDLE handles[2];

    while(WaitForSingleObject(stopEvent, 0) == WAIT_TIMEOUT){

        handles[0] = FindFirstChangeNotification(directoryPath, FALSE, FILE_NOTIFY_CHANGE_LAST_WRITE);
        handles[1] = stopEvent;

        DWORD wait = WaitForMultipleObjects(2, handles, FALSE, INFINITE);
        if (wait == WAIT_OBJECT_0){
            Sleep(2000);
            getInformation(filePath, infoPath, messageBoxPath, notificationContent_list, days_list, hour_list, minute_list, false);
        }
    }
}

void showReminders(const std::string &notificationPath, const std::list <std::string> &notificationContent_list){

    auto notificationContent_i = notificationContent_list.begin();
    std::string notificationContent;

    for (int i = 0; i < notificationContent_list.size(); i++) {
        notificationContent = *notificationContent_i;
        ShellExecuteA(nullptr, "open", notificationPath.c_str(), notificationContent.c_str(), nullptr, 0);
        Sleep(5200);

        std::advance(notificationContent_i, 1);
    }
}

void reset_txt(const std::string &infoPath, const std::string &messageBoxPath, std::list <std::string> &notificationContent_list, std::list <std::string> &days_list, std::list <int> &hour_list, std::list <int> &minute_list, bool english){

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
        newFile.close();

        lang = 0;
        myMessageBox(messageBoxPath, "CReminders", "The file has been reset");
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
        newFile.close();

        lang = 1;
        myMessageBox(messageBoxPath, "CReminders", "info.txt se ha reseteado");
    }
}

int main(int argc, char *argv[]){

    const LPCSTR value = "CReminders";
    std::string directoryPath, infoPath, notificationPath, messageBoxPath;

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
    infoPath = directoryPath + R"(\info.txt)";
    notificationPath = directoryPath + R"(\ToastNotification\dist\toastNotification.exe)";
    messageBoxPath = directoryPath + R"(\ToastNotification\dist\messageBox.exe)";

    if(argc > 1 && argv[1] == std::string("error")){
        HANDLE fileChange;

        fileChange = FindFirstChangeNotification(directoryPath.c_str(), FALSE, FILE_NOTIFY_CHANGE_LAST_WRITE);
        WaitForSingleObject(fileChange, INFINITE);
        Sleep(2000);
    }

    if(argc == 1 || argv[1] == std::string("error")) {

        getInformation(filePath, infoPath, messageBoxPath, notificationContent_list, days_list, hour_list, minute_list, false);
        SetConsoleCtrlHandler((PHANDLER_ROUTINE)HandlerRoutine, TRUE );
        std::thread threadLastWritten(notifyLastWritten, directoryPath.c_str(), filePath, infoPath, messageBoxPath, std::ref(notificationContent_list), std::ref(days_list), std::ref(hour_list), std::ref(minute_list));
        threadLastWritten.detach();

        while (true) {

            auto notificationContent_i = notificationContent_list.begin();
            auto hour_i = hour_list.begin();
            auto minute_i = minute_list.begin();

            for (int i = 0; i < notificationContent_list.size(); i++) {
                notificationContent = *notificationContent_i;
                hour = *hour_i;
                minute = *minute_i;

                if (minute == getCurrentTime("min", now, ltm) && hour == getCurrentTime("hour", now, ltm) &&
                    isToday(days_list, i, now, ltm)) {
                    ShellExecuteA(nullptr, "open", notificationPath.c_str(), notificationContent.c_str(), nullptr, SW_HIDE);
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
        getInformation(filePath, infoPath, messageBoxPath, notificationContent_list, days_list, hour_list, minute_list, true);
        showReminders(notificationPath, notificationContent_list);
    }

    else if(argv[1] == std::string("deactivate") || argv[1] == std::string("desactivar"))
        deactivate(value, messageBoxPath);

    else if(argv[1] == std::string("reset_en"))
        reset_txt(infoPath, messageBoxPath, notificationContent_list, days_list, hour_list, minute_list, true);

    else if(argv[1] == std::string("reset_es"))
        reset_txt(infoPath, messageBoxPath, notificationContent_list, days_list, hour_list, minute_list, false);

    return 0;
}

#pragma clang diagnostic pop