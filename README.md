# PixelSpacePlatformer
Project for Graphics 1 module
Written in C++ and OpenGL
Using nvImage loader and SDL_mixer for audio
Audio and image assets used are not my own

## Key Features:
## Weapon Collision Methods 

### Projectile
Projectile weapons are used in the game with weapons that have large and/or slow moving projectiles. 
This is because fast moving projectiles may be ahead of the target in one frame and behind it in the following frame, leading to a missed collision.
Projectile based weapons also allow the projectile movement to be seen traveling along its trajectory.
Projectile weapons in the game are modelled as follows:

	θ Represents the angle the weapon has been rotated
	ΔT Represents a variable used to control the speed of actions within the game
	
1.  The projectile is created at the weapon muzzle, and a heading vector is generated.
	1.	Heading vector is generated: (x,y)=((cos⁡(θ)),(sin⁡(-θ)))
1.  The projectile is rotated about its centre θ degrees
1. Each frame, the projectile is translated by its heading multiplied by ΔT
1. The projectile is then checked against a vector of bounding boxes for level geometry.
	1.	An axis aligned bounding box collision check is performed
	1.	If the axis aligned collision check returns true, we perform the more costly separating axis test to confirm the collision
1. If the projectile has collided, we remove it from our list of projectiles and create and explosion effect at the point of collision.
1. If the projectile did not collide with the level geometry, we then check if it has collided with the player or AI players
	1.	The same two tier method as for the terrain is used
1. If the collision has occurred between a player and the projectile, we damage the player accordingly, remove the projectile from the list, and generate a texture at the collision point.

### Hitscan
A second method of weapon collision detect was added in later on in development to address some of the issues with projectile weapons. The hitscan method is more suited to weapons in which the projectile would reach their target instantaneous (or there about). Weapons that were categorised into this included beam and bullet based weapons. Due to time constraints, a laser beam weapon was chosen to demonstrate this method.

Hitscan weapons in the game are modelled as follows:

	θ Represents the angle the weapon has been rotated
	ΔT Represents a variable used to control the speed of actions within the game

1. A maximum point is generated in a similar fashion to the heading vector for the projectile weapons.
	1.	Max point is generated: (x,y)=((cos(θ)/sin(−θ))∗r)+(mX,mY):

   		Where r is the weapons range
    		Where mX and mY are the coordinates of the weapon muzzle
	
1. A line is generated between the muzzle and the endpoint.
1. This line is compared to the lines for the bounding boxes for the level terrain geometry.  If an intersection occurs, this become the new maxPoint.
  	1. 	Two versions of this line intersection algorithm are used in the game. Hitscan weapons use the version in which all lines of a bounding box are checked, and the one closest to the weapon muzzle is returned as the point of intersection.
1. The same process is then taken but with players and AI. A pointer to the closest collision is kept.
1. If a collision occurred with a player or AI, damage is applied. As the beam weapon will hit every frame due to no travel time, the damage is a low value multiplied by ΔT.
	
	
	
