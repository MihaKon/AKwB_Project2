#include <algorithm>
#include <iostream>
#include <vector>
#include <fstream>
#include <utility>
#include <string>
#include <sstream>

const int SEQUENCE_MARGIN = 10;
const bool PRINT_ALL = false;

struct Node
{
    std::string sub_sequence;
    int seq_position;
    std::vector<std::pair<int, int>> adjacency;
};

struct Sequence
{
    std::string original_sequence;
    std::string modified_sequence = "";
    std::vector<int> quality;
    std::vector<int> correct_positions;
    std::vector<int> incorrect_positions;
    std::vector<Node> graph;
};

bool get_node_with_more_joints(const Node &n_one, const Node &n_two)
{
    return n_one.adjacency.size() > n_two.adjacency.size();
}

std::vector<Sequence> get_sequences(std::string file_name)
{

    std::ifstream file(file_name);
    Sequence currentSequence;
    std::string line = "";
    std::vector<Sequence> sequences;

    while (std::getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }
        if (line[0] == '>')
        {
            sequences.push_back(Sequence());
            continue;
        }
        if (isdigit(line[0]))
        {
            std::istringstream iss(line);
            int quality;
            while (iss >> quality)
            {
                sequences.back().quality.push_back(quality);
            }
        }
        else
        {
            sequences.back().original_sequence += line;
        }
    }

    file.close();
    return sequences;
}

Sequence remove_low_cred_nucleotides(Sequence seq, int min_credential_threshold)
{
    if (seq.original_sequence.length() != seq.quality.size())
    {
        std::cout << "ERROR: dlugosc sekwencji nie zgadza sie z dlugoscia jakosci\n";
        exit(1);
    }
    for (int j = 0; j < seq.original_sequence.length(); j++)
    {
        if (seq.quality[j] < min_credential_threshold)
        {
            seq.incorrect_positions.push_back(j);
            continue;
        }
        seq.modified_sequence += seq.original_sequence[j];
        seq.correct_positions.push_back(j);
    }
    return seq;
}

std::vector<Node> get_nodes(Sequence seq, int substring_len)
{
    std::vector<Node> graph;
    for (int j = 0; j < seq.modified_sequence.length() - substring_len; j++)
    {
        Node temp;
        temp.sub_sequence = seq.modified_sequence.substr(j, substring_len);
        temp.seq_position = seq.correct_positions[j];
        graph.push_back(temp);
    }
    return graph;
}

int get_shortest_sequence_index(const std::vector<Sequence> &sequences)
{
    int index = 0, shortest_sequence_length = sequences[0].modified_sequence.length();

    for (int i = 0; i < sequences.size(); i++)
    {
        if (sequences[i].modified_sequence.length() >= shortest_sequence_length)
        {
            continue;
        }
        shortest_sequence_length = sequences[i].modified_sequence.length();
        index = i;
    }

    return index;
}

bool is_all_true(const std::vector<bool> &exist)
{
    for (int i = 0; i < exist.size(); i++)
    {
        if (!exist[i])
        {
            return false;
        }
    }
    return true;
}

int main(int argc, char *argv[])
{
    std::string file_name = "";
    int min_credibility_threshold = 0;
    int substring_len = 0;

    switch (argc)
    {
    case 1:
        std::cout << "Podaj minimalny prog wiarygodnosci: ";
        std::cin >> min_credibility_threshold;
        std::cout << "Podaj minimalna dlugosc podciagu: ";
        std::cin >> substring_len;
        std::cout << "Podaj nazwę pliku:";
        std::cin >> file_name;
        break;
    case 2:
        min_credibility_threshold = std::stoi(argv[1]);
        std::cout << "Podaj minimalna dlugosc podciagu:  ";
        std::cin >> substring_len;
        std::cout << "Podaj nazwę pliku:";
        std::cin >> file_name;
        break;
    case 3:
        min_credibility_threshold = std::stoi(argv[1]);
        substring_len = std::stoi(argv[2]);
        std::cout << "Podaj nazwę pliku:";
        std::cin >> file_name;
        break;
    case 4:
        min_credibility_threshold = std::stoi(argv[1]);
        substring_len = std::stoi(argv[2]);
        file_name = argv[3];
        break;
    default:
        std::cout << "ERROR\n";
        return 1;
    }

    std::vector<Sequence> sequences = get_sequences(file_name);

    for (Sequence &seq : sequences)
    {
        seq = remove_low_cred_nucleotides(seq, min_credibility_threshold);
        seq.graph = get_nodes(seq, substring_len);
    }

    for (int i = 0; i < sequences.size(); i++)
    {
        for (int l = i + 1; l < sequences.size(); l++)
        {
            for (int j = 0; j < sequences[i].modified_sequence.length() - substring_len; j++)
            {
                int seq_start = std::max(0, j - SEQUENCE_MARGIN * substring_len);
                int seq_end = sequences[l].graph.size();
                seq_end = std::min(seq_end, j + SEQUENCE_MARGIN * substring_len);

                for (int z = seq_start; z < seq_end; z++)
                {
                    if (sequences[i].graph[j].sub_sequence != sequences[l].graph[z].sub_sequence)
                    {
                        continue;
                    }
                    sequences[i].graph[j].adjacency.push_back(std::make_pair(l, z));
                    sequences[l].graph[z].adjacency.push_back(std::make_pair(i, j));
                }
            }
        }
    }

    int shortest_seq_id = get_shortest_sequence_index(sequences);
    std::sort(sequences[shortest_seq_id].graph.begin(), sequences[shortest_seq_id].graph.end(), get_node_with_more_joints);

    for (int i = 0; i < sequences[shortest_seq_id].graph.size(); i++)
    {
        if (sequences[shortest_seq_id].graph[i].adjacency.size() < sequences.size()-1)
        {
            break;
        }

        std::vector<bool> exist(sequences.size(), false);
        exist[shortest_seq_id] = 1;

        for (int j = 0; j < sequences[shortest_seq_id].graph[i].adjacency.size(); j++)
        {
            exist[sequences[shortest_seq_id].graph[i].adjacency[j].first] = 1;
        }
        if (!is_all_true(exist))
        {
            continue;
        }

        std::cout << sequences[shortest_seq_id].graph[i].sub_sequence << "\n";
        for (int j = 0; j < sequences.size(); ++j)
        {
            if (j == shortest_seq_id)
            {
                std::cout << "sekwencja nr " << j + 1 << " pozycja " << sequences[shortest_seq_id].graph[i].seq_position + 1 << "\n";
            }
            else
            {
                for (int l = 0; l < sequences[shortest_seq_id].graph[i].adjacency.size(); ++l)
                {
                    if (sequences[shortest_seq_id].graph[i].adjacency[l].first == j)
                    {
                        std::cout << "sekwencja nr " << j + 1 << " pozycja " << sequences[j].graph[sequences[shortest_seq_id].graph[i].adjacency[l].second].seq_position + 1 << "\n";
                        break;
                    }
                }
            }
        }
        if (!PRINT_ALL)
        {
            break;
        }
    }

    return 0;
}
