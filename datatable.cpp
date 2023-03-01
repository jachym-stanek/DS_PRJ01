#include "datatable.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <regex>



// remove white spaces and split into vector (size of strings unknown in advance)
std::vector<std::string> stringSplit(std::string line) {
    // Create a stringstream from the line
    std::stringstream ss(line);

    // Split the line into a vector of strings
    std::vector<std::string> tokens;
    std::string token;
    while (ss >> token) {
        // Remove all whitespaces from the token
        token.erase(std::remove_if(token.begin(), token.end(), [](char c) {
            return std::isspace(static_cast<unsigned char>(c));
        }), token.end());

        // Add the token to the vector
        tokens.push_back(token);
    }
    return tokens;
}


std::vector<int> getColumnSizes(std::string line) {
    std::vector<int> column_sizes;
    int size = 0;
    for (auto &ch : line) {
        if (ch == '-') {
            size++;
        }
        else {
            column_sizes.push_back(size);
            size = 0;
        }
    }
    return column_sizes;
}


std::vector<std::string> getRow(std::string line, std::vector<int> column_sizes) {
    std::vector<std::string> row;

    int line_pointer = 0;
    std::string slot;
    // read each slot
    for (auto &size : column_sizes) {
        // read slot
        slot = "";
        for (int i = 0; i < size; i ++) {
            slot += line[line_pointer];
            line_pointer++;
        }
        line_pointer++;     // skip space
        // remove leading and trailing white space
        slot = std::regex_replace(slot, std::regex("^ +| +$|( ) +"), "$1");
        row.push_back(slot);
    }
    return row;   
}


// constructor
Datatable::Datatable(std::string filename) {
    // read file contents and construct Datatable class
    // Open the file for reading
    std::ifstream input_file(filename);

    // Check if the file is opened successfully
    if (!input_file.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        throw std::invalid_argument("Unable to open file");
    }

    // Read the file line by line
    std::vector<int> column_sizes;
    int line_num = 1;
    std::string line;
    while (std::getline(input_file, line)) {
        // put line data into table data
        // line 1 is column names
        if (line_num == 1) {
            column_names = stringSplit(line);
        }
        // line 2 - get lengths for each column
        else if (line_num == 2) { 
            column_sizes = getColumnSizes(line);
        }
        // line is database entry, add as a row to table
        else {
            rows.push_back( getRow(line,column_sizes) );
        }

        line_num++;
    }

    // Close the file
    input_file.close();
}


void Datatable::printColumnNames(void) {
    for (auto i: column_names) {std::cout << i << ' ';}
    std::cout << "\n";
}


int Datatable::getColumnIndex(std::string column_name) {
    ptrdiff_t pos = std::distance(column_names.begin(), find(column_names.begin(), column_names.end(), column_name));
    if(pos >= column_names.size()) {
        return -1;
    }
    return pos;
}
