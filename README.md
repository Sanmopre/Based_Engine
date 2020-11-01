#BASED Engine

##Contributors:
Moliner Préjano, Santiago - [GitHub account](https://github.com/Sanmopre)
Rami Rodríguez, David   - [GitHub account](https://github.com/Paideieitor)

##Controls:
A,S,D,W -> Camera movement
SHIFT(hold)-> Faster camera movement
ESC->Quit application

##UI:
GameObjects Tree window
-Create Group: hover or click to open the GameObject creating menu 
(all GameObjects created from here are child to the selected GameObject at the moment of creation)
        - Create GameObject Button: Create a new empty GameObject
        -Create Cube Button: Create a new GameObject with a cube mesh
        -Create Sphere Button: Create a new GameObject with a sphere mesh
        -Create Pyramid Button: Create a new GameObject with a pyramid mesh
        -Create Cylindre Button: Create a new GameObject with a cylindre mesh
        -Create Plane Button: Create a new GameObject with a plane mesh
-GameObject Tree Nodes: left click to select, if they have children double click or click the arrow to open or close
-Deselect Button: deselects selected GameObject

Selected GameObject Window
-GameObject Name Text Input: write and press Enter to change the GameObject name (if the name is already in use by one of its siblings this will fail)
-Delete GameObject Button: deletes the GameObject
-Position Float3 Input: write to modify the GameObjects position
-Rotation Float3 Input: currently useless
-Scale Float3 Input: write to modify the GameObjects scale (if a 0 is inputed it will be ignored and changed for the previous value)
-Mesh Component Header:
    -Active CheckBox: activates and deactivates the rendering of the mesh
-Normals CheckBox: activates and deactivates the rendering of the normals of the mesh
-Delete Mesh Button: deletes the Mesh Component
-Mesh Name Text Input: write and press Enter to change the Meshes name (if the name is already in use by another component this will fail)
-Mesh Path Text Input: write and press Enter to change the mesh (if the path is already in use or the loading of the new mesh is unsuccessful this will fail)
-Texture Path Text Input: write and press Enter to change the texture (if the path is already in use this will fail)
-Add Mesh Component Input Box: write the path to a fbx or obj in the hard drive and it will create a new mesh Component with that mesh

##Additional functionality:
Window docking
GameObject position and scale are editable

##Warning: 
Changing the size of the scene window deforms the view.
