// Copyright 2023 Jachym Stanek

#include <string>
#include <vector>

#ifndef DATATABLE_H_
#define DATATABLE_H_

class Datatable {
 private:
    // table data
    std::vector<std::string> column_names;
    std::vector< std::vector<std::string> > rows;

 public:
    Datatable(std::string filename);
    std::vector<std::string> getColumnNames(void) { return column_names; }
    std::vector< std::vector<std::string> > getRows(void) { return rows; }
    void printColumnNames(void);
    int getColumnIndex(std::string column_name);
};

#endif   // DATATABLE_H_
