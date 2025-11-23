cl.exe /c /EHsc OGL.cpp /I C:\glew-2.1.0\include /I "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v12.8\include"
rc.exe OGL.rc
link.exe OGL.obj OGL.res User32.lib GDI32.lib /SUBSYSTEM:WINDOWS /LIBPATH:C:\glew-2.1.0\lib\Release\x64 /LIBPATH:"C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v12.8\lib\x64"