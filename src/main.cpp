#include <iostream>
#include <fstream>
#include <vector>

struct Graph{
	std::string name;
	int numberOfVertexes;
	int numberOfJoints;
	std::vector<int> adjMatrix;
}

class GraphReader{
private:
    std::string fileName;
    std::vector<Graph> GraphsVector;
public:
    GraphReader(std::string fileName){
        fileName = fileName;
    }
    GraphReader(char fileName){
        fileName = fileName;
    }
    void readFile(){
        if(fileName.empty()){
            std::cout << "No file name parsed!" << std::endl;
            return;
        }
        std::fstream fileGraph;
        fileGraph.open(fileName, std::ios::in);
        if (!fileGraph) {
            std::cout << "No such file" << std::endl;;
            return;
        }
        std::string fileContent;
        while (1) {
			fileGraph >> fileContent;
			if (fileGraph.eof())
				break;
        }
        fileContent.getline();
    }
    void convertToAdjMatrix(){
	
    }
};

int main() {
    std::string name = "Test.txt";
    GraphReader test(name);
    return 0;
}