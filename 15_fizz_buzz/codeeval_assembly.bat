type macrodef.h readbuf.h challenge.h readbuf.c challenge.c main.c > codeeval_assembly.c
powershell -Command "(gc codeeval_assembly.c) -replace '#define DBGFLAG', '//#define DBGFLAG' | Out-File codeeval_assembly.c"
powershell -Command "(gc codeeval_assembly.c) -replace '#include \"macrodef.h\"', '//#include \"macrodef.h\"' | Out-File codeeval_assembly.c"
powershell -Command "(gc codeeval_assembly.c) -replace '#include \"readbuf.h\"', '//#include \"readbuf.h\"' | Out-File codeeval_assembly.c"
powershell -Command "(gc codeeval_assembly.c) -replace '#include \"challenge.h\"', '//#include \"challenge.h\"' | Out-File codeeval_assembly.c"