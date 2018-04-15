# Defend Vancouver

Project for CPSC 436D

## Members:
* Andy Wong (x4j0b)
* Gowtham Mohan (i2k0b)
* Tanwin Nun (f8x8)
* Shrey Nayak (v0z9a)
* Anun Ganbat (e5e0b)

## FINAL MILESTONE

Gowtham Mohan

- Implemented Particle System
	- With OpenGL Instantiation
	- Occurs when hitting an enemy, or whe bomb explodes
	- Used ALPHA blending as well
- Implemented updated delay on Chaser
	- Update happens less regurarily, to allow time for path recalculation
	- Updated the 0s in the A Star Grid placement, whenever bomb is spawned
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

Tanwin Nun 

- Implemented UI Screens (State switching)
	- Game Over Screen
	- Start Screen
	- Highscore Screen 
	- State changes are tracked by using stack manipulation

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

Shrey Nayak

- In Game UI
	- Player Icon
	- Player Health
	- Score sprite rendering 
	- Wave sprite rendering 
- Bomb sprite sheet animation fix
- Player death affect
- Rendering score on all UI screens (Game Over and High Score)

NOTE: For the gameover screen there was a last minute bug that Shrey could not fix.
	BUT HIghscore implementation works when pressing H. Just the game over screen.

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

Anun Ganbat

- Implemented sound affect (In local repository, due to SDL)
	- Runs locally
	- BG Music
	- Explosion Music
	- Shooting Music

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

Andy Wong

- Implemented Highscore storing system
- Fixed entity spawn rates and tuned the Wave System
- Implement Bombs to be able to damage multiple Entities
- Player invulnerability and shield code implementation
- Clean Up collision logic in World
- Implementation of indicator arrows
- Crosshair Implementation for cursor
- Create vbo/vao/ibo only once per Entity type
 

DISCLAIMER: Sound only works on MAC due to the SDL library issues on windows




