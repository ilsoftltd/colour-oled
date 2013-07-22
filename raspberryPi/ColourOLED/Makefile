#
#  Makefile
#  
#  Colour OLED Breakout Board Library
#  
#  Copyright (c) 2013, ILSoft Ltd
#  All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without modification, 
#  are permitted provided that the following conditions are met:
#
#  * Redistributions of source code must retain the above copyright notice, this list 
#    of conditions and the following disclaimer.
#    
#  * Redistributions in binary form must reproduce the above copyright notice, this 
#    list of conditions and the following disclaimer in the documentation and/or other 
#    materials provided with the distribution.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
#  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
#  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
#  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
#  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
#  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
#  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
#  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
#  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
#  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
#  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
#  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
#  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  
#
DYN_VERS_MAJ = 1
DYN_VERS_MIN = 0

VERSION = $(DYN_VERS_MAJ).$(DYN_VERS_MIN)
DESTDIR = /usr
PREFIX = /local

STATIC = libcolourOled.a
DYNAMIC = libcolourOled.so.$(VERSION)

DEBUG	= -O2
CC		= g++
INCLUDE	= -I.
DEFS	= -D_GNU_SOURCE
CFLAGS	= $(DEBUG) $(DEFS) -Wformat=2 -Wall -Winline $(INCLUDE) -pipe -fPIC

LIBS	= wiringPi

SRC		= 	SSD1351b.cpp SSD1351d.cpp			\
			Fontb.cpp Fontd.cpp					\
			Drawing.cpp Bitmap.cpp				\
			DisplayCom.cpp

OBJ		= $(SRC:.cpp=.o)

all:	$(DYNAMIC)
static:	$(STATIC)

$(STATIC):	$(OBJ)
	@echo "[Link (Static)]"
	@ar rcs $(STATIC) $(OBJ)
	@ranlib $(STATIC)
	@size   $(STATIC)
	
$(DYNAMIC):	$(OBJ)
	@echo "[Link (Dynamic)]"
	@$(CC) -shared -Wl,-soname,libcolourOled.so -o libcolourOled.so.$(VERSION) -lpthread $(OBJ)
	
.c.o:
	@echo [Compile] $<
	@$(CC) -c $(CFLAGS) $< -o $@
	
.PHONEY:	clean
clean:
	@echo "[Clean]"
	@rm -f $(OBJ) *~ core tags libcolourOled.*

.PHONEY:	tags
tags:	$(SRC)
	@echo [ctags]
	@ctags $(SRC)
	
.PHONEY:	install-headers
install-headers:
	@echo "[Install Headers]"
	@install -m 0755 -d											$(DESTDIR)$(PREFIX)/include
	@install -m 0644 oledBuffered.h								$(DESTDIR)$(PREFIX)/include
	@install -m 0644 oledDirect.h								$(DESTDIR)$(PREFIX)/include
	@install -m 0644 defs.h										$(DESTDIR)$(PREFIX)/include
	@install -m 0644 SSD1351b.h									$(DESTDIR)$(PREFIX)/include
	@install -m 0644 SSD1351d.h									$(DESTDIR)$(PREFIX)/include
	@install -m 0644 Fontb.h									$(DESTDIR)$(PREFIX)/include
	@install -m 0644 Fontd.h									$(DESTDIR)$(PREFIX)/include
	@install -m 0644 Drawing.h									$(DESTDIR)$(PREFIX)/include
	@install -m 0644 Bitmap.h									$(DESTDIR)$(PREFIX)/include
	@install -m 0644 DisplayCom.h								$(DESTDIR)$(PREFIX)/include
	
.PHONEY:	install
install:	$(DYNAMIC) install-headers
	@echo "[Install Dynamic Lib]"
	@install -m 0755 -d											$(DESTDIR)$(PREFIX)/lib
	@install -m 0755 libcolourOled.so.$(VERSION)				$(DESTDIR)$(PREFIX)/lib/libcolourOled.so.$(VERSION)
	@ln -sf $(DESTDIR)$(PREFIX)/lib/libcolourOled.so.$(VERSION)	$(DESTDIR)/lib/libcolourOled.so
	
.PHONEY:	uninstall
uninstall:
	@echo "[UnInstall]"
	@rm -f $(DESTDIR)$(PREFIX)/include/oledBuffered.h
	@rm -f $(DESTDIR)$(PREFIX)/include/oledDirect.h
	@rm -f $(DESTDIR)$(PREFIX)/include/defs.h
	@rm -f $(DESTDIR)$(PREFIX)/include/SSD1351b.h
	@rm -f $(DESTDIR)$(PREFIX)/include/SSD1351d.h
	@rm -f $(DESTDIR)$(PREFIX)/include/Fontb.h
	@rm -f $(DESTDIR)$(PREFIX)/include/Fontd.h
	@rm -f $(DESTDIR)$(PREFIX)/include/Drawing.h
	@rm -f $(DESTDIR)$(PREFIX)/include/Bitmap.h
	@rm -f $(DESTDIR)$(PREFIX)/include/DisplayCom.h
	
.PHONEY:	depend
depend:
	makedepend -Y $(SRC)