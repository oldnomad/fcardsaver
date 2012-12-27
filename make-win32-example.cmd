@echo off
setlocal
rem Use GNUWin32 for gzip and wget
rem Note that xsltproc must be updated to the latest (see README.win32)
call C:\Progra~1\gnuwin32\bin\set_gnuwin32.bat
rem NOTE: XSLDIR must have only forward slashes
set XSLDIR=C:/docbook-xsl-ns-1.78.0
rem Use opensource QT framework v4.8.4 from default directory
set QTDIR=C:\Qt\4.8.4
rem Use NSIS from default directory
set NSISDIR=C:\Progra~1\NSIS
rem Use MinGW from default directory
set PATH=C:\MinGW\bin;%PATH%
mingw32-make QMAKE=%QTDIR%\bin\qmake.exe DOCBOOK_XSL=%XSLDIR% MAKENSIS=%NSISDIR%\makensis.exe
endlocal
