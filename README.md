# CReminders

![img1](cmake-build-debug/resources/img2.png?raw=true)
<div align="center">
  <a href="https://www.google.com">How to install CReminders</a> | 
  <a href="https://forms.gle/6A2fwbND7u2C9yGb8">Feedback</a> |
  <a href="https://www.google.com">Como instalar CReminders</a> | 
  <a href="https://forms.gle/R6eP34ekC58Pk5UT9">Retroalimentacion</a> |
  <a href="https://github.com/GioByte10/CReminders#español"> Español</a>
</div>


# English
<img align="right" width="216" height="103" src="https://raw.githubusercontent.com/GioByte10/CReminders/main/cmake-build-debug/resources/img4.png">
CReminders is a C++ and Python written program that runs in the background and lets you create custom reminders using Windows ToastNotifications (pop-ups). In addition to a custom title, message, days, and time, you can add an icon and buttons for the notification.

<br>
<br>

The way CReminders works is by adding itself ([main.cpp](https://github.com/GioByte10/CReminders/blob/main/main.cpp)) to the `CURRENT_USER`'s start up through the [Windows Registry](https://en.wikipedia.org/wiki/Windows_Registry). The program accesses the [info.txt](https://github.com/GioByte10/CReminders/blob/main/cmake-build-debug/info_example.txt) file where the user adds all necessary information for it to run. CReminders then waits for the specified day(s) and time and then creates the notification by calling [toastNotification.py](https://github.com/GioByte10/CReminders/blob/main/cmake-build-debug/ToastNotification/toastNotification.py), using:
<br>
```C++
ShellExecuteA(nullptr, "open", notificationPath.c_str(), notificationContent.c_str(), nullptr, 0);
```

CReminders passes the content of the notification as an argument, which is received by toastNotification. After some off-camera parsing, the notification is displayed using Versa Syahputra's [Winotify](https://github.com/versa-syahptr/winotify) module.
<br>
<br>
## info.txt file
This file holds all the data necessary for the program to run, this includes:
| Parameter   | Description                                                                                                                                  |
|-------------|-------------                                                                                                                                 |
| Active      | Whether or not this reminder is active                                                                                                       |
| Title       | Reminder's title                                                                                                                             |
| Message     | Reminder's message                                                                                                                           |
| Icon        | Absolute path of the image<br>e.g., → C:\Users\User1\pictures\img1.png                                                                       |
| Duration    | Time the notification will pop-up before being added to the Action Center<br>short = 5s, long = 30s                                          |
| Button      | Whether or not this reminder will have a button                                                                                              |
| ButtonText  | The text shown over the button                                                                                                               |
| ButtonLaunch| The link (if any) the button will launch when clicked. Must contain "**https://**" &emsp;&emsp;&emsp;<br>e.g., → https://www.google.com      |
| Time        | 24 hour format<br>e.g., → 16:30                                                                                                              |
| Days        | Days in which the reminder will be active<br>e.g., → monday, wednesday, friday<br>e.g., → everyday                                           |

<br>

Attached below is an example of an info.txt file. Notice the _ticketing_ (-----) enclosing for each notification _block_. It is crucial that every block is properly enclosed in its upper and lower boundaries. Blocks must share enclosings when adjacent to each other, as shown here.

<img src="https://raw.githubusercontent.com/GioByte10/CReminders/main/cmake-build-debug/resources/img5.png" alt="drawing" width="800"></img><br><br>

## commands.exe file
This executable allows the user to perform several useful features, these are as follows:<br>
|Command     |Description                                                                                                                                   |
|------------|-----------                                                                                                                                   |
|`show`      |Quickly gives a preview of all reminders                                                                                                      |
|`deactivate`|Removes the program from the Registry and stops its execution. To reactivate it you must run CReminders.exe again                             |
|`reset en`  |Resets the info.txt file to its [original state](https://github.com/GioByte10/CReminders/blob/main/cmake-build-debug/resources/info_original.txt), in english. Note that this will delete all the reminders that you created                                               |
<br>


# Español
<img align="right" width="216" height="103" src="https://raw.githubusercontent.com/GioByte10/CReminders/main/cmake-build-debug/resources/img4.png">
CReminders es un programa escrito en C++ y Python, el cual corre en segundo plano y te permite crear recordatorios personalizados usando notificaciones de Windows (pop-ups). Ademas de un titulo, mensaje, dias, y hora personalizada, puedes agregar un icono y botones para la notificacion.

<br>
<br>

Para lograr eso, CReminders se anade a si mismo ([main.cpp](https://github.com/GioByte10/CReminders/blob/main/main.cpp)) al start up de `CURRENT_USER` a traves del [Registro de Windows](https://es.wikipedia.org/wiki/Registro_de_Windows). El programa accesa el archivo [info.txt](https://github.com/GioByte10/CReminders/blob/main/cmake-build-debug/info_ejemplo.txt), en el cual el usuario anade toda la informacion necesaria para que este se ejecute. Tras eso, CReminders espera por el dia y tiempo especificado y crea la notificacion ejecutando [toastNotification.py](https://github.com/GioByte10/CReminders/blob/main/cmake-build-debug/ToastNotification/toastNotification.py), usando:
<br>
```C++
ShellExecuteA(nullptr, "open", notificationPath.c_str(), notificationContent.c_str(), nullptr, 0);
```

CReminders pasa el contenido de la notificacion como argumento, el cual es recibido por toastNotification. Tras eso la notificacion se muestra usando el modulo [Winotify](https://github.com/versa-syahptr/winotify) de Versa Syahputra.

<br>
<br>

## Archivo info.txt
Este archivo contiene toda la informacion necesaria para que el programa se ejecute, esta incluye:
| Parametro   | Descripcion                                                                                                                                  |
|-------------|-------------                                                                                                                                 |
| Activo      | Si esta notificacion estara activa o no                                                                                                      |
| Titulo      | Titulo de la notificacion                                                                                                                    |
| Mensaje     | Mensaje de la notificacion                                                                                                                   |
| Icono       | Ruta absoluta de la image <br>e.g., → C:\Users\User1\pictures\img1.png                                                                       |
| Duracion    | Tiempo que la notificacion durara como pop-up antes de ser movido al Centro de Actividades<br>corta = 5s, larga = 30s                        |
| Boton       | Si esta notificacion tendra un boton o no                                                                                                    |
| BotonTexto  | El texto que se mostrara en el boton                                                                                                         |
| BotonLaunch | El link (si es que hay) que el boton abrira al ser presionado. Debe contener "**https://**" &emsp;&emsp;&emsp;<br>e.g., → https://www.google.com|
| Hora        | En formato de 24 horas<br>e.g., → 16:30                                                                                                      |
| Dias        | Dias en los cuales la notificacion estara activa<br>e.g., → lunes, miercoles, viernes<br>e.g., → diario                                      |

<br>

Abajo esta adjunto un ejemplo de un archivo info.txt. Note como cada _bloque_ de notificacion esta _encapsulado_ por barras horizontales (------). Es de suma importancia que cada bloque este correctamente encapsulado en los limites superiores e inferiores. Estos limites deben ser compartidos cuando hay bloques adyacentes, tal como se muestra abajo.

<img src="https://raw.githubusercontent.com/GioByte10/CReminders/main/cmake-build-debug/resources/img6.png" alt="drawing" width="800"></img><br><br>

## Archivo commands.exe
Este ejecutable le permite al usuario realizar varias funciones, estas son las siguientes:<br>
|Comando     |Descripcion                                                                                                                                   |
|------------|-----------                                                                                                                                   |
|`mostrar`   |Muestra rapidamente como se verian los recordatorios                                                                                          |
|`desactivar`|Elimina el programa del registro y detiene su ejecucion. Para volver a activarlo se debe ejecutar CReminders.exe de nuevo                     |
|`reset es`  |Resetea el archivo info.txt a su [estado inicial](https://github.com/GioByte10/CReminders/blob/main/cmake-build-debug/resources/info_original_.txt), en espanol. Note que esto borrara todos los recordatorios que haya creado                    |
<br>
