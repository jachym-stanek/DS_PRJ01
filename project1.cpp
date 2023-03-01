/* 
Created by Jachym Stanek
stanejac@postech.ac.kr
*/


#include "datatable.h"

#include <iostream>
#include <string>
#include <stdexcept>
#include<map>


void querry1(std::string customer_filename, std::string zonecost_filename) {
	Datatable customer_table = Datatable(customer_filename);
	Datatable zonecost_table = Datatable(zonecost_filename);
	
	// find zoneid of 'Toronto'
	std::string toronto_zone_id = "";
	for (auto entry: zonecost_table.getRows()) {
		if ( entry[zonecost_table.getColumnIndex("ZONEDESC")] == "Toronto") {
			toronto_zone_id = entry[zonecost_table.getColumnIndex("ZONEID")];
		}
	}

	// List all the last names (LNAME) of the active customers that have zoneid matching Toronto
	for (auto entry: customer_table.getRows()) {
		// if zone matches Toronto and field ACTIVE is true, print lastname
		if (entry[customer_table.getColumnIndex("ZONE")] == toronto_zone_id && entry[customer_table.getColumnIndex("ACTIVE")] == "1") {
    		std::cout << entry[customer_table.getColumnIndex("LNAME")] << '\n';
		}
	}
}


void querry2(std::string lineitem_filename, std::string products_filename) {
	Datatable lineitem_table = Datatable(lineitem_filename);
	Datatable products_table = Datatable(products_filename);

	// create dictionary- barcode : #occurences
	std::map<std::string, int> occurences;
	std::map<std::string, std::string> barcode_name_dict;

	// fill in dictionary
	std::string barcode;
	for (auto entry: lineitem_table.getRows()) {
		barcode = entry[lineitem_table.getColumnIndex("BARCODE")];
		if (occurences.find(barcode) == occurences.end()) {
			// entry does not exist in dict
			occurences[barcode] = 1;
		} else {
			// entry exists in dict
			occurences[barcode] += 1;
		}
	}

	// find matches for barcode - product name
	for (auto entry: products_table.getRows()) {
		barcode = entry[products_table.getColumnIndex("BARCODE")];
		barcode_name_dict[barcode] = entry[products_table.getColumnIndex("PRODDESC")];
	}

	// print out results
	for (auto const& x : occurences) {
		if (x.second >= 2) {
			// print barcode
			std::cout << x.first << "                 ";
			// print product name
			std::cout << barcode_name_dict[x.first] << "\n";
		}
	}
}


int main(int argc, char** argv) {
	// scan input and decide querry
	try {
		if (argc < 4) {
			throw std::invalid_argument("Not enough input arguments");
		}
		else if (std::string(argv[1]) == "q1") {
			querry1(std::string(argv[2]), std::string(argv[3]));
		}
		else if (std::string(argv[1]) == "q2") {
			querry2(std::string(argv[2]), std::string(argv[3]));
		}
		else {
			throw std::invalid_argument("Unrecognizable querry");
		}
	}
	catch( const std::invalid_argument& e ) {
    	std::cout << "Please run the file in correct format!\n";
	}


	return 0;
}
