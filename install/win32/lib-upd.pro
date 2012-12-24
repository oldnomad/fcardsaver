TEMPLATE = subdirs
MAKEFILE = \\\\.\\NUL

gcc_path = $$system(for %i in (gcc.exe) do @echo.%~$PATH:i)
gcc_path = $$replace(gcc_path,/,\\)
MINGW_DIR = $$dirname(gcc_path)
system(xcopy /f /u /y $${MINGW_DIR}\\*.dll lib\\)
system(xcopy /f /u /y $${QMAKE_LIBDIR_QT}\\*.dll lib\\)
