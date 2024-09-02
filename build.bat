tcc -m32 -o LightSimulator.exe src/init.c -lkernel32 -luser32 -lgdi32 -lopengl32 -Wl,-subsystem=windows
.\LightSimulator.exe