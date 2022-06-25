#ifndef GRAPH_H
#define GRAPH_H
#include <stack>
#include <list>
#include <stack>
#include <vector>
#include <iostream>
#include <queue>
#include <algorithm>

using std::endl;
using std::cout;
using std::ostream;
using std::stack;
using std::vector;

template <typename T>
class Graph;

template <typename T>
struct Vertex {
    T vertexValue;
    std::list<T>* neighbors = nullptr;
    T predecessor;
    bool visited = false;
    Vertex<T>* next = nullptr;
};


template <typename T>
class Graph{
    private:
      //Declare any variables needed for your graph
      int maxVertices{};
      int currentVertices{};
      Vertex<T> *adjacencyList;

    public:
      Graph();
      Graph(int);
      ~Graph();
      Graph(const Graph<T>& other);
      void addVertex(T vertex);
      void addEdge(T source, T target);
      vector<T> getPath(T, T);
      int findVertexPos(T item);
      int getNumVertices();
      Vertex<T>* findVertex(T source);
    /*********************************************
    * Returns a display of the graph in the format
    * vertex: edge edge
    Your display will look something like the following
      9: 8 5
      2: 7 0
      1: 4 0
      7: 6 2
      5: 6 8 9 4
      4: 5 1
      8: 6 5 9
      3: 6 0
      6: 7 8 5 3
      0: 1 2 3
    *********************************************/
      friend ostream& operator << (ostream & out, Graph<T>& g) {
          bool discovered[g.currentVertices];
          for(auto & item : discovered) {
              item = false;
          }
          std::queue<Vertex<T>*> queue;
          queue.push(g.adjacencyList->next);
          discovered[g.adjacencyList->next->vertexValue] = true;
          while(!queue.empty()) {
              std::list<T> current = *queue.front()->neighbors;
              out << queue.front()->vertexValue << " ";
              queue.pop();
              for(auto iter : current) {
                  if(!discovered[iter]) {
                      discovered[iter] = true;
                      queue.push(g.findVertex(iter));
                  }

              }
          }

          return out;
      }
};



/*********************************************
* Constructs an empty graph with a max number of verticies of 100
* 
*********************************************/
template<typename T>
Graph<T>::Graph() {
    this->maxVertices = 100;
    this->currentVertices = 0;
    adjacencyList = new Vertex<T>;
    adjacencyList->neighbors = nullptr;

}


/*********************************************
* Constructs an empty graph with a given max number of verticies
* 
*********************************************/
template<typename T>
Graph<T>::Graph(int maxVerticies){
    maxVertices = maxVerticies;
    currentVertices = 0;
    adjacencyList = new Vertex<T>;
    adjacencyList->neighbors = nullptr;

}




/*********************************************
* Adds a Vertex to the GraphIf number of verticies is less than the 
* Max Possible number of verticies.  
*********************************************/
template <typename T>
void Graph<T>::addVertex(T vertex){
    // if more than max, do not add
    if(currentVertices < maxVertices) {
        // create new vertex
        auto temp = new Vertex<T>;
        // update value
        temp->vertexValue = vertex;
        temp->neighbors = new std::list<T>;
        // if vertex list empty, insert directly after dummy vertex
        if(currentVertices == 0) {
            adjacencyList->next = temp;
        }
        // else traverse through vertexes until reaching the end to add new
        else {
            Vertex<T> *vPtr = adjacencyList->next;
            while (vPtr->next != nullptr) {
                vPtr = vPtr->next;
            }
            vPtr->next = temp;
        }
    }
        ++currentVertices;
}


/*********************************************
* Returns the current number of vertices
* 
*********************************************/
template<typename T>
int Graph<T>::getNumVertices(){
    // very simple, just return current vertices
    return currentVertices;
}



/*********************************************
* Returns the position in the verticies list where the given vertex is located, -1 if not found.
* 
*********************************************/
template <typename T>
int Graph<T>::findVertexPos(T item){
    int count = 0;
    Vertex<T>* vPtr = adjacencyList->next;
    // continually increases count value while the current vertex pointer reaches the sought item
    while(vPtr != nullptr) {
        if(vPtr->vertexValue == item) {
            return count;
        }
        vPtr = vPtr->next;
        ++count;
    }
    return -1; //return negative one
}//End findVertexPos

/*********************************************
* Adds an edge going in the direction of source going to target
* 
*********************************************/
template <typename T>
void Graph<T>::addEdge(T source, T target){
    int vertexPosition = findVertexPos(source);
    // this is mostly to ensure that the vertex exists
    if (vertexPosition >= 0) {
        auto current = findVertex(source);
        // push back the target into the list in the vertex, creating the edge or link
        current->neighbors->push_back(target);
    }
}

/*
  getPath will return the shortest path from source to dest.  
  You may use any traversal/search algorithm you want including
  breadth first, depth first, dijkstra's algorithm, or any
  other graph algorithm.
  You will return a vector with the solution.  The source will be in position 1
  the destination is in the last position of the solution, and each node in between 
  are the verticies it will travel to get to the destination.  There will not be any
  other verticies in the list.

  Given the test graph:
  
[0]-----------[2]
|  \            \
|   \            \
[1]  [3]----[6]---[7]   
|          /  \
|         /    \
|        /      \
|     [5]--------[8]
|    /   \       /
|   /     \     /
|  /       \   /
[4]         [9]

getPath(0, 5) should return 
0 -> 1 -> 4 -> 5   or   0 -> 3 -> 6 -> 5
    
  Hint: As you find the solution store it in a stack, pop all the items of the stack 
  into a vector so it will be in the correct order.

*/
template <typename T>
vector<T> Graph<T>::getPath(T source, T dest){
    vector<T> solution;
    std::stack<T> stack;
    // queue of vertex pointers to traverse for BFS
    std::queue<Vertex<T>*> queue;
    // finds the first vertex from the starting point
    Vertex<T>* first = findVertex(source);
    // setting the first starting point to 'visited'
    first->visited = true;
    queue.push(first);
    while(!queue.empty()) {
        Vertex<T>* current = queue.front();
        queue.pop();
        // for each that grabs each item from the neighbors list in each unvisited vertex
        for(T & item : *current->neighbors ) {
            // convert from T item back to vertex for manipulation
            Vertex<T>* iterator = findVertex(item);
            if(iterator->visited == false) {
                iterator->visited = true;
                queue.push(iterator);
                // using vertexValues to track predecessor
                iterator->predecessor = current->vertexValue;
            }
        }
    }
    // after BFS traversal and setting all paths to shortest path, reverse traverse to the source
    Vertex<T>* parentFinder = findVertex(dest);
    while(parentFinder != nullptr) {
        // adding path in reverse order to stack
        stack.push(parentFinder->vertexValue);
        parentFinder = findVertex(parentFinder->predecessor);
        // this is used because there are certain items that have both the same predecessor and vertexValue
        if(stack.top() == findVertex(source)->vertexValue) {
            break;
        }
    }
    // pop the stack and create the solution path
    while(!stack.empty()) {
        solution.push_back(stack.top());
        stack.pop();
    }
    return solution;
}

/**
 * @brief - this is an extra function that I created to return the memory address of a specific vertex
 * @tparam T
 * @param source
 * @return
 */

template <typename T>
Vertex<T>* Graph<T>::findVertex(T source) {
    // starting the vertex traversal after the head of the list (head of list is dummy vertex)
    Vertex<T>* vPtr = adjacencyList->next;
    while(vPtr != nullptr) {
        if(vPtr->vertexValue == source) {
            return vPtr;
        }
        vPtr = vPtr->next;
    }
}

/**
 * @brief - this is the destructor for the graph class.
 * @tparam T
 */


template<typename T>
Graph<T>::~Graph<T>() {
    Vertex<T>* current = adjacencyList->next;
    while(current != nullptr) {
        auto temp = current->next;
        if(current->neighbors->size() != 0) {
            delete current->neighbors;
        }
        delete current;
        current = nullptr;
        current = temp;
    }
    delete adjacencyList;
    adjacencyList = nullptr;
}

/**
 * @brief - this is the copy constructor. It creates a deep copy of another graph
 * @tparam T
 * @param other
 */

template<typename T>
Graph<T>::Graph(const Graph<T> &other) {
    // setting the vertex amounts to the other
    this->maxVertices = other.maxVertices;
    this->currentVertices = other.currentVertices;
    this->adjacencyList = new Vertex<T>;
    this->adjacencyList->neighbors = nullptr;
    auto current = this->adjacencyList;
    auto otherCurrent = other.adjacencyList->next;
    // iterating through the graph using a linked list style iterator
    while(otherCurrent != nullptr) {
        // creating new vertexes from the other graph and assigning values
        auto temp = new Vertex<T>;
        temp->vertexValue = otherCurrent->vertexValue;
        temp->visited = false;
        current->next = temp;
        // setting the vertex to the next vertex in the list
        current = current->next;
        otherCurrent = otherCurrent->next;
    }
    current = this->adjacencyList->next;
    otherCurrent = other.adjacencyList->next;
    // while loop that adds all neighbors into vertex lists
    while(otherCurrent != nullptr) {
        current->neighbors = new std::list<T>;
        // for each item in the other current vertex's neighbors list, create an edge in the new graph
        for (T & item: *otherCurrent->neighbors) {
            addEdge(otherCurrent->vertexValue, item);
        }
        // traversing to the next vertex in each graph
        otherCurrent = otherCurrent->next;
        current = current->next;
    }
}
#endif
