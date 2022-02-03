# cg-2d-curves-3d
First try at making a basic renderer

## Current state
This project uses GTK which is not included in the project.
I've moved all files into separate folders but haven't updated the code yet, so right now it won't compile. 
In order to do so first all file references must be updated as well as a GTK compatible version must be included.

## What it does
Using a GTk window I first started rendering 2D elements:
* Point
* Line
* Circle
* Curves
    * Bezier
    * Spline
    
With that is was possible to start thinking in 3D:
* Added depth and perspective to the camera.
* Clipping objects outside of camera.
* Included navigation interface to move and rotate camera.
* Started rendering basic 3D objects
  * Planes
  * Cubes
  * Spheres
* Load an .obj  and draw it (Has a bug with a couple of vertices that are rendering in the wrong place)


## Next steps
* Add pixel shading 

Honestly now that I've started making a raytracer I don't see myself coming back to this, but I like to keep it as reference, already used the bezier implementation in another project.




