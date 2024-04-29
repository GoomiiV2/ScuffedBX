# ScuffedBX

A scuffed wrapper and binding to import and export FBX files :>

Can import and export from a simple model format to and from FBX.

Supports
* Normals
* Tangents
* Colors
* Uv 0
* Skeletons
* Basic Materials
* Bone Weights

# Build Setup
You will need to download the Autodesk FBX SDK and install it, I assume the default location in this project, if you install it else where make sure to update it in the settings for ``ScuffedBX.Native``.

After that you should be able to just build ``ScuffedBX`` and get your files :>

The native dlls are copied to ``ScuffedBX/Build/x64/(Debug or Release)``. You need both ``libfbxsdk.dll`` (taken from the FBX SDK install on build) and ``ScuffedBX.Native.dll``

Reference the ``ScuffedBX`` from your own project and make sure the 2 above dlls are along side your exe.

# Examples

For examples please look at ``ScuffedBX.Test/Program.cs``

This test project has examples on importing, exportign and creating models.

## Build Order
The build order and setup is abit, of a pipeline and kinda scuffed :>
* ScuffedBX.Gen is built
* ScuffedBx.Native Starts to build
  * Copies the fbxsdk dll to the build dir
  * Runs the ScuffedBX.Gen to create the c++ files that need to be compiled into the dll
  * Compiles the ScuffedBX.Native
* Compiles the ScuffedBX c# lib
* Compiles the ScuffedBX.Text project
  * Copies the fbx dll from the native output
  * Copies the ScuffedBX.Native.dll from the output dir