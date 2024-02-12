#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <string>
#include <algorithm>

class Sequence
{
private:
    std::vector<int> inputData;
    std::vector<bool> isInputChecked;
    std::vector<int> resultMap;
    bool isInputSizeCorrect;

public:
    Sequence(std::vector<int> input)
    {
        std::sort(input.begin(), input.end());
        inputData = input;
        isInputChecked = std::vector<bool>(input.size(), false);
    }

    bool checkInputSize()
    { 
        if (ceil((sqrt(1 + 8 * inputData.size()) - 3) / 2) != floor((sqrt(1 + 8 * inputData.size()) - 3) / 2))
        {
            isInputSizeCorrect = false;
            return false;
        }
        isInputSizeCorrect = true;
        return true;
    }

    void insertFirstMapValue()
    {
        int value = inputData.rbegin()[0] - inputData.rbegin()[1];

        resultMap.push_back(value);
        for (int i = 0; i < inputData.size(); i++)
        {
            if (inputData[i] != value)
            {
                continue;
            }
            isInputChecked[i] = true;
            break;
        }
    }

    bool getResultMap()
    {
        if (std::all_of(isInputChecked.begin(), isInputChecked.end(), [](bool v)
                        { return v; }))
        {
            return true;
        }

        std::vector<int> positions;

        for (int i = 0; i < inputData.size(); i++)
        {
            if (isInputChecked[i])
            {
                continue;
            }
            resultMap.push_back(inputData[i]);
            int sum = 0;
            bool isValid = true;
            for (int j = resultMap.size() - 1; j >= 0; j--)
            {
                sum += resultMap[j];
                bool isFound = false;
                for (int k = 0; k < inputData.size(); k++)
                {
                    if (inputData[k] == sum && !isInputChecked[k])
                    {
                        isInputChecked[k] = true;
                        positions.push_back(k);
                        isFound = true;
                        break;
                    }
                }
                if (!isFound)
                {
                    isValid = false;
                    break;
                }
            }

            if (isValid)
            {
                if (getResultMap())
                {
                    return true;
                }
            }

            resultMap.pop_back();
            for (int pos : positions)
            {
                isInputChecked[pos] = false;
            }
        }
        return false;
    }

    void printResultMap()
    {
        std::cout << "Result map: (";
        for (int n : resultMap)
        {
            std::cout << n << ", ";
        }
        std::cout << "\b\b)\n";
    }
};

std::vector<int> read_file(const std::string &file_name)
{
    std::string line;
    std::vector<int> set;
    std::ifstream file(file_name);

    while (file >> line)
    {
        set.push_back(stoi(line));
    }
    file.close();

    return set;
}

int main(int argc, char *argv[])
{
    std::string file_name;
    switch (argc)
    {
    case 1:
        std::cout << "Enter file name: ";
        std::cin >> file_name;
        break;
    case 2:
        file_name = argv[1];
        break;
    default:
        std::cout << "No file name parsed!\n";
        return 1;
    }

    Sequence seq(read_file(file_name));

    if (!seq.checkInputSize())
    {
        std::cout << "Incorrect size of map!\n";
        std::cout << "No solution";
        return 1;
    }

    std::chrono::high_resolution_clock::time_point begin = std::chrono::high_resolution_clock::now();
    seq.insertFirstMapValue();
    bool is_map = seq.getResultMap();
    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();

    std::cout << "Time duration: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms\n";

    if (!is_map)
    {
        std::cout << "No solution";
        return 1;
    }

    seq.printResultMap();

    return 0;
}