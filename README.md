## How To Compile
`cd build`
`..\compile`

`-d` for debug build
`-r` for release build

--------------------
- main
    - level
        - grid
            - grid helper
        - terrain
            - road
            - base
            - spawner
            - platform
        - unit
            - tower
                - bullet
            - enemy
                - path finding

# Todo (refactor)
- [x] Shooting enemy
- [x] Move update terrain to `update_terrain.hpp` and unit to `update_unit.hpp`
- [ ] Refactor and clean up first even if there bug, then fix the bug later

# Todo (polish)
- [ ] Create gridPosition and localPosition in `entity_data.hpp`
- [ ] Make bullet have a pointer to tower that shoot it to give exp and detach bullet from tower
    - Use signal instead

# EXP
Bullet hit enemy take enemy exp pool and send it to tower that shoot it.
kill it have multiplier.
so tower will get exp just by shooting.
For example tower shoot enemy and each bullet damage its by 5%, each bullet take 5% of enemy exp pool and send it to the tower,
if the bullet kill enemy, multiply the exp.

# main (game)
The main menu
Pick level --go to--> level

## Level Manager // delete level manager and just use level directly
Generate level --pass--> gridMap

handle update and draw.

update:
- collision
- moving
- etc


### Grid
Hold information of object in grid
- primary entity
- secondary entity

Have function to move moving entity between cell.

own: Terrain entity
own: unit entity

### Primary/Terrain Entity
Own by level.
static object.
each cell can only have one on these.
- road
- base
- spawn
- platform

### Secondary/Unit Entity
Can only exist on top of primary/terrain entity.
The main entity.
- Enemy
- Tower

#### Enemy
Enemy move from spawn along the roads to base.

##### Path Finding

#### Tower
Tower can only be placed on top of platform, it shoot enemy.

##### Bullet
