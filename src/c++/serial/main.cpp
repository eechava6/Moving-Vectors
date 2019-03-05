#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
int main()
{
    string line;
    vector<vector<string>> lines;
    ifstream file("articles1.csv");

    if (file)
    {
        while (getline(file, line))
        {
            size_t n = lines.size();
            lines.resize(n + 1);

            istringstream ss(line);
            string field, push_field("");
            bool no_quotes = true;

            while (getline(ss, field, ',')) 
            {
                if (static_cast<size_t>(count(field.begin(), field.end(), '"')) % 2 != 0)
                {
                    no_quotes = !no_quotes;
                }

                push_field += field + (no_quotes ? "" : ",");

                if (no_quotes)
                {
                    lines[n].push_back(push_field);
                    push_field.clear();
                }
            }
        }
    }else{
        cout << "File not found";
    }

    for (auto line : lines)
    {
        for (auto field : line)
        {
            cout << "| " << field << " |";
        }

        cout << endl << endl;
    }

    return 0;
}