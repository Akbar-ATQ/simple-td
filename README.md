## How To Compile
`cd build`
`..\compile`

`-d` for debug build
`-r` for release build

--------------------
# Todo
- [ ] Refactor
    - Separate bullet from tower
    - Place bullet and path_finding in /unit/component
    - Separate header and implementation
    - Maybe create base class `Terrain`
- [ ] Level editor
- [ ] Unit variations

# EXP
Bullet hit enemy take enemy exp pool and send it to tower that shoot it.
kill it have multiplier.
so tower will get exp just by shooting.
For example tower shoot enemy and each bullet damage its by 5%, each bullet take 5% of enemy exp pool and send it to the tower,
if the bullet kill enemy, multiply the exp.
