# CReminders

![img1](cmake-build-debug/resources/img2.png?raw=true "hello")
<div align="center">
  <a href="https://www.google.com">How to install CReminders</a> | 
  <a href="https://forms.gle/6A2fwbND7u2C9yGb8">Feedback</a> |
  <a href="https://www.google.com">Como instalar CReminders</a> | 
  <a href="https://forms.gle/R6eP34ekC58Pk5UT9">Retroalimentacion</a> |
  <a href="https://github.com/GioByte10/CReminders#español"> Español</a>
</div>


## English
<img align="right" width="216" height="103" src="https://raw.githubusercontent.com/GioByte10/CReminders/main/cmake-build-debug/resources/img4.png">
CReminders is a C++ and Python written program that runs in the background and lets you create custom reminders using Windows ToastNotifications (pop-ups). In addition to a custom title, message, days, and time for the notification, you can add an icon and buttons with links as well.

<br>
<br>

The way CReminders works is by adding itself, the [main](https://github.com/GioByte10/CReminders/blob/main/main.cpp) program, to the Current User's start up through the Windows Registry. The program accesses the [info.txt](https://github.com/GioByte10/CReminders/blob/main/cmake-build-debug/info_example.txt) file where the users adds all necessary information for CReminders to create the notifications, this includes:

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
<br>

## Español
