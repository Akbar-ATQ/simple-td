# A simple tower defense game with raylib.
A pretty simple tower defense game using cpp and raylib.

# Features
- Basic enemy and tower
- Tower level and upgrade
    - Can upgrade tower stats
- Level editor

# Notes
## Level Lists
I actually want to make a list of level that exist in `/assets/level/` but for whatever reason I can't `#include <filesystem>`.
So if you create new level then you need to add a button to your level in `main.cpp:MainMenu()` manually.
Just copy paste `SelectLevel(order, isLoadLevel, levelName, "LEVEL 1");` and change the string to your level name.

After some searching it seems gcc 8.1 not yet suporting `<filesystem>`.
As for why I still use gcc 8.1 is beacause I still using win7 32bit.
