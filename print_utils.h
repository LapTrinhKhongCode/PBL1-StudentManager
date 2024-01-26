#ifndef __PRINT_UTILS_H__
#define __PRINT_UTILS_H__

#include <string>
#include <sstream>
#include <iomanip>


using namespace std;

// Display config 

const int order_column_width = 8;
const int name_column_width = 20;
const int age_column_width = 8;
const int score_column_width = 8;

const string corner_char = "+";
const string vertical_border_char = "|";
const string horizontal_border_char = "-";

void print_row(string order, string name, string age, string score) {
	cout << "\t" << std::left
    	 << vertical_border_char << setw(order_column_width) << " " + order
    	 << vertical_border_char << setw(name_column_width) << " " + name
    	 << vertical_border_char << setw(age_column_width) << " " + age
    	 << vertical_border_char << setw(score_column_width) << " " + score
	     << "|\n";
}

void print_row(int order, string name, int age, float score) {
    std::stringstream score_ss;
    score_ss << std::setprecision(3) << score;
	print_row(std::to_string(order), name, std::to_string(age), score_ss.str());
}

std::string line(int border_width, std::string c) {
    std::string s;
	for (int i = 0; i < border_width; i++) {
		s+= c;
	}
	return s;
}

void print_row() {
	cout << "\t"
        << corner_char << line(order_column_width, horizontal_border_char)
	    << corner_char << line(name_column_width, horizontal_border_char)
	    << corner_char << line(age_column_width, horizontal_border_char)
	    << corner_char << line(score_column_width, horizontal_border_char)
	    << corner_char
	    << "\n";
}



#endif