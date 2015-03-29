CXXFLAGS = -Wall -pedantic -Wno-long-long -O0 -ggdb
CXX = g++
RM = rm -rf
SRCDIR = src
DOCDIR = doc
DOXY = Doxyfile
BINARY = bartyrad
OBJECTS =\
	$(SRCDIR)/CBackupFile.o\
	$(SRCDIR)/CDir.o\
	$(SRCDIR)/CFile.o\
	$(SRCDIR)/CFileInBackup.o\
	$(SRCDIR)/CGenericFile.o\
	$(SRCDIR)/CTime.o\
	$(SRCDIR)/HDD.o\
	$(SRCDIR)/UI.o\
	$(SRCDIR)/main.o
#--------End of macros


all: $(OBJECTS) $(DOXY)
	make compile
	make doc

compile: $(BINARY)

run: 
	./$(BINARY)

doc:
	( cd ./src ; doxygen ../$(DOXY) 2> /dev/null > /dev/null )

clean: 
	$(RM) *~ $(OBJECTS) $(SRCDIR)/*~ $(DOCDIR) $(BINARY)

count: 
	wc -l $(SRCDIR)/*.cpp $(SRCDIR)/*.h


#-------Make objects
$(BINARY): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(BINARY)

$(SRCDIR)/CBackupFile.o: $(SRCDIR)/CBackupFile.cpp $(SRCDIR)/CFile.h $(SRCDIR)/CGenericFile.h $(SRCDIR)/CFileInBackup.h \
	$(SRCDIR)/CBackupFile.h $(SRCDIR)/HDD.h $(SRCDIR)/CDir.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(SRCDIR)/CDir.o: $(SRCDIR)/CDir.cpp $(SRCDIR)/CDir.h $(SRCDIR)/CGenericFile.h $(SRCDIR)/HDD.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(SRCDIR)/CFile.o: $(SRCDIR)/CFile.cpp $(SRCDIR)/CFile.h $(SRCDIR)/CGenericFile.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(SRCDIR)/CFileInBackup.o: $(SRCDIR)/CFileInBackup.cpp $(SRCDIR)/CFile.h $(SRCDIR)/CGenericFile.h $(SRCDIR)/CFileInBackup.h \
	$(SRCDIR)/HDD.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(SRCDIR)/CGenericFile.o: $(SRCDIR)/CGenericFile.cpp $(SRCDIR)/CGenericFile.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(SRCDIR)/CTime.o: $(SRCDIR)/CTime.cpp $(SRCDIR)/CTime.h

$(SRCDIR)/HDD.o: $(SRCDIR)/HDD.cpp $(SRCDIR)/HDD.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(SRCDIR)/UI.o: $(SRCDIR)/UI.cpp $(SRCDIR)/UI.h $(SRCDIR)/CBackupFile.h $(SRCDIR)/CFile.h $(SRCDIR)/CGenericFile.h $(SRCDIR)/CFileInBackup.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(SRCDIR)/main.o: $(SRCDIR)/main.cpp $(SRCDIR)/CGenericFile.h $(SRCDIR)/CFileInBackup.h $(SRCDIR)/CFile.h $(SRCDIR)/CBackupFile.h \
	$(SRCDIR)/HDD.h $(SRCDIR)/CDir.h $(SRCDIR)/UI.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<
