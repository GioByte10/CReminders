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

The way CReminders works is by adding itself ([main.cpp](https://github.com/GioByte10/CReminders/blob/main/main.cpp)) to the Current User's start up through the Windows Registry. The program accesses the [info.txt](https://github.com/GioByte10/CReminders/blob/main/cmake-build-debug/info_example.txt) file where the user adds all necessary information for it to run. CReminders then waits for the specified day(s) and time and then creates the notification by calling [toastNotification.py](https://github.com/GioByte10/CReminders/blob/main/cmake-build-debug/ToastNotification/toastNotification.py), using:
<br>
```C++
ShellExecuteA(nullptr, "open", notificationPath.c_str(), notificationContent.c_str(), nullptr, 0);
```

CReminders passes the content of the notification as an argument, which is received by toastNotification. After some off-camera parsing, the notification is displayed using Versa Syahputra's [Winotify](https://github.com/versa-syahptr/winotify) module.
<br>
<br>
## Info file
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

Attached below is an example of an info.txt file. Notice the _ticketing_ enclosing for each notification _block_. It is crucial that every block is properly enclosed in its upper and lower boundaries. Blocks must share enclosings when adjacent to each other, as shown here.

<img src="https://raw.githubusercontent.com/GioByte10/CReminders/main/cmake-build-debug/resources/img5.png" alt="drawing" width="800"></img><br><br>

## Commands file
This executable allows the user to perform several useful features, these are as follows:<br>
|Command     |Description                                                                                                                                   |
|------------|-----------                                                                                                                                   |
|`show`      |Quickly gives a preview of all reminders                                                                                                      |
|`deactivate`|Removes the program from the Registry and stops its execution. To reactivate it you must run CReminders.exe again                             |
|`reset en`  |Resets the info.txt file to its [original state](https://github.com/GioByte10/CReminders/blob/main/cmake-build-debug/resources/info_original.txt), in english. Note that this will delete all the reminders that you created                                               |
<br>

# Español
