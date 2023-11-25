#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

struct Graph
{
    std::string name;
    int numberOfVertexes = 3;
    int numberOfJoints;
    std::vector<std::vector<int>> adjList;
    bool isCoupled = false;
    bool isLinear = false;
};

class GraphReader
{
private:
    std::string fileName;
    std::vector<Graph> graphs;
    std::vector<Graph> convertedGraphs;

public:
    GraphReader(std::string file)
    {
        fileName = file;
    }
    void readFile()
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
                bool common = checkCommon(graph.adjList[i],graph.adjList[j]);
                if (common && !graph.adjList[j].empty()&& graph.adjList[i] != graph.adjList[j])
                    return false;
            }
        }
        return true;
    }
    bool isGraphLinear(Graph graph)
    {
        for (int i = 0; i < graph.numberOfVertexes; i++)
        {
            const bool hasDuplicates = std::adjacent_find(graph.adjList[i].begin(), graph.adjList[i].end()) != graph.adjList[i].end();
            if (hasDuplicates)
                return false;
            for (int j = i + 1; j < graph.numberOfVertexes; j++)
            {
                if (!graph.adjList[j].empty() && graph.adjList[i] != graph.adjList[j])
                    return false;
            }
        }
        return true;
    }
    void convert(Graph graph)
    {
        Graph convertedGraph;
        for (int i = 0; i < graph.numberOfVertexes * 2; i++)
        {
            if (i % 2 == 0 || i == 0)
                convertedGraph.adjList.push_back(std::vector<int>{i + 1});
            else
                convertedGraph.adjList.push_back(std::vector<int>{});
        }
        bool isOdd = true;
        for (int i = 0; i < graph.numberOfVertexes; i++)
        {
            if (graph.adjList[i].empty())
                continue;
            for (int j = 0; j < graph.adjList[i].size(); j++)
            {
                int vertex = graph.adjList[i][j];
                convertedGraph.adjList[vertex].push_back(vertex + 1);
                convertedGraph.adjList[vertex + 1] = {};
                convertedGraph.adjList.erase(convertedGraph.adjList.begin() + 2);
            }
        }
    }
    void writeFile()
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
    GraphReader test(name);
    std::cout << "Reading " << name << std::endl;
    test.readFile();
    // std::cout << "Checking" << std::endl;
    // test.runForEeveryGraph();
    // // std::cout << "Saving results" << std::endl;
    // // test.writeFile();
    // std::cout << "END" << std::endl;
    return 0;
}
