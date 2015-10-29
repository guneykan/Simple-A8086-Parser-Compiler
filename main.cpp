//============================================================================
// Name        : parser.cpp
// Author      : Guneykan Ozgul.
// Description : Implementation of 'MyLang Parser' in C++.
//============================================================================
#include "parser.h"
//Constructor method.

Parser:: Parser(string infile,string outfile) {
	counter=1;
	label="LABL";
    input = "EMPTY";
    reader.open(infile.c_str());
    writer.open(outfile.c_str());	
    if (reader.good())
        reader >> input;
    temp = input;
}
//Starts to parse input.
void Parser::parse(){
    writer<<"code segment"<<endl;
    stat();
   // cout<<"stop"<<endl;
	writer<<"  int  20h       ; exit to dos "<<endl;
  writer<<endl;
  printread();
  writer<<endl;
  printer();
  writer<<endl;
   while (!vars.empty()) {
    writer << 'v' << *vars.begin()<<" "<<"dw (?)"<<endl;
    vars.erase(vars.begin());
  }
  writer<<"code ends"<<endl;
}

//Gets the next token from input.
void Parser:: update() {
    if (reader.good())
        reader >> input;
    else
        input = " ";
}
//Checks if the given string is matched with the current token. If it is matched, updates the current token.
bool Parser::receive(string keyword) {
    if (input.compare(keyword) == 0) {
        temp = input;
        update();
        return true;
    }
    return false;
}
//Checks if the given string is matched with the current token. If it is matched, updates the current token.
//This function is used to detect errors.
bool Parser::check(string keyword) {
    if (input.compare(keyword) == 0) {
      // temp=input;
        update();
        return true;
    }
    cout << "Syntax error: "<<"Unexpected token:"<<input<<", "<<keyword<<" expected"<<endl;
    return false;
}
//Represents the nonterminal '<stat>'.
void Parser::stat() {
    if (isVar()) {
		string tem=input;
        update();
        if (check("=")) {
            //cout << "push-addr-var " << tem << " " << endl;
             writer<<"  push offset v"<<tem<<endl;
            vars.insert(tem);
            expr();
           writer<<"  pop ax"<<endl;
            writer<<"  pop bp"<<endl;
            writer<<"  mov [bp],ax"<<endl;
			//writer<<"  mov v"<<tem<<",ax"<<endl;

//cout << "assign" << endl;
        } else {
            return;
        }
    }
    else if (receive("if")) {
        //cout << "if \n" << endl;
        expr();
        int c=counter;
        writer<<"  pop ax"<<endl;
        writer<<"  cmp ax,0"<<endl;
        writer<<"  if z jmp "<<"LABL"<<counter<<endl;
        counter++;
        if (check("then")) {
           // cout << "\n then \n" << endl;
            stat();
            writer<<"LABL"<<c<<" :"<<endl;
            //cout << "\n";
        }
        else {
            return;
        }
    }
    else if (receive("print")) {
        //cout << "print "; 
       // cout<<"  push v"<<input<<" "<<endl;
         expr();
        writer<<"  pop ax"<<endl;
        writer<<"  call myprint"<<endl;

    }
    else if (receive("read")) {
        writer<<"  call myread"<<endl;
        writer<<"  mov "<<"v"<<input<<",cx"<<endl;
		//cout << "read ";
        if(isVar()){
                        vars.insert(input);
			update();

		}
    }
    else if (receive("while")) {
      //  cout << "while \n" << endl;	
        int counter1=counter;
        writer<<"LABL"<<counter1<<":"<<endl;
        counter++;
        expr();
        writer<<"  pop ax"<<endl;
        writer<<"  cmp ax,0"<<endl;
        //cout<<"jump-if-false "<<"LABL"<<counter<<endl;
        writer<<"  if z jmp LABL"<<counter<<endl;
        int counter2=counter;
		counter++;
        if (check("do")) {
        //    cout << "\ndo \n" << endl;
			stat();
			//cout<<"jump "<<"LABL"<<counter1<<endl;
			writer<<"  jmp "<<"LABL"<<counter1<<endl;
	        writer<<"LABL"<<counter2<<":"<<endl;
           // cout << "\n";
        }
        else {
            return;
        }
    }
    else if (receive("begin")) {
       // cout << "begin \n" << endl;
        opt_stms();
        if (check("end")) {
           // cout << "\n end" << endl;
        }else{
            return;
        }
    }
    else if(input.compare(";")!=0){
        cout <<"Syntax Error";
    }
}
//Represents the nonterminal '<expr>'.
void Parser::expr() {
    term();
    moreterm();
}
void Parser::term() {
    factor();
    morefactors();
}
void Parser::moreterm() {
    if (receive("+")) {
        term();
        //cout << "+" << endl;
		writer<<"  mov	dx,0"<<endl;
        writer<<"  pop cx"<<endl;
        writer<<"  pop ax"<<endl;
        writer<<"  add ax,cx"<<endl;
        writer<<"  push ax"<<endl;
        moreterm();
    }
    else if (receive("-")) {
        term();
        //cout << "-" << endl;
        writer<<"  pop cx"<<endl;
        writer<<"  pop ax"<<endl;
        writer<<"  sub ax,cx"<<endl;
        writer<<"  push ax"<<endl;
        moreterm();
    }
    else {
    }
}
//Represents the nonterminal '<factor>'.
void Parser::factor() {
    if (receive("(")) {
        expr();
        check(")");
    }
    else if (isVar()) {
       // cout << "push-val-var " << input << endl;
        writer<<"  push v"<<input<<" w"<<endl; 
	vars.insert(input);
        temp=input;
        update();
        
    }
    else if (isNum()) {
        //cout << "push-num " << input << endl;
		writer<<"  push "<<input<<endl;
        temp=input;
        update();
    }
    else {
        cout << "Syntax error: Unexpected token:"<<input<< endl;
    }
}
//Represents the nonterminal '<morefactors>'.
void Parser::morefactors() {
    if (receive("*")) {
        factor();
        //cout << "*" << endl;
        writer<<"  pop ax"<<endl;
        writer<<"  pop cx"<<endl;
        writer<<"  mul cx"<<endl;
        writer<<"  push ax"<<endl;
        morefactors();
    }
    else if (receive("/")) {
        factor();
       // cout << "div" << endl;
	writer<<"  xor dx,dx"<<endl;
        writer<<"  pop cx"<<endl;
        writer<<"  pop ax"<<endl;
        writer<<"  div cx"<<endl;
        writer<<"  push ax"<<endl;


        morefactors();
    }
    else if (receive("%")) {
        factor();
        //cout << "%" << endl;
        writer<<"  xor dx,dx"<<endl;
        writer<<"  pop cx"<<endl;
        writer<<"  pop ax"<<endl;
        writer<<"  div cx"<<endl;
        writer<<"  push dx"<<endl;

        morefactors();
    }
}
//Represents the nonterminal '<opt_stms>'.
void Parser::opt_stms() {
    if (input.compare("end") != 0) {
       stmt_list();
    }
}
//Represents the nonterminal '<stmt_list>'.
void Parser::stmt_list() {
    stat();
    if (input.compare(";") == 0) {
		temp=input;
		update();
       		 stmt_list();
    }
}
//Checks if the current token is a variable.
bool Parser::isVar() {
    char first = input.at(0);
    bool isLetter=('A' <= first && first <= 'Z') || ('a' <= first && first <= 'z');
    bool isSymbol=(input.compare("if") == 0 || input.compare("then") == 0 || input.compare("while") == 0
            || input.compare("print") == 0 || input.compare("read") == 0 || input.compare("begin") == 0 || input.compare("end") == 0);
    return (isLetter&&!isSymbol);
}
//Checks if the current token is a number.
bool Parser::isNum() {
    int length = input.length();
    for (unsigned int i = 0; i < length; i++) {
        if (!isdigit(input.at(i)))
            return false;
    }
    return true;
}
void Parser::printer(){	
   writer <<"myprint:"<<endl; 
   writer<<"  mov    si,10d"<<endl;
   writer<<"  xor    dx,dx"<<endl;
   writer<<"  push   0Ah    ; push newline"<<endl; 
   writer<<"  mov    cx,1d"<<endl;
   writer<<"nonzero:"<<endl;
   writer<<"  div    si"<<endl;
   writer<<"  add    dx,48d"<<endl;
   writer<<"  push   dx"<<endl;
   writer<<"  inc    cx"<<endl;
   writer<<"  xor    dx,dx"<<endl;
   writer<<"  cmp    ax,0h"<<endl;
   writer<<"  jne    nonzero"<<endl;
   writer<<"writeloop:"<<endl;
   writer<<"  pop    dx"<<endl;
   writer<<"  mov    ah,02h"<<endl;
   writer<<"  int    21h"<<endl;
   writer<<"  dec    cx"<<endl;
   writer<<"  jnz    writeloop"<<endl; 
   writer<<"  ret"<<endl;
}
void Parser::printread(){
   writer <<"myread:"<<endl; 
   writer<<"  MOV  CX,0"<<endl;
   writer<<"morechar:"<<endl;
   writer<<"  mov  ah,01h "<<endl; 
   writer<<"  int  21H"<<endl;
   writer<<"  mov  dx,0"<<endl;
   writer<<"  mov  dl,al"<<endl;
   writer<<"  mov  ax,cx"<<endl;
   writer<<"  cmp  dl,0D"<<endl;
   writer<<"  je   myret"<<endl;
   writer<<"  sub  dx,48d"<<endl;
   writer<<"  mov  bp,dx"<<endl;
   writer<<"  mov  ax,cx"<<endl;
   writer<<"  mov  cx,10d"<<endl;
   writer<<"  mul  cx"<<endl;
   writer<<"  add  ax,bp"<<endl;
   writer<<"  mov  cx,ax"<<endl;
   writer<<"  jmp morechar"<<endl;
   writer<<"  myret:"<<endl; 
   writer<<"  ret "<<endl;
   
   	
}

