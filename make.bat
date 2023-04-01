@ECHO OFF

REM COMPILES THE CODE
REM REQUIRES GBDK

@REM Tiles
ECHO COMPILING TILES
c:\gbdk\bin\lcc -Wf-bo1 -c -o SpaceTileset.o Tiles/SpaceTileset.c
c:\gbdk\bin\lcc -Wf-bo1 -c -o MarsTileset.o Tiles/MarsTileset.c
c:\gbdk\bin\lcc -Wf-bo1 -c -o ShipSprites.o Tiles/ShipSprites.c
c:\gbdk\bin\lcc -Wf-bo1 -c -o UI.o Tiles/UI.c

@REM Maps
ECHO COMPILING MAPS
c:\gbdk\bin\lcc -Wf-bo1 -c -o SpaceMap.o Maps/SpaceMap.c
c:\gbdk\bin\lcc -Wf-bo1 -c -o MarsMap.o Maps/MarsMap.c

@REM Classes
ECHO COMPILING CLASSES
c:\gbdk\bin\lcc -c -o Ship.o Classes/Ship.c
c:\gbdk\bin\lcc -c -o Bullet.o Classes/Bullet.c

@REM Sound
ECHO COMPILING SONGS
c:\gbdk\bin\lcc -Wf-bo2 -c -o Placeholder.o Sound/Musics/Placeholder.c
c:\gbdk\bin\lcc -Wf-bo3 -c -o Wyrmhole.o Sound/Musics/Wyrmhole.c
c:\gbdk\bin\lcc -Wf-bo7 -c -o Stage1.o Sound/Musics/Stage1.c

ECHO COMPILING MAIN CODE
c:\gbdk\bin\lcc -c -o main.o main.c
@IF %ERRORLEVEL% == 1 GOTO ERROR
@REM -Wm-ya3
ECHO MAKING ROM
C:\gbdk\bin\lcc -Wm-yn"SHOOTER" -Wl-lhUGEDriver.lib -Wm-yo8 -Wm-ya4 -Wm-yt19 -Wm-yc -o game.gb SpaceTileset.o MarsTileset.o UI.o SpaceMap.o MarsMap.o ShipSprites.o Bullet.o Ship.o Placeholder.o Stage1.o Wyrmhole.o main.o
@IF %ERRORLEVEL% == 1 GOTO ERROR

ECHO :D
GOTO END

:ERROR
@ECHO D:
GOTO END

:END
@del *.ihx
@del *.lst
@REM @del *.noi
@REM @del *.map
@del *.o
@del *.sym
@del *.asm