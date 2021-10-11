
#include "nlohmann/json.hpp"
#include "Python.h"

#include <map>
#include <fstream>

void plot(const std::vector<autodiff::node>& ancestors){

	using json = nlohmann::json;

	json data;
	data["nodes"] = {};
	data["links"] = {};

	for(int i=0; i<ancestors.size(); i++){
		const autodiff::node& n = ancestors[i];
		json node_data;
		node_data["label"] = n->label;
		node_data["eq"] = n->eq;
		data["nodes"].push_back(node_data);
	}

	for(const autodiff::node& n : ancestors){
		if(n->parents.first == nullptr) continue;
		json link_data;
		link_data["source"] = n->parents.first->eq;
		link_data["target"] = n->eq;
		data["links"].push_back(link_data);
		if(n->parents.second == nullptr) continue;
		json link2_data;
		link2_data["source"] = n->parents.second->eq;
		link2_data["target"] = n->eq;
		data["links"].push_back(link2_data);
	}

	std::ofstream out("plot.json");
	out << data.dump(4);
	out.close();

	// Run Python code to plot network graph
	Py_Initialize();
	// Add custom path
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append(\'./\')");
	PyRun_SimpleString("sys.path.append(\'./pip/\')");
	// Run local script
	FILE* fp = _Py_fopen("plot.py", "r");
	PyRun_SimpleFile(fp, "plot.py");
	Py_Finalize();

}
