LUA.Editor - Quickly write and test scripts - Fully functional

Save/load/one click copy all .lua/.out to specified folders. 

Simple to use & featherweight editor with compiler and interpreter.

Exists because it was a much better improvement over Notepad++ 

Follow the welcome guide to test program.

LUA.Editor.exe main application.

Features :-


- compiler 	LUAC.EXE -Called within main application	(converts to .cout) 5.3
- interpreter LUA.EXE -Called within main application (Run script or .cout) 5.3
- Multiple workspaces
- Prefab scripts
- Setup directories to quickly move scripts.
- Code snippets pad (Copy / paste)
- Complete documentation


Last TODO - All WIP.
											
- Undo/redo			
- Right click copy/paste 
- Gui save as / open window
- Web browser should read HTML 1.0 (text) using boost:ASIO. CHECK SOURCE! STILL SKELLY
- Web browser settings uses batch for ipconfig
- Web browser really needs HTML 4.0 or better. SDK maybe?
- Issue with WindowFocus(); on Editor tools window causing tooltip flicker.
- Will probably need freeimage for the web browser for image format support.
- Need to rewrite save/load code for boost (eughh :|)

Bugs fixed.

- Fixed bug with life.lua loading incorrectly because of special character.

Ponders

- Can i use a combination of boost asio/filesystem for FTP client?
- Could i also do the same for cloud access?
- Going to add folders for github. move pics & templates into them.
- Make a tcp server imgui app with networking tools & webpage/ftp folder - that would be neat.

Check source for details & notes.
