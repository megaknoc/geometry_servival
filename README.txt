Needed packets
--------------

SFML (http://www.sfml-dev.org/)

Control
-------

Use left and right arrow keys to move the player. Press R to restart the game
when the player is dead.

Game organization
-----------------

Coordinates of the game start in the upper left corner.
0,0 ............. WIDTH, 0
.
.
.
0, HEIGHT ....... WIDTH, HEIGHT



playfield is organized in this way:

 |----------|
 |          |
 |    *  <------- player's position (rotates around the center)
 |    O <-------- center polygon
 |   ---  <------ bar with distance 1
 |  -----  <----- bar with distance 2
 |----------|

 |----|-----|
 |    |     |
 |  3 | 0   | <-- sectors are enumerated in this way
 |----|-----|
 |  2 | 1   |
 |    |     |
 |----|-----|

 Bars spawn at the outer edge and getting closer to the center.
 If they hit the player, the game is over.
 If they reach the center, they disappear

