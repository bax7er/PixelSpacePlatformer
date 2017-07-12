# PixelSpacePlatformer
Project for Graphics 1 module

## Key Features:
### Weapon Collision Methods 

#### Projectile
Projectile weapons are used in the game with weapons that have large and/or slow moving projectiles. 
This is because fast moving projectiles may be ahead of the target in one frame and behind it in the following frame, leading to a missed collision.
Projectile based weapons also allow the projectile movement to be seen traveling along its trajectory.
Projectile weapons in the game are modelled as follows:
    *θ Represents the angle the weapon has been rotated
    *ΔT Represents a variable used to control the speed of actions within the game
	1. The projectile is created at the weapon muzzle, and a heading vector is generated.
	 *Heading vector is generated: (x¦y)=((cos⁡(θ))¦(sin⁡(-θ)))
	1. The projectile is rotated about its centre θ degrees
	1. Each frame, the projectile is translated by its heading multiplied by ΔT
	1. The projectile is then checked against a vector of bounding boxes for level geometry.
	  ⋅⋅*An axis aligned bounding box collision check is performed
	If the axis aligned collision check returns true, we perform the more costly separating axis test to confirm the collision
	If the projectile has collided, we remove it from our list of projectiles and create and explosion effect at the point of collision.
	If the projectile did not collide with the level geometry, we then check if it has collided with the player or AI players
	The same two tier method as for the terrain is used
	If the collision has occurred between a player and the projectile, we damage the player accordingly, remove the projectile from the list, and generate a texture at the collision point.
