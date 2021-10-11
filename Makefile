
DEFS=-DPLOT
INCLUDES=-I./json/single_include/ `pkg-config python3-embed --libs --cflags`

all: autodiff.hpp autodiff.cpp plot.cpp main.cpp
	g++ $(DEFS) $(INCLUDES) autodiff.hpp main.cpp
	@echo "type './a.out' to run."
	
clean:
	rm -f plot.json || true
	rm -f *.out || true
	rm -f *.gch || true
	clear
	
optional: # Optional dependencies for plotting
	@echo -e "\u001b[36m[INFO] Fetching optional dependencies for plotting\u001b[0m"
	git clone https://github.com/nlohmann/json.git || true
	mkdir -p pip
	pip install --target=./pip/ networkx matplotlib pandas

env: 
	@read -p "Do you want to fetch dependencies for plotting? (y/n):" res;\
	[[ $$res != "y" ]] || $(MAKE) optional
