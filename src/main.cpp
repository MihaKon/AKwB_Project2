#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <set>

struct Graph
{
    int numberOfVertexes;
    std::vector<std::vector<int>> adjList;
    bool isCoupled = false;
    bool isLinear = false;
};

void writeFile(const std::string &fileName, const Graph &graph)
{
    std::string toFileName = "result_" + fileName;
    std::ofstream fileGraph;
    fileGraph.open(toFileName, std::ios::out);
    if (!fileGraph)
    {
        std::cout << "Cannot create/open " << toFileName << std::endl;
        return;
    }

    fileGraph << graph.adjList.size() << "\n";
    for (int i = 0; i < graph.adjList.size(); i++)
    {
        fileGraph << i << ":";
        for (int j = 0; j < graph.adjList[i].size(); j++)
        {
            fileGraph << " " << graph.adjList[i][j];
        }
        fileGraph << "\n";
    }
    fileGraph << "\n";
    fileGraph.close();
}

void readFile(const std::string &filename, Graph &graph)
{
    std::ifstream file(filename);

    if (!file.is_open())
    {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    if (!(file >> graph.numberOfVertexes))
    {
        std::cerr << "Error reading the number of vertices from file." << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }

        std::stringstream ss(line);
        std::vector<int> nums;
        std::string temp;
        int found;

        ss >> temp;

        while (!ss.eof())
        {
            ss >> temp;
            if (std::stringstream(temp) >> found)
                nums.push_back(found);
            temp = "";
        }
        graph.adjList.push_back(nums);
    }

    file.close();
}

std::vector<std::vector<int>> sortAdjList(std::vector<std::vector<int>> adjList, int numberOfVertexes)
{
    for (int i = 0; i < numberOfVertexes; i++)
    {
        std::sort(adjList[i].begin(), adjList[i].end());
    }
    return adjList;
}

bool checkCommon(std::vector<int> &vectorOne, std::vector<int> &vectorTwo)
{
    return std::find_first_of(vectorOne.begin(), vectorOne.end(), vectorTwo.begin(), vectorTwo.end()) != vectorOne.end();
}

bool isGraphCoupled(Graph &graph)
{
    for (int i = 0; i < graph.numberOfVertexes; i++)
    {
        const bool hasDuplicates = std::adjacent_find(graph.adjList[i].begin(), graph.adjList[i].end()) != graph.adjList[i].end();
        if (hasDuplicates)
            return false;
        for (int j = i + 1; j < graph.numberOfVertexes; j++)
        {
            if (graph.adjList[i] == graph.adjList[j])
                continue;
            bool common = checkCommon(graph.adjList[i], graph.adjList[j]);
            if (common && !graph.adjList[j].empty())
                return false;
        }
    }
    return true;
}

std::vector<int> findPredecessors(Graph &graph, int targetVertex)
{
    std::vector<int> predecessors;

    for (int vertex = 0; vertex < graph.adjList.size(); vertex++)
    {
        std::vector<int> neighbors = graph.adjList[vertex];
        if (find(neighbors.begin(), neighbors.end(), targetVertex) != neighbors.end())
        {
            predecessors.push_back(vertex + 1);
        }
    }

    return predecessors;
}

bool isGraphLinear(Graph &graph)
{
    bool nexts = true;
    bool prevs = false;
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
                nexts = false;
                continue;
            }
            std::vector<int> predecessorsVertex = findPredecessors(graph, j);
            std::vector<int> predecessorsNeighbor = findPredecessors(graph, i);

            if (predecessorsVertex == predecessorsNeighbor)
            {
                prevs = true;
            }
            if (prevs && nexts)
                return false;
        }
    }
    return true;
}

int countBigger(int v, const std::set<int> &removedVertices)
{
    auto it = removedVertices.lower_bound(v);
    int count = std::distance(removedVertices.begin(), it);

    return count;
}
Graph convert(Graph graph)
{
    Graph convertedGraph;
    std::set<int> removedVertices;
    for (int i = 0; i < graph.adjList.size() * 2; i++)
    {
        if (i % 2 == 0)
            convertedGraph.adjList.push_back(std::vector<int>{i + 1});
        else
            convertedGraph.adjList.push_back(std::vector<int>{});
    }
    for (int i = 0; i < graph.adjList.size(); i++)
    {
        if (graph.adjList[i].empty())
        {
            continue;
        }
        convertedGraph.adjList[i * 2] = {};
        removedVertices.insert(i * 2 + 1);
        for (int j = 0; j < graph.adjList[i].size(); j++)
        {
            convertedGraph.adjList[i * 2].push_back(graph.adjList[i][j] * 2);
        }
    }

    for (int i = 0; i < convertedGraph.adjList.size(); i++)
    {
        for (int j = 0; j < convertedGraph.adjList[i].size(); j++)
        {
            int value = convertedGraph.adjList[i][j];
            convertedGraph.adjList[i][j] = value - countBigger(value, removedVertices);
        }
    }
    int count = 0;
    for (auto i : removedVertices)
    {
        convertedGraph.adjList.erase(convertedGraph.adjList.begin() + (i - count));
        count++;
    }
    convertedGraph.numberOfVertexes = convertedGraph.adjList.size();
    return convertedGraph;
}

int main(int argc, char *argv[])
{
    std::string name = "Test.txt";
    Graph data, convertedGraph;
    std::cout << "Reading " << name << std::endl;
    readFile(name, data);
    data.adjList = sortAdjList(data.adjList, data.numberOfVertexes);
    data.isCoupled = isGraphCoupled(data);
    if (data.isCoupled)
    {
        if (isGraphLinear(data))
        {
            data.isLinear = true;
            std::cout << "Graph is coupled and Linear" << std::endl;
        }
        else
        {
            std::cout << "Graph is coupled, but not Linear" << std::endl;
        }
        convertedGraph = convert(data);
    }
    else
    {
        std::cout << "Graph is not coupled" << std::endl;
    }
    writeFile(name, convertedGraph);
    std::cout << "END" << std::endl;
    return 0;
}
