cl.exe /c /EHsc OGL.cpp /I C:\glew-2.1.0\include
rc.exe OGL.rc
link.exe OGL.obj OGL.res User32.lib GDI32.lib Kernel32.lib glu32.lib Winmm.lib /SUBSYSTEM:WINDOWS /LIBPATH:C:\glew-2.1.0\lib\Release\x64

