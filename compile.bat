:: This batch file compiles Duality Engine using the MSVC++ compiler
:: Use it if you're in Windows and don't want to play whack-a-mole with visual studio.
:: the environment variables VS140COMNTOOLS, GLEW_PATH, SDL2_PATH, ASSIMP_PATH, and FREETYPE_PATH must be defined
:: VS140COMNTOOLS should be defined if you have VS 2015 installed.
:: You'll have to create the other environment variables and point them to the directories where their
:: respective 'include' folders are located. Obviously you'll need the development headers and libs for each of them.

:: TO USE:
:: do not move this file from the DualityEngine directory.
:: rather, call it from the location where you want the executable to appear.
:: For example, make a folder called 'build', then go into command prompt and
:: navigate to 'build'.  Now run the batch file. So if your build directory were in
:: the DualityEngine directory, you'd type the command "call ../compile.bat"
:: You can re-run it in the same directory again to only compile what's changed
:: since the last time you compiled.
:: For now, any time you add, remove, or rename a source file listed below, you'll have to update this file to
:: reflect the change.

:: don't repeat commands to terminal
@echo off

:: enter a local scope in which to set variables
setlocal

:: set variable 'batdir' to be the directory in which this file resides (VulkanBoilerplate)
set "batdir=%~dp0"

:: call the batch file that sets up the VS developer console environment
call "%VS140COMNTOOLS%VsDevCmd.bat"

:: compile
cl /c /I"%GLEW_PATH%\include" /I"%SDL2_PATH%\include" /I"%ASSIMP_PATH%\include" ^
 /I"%FREETYPE_PATH%\include" /I"%FREETYPE_PATH%\include\freetype2" ^
 /I"%batdir%source"                                     ^
 /I"%batdir%source\assetManagement"                     ^
 /I"%batdir%source\assetManagement\programmaticAssets"  ^
 /I"%batdir%source\bank"                                ^
 /I"%batdir%source\components"                          ^
 /I"%batdir%source\entities"                            ^
 /I"%batdir%source\math"                                ^
 /I"%batdir%source\systems"                             ^
 /I"%batdir%source\utilities"                           ^
 /nologo /W1 /WX- /O2 /Ob2 /Oy- /D WIN32 /D _WINDOWS /D _REENTRANT /D NDEBUG /D _MBCS /Zi /Gm /EHsc /MD /GS ^
 /Zc:wchar_t /Zc:forScope /Zc:inline /Gd /TP /analyze-                      ^
 "%batdir%main.cpp"                                                         ^
 "%batdir%source\Game.cpp"                                                  ^
 "%batdir%source\assetManagement\fileIO.cpp"                                ^
 "%batdir%source\assetManagement\loadCubeMap.cpp"                           ^
 "%batdir%source\assetManagement\loadShaders.cpp"                           ^
 "%batdir%source\assetManagement\MeshRepository.cpp"                        ^
 "%batdir%source\assetManagement\ShaderRepository.cpp"                      ^
 "%batdir%source\assetManagement\programmaticAssets\DebugBackground.cpp"    ^
 "%batdir%source\bank\bank.cpp"                                             ^
 "%batdir%source\entities\Box.cpp"                                          ^
 "%batdir%source\math\extendGlm.cpp"                                        ^
 "%batdir%source\math\functions.cpp"                                        ^
 "%batdir%source\systems\Control_SS.cpp"                                    ^
 "%batdir%source\systems\PhysCollide.cpp"                                   ^
 "%batdir%source\systems\PhysMove.cpp"                                      ^
 "%batdir%source\systems\Render_Background.cpp"                             ^
 "%batdir%source\systems\Render_BiTcubes.cpp"                               ^
 "%batdir%source\systems\Render_Console.cpp"                                ^
 "%batdir%source\systems\Render_Master.cpp"                                 ^
 "%batdir%source\systems\Render_Models.cpp"                                 ^
 "%batdir%source\systems\Scripting.cpp"                                     ^
 "%batdir%source\systems\UserInput.cpp"                                     ^
 "%batdir%source\utilities\Console.cpp"                                     ^
 "%batdir%source\utilities\Event.cpp"                                       ^
 "%batdir%source\utilities\helpDocs.cpp"                                    ^
 "%batdir%source\utilities\settings.cpp"                                    ^
 "%batdir%source\utilities\TimePiece.cpp"                                   ^
 "%batdir%source\utilities\toString.cpp"

:: link
link /OUT:DualityEngine.exe /INCREMENTAL:NO /NOLOGO kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ^
 ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib          ^
 "%SDL2_PATH%\lib\x86\SDL2main.lib" "%SDL2_PATH%\lib\x86\SDL2.lib"  ^
 "%GLEW_PATH%\lib\Release\Win32\glew32.lib" glu32.lib opengl32.lib  ^
 "%FREETYPE_PATH%\lib\freetype.lib"                                 ^
 "%ASSIMP_PATH%\lib\Release\assimp-vc130-mt.lib"                    ^
 /MANIFEST /MANIFESTUAC:"level='asInvoker' uiAccess='false'" /manifest:embed /SUBSYSTEM:CONSOLE /TLBID:1 /DYNAMICBASE ^
 /NXCOMPAT /IMPLIB:vkbp.lib /MACHINE:X86 /SAFESEH  /machine:X86     ^
 main.obj                  ^
 Game.obj                  ^
 fileIO.obj                ^
 loadCubeMap.obj           ^
 loadShaders.obj           ^
 MeshRepository.obj        ^
 ShaderRepository.obj      ^
 DebugBackground.obj       ^
 bank.obj                  ^
 Box.obj                   ^
 extendGlm.obj             ^
 functions.obj             ^
 Control_SS.obj            ^
 PhysCollide.obj           ^
 PhysMove.obj              ^
 Render_Background.obj     ^
 Render_BiTcubes.obj       ^
 Render_Console.obj        ^
 Render_Master.obj         ^
 Render_Models.obj         ^
 Scripting.obj             ^
 UserInput.obj             ^
 Console.obj               ^
 Event.obj                 ^
 helpDocs.obj              ^
 settings.obj              ^
 TimePiece.obj             ^
 toString.obj

:: copy required dll's to build location (These hopefully correspond to the .lib files linked against)
xcopy /h/y/Q "%GLEW_PATH%\bin\Release\Win32\glew32.dll"         "."
xcopy /h/y/Q "%SDL2_PATH%\lib\x86\SDL2.dll"                     "."
xcopy /h/y/Q "%ASSIMP_PATH%\bin\Release\assimp-vc130-mt.dll"    "."
xcopy /h/y/Q "%FREETYPE_PATH%\bin\freetype6.dll"                "."
xcopy /h/y/Q "%FREETYPE_PATH%\bin\zlib1.dll"                    "."

:: copy assets tree to build location
xcopy /s/e/h/y/I/Q "%batdir%assets" ".\assets"

:: leave the local scope
endlocal

:: 'press any key to exit'
pause