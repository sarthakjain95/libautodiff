
all: autodiff.hpp autodiff.cpp main.cpp
	g++ main.cpp autodiff.hpp
	@echo "type './a.out' to run."

clean:
	rm -f *.out || true
	rm -f *.gch || true
	clear
