
namespace autodiff{

//////////////////
// Constructors //
//////////////////

scalar::scalar(const double a): 
	val{a} {
	parents.first = nullptr;
	parents.second = nullptr;
	nodes.push_back(this);
	eq = default_code + std::to_string(rounds); // Assign a default name
	label = eq;
	if(default_code[0] == 'z') default_code[0] = 'a' and rounds++;
	else default_code[0]++;
}

// scalar::scalar(const double a, std::string _name): 
// 	val{a} {
// 	parents.first = nullptr;
// 	parents.second = nullptr;
// 	nodes.push_back(this);
// 	name = _name; 
// }

// scalar::scalar(const scalar& a): 
// 	val{a.val} {
// 	parents.first = nullptr;
// 	parents.second = nullptr;
// 	nodes.push_back(this);
// 	name = default_code + std::to_string(rounds); // Assign a default name
// 	if(default_code[0] == 'z') default_code[0] = 'a' and rounds++;
// 	else default_code[0]++;
// }

// scalar::scalar(const scalar& a, std::string _name): 
// 	val{a.val} {
// 	parents.first = nullptr;
// 	parents.second = nullptr;
// 	nodes.push_back(this);
// 	name = _name; 
// }

// scalar::scalar(const scalar&& a): 
// 	val{a.val} {
// 	parents.first = nullptr;
// 	parents.second = nullptr;
// 	nodes.push_back(this);
// 	name = default_code + std::to_string(rounds); // Assign a default name
// 	if(default_code[0] == 'z') default_code[0] = 'a' and rounds++;
// 	else default_code[0]++;
// }

// scalar::scalar(const scalar&& a, std::string _name): 
// 	val{a.val} {
// 	parents.first = nullptr;
// 	parents.second = nullptr;
// 	nodes.push_back(this);
// 	name = _name; 
// }

scalar::scalar(const double a, const node first, const node second, const std::string op = ""): 
	val{a} {
	parents.first = first;
	parents.second = second;
	label = op;
	eq = "(" + first->eq + op + (second == nullptr ? "" : second->eq) + ")";
	nodes.push_back(this);
}

// scalar::scalar(const scalar& a, const node first, const node second, const std::string op = ""): 
// 	val{a.val} {
// 	parents.first = first;
// 	parents.second = second;
// 	name = "(" + first->name + op + (second == nullptr ? "" : second->name) + ")";
// 	nodes.push_back(this);
// }

//////////////
// Backward //
//////////////

void collect_ancestors(node a, std::vector<node>& ancestors, std::set<node>& visited){
	// Check if node is already present
	if(a == nullptr or visited.find(a) != visited.end()) return;
	// Add node
	visited.insert(a);
	// Collect parents
	collect_ancestors(a->parents.first, ancestors, visited);
	collect_ancestors(a->parents.second, ancestors, visited);
	// Add current node 
	ancestors.push_back(a);
}

void scalar::backward(bool generate_graph = false){
	// Collect all ancestors in topological order
	std::vector<node> ancestors;
	std::set<node> visited;
	collect_ancestors(this, ancestors, visited);
	std::reverse(ancestors.begin(), ancestors.end());
	// Reset gradients for all ancestors
	for(const node& n : ancestors) n->grad = 0;
	grad = 1; // set self grad to 1
	for(const node& n : ancestors) std::cout << ((uint64_t)n) << ' ' << *n << std::endl;
	for(auto n : ancestors) n->compute_grad(n);
	// Generate graph for nodes
	if(generate_graph == true){
		#ifdef PLOT
			plot(ancestors);
		#else
			std::cout << "[WARNING] This version was not compiled with support for Plotting. ";
			std::cout << "To remove this message, call 'backward' without any arguments." << std::endl;
			std::cout << "[INFO] Compile with -DPLOT to get this functionality." << std::endl;
		#endif
	}
}

//////////////////////////
// Arithmetic Operators //
//////////////////////////

scalar& scalar::operator + (scalar& b){
	node result = new scalar(val + b.val, this, &b, "+");
	result->compute_grad = [](node c){
		c->parents.first->grad += c->grad;
		c->parents.second->grad += c->grad;
	};
	return *result;
}

scalar& scalar::operator + (scalar&& b){
	node result = new scalar(val + b.val, this, &b, "+");
	result->compute_grad = [](node c){
		c->parents.first->grad += c->grad;
		c->parents.second->grad += c->grad;
	};
	return *result;
}

// scalar scalar::operator + (double b_val){
// 	scalar b{b_val};
// 	node result = new scalar(val + b_val, this, &b, "+");
// 	result->compute_grad = [](node c){
// 		c->parents.first->grad += c->grad;
// 		c->parents.second->grad += c->grad;
// 	};
// 	return *result;
// }

scalar& scalar::operator * (scalar& b){
	node result = new scalar(val * b.val, this, &b, "*");
	result->compute_grad = [](node c){
		c->parents.first->grad += c->grad * c->parents.second->val;
		c->parents.second->grad += c->grad * c->parents.first->val;
	};
	return *result;
}

scalar& scalar::operator * (scalar&& b){
	node result = new scalar(val * b.val, this, &b, "*");
	result->compute_grad = [](node c){
		c->parents.first->grad += c->grad * c->parents.second->val;
		c->parents.second->grad += c->grad * c->parents.first->val;
	};
	return *result;
}

// scalar scalar::operator * (double b_val){
// 	scalar b{b_val};
// 	node result = new scalar(val * b_val, this, &b, "*");
// 	result->compute_grad = [](node c){
// 		c->parents.first->grad += c->grad * c->parents.second->val;
// 		c->parents.second->grad += c->grad * c->parents.first->val;
// 	};
// 	return *result;
// }

/////////////////////
// Other Operators //
/////////////////////

std::ostream& operator << (std::ostream& os, const scalar& a){
	os << a.eq << "(val=" << a.val << ", grad=" << a.grad << ")";
	return os;
}

////////////////////////////////
// Other Arithmetic Functions //
////////////////////////////////

scalar& pow(scalar& a, const double&& p_val){
	std::string p{std::to_string(p_val)};
	while(p.length() > 3 and p[p.length() - 2] == '0' and p[p.length() - 1] == '0') 
		p.pop_back(); // Remove trailing zeros from string representation
	node result = new scalar(std::pow(a.val, p_val), &a, nullptr, "**" + p);
	result->compute_grad = [&p_val](node c){
		c->parents.first->grad += p_val * std::pow(c->parents.first->val, p_val-1) * c->grad;
	};
	return *result; 
}

scalar& pow(scalar&& a, const double&& p_val){
	std::string p{std::to_string(p_val)};
	while(p.length() > 3 and p[p.length() - 2] == '0' and p[p.length() - 1] == '0') 
		p.pop_back(); // Remove trailing zeros from string representation
	node result = new scalar(std::pow(a.val, p_val), &a, nullptr, "**" + p);
	result->compute_grad = [&p_val](node c){
		c->parents.first->grad += p_val * std::pow(c->parents.first->val, p_val-1) * c->grad;
	};
	return *result;   
}

} // namespace autodiff
