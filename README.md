# BASED Engine

## Contributors:
Moliner Préjano, Santiago - [GitHub account](https://github.com/Sanmopre)
Rami Rodríguez, David   - [GitHub account](https://github.com/Paideieitor)

## Controls:

A,S,D,W	    -> Camera movement
Right Click -> Camera rotation
SHIFT(hold) -> Faster camera movement
Arrow Keyys -> Move ball around (Physics Demo)
SPACE 	    -> Shoot a ball (Physcics Demo)
ESC         -> Quit application

## UI:

### GameObjects Tree window
	- Create Group: hover or click to open the GameObject creating menu 
	(all GameObjects created from here are child to the selected GameObject at the moment of creation)
		- Create GameObject Button: Create a new empty GameObject
		- Create Cube Button: Create a new GameObject with a cube mesh
		- Create Sphere Button: Create a new GameObject with a sphere mesh
		- Create Pyramid Button: Create a new GameObject with a pyramid mesh
		- Create Cylindre Button: Create a new GameObject with a cylindre mesh
		- Create Plane Button: Create a new GameObject with a plane mesh
	- GameObject Tree Nodes: left click to select, if they have children double click or click the arrow to open or close
	- Deselect Button: deselects selected GameObject
	- Drag and drop: use the drag and drop fucntionality to reparent objects to a new game object

### Play/Pause Button
	-Start and pause al the physics simulations in the engine

### Selected GameObject Window

	- GameObject Name Text Input: write and press Enter to change the GameObject name (if the name is already in use by one of its siblings this will fail)
	- Delete GameObject Button: deletes the GameObject
	- Position Float3 Input: write to modify the GameObjects position
	- Rotation Float3 Input: write to modify the GameObjects rotation
	- Scale Float3 Input: write to modify the GameObjects scale (if a 0 is inputed it will be ignored and changed for the previous value)
	
### GameObjects Components

	- Mesh Component:
		- Active CheckBox: activates and deactivates the rendering of the mesh
		- Normals CheckBox: activates and deactivates the rendering of the normals of the mesh
		- AABB CheckBox: activates and deactivates the rendering of the AABB of the mesh
		- Delete Mesh Button: deletes the Mesh Component
		- Mesh Name Text Input: write and press Enter to change the Meshes name (if the name is already in use by another component this will fail)
		- Mesh Path Text Input: write and press Enter to change the mesh (if the path is already in use or the loading of the new mesh is unsuccessful this will fail)
		- Texture Path Text Input: write and press Enter to change the texture (if the path is already in use this will fail)
	
	- Camera Component:
		- Camera active CheckBox: activates and deactivates the camera component
		- Camera culling CheckBox: activates and deactivates the camera scene culling
		- Delete Camera Button: deletes the Camera Component
		- Position Float3 Input: write to modify the Camera position	
		- Near Plane Distance float: write and press Enter to change the Near Plane Distance value 
		- Far PLane Distance float: write and press Enter to change the Far PLane Distance value
		- Vertical Fov value: write and press Enter to change the Vertical Fov value
		- Horizontal Fov value: write and press Enter to change the Horizontal Fov value

	- RigidBoy Component:
		- Active CheckBox: activates and deactivates the RigidBody component
		- Delete RigidBody Button: deletes the RigidBody Component
		- Mass value: write and press Enter to change the Mass value
		- Density value: write and press Enter to change the Density value
		- Use gravity CheckBox: activates and deactivates the gravity setting 
		- Is kinematic CheckBox: activates and deactivates the kinematic setting 
		- Freeze rotation and position CheckBoxes: activates and deactivates the Fix ritation and position settings 

	- Box Collider:
		- Active CheckBox: activates and deactivates the Collider component
		- IsTrigger CheckBox:  activates and deactivates the Trigger setting 
		- Delete Collider Button: deletes the Collider Component
		- Offset float3: write and press Enter to change the Offset value
		- Size float3: write and press Enter to change the Size value

	- Sphere Collider:
		- Active CheckBox: activates and deactivates the Collider component
		- IsTrigger CheckBox:  activates and deactivates the Trigger setting  
		- Delete Collider Button: deletes the Collider Component
		- Offset float3: write and press Enter to change the Offset value
		- Radius value: write and press Enter to change the Radius value
		
	- Capsule Collider:
		- Active CheckBox: activates and deactivates the Collider component
		- IsTrigger CheckBox: activates and deactivates the Trigger setting 
		- Delete Collider Button: deletes the Collider Component
		- Offset float3: write and press Enter to change the Offset value
		- Radius value: write and press Enter to change the Radius value
		- Height value: write and press Enter to change the Height value
	
	- Player Controller:
		- Active CheckBox: activates and deactivates the Player Controller component
		- Delete Controller Button: deletes the Collider Component
		- Max speed value: write and press Enter to change the Max speed value
		- Acceleraiton value: write and press Enter to change the Acceleraiton value
		- Deceleration value: write and press Enter to change the Deceleration value