#include "OrgChart.hpp"

#include <iostream>
#include <vector>
#include <queue>
#include <stack>

using namespace std;

namespace ariel {

    // Node constructors with one and two arguments
    Node::Node(string _data, vector<Node*> _children) : data (move(_data)), children(move(_children)){};
    Node::Node(string _data) : data(move(_data)){};

    
    Node::~Node(){};

    /* 
        This function represents the level order traversal - works like BFS algorithm first we start by pushing 
        the tree's root into the queue and for each node we pop it, and push all its children in the queue.
        Use the iterator's vector to store them.   
    */

    void OrgChart::Iterator::level_order(Node *root) {
        
        if (root == nullptr) {
            return;
        }
        
        // Create a new queue
        queue<Node *> queue_lvl;  

        // Enqueue the tree's root
        queue_lvl.push(root); 
        tree_list.push_back(root);

        while (!queue_lvl.empty()) {

            int size = queue_lvl.size();

            // Checking if this node has children
            while (size > 0) {
                

                // Dequeue an element from queue 
                Node * element = queue_lvl.front();
                queue_lvl.pop();
    
                // Enqueue all children of the dequeued element
                for (size_t i=0; i<element->children.size(); i++) {
                    queue_lvl.push(element->children[i]);
                    tree_list.push_back(element->children[i]);
                }
                size--;
            }
        }
    }
    /* 
        This function represents the reverse order traversal - works like BFS algorithm but use stack, first we start by pushing 
        the tree's root into the queue and for each node we pop it and push it into the stack, and push all its children in the queue 
        from right children to left, then pop all the nodes from the stack and push them into the vector.
    */

    void OrgChart::Iterator::reverse_order(Node *root) {

        if (root == nullptr) {
            return;
        }

        // Create a new queue and stack
        queue <Node *> queue_rvs;
        stack<Node*> stack_rvs;

        // Enqueue the tree's root
        queue_rvs.push(root);

        while (!queue_rvs.empty()) {

            //Dequeue node and make it root 
            root = queue_rvs.front();
            queue_rvs.pop();
            stack_rvs.push(root);
            
            // Enqueue all children of the dequeued node from right to left
            for (size_t i=0; i<root->children.size(); i++) {
                if (root->children[root->children.size()-i-1] != nullptr) {
                    queue_rvs.push(root->children[root->children.size()-i-1]);
                }
            }
        }
    
        // Pop all nodes from stack one by one and push them into the vector
        while (!stack_rvs.empty()) {
            root = stack_rvs.top();
            tree_list.push_back(root);
            stack_rvs.pop();
        }    
    }

    /* 
        This function represents the reverse order traversal - works like DFS algorithm using stack, first we start by pushing 
        the tree's root into the stack and for each node we pop it and push it into the stack, and push all its children in the stack 
        from right children to left, at the end we get all the nodes of the tree in preoder traversal
    */
    void OrgChart::Iterator::preorder(Node *root) {
        
        if (root == nullptr) {
            return;
        }

        // Create a new stack
        stack<Node*> stack_pre;      

        // Push the tree's root        
        stack_pre.push(root);
    
        while (!stack_pre.empty()) {
    
            // Store the current node and pop it from the stack
            Node* curr = stack_pre.top();
            stack_pre.pop();

            // Store the visited node in preorder vector
            this->tree_list.push_back(curr);
            size_t size = curr->children.size() -1;
            for ( int i =(int) size; i >= 0; i--) {
                stack_pre.push(curr->children.at((size_t)i));
            }
        }     
    }

    // Default constructor of Iterator
    OrgChart::Iterator::Iterator() {
        current_index=0;
        pointer_to_current_node = nullptr;
    }

    // Iterator constructor - works with enum to know to which function need to send the root
    OrgChart::Iterator::Iterator(Node *root, Order order) : current_index(0), pointer_to_current_node(nullptr) {


        if (order == Order::level_order) {
                level_order(root);
                tree_list.push_back(nullptr);

        } else if (order == Order::preoder) {
                preorder(root);
                tree_list.push_back(nullptr);

        } else {
                reverse_order(root);
                tree_list.push_back(nullptr);
        }
    }
            

    // operator ++ prefix
    OrgChart::Iterator& OrgChart::Iterator::operator++(){
        if(current_index<tree_list.size()){
            current_index++;
            return *this;
        }
        throw runtime_error("Error, you increment to an index out of range!");
        
    }

    // operator ++ postfix
    OrgChart::Iterator OrgChart::Iterator::operator++(int) {
        Iterator itr = *this; 
        current_index++; 
        return itr;
    }

    // operator *
    string& OrgChart::Iterator::operator*() const {
        return tree_list[current_index]->data; 
    }

    // operator ->
    string* OrgChart::Iterator::operator->() const {
        return &(tree_list[current_index]->data); 
    }

    // operator ==
    bool OrgChart::Iterator::operator==(Iterator const &other) const {
        return tree_list[current_index] == other.tree_list[other.current_index]; 
    }

    // operator !=
    bool OrgChart::Iterator::operator!=(Iterator const &other) const { 
        return !(*this==other); 
    }

    // Default constructior OrgChart
    OrgChart::OrgChart() {root = nullptr;}


    // Deconstructor OrgChart
    OrgChart::~OrgChart() {free_all_nodes(root);}


    // This function iterates over all nodes of the tree in recursion and delete it  - use for free memory
    void OrgChart::free_all_nodes(Node* root){

        if (root == nullptr) {
            return;
        }

        for(size_t i = 0; i <root->children.size(); i++ ){
            
            free_all_nodes(root->children.at(i));
        }
        delete root;
    }


    
    // Copy assigment operator
    OrgChart& OrgChart::operator=(const OrgChart& other) {
            if (this == &other) {
                return *this;
            }
            delete(this->root);
            root = new Node(other.root->data);
            return *this;
    }


    // Check if the root already exist - replace it, else create a new root
    OrgChart& OrgChart::add_root( const string& chart_root) { 
        
        if (root!= nullptr) {
            root->data = chart_root;
        }else {
            root = new Node(chart_root);
        }
        return *this;
        
    }

    // Search in the tree if there is a node n with the val data in recursion - else return nullpointer
    Node* OrgChart::find_node(Node *n ,const string &val) {
    
        if (n == nullptr || n->data == val) {
            return n;
        }
        for (size_t i = 0; i < n->children.size(); i++){
    
            Node* sub = find_node(n->children[i], val);
            if (sub != nullptr){
                    return sub;}
        }
        return nullptr;           
    }     

           
    // Add new child to a given node 
    OrgChart& OrgChart::add_sub(const string& dad_data, const string& son_data) {

        // Check exceptions - if root exists
        if(root == nullptr){
            throw invalid_argument("Error, there isn't any nodes in the tree!");
        }

        if( dad_data.empty() || son_data.empty() || dad_data == "\n" || dad_data == "\t" 
            || son_data == "\r" || son_data == "\n" || son_data == "\t" || son_data == "\r") {
            throw invalid_argument("Error, invalid input - or there is at least one empty string!");
        }

        // Call for find function
        Node *n = find_node(root, dad_data);

        if (n == nullptr) {
            throw invalid_argument("Error, this value not exists in the tree!");
        }
        
        // Add new child to n if n is his "father"
        if (n->data == dad_data) {
            Node *child = new Node(son_data);
            n->children.push_back(child);
            return *this;
        }

        throw  runtime_error("Error, dad not found");

    }

    /************* Iterator functions ************/

    OrgChart::Iterator OrgChart::begin() const{
        if (root == nullptr) {
            throw invalid_argument("chart is empty!");
        }
        return begin_level_order();
    }


    OrgChart::Iterator OrgChart::end() const{
            if (root == nullptr) {
            throw invalid_argument("chart is empty!");
        }
        return end_level_order();
    }


    OrgChart::Iterator OrgChart::begin_preorder() const{ 
            if (root == nullptr) {
            throw invalid_argument("chart is empty!");
        }
        return Iterator(root,Order::preoder); 
    }


    OrgChart::Iterator OrgChart::end_preorder() const{
            if (root == nullptr) {
            throw invalid_argument("chart is empty!");
        }
        return Iterator(nullptr,Order::preoder);
    };


    OrgChart::Iterator OrgChart::begin_level_order() const{
            if (root == nullptr) {
            throw invalid_argument("chart is empty!");
        }
        return Iterator(root, Order::level_order);
    }


    OrgChart::Iterator OrgChart::end_level_order() const{ 
            if (root == nullptr) {
            throw invalid_argument("chart is empty!");
        }
        return Iterator(nullptr,  Order::level_order);
    }


    OrgChart::Iterator OrgChart::begin_reverse_order() const{ 
            if (root == nullptr) {
            throw invalid_argument("chart is empty!");
        }
        return Iterator(root, Order::reverse_order); 
    }


    OrgChart::Iterator OrgChart::reverse_order() const{ 
            if (root == nullptr) {
            throw invalid_argument("chart is empty!");
        }
        return Iterator(nullptr, Order::reverse_order);
    }


    // Print the tree in a pretty format
    void print_tree(Node* root, ostream& output, string first_prefix, bool last_child){

        output << first_prefix << "●———" << root->data << endl;
    
        if(last_child) {
            first_prefix += "     ";
        }
        else {
        first_prefix +=  "|    ";}

        for(size_t i = 0; i < root->children.size(); i++){
            print_tree(root->children.at(i), output, first_prefix, i + 1 == root->children.size());
        } 

    }  

    // Output operator
    ostream& operator<<( ostream& os, OrgChart & og){
        print_tree(og.root, os, "", true);
        return os;
    }

};
