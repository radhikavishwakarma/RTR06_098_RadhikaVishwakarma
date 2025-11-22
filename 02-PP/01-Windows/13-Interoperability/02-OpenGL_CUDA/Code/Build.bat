rc.exe OGL.rc
nvcc.exe -I "C:\glew-2.1.0\include" -L "C:\glew-2.1.0\lib\Release\x64" -o OGL.exe OGL.res User32.lib GDI32.lib OGL.cu 
