#Path Tracer

Tom Minor, Bournemouth, NCCA


##Summary

My original goals for this project were to create a path tracer with Open Shading Language integration, possibly using Photon Mapping as an optimisation technique. However, it's current state is simply a global illumination renderer that supports:

- Loading primitive/material data from simple text files.
- Global Illumination approximation in the scene, light is sampled from any primitive using an emissive material type.
- Controlling the sample rate and render size from the user interface.

Once the renderer is more mature I hope to carry out my original plans of a usable path tracer integrated with OSL.

![Example Render](examples/example.png?raw=true "Example")

##Usage

###3D Viewport

Displays a real time visualisation of the scene, the camera perspective of this view is used in the final render. Automatically updates when a loaded scene file is changed.

+ **Left Mouse Button + Drag -** Orbit around the viewport.

+ **Right Mouse Button + Drag -** Pan around the viewport

+ **Scroll wheel -** Zoom in/out.

+ **File Menu-** Open scene files.

###Render Settings

Control various aspects of the render output.

+ **Samples -** The amount of samples per pixel, higher values will result in longer render times but more accurate images.

+ **Output Size -** The resolution of the rendered image.

+ **File Name -** The output file name *(only **.ppm** files are supported)*

+ **Render Button -** Render the scene to the file. *Note - This will render in the background, right now the progress is only printed to standard output and there is no indication in the UI when the render is complete*

### Scene files

All scene data is stored in simple text files, empty/commented lines are ignored.

Comments begin with #.

####Data Types

+ **Primitives**

   Translation XYZ = `tx ty tz`<br>
   Rotation XYZ = `rx ry rz`<br>
   Scale XYZ = `sx sy sz`<br>
   Colour RGB = `cr cg cb`<br>
   Vertex XYZ = `vx vy vz`<br>
 + **Sphere**  `$sphere tx ty tz sx sy sz rx ry rz cr cg cb `
 + **Triangle** `$tri v0x v0y v0z v1x v1y v1z v2x v2y v2z tx ty tz sx sy sz rx ry rz cr cg cb`
 + **Plane** *Will be supported in future versions, right now two triangle primitives are used*

+ **Camera** `$camera  tx ty tz sx sy sz rx ry rz fov nearz far z`
+ **Background** `$background  cr cg cb`

##Algorithms/Mathematical Methods

### Tracing the world 

Proved to me how handy matrices are for storing transformations.

1. Create ray in world space from camera into scene.
2. Convert into object space for intersection.
3. Convert impact point and light sample point to world space to calculate new direction vector.
4. Convert shadow ray to object space for each intersection.

###Global Illumination

```
repeat for many samples per pixel
    fire a single ray into the scene
         for each light source in scene
              if not shaded
                  colour += object colour                  
```
- Sum all light sources.       
- Only lose energy whenever a ray bounces.
- Use inverse square rule to create light falloff.
- Use tonemapping to scale luminance
 - Gamma correct the image


### Refraction
I didn't get chance to look into these in much detail, but I began trying to implement them for the (unfinished) refraction aspect of the path tracing engine.

- Schlick's approximation
- Snell's law

##Known Problems

- Due to the use of popping off the token stack in the file reading implementation, the tokens are read in the wrong order for some values. Not a huge problem as the input can be inverted, XYZ becomes ZYX for example. All the more reason to use a well established format.

- Depth refuses to work correctly, right now primitives are simply drawn in the order they are read from the file. Using the intersection t value and the world space distance has failed, presumably related to coordinate spaces when calculating intersections.

- Spheres are rendered almost fully black, yet shadow rays work correctly and there is a small amount of light hitting the object.

- Reflection is roughly implemented, but there are strange artifacts on the triangle reflections and sphere reflections don't seem to exist at all. Refraction doesn't work at all.

- No BSDF/BRDF's implemented, only the diffuse colour and global illumination.

##Implementation Challenges
- Understanding when to convert between object and world space produced many time consuming bugs, some still aren't fixed.

- Unsure where parts of classes such as Scene, Camera & RenderContext should go, for example scene will always need at least one camera (and for rendering only one camera will be needed), but logically the render camera is a separate object.

- Updating the GUI while rendering seemed to require a fairly complex threading setup that wasn't worth the time to implement, although adding a background render thread for the render process was fairly easy.


## Planned Features / Improvements
- Acceleration structures such as KD-Trees for faster triangle intersection, other structures for faster object testing.
- Open Shading Language to handle surface properties.
- Photon mapping to improve the render speed and image quality.
- Multi-threaded rendering.
- Real-time visualisation in the viewport by rendering in a background thread that updates a framebuffer object.


## Design Decisions

- Tried to make path tracer separate from visualisation, for example all NGL math classes could be replaced with *glm* or *oiio* fairly painlessly. Ideally the renderer should be a standalone program that the interface controls with command line arguments.

- Debugging was a challenge, it was difficult to set breakpoints for certain conditions since so many pixels/rays are evaluated. Other raytracing projects seemed to implement a special class that would print debug info and allow for breakpoints under very specific conditions, this seems to be a good approach to ease further development.

- The file structure is fairly robust for such a simple filetype, if corrupt data is found then it is simply skipped and an error message is emitted. However, it is tedious to extend for new types of data/primitives and doesn't allow for more advanced concepts such as relative transformations/parenting. I would have liked to look at the RIB format instead, but I decided to focus on the path tracing aspect of the project.
