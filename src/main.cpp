#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <unordered_set>
#include <set>

struct Graph
{
    int numberOfVertexes;
    std::vector<std::vector<int>> adjList;
    bool isCoupled = false;
    bool isLinear = false;
};

struct ConvertedGraph
{
    int index_vertex;
    std::vector<int> adjList;

    ConvertedGraph(int index, std::vector<int> adj) : index_vertex(index), adjList(adj){};
    ConvertedGraph(){};
};

struct Vertex
{
    int entrance;
    int exit;

    Vertex(int ent, int ex) : entrance(ent), exit(ex){};
    Vertex(){};
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

std::vector<std::vector<int>> sortAdjList(std::vector<std::vector<int>> adjList)
{
    for (int i = 0; i < adjList.size(); i++)
    {
        std::sort(adjList[i].begin(), adjList[i].end());
    }
    return adjList;
}

bool checkCommon(const std::vector<int> &vec1, const std::vector<int> &vec2)
{
    std::unordered_set<int> set2(vec2.begin(), vec2.end());
    for (int element : vec1)
    {
        if (set2.count(element) > 0)
        {
            return true;
        }
    }
    return false;
}

bool isGraphCoupled(const Graph &graph)
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

std::vector<std::vector<int>> getAdjMatrix(const Graph &graph)
{
    std::vector<std::vector<int>> matrix(graph.adjList.size(), std::vector<int>(graph.adjList.size(), 0));
    for (int i = 0; i < graph.adjList.size(); i++)
    {
        for (int j = 0; j < graph.adjList[i].size(); j++)
        {
            matrix[i][graph.adjList[i][j]] = 1;
        }
    }
    return matrix;
}
bool isGraphLinear(const Graph &graph)
{
    std::vector<std::vector<int>> matrix = getAdjMatrix(graph);
    for (int i = 0; i < graph.adjList.size(); i++)
    {
        for (int j = i + 1; j < graph.adjList.size(); j++)
        {
            if (!checkCommon(graph.adjList[i], graph.adjList[j]))
            {
                continue;
            }
            if (graph.adjList[i] != graph.adjList[j])
            {
                return false;
            }
            for (int index = 0; index < graph.adjList.size(); index++)
            {
                if (matrix[index][i] && matrix[index][i] == matrix[index][j])
                {
                    return false;
                }
            }
        }
    }

    return true;
}

Graph convert(Graph graph)
{
    Graph convertedGraph;
    std::vector<ConvertedGraph> adjListH;
    std::vector<Vertex> edges;

    for (int i = 0; i < graph.adjList.size(); ++i)
    {
        edges.push_back(Vertex(2 * i + 1, 2 * i));
    }

    for (int i = 0; i < graph.adjList.size(); i++)
    {
        for (int j = 0; j < graph.adjList.size(); j++)
        {
            if (graph.adjList[j].empty() || graph.adjList[j].empty() || graph.adjList[i] != graph.adjList[j])
            {
                continue;
            }
            edges[j].entrance = edges[i].entrance;
        }
        for (int j = 0; j < graph.adjList[i].size(); j++)
        {
            edges[graph.adjList[i][j]].exit = edges[i].entrance;
        }
    }

    for (int i = 0; i < edges.size(); i++)
    {
        bool isEntrance(false), isExit(false);
        int index;

        for (int j = 0; j < adjListH.size(); j++)
        {
            if (adjListH[j].index_vertex == edges[i].exit)
            {
                index = j;
                isExit = true;
                break;
            }
        }

        if (isExit)
        {
            adjListH[index].adjList.push_back(edges[i].entrance);
        }
        else
        {
            adjListH.push_back(ConvertedGraph(edges[i].exit, {edges[i].entrance}));
        }

        for (int j = 0; j < adjListH.size(); j++)
        {
            if (adjListH[j].index_vertex == edges[i].entrance)
            {
                isEntrance = true;
                break;
            }
        }
        if (!isEntrance)
        {
            adjListH.push_back(ConvertedGraph(edges[i].entrance, {}));
        }
    }

    for (int i = 0; i < adjListH.size(); i++)
    {
        for (int j = 0; j < adjListH[i].adjList.size(); j++)
        {
            for (int k = 0; k < adjListH.size(); k++)
            {
                if (adjListH[k].index_vertex == adjListH[i].adjList[j])
                {
                    adjListH[i].adjList[j] = k;
                    break;
                }
            }
        }
    }

    for (ConvertedGraph i : adjListH)
    {
        convertedGraph.adjList.push_back(i.adjList);
    }

    return convertedGraph;
}

int main(int argc, char *argv[])
{
    std::string name = "Test.txt";
    Graph originalGraph, convertedGraph;
    std::cout << "Reading " << name << std::endl;
    readFile(name, originalGraph);
    originalGraph.adjList = sortAdjList(originalGraph.adjList);
    originalGraph.isCoupled = isGraphCoupled(originalGraph);
    if (originalGraph.isCoupled)
    {
        if (isGraphLinear(originalGraph))
        {
            originalGraph.isLinear = true;
            std::cout << "Graph is coupled and linear" << std::endl;
        }
        else
        {
            std::cout << "Graph is coupled, but not linear" << std::endl;
        }
        convertedGraph = convert(originalGraph);
    }
    else
    {
        std::cout << "Graph is not coupled" << std::endl;
    }
    writeFile(name, convertedGraph);
    std::cout << "END" << std::endl;
    return 0;
}
