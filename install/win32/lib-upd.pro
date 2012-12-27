TEMPLATE = subdirs
MAKEFILE = \\\\.\\NUL

DESTDIR = $$replace(DESTDIR,/,\\)
cc_path = $$system(for %i in (cc.exe) do @echo.%~$PATH:i)
cc_path = $$replace(cc_path,/,\\)
MINGW_DIR = $$dirname(cc_path)
system(xcopy /f /u /y $${MINGW_DIR}\\*.dll $${DESTDIR})
system(xcopy /f /u /y $${QMAKE_LIBDIR_QT}\\..\\bin\\Qt*.dll $${DESTDIR})
