#include <iostream>
#include <fstream>
#include <vector>

struct Joint {
    std::string name;
    int vFrom;
    int vTo;
}

struct Graph{
	std::string name;
	int numberOfVertexes;
	int numberOfJoints;
	std::vector<Joint> joints;
}

class GraphReader{
private:
    std::string fileName;
    std::vector<Graph> graphs;
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
    bool isCoupled(){}
    bool isLIne() {}
    void convertToCoupled() {}
    void writeFile(){
        std::string toFileName;
        std:: fstream fileGraph;
        fileGraph.open(toFileName, std::ios::out);
        if (!fileGraph) {
            std::cout << "Cannot create/open " << toFileName << std::endl;
            return
        }
        for (Graph graph : graphs) {
            fileGraph << ">" << graph.name << ",";
            fileGraph << graph.numberOfVertexes << "," << graph.numberOfJoints << "\n";
            for (Joint joint : graph.joints) {
                fileGraph << "(";
                if(joint.name) {
                    fileGraph << joint.name << ",";
                }
                fileGraph << joint.vFrom;
                if(joint.vTo) {
                    fileGraph << "-" << joint.vTo;
                }
                fileGraph << "),";
            }
            fileGraph << ";\n";
        }
        fileGraph.close();
        
    }
};

int main() {
    std::string name = "Test.txt";
    GraphReader test(name);
    return 0;
}
