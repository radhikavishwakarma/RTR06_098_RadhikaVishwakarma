cl.exe /c /EHsc D3D.cpp /I C:\glew-2.1.0\include
rc.exe D3D.rc
link.exe D3D.obj D3D.res User32.lib GDI32.lib /SUBSYSTEM:WINDOWS /LIBPATH:C:\glew-2.1.0\lib\Release\x64
