@echo off
setlocal
call C:\progra~1\gnuwin32\bin\set_gnuwin32.bat
set PATH=%PATH%;C:\QtSDK\mingw\bin
mingw32-make QMAKE=C:\QtSDK\Desktop\Qt\4.7.4\mingw\bin\qmake DOCBOOK_XSL=C:/docbook/docbook-xsl-ns-1.78.0 MAKENSIS=C:\progra~1\nsis\makensis
endlocal
