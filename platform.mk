ifeq ($(OS),Windows_NT)
    PLATFORM      := win32
    QMAKE         := qmake
    MKDIR         := mkdir
    RMDIR         := rmdir
    INSTALL_FILE  := copy /y
    DELETE_FILE   := del
    GZIP          := gzip
    WGET          := wget
    XSLTPROC      := xsltproc
    PERL          := perl
else
    PLATFORM      := linux
    QMAKE         := qmake
    MKDIR         := mkdir -m 755 -p
    RMDIR         := rmdir
    INSTALL_FILE  := install -m 644 -p
    DELETE_FILE   := rm -f
    GZIP          := gzip
    WGET          := wget
    XSLTPROC      := xsltproc
    PERL          := perl
    DB2X_XSLTPROC := db2x_xsltproc
    DB2X_MANXML   := db2x_manxml
endif
