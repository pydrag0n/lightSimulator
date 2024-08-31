tcc -m32 -o LightSimulator.exe init.c -lkernel32 -luser32 -lgdi32 -lopengl32 -lglut32 -lglu32 -Wl,-subsystem=windows
.\LightSimulator.exe