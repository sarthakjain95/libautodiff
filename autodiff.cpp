
namespace autodiff{

//////////////////
// Constructors //
//////////////////

scalar::scalar(double a): val{a} {
    parents.first = nullptr;
    parents.second = nullptr;
    nodes.push_back(this);
}

scalar::scalar(scalar& a): val{a.val} {
    parents.first = nullptr;
    parents.second = nullptr;
    nodes.push_back(this);
}

scalar::scalar(double a, node first, node second): 
    val{a} {
    parents.first = first;
    parents.second = second;
    nodes.push_back(this);
}

scalar::scalar(scalar& a, node first, node second): 
    val{a.val} {
    parents.first = first;
    parents.second = second;
    nodes.push_back(this);
}

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

void scalar::backward(){
    // Collect all ancestors in topological order
    std::vector<node> ancestors;
    std::set<node> visited;
    collect_ancestors(this, ancestors, visited);
    std::reverse(ancestors.begin(), ancestors.end());
    // Reset gradients for all ancestors
    for(node n : ancestors) n->grad = 0;
    grad = 1; // set self grad to 1
    // for(auto n : ancestors) std::cout << *n << std::endl;
    for(auto n : ancestors) n->compute_grad(n);
}

//////////////////////////
// Arithmetic Operators //
//////////////////////////

scalar scalar::operator + (scalar& b){
    scalar result(val + b.val, this, &b);
    result.compute_grad = [](node c){
        c->parents.first->grad += c->grad;
        c->parents.second->grad += c->grad;
    };
    return result;
}

scalar scalar::operator + (scalar&& b){
    scalar result(val + b.val, this, &b);
    result.compute_grad = [](node c){
        c->parents.first->grad += c->grad;
        c->parents.second->grad += c->grad;
    };
    return result;
}

scalar scalar::operator + (double b_val){
    scalar b{b_val};
    scalar result(val + b_val, this, &b);
    result.compute_grad = [](node c){
        c->parents.first->grad += c->grad;
        c->parents.second->grad += c->grad;
    };
    return result;
}

scalar scalar::operator * (scalar& b){
    scalar result(val * b.val, this, &b);
    result.compute_grad = [](node c){
        c->parents.first->grad += c->grad * c->parents.second->val;
        c->parents.second->grad += c->grad * c->parents.first->val;
    };
    return result;
}

scalar scalar::operator * (scalar&& b){
    scalar result(val * b.val, this, &b);
    result.compute_grad = [](node c){
        c->parents.first->grad += c->grad * c->parents.second->val;
        c->parents.second->grad += c->grad * c->parents.first->val;
    };
    return result;
}

scalar scalar::operator * (double b_val){
    scalar b{b_val};
    scalar result(val * b_val, this, &b);
    result.compute_grad = [](node c){
        c->parents.first->grad += c->grad * c->parents.second->val;
        c->parents.second->grad += c->grad * c->parents.first->val;
    };
    return result;
}

/////////////////////
// Other Operators //
/////////////////////

std::ostream& operator<<(std::ostream& os, const scalar& a){
    os << "scalar(val=" << a.val << ", grad=" << a.grad << ")";
    return os;
}

////////////////////////////////
// Other Arithmetic Functions //
////////////////////////////////

scalar pow(scalar& a, const double&& p){
    scalar result(std::pow(a.val, p), &a, nullptr);
    result.compute_grad = [&p](node c){
        c->parents.first->grad += p * std::pow(c->parents.first->val, p-1) * c->grad;
    };
    return result; 
}

scalar pow(scalar&& a, const double&& p){
    scalar result(std::pow(a.val, p), &a, nullptr);
    result.compute_grad = [&p](node c){
        c->parents.first->grad += p * std::pow(c->parents.first->val, p-1) * c->grad;
    };
    return result;   
}

} // autodiff [namespace#end]
