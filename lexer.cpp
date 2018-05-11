#include "lexer.hpp"
#include "token.hpp"
#include <string>
#include <iostream>
#include <cstring>
using namespace std;

// implement the lexer function here
TokenList tokenize(istream & ins)
{
	TokenList mylist;
	bool isAssign = false;
	bool isOpen = false;
	size_t delimcount = 0;
	ins.seekg(0, ins.end);
	size_t size = ins.tellg();
	ins.seekg(0, ins.beg);
	size_t linecount = 0;
	size_t sizecount = 0;
	string str;
	char c;
	while (ins.get(c)) {
		sizecount++;
		if (linecount == 0) {
			linecount++;
		}
		if (delimcount == 1) {
			if (c == 0x0a) {
				mylist.emplace_back(STRING, linecount, str);
				str.clear();
				mylist.emplace_back(ERROR, linecount, "Error: String delim span multi lines.");
				break;
			}
			if (c == 0x22) {
				isAssign = true;
				mylist.emplace_back(STRING, linecount, str);
				mylist.emplace_back(STRING_DELIM, linecount);
				//mylist.emplace_back(EOL, linecount);
				str.clear();
				delimcount = 0;
			}
			else {
				isAssign = true;
				str = str + c;
			}
			if (sizecount == size) {
				mylist.emplace_back(EOL, linecount);
			}

		}
		else if (c == 0x2c || c == 0x0a || c == 0x22 || c == 0x0d || c == 0x28 || c == 0x29 || c == 0x20 || c == 0x09 || c == 0x0b || c == 0x0c || c == 0x3d || c==0x23) {
			if (delimcount != 0) {
				str = str + c;
			}
			else {
				if (str.length() >= 1) {
					mylist.emplace_back(STRING, linecount, str);
					str.clear();

					
					if (c == 0x0a) {
						mylist.emplace_back(EOL, linecount);
						isAssign = false;
						if (isOpen) {
							mylist.emplace_back(ERROR, linecount, "Error: Unmatched paren");
						}
						linecount++;
						//str.clear();
					}
					else if (c == 0x23) {

						char c1;
						ins.get(c1);
						while (c1 != 0x0a) {
							ins.get(c1);
						}
						if(isAssign)
							mylist.emplace_back(EOL, linecount);
						linecount++;
						isAssign = false;
					}
					else if (c == 0x2c) {
						mylist.emplace_back(SEP, linecount);
						//str.clear();
					}
					else if (c == 0x28) {
						if (isOpen) {
							mylist.emplace_back(ERROR, linecount, "Error: Unmatched paren");
							break;
						}
						else {
							isOpen = true;
							mylist.emplace_back(OPEN_PAREN, linecount);
						}

					}
					else if (c == 0x29) {
						if (!isOpen) {
							mylist.emplace_back(ERROR, linecount, "Error: Unmatched paren");

							break;
						}
						else {
							isOpen = false;
							mylist.emplace_back(CLOSE_PAREN, linecount);
						}

					}

					else if (c == 0x22) {
						delimcount++;
						mylist.emplace_back(STRING_DELIM, linecount);
					}
					else if (c == 0x3d) {
						mylist.emplace_back(EQUAL, linecount);
					}
					//isAssign = false;
				}
				else {
					if (c == 0x0a) {
						if (isAssign) {
							mylist.emplace_back(EOL, linecount);
						}
						/*if (str.length() >= 1) {
							mylist.emplace_back(EOL, linecount);
							linecount++;
						}
						else*/
							linecount++;
							isAssign = false;

					}
					else if (c == 0x23) {
						char c1;
						ins.get(c1);
						while (c1 != 0x0a) {
							ins.get(c1);
						}
						if (isAssign) {
							mylist.emplace_back(EOL, linecount);
						}
						isAssign = false;
						linecount++;
					}
					else if (c == 0x2c) {
						mylist.emplace_back(SEP, linecount);
						//str.clear();
					}
					else if (c == 0x28) {
						if (isOpen) {
							mylist.emplace_back(ERROR, linecount, "Error: Unmatched paren");
							break;
						}
						else {
							isOpen = true;
							mylist.emplace_back(OPEN_PAREN, linecount);
						}

					}
					else if (c == 0x29) {
						if (!isOpen) {
							mylist.emplace_back(ERROR, linecount, "Error: Unmatched paren");
							break;
						}
						else {
							isOpen = false;
							mylist.emplace_back(CLOSE_PAREN, linecount);
						}

					}

					else if (c == 0x22) {
						mylist.emplace_back(STRING_DELIM, linecount);
						delimcount++;
					}
					else if (c == 0x3d) {
						mylist.emplace_back(EQUAL, linecount);
					}
				}
			}
		}
		else {
			// if(c!=0x0)
			str = str + c;
			isAssign = true;
			if (sizecount == size && str.length() >= 1) {
				mylist.emplace_back(STRING, linecount, str);
				mylist.emplace_back(EOL, linecount);
			}
		}

	}
	//TokenList::iterator it = mylist.end();
	//if (it->type() != EOL) {}
	//if (mylist.begin()->type()!=EOL) {}
	/*if (isOpen) {

	}*/

		
	return mylist;
}
