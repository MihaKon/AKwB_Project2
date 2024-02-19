#include <algorithm>
#include <iostream>
#include <vector>
#include <fstream>
#include <utility>
#include <string>

using namespace std;
struct Sequence
{
    string orginal_sequence;
    string modified_sequence;
    vector<int> quality;
    vector<int> correct_positions;
};

struct Node
{
    string sub_sequence;
    int seq_position;
    vector<pair<int,int>> adjecency;
};

vector<Sequence> sequences(6);
vector<vector<Node>> graph(6);

bool Mr_sorter(Node key_1, Node key_2){
    return key_1.adjecency.size() > key_2.adjecency.size();
}

int main()
{
    ifstream fasta("instance_5.txt");
    string line = "";
    int k = 0;
    int q = 0;

     for(int i=0; i >= 0 ; i++)
     {
         cout << "Enter the length of the oligonucleotide (the motif you are looking for):";
         cin >> k;
         if(k>=4 && k <=9)
         {
             break;
         }
         else
         {
             cout << "ERROR" << endl;
             continue;
         }
     }
     cout << "Enter the reliability of an individual nucleotide: ";
     cin >> q;



    for(int i = 0; getline(fasta, line);)
    {

        if(line[0] == '>')
        {
            ++i;
            continue;
        }

        if(isdigit(line[0]))
        {
            for(int j = 0; j < line.length(); j+= 3)
            {
                string number = "";
                number += line[j];
                number += line[j+1];
                sequences[i].quality.push_back(stoi(number));
            }
        }
        else
        {
            sequences[i].orginal_sequence = line;
        }
    }

    for(int i = 1; i<6; i++)
    {
        if(sequences[i].orginal_sequence.length() == sequences[i].quality.size())
        {
            sequences[i].modified_sequence = "";
            for(int j = 0; j < sequences[i].orginal_sequence.length(); ++j)
            {
                if(sequences[i].quality[j] >= q)
                {
                    sequences[i].modified_sequence += sequences[i].orginal_sequence[j];
                    sequences[i].correct_positions.push_back(j+1);
                }
            }
        }
        else
        {
            cout << "ERROR";
        }
    }

    string k_mer = "";

    for(int i = 1; i < 6; i++)
    {
        for(int j = 0; j < sequences[i].modified_sequence.length() - k; ++j)
        {
            Node temp_graph;

            temp_graph.sub_sequence = sequences[i].modified_sequence.substr(j, k);
            temp_graph.seq_position = sequences[i].correct_positions[j];
            graph[i].push_back(temp_graph);
        }
    }

    for(int i = 1; i < 6; ++i)
    {
        for(int j = 0; j < graph[i].size(); ++j)
        {
            for(int l = i+1; l < 6; ++l)
            {
                if (i==l)
                {
                    continue;
                }

                int m, m_end;
                if(j - 10 * k < 0)
                {
                    m = 0;
                }
                else
                {
                    m = j - 10 * k;
                }
                if(j + 10 * k < graph[l].size())
                {
                    m_end = j + 10 * k;
                }
                else
                {
                    m_end = graph[l].size();
                }

                for(; m < m_end; ++m)
                {
                    if(graph[i][j].sub_sequence == graph[l][m].sub_sequence)
                    {
                        graph[i][j].adjecency.push_back(make_pair(l,m));
                        graph[l][m].adjecency.push_back(make_pair(i,j));
                    }
                }
            }
        }
    }

    int shortest_sequence = sequences[1].modified_sequence.length();
    int ID = 1;

    for(int i = 2; i<6; i++)
    {
        if(shortest_sequence > sequences[i].modified_sequence.length())
        {
            shortest_sequence = sequences[i].modified_sequence.length();
            ID = i;
        }
    }

    sort(graph[ID].begin(), graph[ID].end(), Mr_sorter);

    for(int i = 0; i < graph[ID].size(); ++i)
    {
        if(graph[ID][i].adjecency.size() >= 4)
        {
            bool exist[6] = {1,0,0,0,0,0};
            exist[ID] = 1;
            bool check = true;

            for(int j = 0; j < graph[ID][i].adjecency.size(); ++j)
            {
                exist[graph[ID][i].adjecency[j].first] = 1;
            }

            for(int l = 0; l < 6; ++l)
            {
                if(!exist[l])
                {
                    check = false;
                    break;
                }
            }

            if(check)
            {
                cout << graph[ID][i].sub_sequence << endl;
                for(int j = 1; j < 6; ++j)
                {
                    if(j == ID)
                    {
                        cout << "sekwencja nr " << j << " pozycja " << graph[ID][i].seq_position << endl;
                    }
                    else
                    {
                        for(int l = 0; l < graph[ID][i].adjecency.size(); ++l)
                        {
                            if(graph[ID][i].adjecency[l].first == j)
                            {
                                cout << "sekwencja nr " << j << " pozycja " << graph[j][graph[ID][i].adjecency[l].second].seq_position << endl;
                                break;
                            }
                        }
                    }
                }
                break;
            }
        }
        else
        {
            break;
        }
    }



    return 0;
}
