#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

void writeFile(std::string fileName, const Graph& graph)
{
    std::string toFileName = "result_" + fileName;
    std::ofstream fileGraph;
    fileGraph.open(toFileName, std::ios::out);
    if (!fileGraph)
    {
        std::cout << "Cannot create/open " << toFileName << std::endl;
        return;
    }

    for (Graph graph : graphs)
    {
        if (graph.name.empty())
            continue;

        fileGraph << graph.name << ",";
        fileGraph << graph.numberOfVertexes << "," << graph.numberOfJoints << "\n";
        // TODO: ADD reading adj matrix
        fileGraph << ";\n";
    }
    fileGraph.close();
}

void readFile(std::string filename)
{
    std::ifstream fileGraph;
    fileGraph.open(fileName, std::ios::in);
    if (!fileGraph.is_open())
    {
        std::cout << "No such file" << std::endl;
        return;
    }
    std::string line;
    while (std::getline(fileGraph, line))
    {
        Graph graph;
        if (line.empty())
            continue;
        if (line[0] == '>')
        {
            std::istringstream iss(line.substr(1));
            std::string nv, nj;
            std::getline(iss, graph.name, ',');
            std::getline(iss, nv, ',');
            std::getline(iss, nj, ',');
            graph.numberOfJoints = std::stoi(nv);
            graph.numberOfVertexes = std::stoi(nj);
        }
        else
        {
            
        }
        graphs.push_back(graph);
    }
    fileGraph.close();
}

struct Graph
{
    std::string name;
    int numberOfVertexes;
    int numberOfJoints;
    std::vector<std::vector<int>> adjList;
    bool isCoupled = false;
    bool isLinear = false;
};

class GraphReader
{
private:
    std::vector<Graph> graphs;
    std::vector<Graph> convertedGraphs;

public:
    void addGraph(Graph graph){
        graphs.push_back(graph);
    }

    bool checkCommon(const std::vector<int>& vectorOne, const std::vector<int>& vectorTwo)
    {
        return std::find_first_of(vectorOne.begin(), vectorOne.end(), vectorTwo.begin(), vectorTwo.end()) != vectorOne.end();
    }

    bool isGraphCoupled(Graph graph)
    {
        for (int i = 0; i < graph.numberOfVertexes; i++)
        {
            if (graph.adjList[i].empty())
                continue;
            const bool hasDuplicates = std::adjacent_find(graph.adjList[i].begin(), graph.adjList[i].end()) != graph.adjList[i].end();
            if (hasDuplicates)
                return false;
            for (int j = i + 1; j < graph.numberOfVertexes; j++)
            {   
                bool common = checkCommon(graph.adjList[i],graph.adjList[j]);
                if (common && !graph.adjList[j].empty()&& graph.adjList[i] != graph.adjList[j])
                    return false;
            }
        }
        return true;
    }

    std::vector<int> findPredecessors(const Graph& graph, int targetVertex) {
        std::vector<int> predecessors;

        for (size_t vertex = 0; vertex < graph.numberOfVertexes; ++vertex) {
            const std::vector<int>& neighbors = graph.adjList[vertex];
            if (find(neighbors.begin(), neighbors.end(), targetVertex) != neighbors.end()) {
                predecessors.push_back(vertex + 1);
            }
        }

        return predecessors;
    }

    bool isGraphLinear(Graph graph)
    {
        for (int i = 1; i < graph.numberOfVertexes; i++)
        {   
            for(int j=i-1;j<graph.numberOfVertexes; j++) {
                if (graph.adjList[j].empty() || graph.adjList[i].empty()) {
                    continue;
                }
                if (graph.adjList[j][0] != graph.adjList[i][0]) {
                    return false;
                }
                std::vector<int> predecessorsVertex = findPredecessors(graph, j);
                std::vector<int> predecessorsNeighbor = findPredecessors(graph, i);
                
                if (predecessorsVertex != predecessorsNeighbor) {
                    return false;
                }
            }
        }
        return true;
    }
    
    void convert(const Graph& graph)
    {
        Graph convertedGraph;
        for (int i = 0; i < graph.numberOfVertexes*2; i++)
        {
            if(i % 2 == 0)
                convertedGraph.adjList.push_back(std::vector<int> {i+1});
            else
                convertedGraph.adjList.push_back(std::vector<int> {});
        }
    }

    void runForEeveryGraph()
    {
        for (Graph graph : graphs)
        {
            graph.isCoupled = isGraphCoupled(graph);
            if (graph.isCoupled)
            {
                graph.isLinear = isGraphLinear(graph);
                convert(graph);
            }
        }
    }
};

int main(int argc, char *argv[])
{
    std::string name = "Test.txt";
    GraphReader test;
    std::cout << "Reading " << name << std::endl;
    readFile(name);
    std::cout << "Checking" << std::endl;
    test.runForEeveryGraph();
    // std::cout << "Saving results" << std::endl;
    std::cout << "END" << std::endl;
    return 0;
}
