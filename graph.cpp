//UMBC - CSEE - CMSC 341 - Fall 2021 - Proj1
#include "graph.h"
Graph::Graph(){
    m_head = nullptr;
    m_numNodes = 0;
    cout << "Please enter a file to be loaded. \n~";
    cin >> m_dataFile;
}

Graph::Graph(string dataFile){
    m_head = nullptr;
    m_numNodes = 0;
    m_dataFile = dataFile;
}

Graph::~Graph(){
    Node *temp = m_head;

    //traverse through the linked list and deallocate all memory
    while (temp != nullptr){
        m_head = temp;
        temp = temp->m_next;
        delete m_head;
    }

    //Reset head to nullptr and numNodes to 0
    m_head = nullptr;
    m_numNodes = 0;
}

void Graph::loadData(){
    int numNodes;
    int node, n, e, s, w;
    ifstream dataFile;
    dataFile.open(m_dataFile);
    if (dataFile.is_open()) {
        dataFile >> numNodes;
        m_numNodes = numNodes;
        for (int i = 0; i < numNodes; i++) {
            dataFile >> node >> n >> e >> s >> w;
            insert(node, n, e, s, w);
        }
    }
    else
        //the following statement provides the reason if a file doesn't open
        //please note: not all platforms are providing the same message
        cerr << "Error: " << strerror(errno) << endl;
}

void Graph::insert(int node, int n, int e, int s, int w){
    //If the graph is empty, create a new m_head
    if (empty()){
        //Insert at head the new node
        Node *temp = new Node(node);
        insertAtHead(temp);
        //connect the m_head to the second node
        if (n > 0){
            Node *north = new Node(n);
            temp->setNorth(north);
        }
        if (e > 0){
            Node *east = new Node(e);
            temp->setNorth(east);
        }
        if (s > 0){
            Node *south = new Node(s);
            temp->setSouth(south);
        }
        if(w > 0){
            Node *west = new Node(w);
            temp->setWest(west);
        }
    }
    //If the graph is not empty, insert the node in the desired direction
    else {
        Node *temp = findNode(node);
        if (n > 0){
            Node *north = new Node(n);
            temp->setNorth(north);
        }
        if (e > 0){
            Node *east = new Node(e);
            temp->setNorth(east);
        }
        if (s > 0){
            Node *south = new Node(s);
            temp->setSouth(south);
        }
        if(w > 0){
            Node *west = new Node(w);
            temp->setWest(west);
        }
    }
}

void Graph::insertAtHead(Node * aNode){
    //Check if graph is empty
    //If so add node
    if (empty()){
        m_head = aNode;
    }
    //If the graph has nodes, insert at the head
    else{
        Node *temp = m_head;
        aNode->m_next = m_head;
        m_head = aNode;
        m_head->setNext(temp);
    }
    //Increment number of nodes regardless if its empty || !empty
    m_numNodes++;
}

Node * Graph::findNode(int nodeValue){
    Node *temp = m_head;

    //Traverse through the linked list and find the desired node
    while (temp->m_value != nodeValue && temp != nullptr){
        temp = temp->m_next;
    }

    //return the pointer
    return temp;
}

bool Graph::findPath(int start, int end){
    //We need to clear all information about previous graph (i.e. visited nodes and m_path)
    clearVisited();
    clearResult();

    //Find the desired node and set the start node to the found node
    Node *startNode = findNode(start);

    //Mark the node as already visited, so we don't recursively visit the m_head
    aNode->m_visited = true;

    //If no path is found, return false
    if(!findPath(startNode, end)){
        return false;
    }

    //return true, given the if condition is not met
    return true;

}

bool Graph::findPath(Node* aNode, int end){
    //Add the start node to the stack
    m_path.push(aNode->m_value);

    //Base case
    if (aNode->m_value == end){
        return true;
    }
    //Checks in all directions to find desired path, marks node as visited, and pushes node into the m_path stack to be dumped
    if (aNode->m_east != nullptr && aNode->m_east->m_visited != true){
        aNode->m_east->m_visited = true;
        m_path.push(aNode->m_value);
        return findPath(aNode->m_east, end);
    }
    if (aNode->m_north != nullptr && aNode->m_north->m_visited != true){
        aNode->m_north->m_visited = true;
        m_path.push(aNode->m_value);
        return findPath(aNode->m_north, end);
    }
    if (aNode->m_south != nullptr && aNode->m_south->m_visited != true){
        aNode->m_south->m_visited = true;
        m_path.push(aNode->m_value);
        return findPath(aNode->m_south, end);
    }
    if (aNode->m_west != nullptr && aNode->m_west->m_visited != true){
        aNode->m_west->m_visited = true;
        m_path.push(aNode->m_value);
        return findPath(aNode->m_west, end);
    }
    else{
        //We need to pop the dead-end node from the stack
        m_path.pop();

        //Backtrack and return the node, the dead-end node is now marked as visited and will not be visited again
        return (findNode(m_path.top()), end);
    }

    //If all the cases fail, there is no path, thus return false
    return false;

}

void Graph::dump(){
    //If the stack is empty
    if (m_path.empty()){
        cout << "END";
    }

    //If the stack has items
    else {
        int poppedItem[m_path.size()];
        int stackSize = m_path.size();

        //Prints the items of the stack
        for (int i = 0; i < stackSize; i++) {
            cout << m_path.top() << "=>";
            poppedItem[i] = m_path.top();
            m_path.pop();
        }

        //Reinsert the popped items
        for (int i = 0; i < m_path.size(); i++){
            m_path.push(poppedItem[i]);
        }

        //End of printing items
        cout << "END";
    }
}

void Graph::clearResult(){
    //Pop all the items in the stack until empty
    while (!m_path.empty()){
        m_path.pop();
    }
}

void Graph::clearVisited(){
    Node *temp = m_head;

    //Traverse and reset all nodes->m_visited to false
    while (temp != nullptr){
        temp->m_visited = false;
        temp = temp->m_next;
    }
}

void Graph::buildGraph(string file) {
    //Clear all possible data linked to the old graph
    clearVisited();
    clearResult();
    clearGraph();

    //Set m_datafile to the new file data
    m_dataFile = file;

    //load the new graph
    loadData();
}

void Graph::clearGraph(){
    Node *temp = m_head;

    //Deallocate all the memory and reset the graph
    while (temp != nullptr){
        m_head = temp;
        temp = temp->m_next;
        delete m_head;
    }
    m_head = nullptr;
    m_numNodes = 0;
}

const Graph & Graph::operator=(const Graph & rhs){
    //protect against self-assignment
    if (this != &rhs){
        m_head = rhs.m_head;
        m_numNodes = rhs.m_numNodes;
        m_path = rhs.m_path;
        m_dataFile = rhs.m_dataFile;
    }
    return *this;
}

bool Graph::empty() const		// is the list empty?
{ return m_head == nullptr; }
