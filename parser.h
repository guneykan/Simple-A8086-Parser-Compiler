#ifndef DEF_H_
#define DEF_H_
#include <iostream>
#include <fstream>
#include <set>
using namespace std;

class Parser{
private:
    set<string> vars; 	
    int counter;	
    string label;	
    string input;
    string temp;
    ifstream reader;
    ofstream writer;	
    void update();
    bool receive(std::string keyword);
    bool check(std::string keyword);
    void stat();
    void expr();
    void term();
    void moreterm();
    void factor();
    void morefactors();
    void opt_stms();
    void stmt_list();
    bool isVar();
    bool isNum();
    void printer();	
    void printread();	

public:
    Parser(std::string infile,std::string outfile);
    void parse();


};

#endif
