ifndef PLATFORM
    include ../platform.mk
endif
KDE4_PATH := /usr/share/kde4/services/ScreenSavers
GNOME_PATH := /usr/share/applications/screensavers

all clean distclean:

install: install-kde4 install-gnome

uninstall: uninstall-kde4 uninstall-gnome

install-kde4:
	$(MKDIR) $(DESTDIR)$(KDE4_PATH)
	$(INSTALL_FILE) kde4/fcardsaver.desktop $(DESTDIR)$(KDE4_PATH)

uninstall-kde4:
	$(DELETE_FILE) $(DESTDIR)$(KDE4_PATH)/fcardsaver.desktop

install-gnome:
	$(MKDIR) $(DESTDIR)$(GNOME_PATH)
	$(INSTALL_FILE) gnome/fcardsaver.desktop $(DESTDIR)$(GNOME_PATH)

uninstall-gnome:
	$(DELETE_FILE) $(DESTDIR)$(GNOME_PATH)/fcardsaver.desktop

ifeq ($(PLATFORM),win32)
win32-build:
	$(MAKE) -C win32
else
win32-build:
endif

.PHONY: all install clean distclean
.PHONY: install-kde4 install-gnome win32-build
