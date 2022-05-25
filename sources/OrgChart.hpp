#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <list>

using namespace std;

namespace ariel {

    enum Order{preoder, level_order, reverse_order};

    /*
        This class represents a node in a chart organization, 
        each node have his data (name) and a vector of his children (Node)
    */

    class Node {

        public:
            /********* Fields ***********/
            vector<Node*> children;
            string data;

            /********* Constructors **********/
            Node();
            Node (const Node& n);
            Node(string _data, vector<Node*> _children);         
            Node(string _data);
            Node(Node& other) = default;
            Node(Node&& other) = default;
            Node& operator=(Node&&) = default;
            Node& operator=(const Node& other) = default;
            ~Node();

    };


    class OrgChart {

    /*
        This class represents a global chart organization, 
        each chart have a root (Node)        
    */

        class Iterator {

        /*
            This inner class constitute an iterator for the chart organization, 
            each iterator have a current node, his index and a vector of pointers nodes   
        */

            private:

                /********* Fields ***********/
                Node* pointer_to_current_node;
                size_t current_index;
                vector<Node*> tree_list;


            public:

                /********* Constructors **********/
                Iterator();
                Iterator(Node *root, Order order);

                /********* main functions **********/
                void level_order(Node *root);
                void preorder(Node *root);
                void reverse_order(Node *root);

                /******** Operators functions *******/
                Iterator& operator++();
                Iterator operator++(int);
                string& operator*() const;
                string* operator->() const;
                bool operator==(Iterator const &other) const;
                bool operator!=(Iterator const &other) const;

        };

        public:

            /********* Field **************/
            Node *root;

            /********* Constructors ***********/
            OrgChart();
            OrgChart(const OrgChart& o);
            ~OrgChart();
            OrgChart& operator=(const OrgChart& other);
            OrgChart(OrgChart& other) = default;
            OrgChart(OrgChart&& other) = default;
            OrgChart& operator=(OrgChart&&) = default;
            // OrgChart& operator=(const OrgChart& org) = default;

            /********* main functions **********/
            void free_all_nodes(Node* root);
            Node* find_node(Node *n ,const string &val);
            OrgChart& add_root( const string& chart_root);
            OrgChart& add_sub(const string& dad_data, const string& son_data);

            /********** Iterator functions ******/
            Iterator begin() const;
            Iterator end() const;
            Iterator begin_preorder() const;
            Iterator end_preorder() const;
            Iterator begin_level_order() const;
            Iterator end_level_order() const;
            Iterator begin_reverse_order() const;
            Iterator reverse_order() const;

            friend ostream& operator<<( ostream& os, OrgChart & og);
            void print_tree(Node* root, ostream& os, string prefix, bool isLast);

    };
}
