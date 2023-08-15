## How To Compile
`cd build`
`..\compile`

`-d` for debug build
`-r` for release build

--------------------

# main (game)
The main menu
Pick level --go to--> level

## Level Manager
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
