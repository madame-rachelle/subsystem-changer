# Simple CLI/GUI switcher for GZDoom / VKDoom

This program is for switching GZDoom or VKDoom back and forth between the CLI and GUI subsystems in Windows.

It can also change any other win32 app, simply drag and drop the executable on top of this program.

```
C:\games\gzdoom>subsystem-changer.exe
Current Subsystem: Windows (GUI) subsystem
Change to Console (CLI) subsystem (y/N)? y

Subsystem changed successfully.
```

This program will only be useful on 4.13.1 or later versions of GZDoom (or VKDoom equivalents) since this is when code was introduced to automatically detect the subsystem switch and use the console automatically.
