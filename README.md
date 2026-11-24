# CS-2-Project-Super-Mario

## Level 1 integration

Construct a `Level` with the game's `QGraphicsScene` and call
`loadFromFile(":/levels/level1.txt", &error)`. A successful load adds the
background and level objects and sets the scene rectangle. The player and enemy
spawn getters return scene coordinates. Construct the player and Goombas at those
positions in the game code.

Call `updateAll()` once per frame before checking collisions. Use `getObstacles()`
with `GameObject::getBounds()` for collision rectangles. `StaticObstacle::isHazard()`
identifies spikes, and `MovingObstacle::getDisplacement()` gives the change in
position a rider should inherit. When the player touches a collectible,
`collect()` returns its points once. When the player reaches the flag,
`Flag::activate()` returns true once so the game can transition states.

Reloading replaces the previous level items and resets coins, the flag, and
platforms. Fetch the new item pointers after a reload. The game owns any player
and enemies it adds to the scene.

| Map symbol | Meaning |
| --- | --- |
| `.` or space | Empty tile |
| `A` | Player spawn |
| `G` | Goomba spawn |
| `#`, `B`, `P` | Ground, brick, pipe |
| `S`, `M` | Spikes, moving platform |
| `C`, `F` | Coin, finish flag |
