# qjsynth
Do you want to create your own music program? Take a look at this small and simple project. It can give some ideas for a possible implementation.

Features:
* 4 types of sound
  * Sin
  * Square
  * Triangle
  * Saw
* Java-script driven sound
* XML-based project files
* Cool modern lookig UI

Controls:
Shortcut | Action
------------ | -------------
LMB | create/move/resize/select
Ctrl + LMB | make a copy of selected item
RMB | delete
MMB | adjust playing area
Alt + MMB | reset playing area
Ctrl + C | copy
Ctrl + MMB | paste
Ctrl + Mouse wheel | Zoom in/out

Script arguments:
Argument | Description
------------ | -------------
arg.frame | currently playing frame
arg.frames | size of currently playing item

Sound types:
* snd.SIN
* snd.SQUARE
* snd.TRIANGLE
* snd.SAW

Sound properties:
Property | Description
------------ | -------------
snd.type | synthesizer type (see above)
snd.volume_from | sound volume on the beginning of the frame
snd.volume_to | sound volume on the end of the frame
snd.duty | duty of SQUARE synthesizer
snd.pitch | change of sound frequency (in piano keys)
