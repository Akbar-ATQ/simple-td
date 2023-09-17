## How To Compile
`cd build`
`..\compile`

`-d` for debug build
`-r` for release build

--------------------
# Todo
- [ ] Level, exp and coin

# EXP
Bullet hit enemy take enemy exp pool and send it to tower that shoot it.
kill it have multiplier.
so tower will get exp just by shooting.
For example tower shoot enemy and each bullet damage its by 5%, each bullet take 5% of enemy exp pool and send it to the tower,
if the bullet kill enemy, multiply the exp.

# Notes
## Level Lists
I actually want to make a list of levels that exist in `/assets/level/` but for whatever reason I can't `#include <filesystem>`.
So if you create new level then you need to add a button to your level in `main.cpp:MainMenu()` manually.

After some searching it seems gcc 8.1 not yet suporting `<filesystem>`.
As for why I still use gcc 8.1 is beacause I still using win7 32bit.
