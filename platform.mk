ifeq ($(OS),Windows_NT)
    PLATFORM      := win32
    CXXVERSION    := g++-4.6
    QMAKE         := qmake
    MKDIR         := -mkdir
    RMDIR         := rmdir
    INSTALL_FILE  := copy /y
    DELETE_FILE   := del
    GZIP          := gzip
    WGET          := wget
    XSLTPROC      := xsltproc
    DOCBOOK_XSL   := /docbook-xsl-ns
    PERL          := perl
    MAKENSIS      := makensis
else
    PLATFORM      := linux
    CXXVERSION    := g++
    QMAKE         := qmake
    MKDIR         := mkdir -m 755 -p
    RMDIR         := rmdir
    INSTALL_FILE  := install -m 644 -p
    DELETE_FILE   := rm -f
    GZIP          := gzip
    WGET          := wget
    XSLTPROC      := xsltproc
    DOCBOOK_XSL   := /usr/share/xml/docbook/stylesheet/docbook-xsl-ns
    PERL          := perl
endif
