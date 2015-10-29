//============================================================================
// Name        : parser.cpp
// Author      : Guneykan Ozgul.
// Description : Implementation of 'MyLang Compiler' in C++.
//============================================================================
#include "parser.h"
#include <fstream>
using namespace std;
int main(int argc, char *argv[]) {
   if(argc>2){
        string infile1="./im.txt";
	string infile=argv[1];
	string outfile=argv[2];
        ifstream reader(infile.c_str());
        ofstream writer(infile1.c_str());
	char c;
	char d;
        int i=0;
	while(reader.good()){
	reader.get(c);
         if(c==';'|| c=='(' || c==')'|| c=='+'|| c=='-' || c=='*' || c=='/' || c=='%' || c=='=' ){
		if(i!=0)		
		writer<<d<<" ";
      	}else{
		if(d==';'|| d=='(' || d==')'|| d=='+'|| d=='-' || d=='*' ||d=='/'|| d=='%' || d=='='){
		if(i!=0)
		writer<<d<<" ";	
		}
		else{
		if(i!=0)
		writer<<d;		
		}
	}	
	d=c;
	i++;
  	}
	reader.close();
	writer.close();
        Parser myParser(infile1,outfile);
    	myParser.parse();
	remove(infile1.c_str());
    	return 1;
    }else{
	cout<<"Where is the input file?"<<endl;
	cout<<"Please enter an input file."<<endl;
	}
}


