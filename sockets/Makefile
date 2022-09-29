CXX=g++
CXXOPTIMIZE= -O2
CXXFLAGS= -g -Wall -pthread -std=c++11 $(CXXOPTIMIZE)
USERID=arthurjose_and_rodrigoalmeida
#CLASSES=SUA_LIB_COMUM
all: clean echo-server echo-client
echo-server: 
	$(CXX) -o $@ $^ $(CXXFLAGS) $@.cpp http_request.cpp http_response.cpp http_request.h http_response.h
echo-client: 
	$(CXX) -o $@ $^ $(CXXFLAGS) $@.cpp http_request.cpp http_response.cpp http_request.h http_response.h
multi-thread:
	$(CXX) -o $@ $^ $(CXXFLAGS) $@.cpp
showip:
	$(CXX) -o $@ $^ $(CXXFLAGS) $@.cpp
clean:
	rm -rf *.o *~ *.gch *.swp *.dSYM echo-server echo-client *.tar.gz
tarball: clean
	tar -cvf lab02-$(USERID).tar.gz *
