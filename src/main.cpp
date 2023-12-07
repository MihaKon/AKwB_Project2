#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

struct Graph
{
    std::string name;
    int numberOfVertexes;
    int numberOfJoints;
    std::vector<std::vector<int>> adjList;
    bool isCoupled = false;
    bool isLinear = false;
};

void writeFile(std::string fileName, const std::vector<Graph> graphs)
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
        fileGraph << ">";
        fileGraph << graph.name << ",";
        fileGraph << graph.numberOfVertexes << "," << graph.numberOfJoints << "\n";
        for (int i = 0; i < graph.adjList.size(); i++)
        {
            if (graph.adjList[i].empty()) continue;
            for (int j = 0; j < graph.adjList[i].size(); j++)
            {
                fileGraph << "(" << i+1 << "-" << graph.adjList[i][j]+1 << "),";
            }
        }
        fileGraph << "\n";
    }
    fileGraph.close();
}

std::vector<Graph> readFile(std::string filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        exit(EXIT_FAILURE);
    }

    std::vector<Graph> graphs;

    std::string line;
    while (std::getline(file, line))
    {
        Graph graph;
        line.erase(std::remove(line.begin(), line.end(), '>'), line.end());

        std::istringstream iss(line);

        std::vector<std::string> result;

        while (iss.good())
        {
            std::string substring;
            std::getline(iss, substring, ',');
            result.push_back(substring);
        }

        graph.name = result[0];
        graph.numberOfVertexes = std::stoi(result[1]);
        graph.numberOfJoints = std::stoi(result[2]);

        graph.adjList.resize(graph.numberOfVertexes);

        std::string edges;
        std::getline(file, edges, '\n');
        std::stringstream ss(edges);
        int node1, node2;
        char c;
        for (int i = 0; i < graph.numberOfJoints; i++)
        {
            ss >> c;
            ss >> node1;
            ss >> c;
            ss >> node2;
            ss >> c;
            ss >> c;
            graph.adjList[node1 - 1].push_back(node2 - 1);
        }
        graphs.push_back(graph);
    }

    file.close();

    return graphs;
}

class GraphReader
{
private:
    std::vector<Graph> graphs;
    std::vector<Graph> convertedGraphs;

public:
    std::vector<std::vector<int>>  sortAdjList(std::vector<std::vector<int>> adjList){
        for (std::vector<int> joints : adjList){
            std::sort(joints.begin(), joints.end());
        }
        return adjList;
    }

    bool checkCommon(std::vector<int> vectorOne, std::vector<int> vectorTwo)
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
                bool common = checkCommon(graph.adjList[i], graph.adjList[j]);
                if (common && !graph.adjList[j].empty() && graph.adjList[i] != graph.adjList[j])
                    return false;
            }
        }
        return true;
    }

    std::vector<int> findPredecessors(Graph graph, int targetVertex)
    {
        std::vector<int> predecessors;

        for (int vertex = 0; vertex < graph.numberOfVertexes; vertex++)
        {
            std::vector<int> neighbors = graph.adjList[vertex];
            if (find(neighbors.begin(), neighbors.end(), targetVertex) != neighbors.end())
            {
                predecessors.push_back(vertex + 1);
            }
        }

        return predecessors;
    }

    bool isGraphLinear(Graph graph)
    {
        for (int i = 1; i < graph.numberOfVertexes; i++)
        {
            for (int j = i - 1; j < graph.numberOfVertexes; j++)
            {
                if (graph.adjList[j].empty() || graph.adjList[i].empty())
                {
                    continue;
                }
                if (graph.adjList[j][0] != graph.adjList[i][0])
                {
                    return false;
                }
                std::vector<int> predecessorsVertex = findPredecessors(graph, j);
                std::vector<int> predecessorsNeighbor = findPredecessors(graph, i);

                if (predecessorsVertex != predecessorsNeighbor)
                {
                    return false;
                }
            }
        }
        return true;
    }

    int getCorrespondingIndex(unsigned int index)
    {
        if (index == 0)
        {
            return index;
        }
        else if (index % 2 == 0)
        {
            return index + 2;
        }
        else
        {
            return index + 1;
        }
        throw std::invalid_argument("Index out of scope!");
    }

    int countJoints(std::vector<std::vector<int>> adjList){
        int numberOfJoints = 0;
        for (std::vector<int> joint : adjList){
            numberOfJoints = numberOfJoints + joint.size();
        }
        return numberOfJoints;
    }

    void convert(Graph graph)
    {
        Graph convertedGraph;
        convertedGraph.name = graph.name;
        std::vector<int> removedVertecies;
        for (int i = 0; i < graph.numberOfVertexes * 2; i++)
        {
            if (i % 2 == 0)
                convertedGraph.adjList.push_back(std::vector<int>{i + 1});
            else
                convertedGraph.adjList.push_back(std::vector<int>{});
        }
        for (int i = 0; i < graph.numberOfVertexes; i++)
        {
            if (graph.adjList[i].empty())
            {
                continue;
            }
            convertedGraph.adjList[i] = {};
            for (int j = 0; j < graph.adjList[i].size(); j++)
            {
                int index = getCorrespondingIndex(i);
                convertedGraph.adjList[index].push_back(getCorrespondingIndex(graph.adjList[i][j]));
                removedVertecies.push_back(index);
            }
        }
        convertedGraph.numberOfJoints = countJoints(convertedGraph.adjList);
        convertedGraph.numberOfVertexes = convertedGraph.adjList.size();
        convertedGraphs.push_back(convertedGraph);
    }

    void runForEeveryGraph()
    {
        std::cout << "Checking And Converting" << std::endl;
        for (Graph graph : graphs)
        {
            graph.adjList = sortAdjList(graph.adjList);
            std::cout << graph.name << std::endl;
            graph.isCoupled = isGraphCoupled(graph);
            if (graph.isCoupled)
            {
                std::cout << "Graph is coupled" << std::endl;
                if (isGraphLinear(graph))
                {
                    graph.isLinear = true;
                    std::cout << "Graph is Linear" << std::endl;
                }
                else
                {
                    std::cout << "Graph is not Linear" << std::endl;
                }
                convert(graph);
            }
            else
            {
                std::cout << "Graph is not coupled" << std::endl;
            }
        }
    }
    void readfile(std::string filename)
    {
        graphs = readFile(filename);
    }
    void writeResults(std::string filename)
    {
        std::cout << "Saving results" << std::endl;
        writeFile(filename, convertedGraphs);
    }
};

int main(int argc, char *argv[])
{
    std::string name = "Test.txt";
    GraphReader test;
    std::cout << "Reading " << name << std::endl;
    test.readfile(name);
    test.runForEeveryGraph();
    test.writeResults(name);
    std::cout << "END" << std::endl;
    return 0;
}
