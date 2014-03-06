
#gcc test2.c Statki.c Screen.c Util.c Menu.c -lncurses
#gcc main.c Statki.c Screen.c Util.c Menu.c ResHan.c BoardViewer.c Brain.c -lncurses -Wall
gcc main.c Statki.c Screen.c Util.c Menu.c ResHan.c BoardViewer.c Brain.c BoardSetter.c NumGet.c Logger.c StringGet.c -lncurses -Wall -o statki

