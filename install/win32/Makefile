ifndef PLATFORM
    include ../../platform.mk
endif

all:
	cd stub && $(QMAKE) -makefile "CONFIG += release" stub.pro -r -spec win32-g++
	$(MAKE) -C stub first
	$(QMAKE) -makefile lib-upd.pro
	$(MAKENSIS) install.nsi

.PHONY: all
