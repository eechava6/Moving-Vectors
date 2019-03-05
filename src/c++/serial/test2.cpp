#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

int main()
{
    std::string line;
    std::vector<std::vector<std::string>> lines;
    std::ifstream file("articles1.csv");

    if (file)
    {
        while (std::getline(file, line))
        {
            size_t n = lines.size();
            lines.resize(n + 1);

            std::istringstream ss(line);
            std::string field, push_field("");
            bool no_quotes = true;

            while (std::getline(ss, field, ',')) 
            {
                if (static_cast<size_t>(std::count(field.begin(), field.end(), '"')) % 2 != 0)
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
    }

    for (auto line : lines)
    {
        for (auto field : line)
        {
            std::cout << "| " << field << " |";
        }

        std::cout << std::endl << std::endl;
    }

    return 0;
}