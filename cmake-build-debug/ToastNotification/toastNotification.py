import os

from winotify import Notification, audio
import sys


def cleanArgv(argv):
    argv = argv.replace("_NULL~_", "")
    return argv.replace("_SPC_", " ")


def createNotification(title, msg, icon, duration, button_text, button_launch):

    if not duration:
        duration = "long"

    toast = Notification(app_id="CReminder",
                         title=title,
                         msg=msg,
                         duration=duration,
                         icon=icon)

    toast.set_audio(audio.Default, loop=False)

    if button_launch:
        toast.add_actions(label=button_text, launch=button_launch)

    elif button_text:
        toast.add_actions(label=button_text)

    toast.show()


print(os.getcwd())


if len(sys.argv) == 7:

    argv1 = cleanArgv(sys.argv[1])
    argv2 = cleanArgv(sys.argv[2])
    argv3 = cleanArgv(sys.argv[3])
    argv4 = cleanArgv(sys.argv[4])
    argv5 = cleanArgv(sys.argv[5])
    argv6 = cleanArgv(sys.argv[6])

    createNotification(argv1, argv2, argv3, argv4, argv5, argv6)

else:
    createNotification("Error", f"{str(len(sys.argv))} argument(s) were passed", os.getcwd() + "\\ErrorIcon.png", "long", "Help", "https://github.com/GioByte10/CReminders/wiki")

