AM_CPPFLAGS = -I$(srcdir)/../util $(TINFO_CFLAGS)
AM_CXXFLAGS = $(WARNING_CXXFLAGS) $(PICKY_CXXFLAGS) $(HARDEN_CFLAGS) $(MISC_CXXFLAGS)

if BUILD_IOS_CONTROLLER
  noinst_LIBRARIES = libmoshterminal.a
  libmoshterminal_a_SOURCES = parseraction.cc parseraction.h parser.cc parser.h parserstate.cc parserstatefamily.h parserstate.h parsertransition.h terminal.cc terminaldispatcher.cc terminaldispatcher.h terminaldisplay.cc terminaldisplayinit.cc terminaldisplay.h terminalframebuffer.cc terminalframebuffer.h terminalfunctions.cc terminal.h terminaluserinput.cc terminaluserinput.h
# libmoshterminal_a_SOURCES = parseraction.cc parseraction.h parser.cc parser.h parserstate.cc parserstatefamily.h parserstate.h parsertransition.h terminal.cc terminaldispatcher.cc terminaldispatcher.h terminaldisplay.cc terminaldisplay.h terminalframebuffer.cc terminalframebuffer.h terminalfunctions.cc terminal.h terminaluserinput.cc terminaluserinput.h
else
  noinst_LIBRARIES = libmoshterminal.a
  libmoshterminal_a_SOURCES = parseraction.cc parseraction.h parser.cc parser.h parserstate.cc parserstatefamily.h parserstate.h parsertransition.h terminal.cc terminaldispatcher.cc terminaldispatcher.h terminaldisplay.cc terminaldisplayinit.cc terminaldisplay.h terminalframebuffer.cc terminalframebuffer.h terminalfunctions.cc terminal.h terminaluserinput.cc terminaluserinput.h
endif
