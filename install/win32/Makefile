ifndef PLATFORM
    include ../../platform.mk
endif
OBJDIR_LIBS := ../../build/libs-win32
OBJDIR_STUB := ../../build/stub-win32

build: update-libs $(OBJDIR_STUB)/fcardss.scr
	$(MAKENSIS) install.nsi

$(OBJDIR_STUB)/fcardss.scr:
	cd stub && $(QMAKE) -makefile "CONFIG += release" stub.pro -r -spec win32-g++
	$(MAKE) -C stub first

clean: clean-libs
	cd stub && $(QMAKE) -makefile "CONFIG += release" stub.pro -r -spec win32-g++
	$(MAKE) -C stub clean

distclean: clean-libs
	cd stub && $(QMAKE) -makefile "CONFIG += release" stub.pro -r -spec win32-g++
	$(MAKE) -C stub distclean

update-libs: libraries.list
	$(MKDIR) "$(OBJDIR_LIBS)"
	for /f "eol=#" %%f in ($<) do echo.>$(OBJDIR_LIBS)/%%f
	$(QMAKE) -makefile lib-upd.pro "DESTDIR = $(OBJDIR_LIBS)"

clean-libs:
	$(DELETE_FILE) $(OBJDIR_LIBS)/*

.PHONY: build clean distclean update-libs clean-libs
