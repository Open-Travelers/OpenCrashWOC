# OpenCrashWOC
An open source, reverse engineered of Crash Bandicoot: Wrath Of Cortex.

## Notice
We love Crash Bandicoot! If you plan to play this, you must legally dump assets from your GameCube disk, and please buy one of the latest Crash games to show your support! 
OpenCrashWOC is a fan reverse-engineered version of CrashWOC without having any access or knowledge of the original game code. Via assembly matching, the game's code is reconstructed in a way that compiles to the same thing than the one that the original programmers wrote, producing the same results during gameplay.

## Other Notice
This README will be more complete and detailed with instructions to contribute, build, and play once they exist!

## todo list

### initialisation project
- [x] SDL link
- [x] OpenGL link
- [ ] SDL Mixer li,k
- [ ] OpenGL ES 3 or OpenGL 3 link

### nulib compile
- [ ] nu3dx function (98%)
- [x] nucore function (99%)
- [x] numath function (100%)
- [x] nusound function (dummy)
- [x] nuraster function (100%)
- [ ] system function (90%) 
- [ ] gamelib function (80%)

### nulib test
- [ ] nu3dx validation test
- [ ] nucore validation test 
- [ ] numath validation test 
- [ ] nusound validation test 
- [ ] nuraster validation test 
- [ ] system validation test 
- [ ] gamelib validation test 

### game
- [ ] gamecode (50%)

## Building

WIP..

## Contributing
Check the function list on the `issue section` of this repository, those that have the percentage next to them have already started and are available on the `decomp.me` website, 
just search for the name in the search bar and then fork and you will have your scratch to work on.

You can check for any `DWARF info` (variables name,types,global variables,etc..) by searching for the name of the function you want to work on in this repository's file
`code/src/dump_alphaNGCport_DWARF.txt`

Which will provide the local variables types and names of the function and other information used in the alpha version, it should match in most cases with the retail version (some variables may be missing or totally different sometimes).
Once matched you can make a pull request or send a message on discord

If want to help the project or have any troubles, reach out on the Discord server: https://discord.gg/W8khh4v4Gx  

## Warning
Currently there are problems with the compiler flags for branch prediction (+\-) , so some functions will not fully match. 
If that's the only difference, you can still share the completed function.

