hello!

==========================================================================================================================================
this is the list of what cpp finals have what is needed in what as i did not add post processing in to the finallayer

FinalLayer.cpp - interactive gui, skybox, shadow mapping, deferred rendering, geometry shaders, tesselation shaders, compute shaders with textures, compute shaders with SSBO, physically based rendering lighting model
Lab3.cpp - multiple renderpasses, post processing with colour attachement

before looking at final layer, i recommend turning off pointlights in the gui, just to make it easier to look at things

FinalLayer.cpp
vampire is line 			31 - 109
floor is line 				114 - 164
billboards is line 			170 - 205
pointlights is line 			211 - 220
shadow pass is line 			227 - 255
geomtry pass/deferred rendering is line 261 - 279
skybox pass is line 			313 - 367
light pass/deferred rendering is line 	374 - 424 
compute shaders/noise mapping is line 	430 - 470
compute shaders/normals is line 	477 - 511
compute shaders/particles 		519 - 577
==========================================================================================================================================
interactive gui

the interactive gui will be seperated in to different tab bars,
it will include-
turning on/off point lights and the ambient strength,
changing the light direction and showing a visualised representation of the shadow map,
show the different textures within the deferred rending colour attachments,
change the tesselation level and scale of the floor with tesselation,
changing the different variables of the noise generation, and showing them from the compute shader,
and changing the metalic or roughness of the PBR.

==========================================================================================================================================
skybox

the sky box is shown in all the labs, aswell as the final pass
==========================================================================================================================================
multiple renderpasses

in my final pass,i had a gpass and a light pass, within lab 3, there are multiple different renderpasses which suits this

===============================================================================================================================================================================================================
post processing with colour attachement

simililar to multiple renderpasses, lab 3 will have multiple post processing render passes shown on to the gui, including with a final pass to change the gamma correction, which i removed from the final layer

lab 2 has other post processing shaders, but it seems that sometimes it crashes doing so, so what im showing is lab 3.
in lab 3 there is the blur, depth buffer, fog, depth of view, and the final screen pass. all of which are post processing.
lab 2 however has edge and invert post processing which is not shown in lab 3.

in the lab 2 shaders folder, blur is blurVert.glsl and blurFrag.glsl
in the lab 3 shaders folder, depth is depthVert.glsl and depthFrag.glsl, fog is FogVert.glsl and FogFrag.glsl, 
depth of view is DepthViewVert.glsl and DepthViewFrag.glsl, and final screen pass is ScreenVert.glsl and ScreenFrag.glsl

===============================================================================================================================================================================================================
shadow mapping

the shadow map is shown, with the shadows of the models being shown on to the floor, in the gui, you can change the light direction which will affect the shadows shown on the floor
later on the shadows were calculated within the light pass fragment shader.

shadow mapping is getting the depth information from the lights position, and applying it to a depth map, which is used to calculate the shadows, if the depth of a fragment is further than
the depth in the depth map, it is a shadow
===============================================================================================================================================================================================================
deferred rendering

the gpass is what handles the deffered rendering, with the light pass combining the 3 colour attachements taking from the frame buffer.
it seperates the information, in our case, it is the world position, normals, and the albedo. and also depth, which is passed to the light pass.
which also calculates the lighting information, such as point lights or the directional light with shadows. 

===============================================================================================================================================================================================================
geometry shaders

goometry shaders were used for the billboards, which is to do with primitives, to display the billboard
we pass in points to the geometry shader from the vertex, which will add more points to create the sqaure

===============================================================================================================================================================================================================
tesselation shaders

for the tessleation shaders, it was used to create more tesselation for the floor, and the scale, which can be increased from the IMGUI. there is also a distance variable which will decrease the ammount of
tesselation depending on how far it is from the camera.
in tesselation, we can pass the primitive as a whole, which you can then subdivide in to more primitives, for us, we split trianlges in to more triangles, to give it a better texture, so when we impliment the height maps, 
it is more smooth

===============================================================================================================================================================================================================
compute shaders with textures

for compute shaders, it was used to generate a noise map, which wouldve been handed over to the tesselation shaders of the floor, along with the normals of the noise map. in the IMGUI,
variables such as octaves, frequency, amplitude, persistence and lacunarity are changable to alter the noise map, which is changed real time in the floor. 

===============================================================================================================================================================================================================
compute shaders with SSBO

with creating particles with the SSBO, i had managed to create them and have them visible to the screen, but i had not managed to update them and have them move accordinaly.

===============================================================================================================================================================================================================
physically based rendering lighting model

PBR is used in the final pass, with the roughness and metalicness able to be changed in the IMGUI, making it so that the models can have some parts more shiny. the light direction also affects this,
which can be altered in the lighting/shadow mapping tab of the IMGUI

===============================================================================================================================================================================================================