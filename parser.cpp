#include "parser.hpp"
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <assert.h>
#include <algorithm>
// implement the parser here

parser::parser(TokenList alist) :mylist(alist), myit(mylist.begin()),constCount(0),layoutCount(0),isLexErr(false),textLine(0)
,documentLine(0),firstLine(0){}
parser::~parser() {}

size_t parser::getFirstTextLine()
{
	return firstLine;
}

void parser::getInstructionLine(std::string &inputfile)
{
	array<size_t, 50> aArray;
	size_t index = 0;
	ifstream input(inputfile);
	string str;
	string word;
	operation wordType;
	size_t num=0;
	bool isFirstLine = false;
	while (getline(input,str)) {
		if (str.size() > 1) {
			while (str.at(0) == ' ' || str.at(0) == '\t') {
				str.erase(0, 1);
			}
			if (str.substr(0, 5) == "main:" && str.size()>5) {
				word = str.substr(5, str.find(" "));
				while (word.at(0) == ' ' || word.at(0) == '\t') {
					word.erase(0, 1);
				}
				word = word.substr(0, word.find(" "));

			}
			else {
				word = str.substr(0, str.find(" "));
			}
			
			if (word == "nop") {
				wordType = NOP;
			}
			else if (word == "lw") {
				wordType = LoadWord;
			}
			else if (word == "li") {
				wordType = LoadImmid;
			}
			else if (word == "la") {
				wordType = LoadAdd;
			}
			else if (word == "sw") {
				wordType = StoreWord;
			}
			else if (word == "move") {
				wordType = Move;
			}
			else if (word == "mfhi") {
				wordType = MoveFromHi;
			}
			else if (word == "mflo") {
				wordType = MoveFromLo;
			}
			else if (word == "add") {
				wordType = AddS;
			}
			else if (word == "addu") {
				wordType = AddU;
			}
			else if (word == "sub") {
				wordType = SubS;
			}
			else if (word == "subu") {
				wordType = SubU;
			}
			else if (word == "mult") {
				wordType = MultS;
			}
			else if (word == "multu") {
				wordType = MultU;
			}
			else if (word == "div") {
				wordType = Div2S;
			}
			else if (word == "divu") {
				wordType = Div2U;
			}
			else if (word == "and") {
				wordType = And;
			}
			else if (word == "or") {
				wordType = Or;
			}
			else if (word == "xor") {
				wordType = Xor;
			}
			else if (word == "not") {
				wordType = Not;
			}
			else if (word == "j") {
				wordType = Jump;
			}
			else if (word == "beq") {
				wordType = BranchEq;
			}
			else if (word == "bne") {
				wordType = BranchNe;
			}
			else if (word == "ble") {
				wordType = BranchLe;
			}
			else if (word == "bgt") {
				wordType = BranchGt;
			}
			else if (word == "bge") {
				wordType = BranchGe;
			}
			else if (word == "blt") {
				wordType = BranchLt;
			}
			else if (word == "lw") {
				wordType = LoadWord;
			}
			else {
				wordType = NONE;
			}
			if (!isFirstLine) {
				for (instructionSet::iterator it = instructions.begin(); it != instructions.end(); it++) {
					if (it->op == wordType) {
						isFirstLine = true;
						aArray[index] = num;
						index++;
						firstLine = num;
						break;
					}
				}
			}
			else {
				if (wordType != NONE) {
					aArray[index] = num;
					index++;
				}
				
			}
			num++;
		}
		else {
			num++;
		}
		
	}
	index = 0;
	for (instructionSet::iterator it = instructions.begin(); it != instructions.end(); it++) {
		it->line = aArray[index];
		index++;
	}
}

const string parser::get_error()
{
	if (myit == mylist.end())
		return "";
	return "Error:" + to_string(myit->line()) +
		": "+myit->value() + "\n";
}

const string parser::getlexer_error()
{
	isLexErr = true;
	return "Error:" + to_string(lexit->line()) +
	": " + lexit->value() + "\n";
}

MemoryType parser::getMemory()
{
	return memoryData;
}

LabelMapType parser::getLabelMap()
{
	return DataLabel;
}

instructionSet parser::getInstructions()
{
	return instructions;
}

immidiateSet parser::getImmidiates()
{
	return immidiateLabels;
}

TextLabelType parser::getTextLabels()
{
	return textLabels;
}

vector<immidiate> parser::getMainLabels()
{
	return mainVec;
}

bool parser::isInstruct(TokenList & alist)
{
	if (isNOP(alist)) {
		return true;
	}
	if (isLdword(alist))
		return true;
	/*if (isLdhalf(alist))
		return true;
	if (isLdbyte(alist))
		return true;*/
	if (isLdimm(alist))
		return true;
	if (isLdadd(alist))
		return true;
	if (isLdsw(alist))
		return true;
	//if (isLdsh(alist))
	//	return true;
	//if (isLdsb(alist))
	//	return true;
	if (isLdmove(alist))
		return true;
	if (isLdmfhi(alist))
		return true;
	if (isLdmflo(alist))
		return true;
	/*if (isLdmthi(alist))
		return true;
	if (isLdmtlo(alist))
		return true;*/
	if (isadd(alist))
		return true;
	if (isaddu(alist))
		return true;
	if (issub(alist))
		return true;
	if (issubu(alist))
		return true;
	/*if (ismul(alist))
		return true;
	if (ismulo(alist))
		return true;
	if (ismulou(alist))
		return true;*/
	if (ismult(alist))
		return true;
	if (ismultu(alist))
		return true;
	/*if (isdiv_3(alist))
		return true;
	if (isdivu_3(alist))
		return true;*/
	if (isdiv_2(alist))
		return true;
	if (isdivu_2(alist))
		return true;
	/*if (isrem(alist))
		return true;
	if (isremu(alist))
		return true;
	if (isabs(alist))
		return true;
	if (isneg(alist))
		return true;
	if (isnegu(alist))
		return true;*/
	if (isand(alist))
		return true;
	if (isnor(alist))
		return true;
	if (isnot(alist))
		return true;
	if (isor(alist))
		return true;
	if (isxor(alist))
		return true;
	if (isjump(alist))
		return true;
	if (isbeq(alist))
		return true;
	if (isbne(alist))
		return true;
	if (isblt(alist))
		return true;
	if (isble(alist))
		return true;
	if (isbgt(alist))
		return true;
	if (isbge(alist))
		return true;
	if (isTextLabel(alist))
		return true;
	return false;

}

testTokenType parser::test_classify(TokenList::const_iterator it, TokenList::const_iterator end)
{
	testTokenType inputvalue = TEST_EOT;

	if (it == end) {
		inputvalue = TEST_EOT;
	}
	else if (it->type() == EOL) {
		inputvalue = TEST_EOL;
	}
	else if (it->type() == OPEN_PAREN) {
		inputvalue = TEST_OPEN;
	}
	else if (it->type() == CLOSE_PAREN) {
		inputvalue = TEST_CLOSE;
	}
	else if (it->type() == SEP) {
		inputvalue = TEST_SEP;
	}
	else if (it->type() == STRING && isInt(*it)) {
		inputvalue = OFFSET;
	}
	else if (it->type() == STRING && isLabel_fromData(*it)) {
		inputvalue = TEST_LABEL;
	}
	else if (it->type() == STRING && isImmid(*it)) {
		inputvalue = TEST_IMMID;
	}
	else if (it->type() == STRING && isRegister(*it)) {
		inputvalue = REGISTER;
	}
	else if (it->type() == STRING && isSource(*it)) {//tobechanged
		inputvalue = TEST_SOURCE;
	}

	return inputvalue;
}

bool parser::isMemref(TokenList::const_iterator it, TokenList::const_iterator end)
{
	memrefType state = MEMREF_S0;
	//auto it = instructList.begin();
	do {
		testTokenType input = test_classify(it, end);
		if (it != end) {
			//declarList.pop_front();
			//memrefCount++;
			++it;
		}
		switch (state) {
		case S0:
			isMemref0(state, input);
			break;
		case S1:
			isMemref1(state, input);
			break;
		case S2:
			isMemref2(state, input);
			break;
		case S3:
			isMemref3(state, input);
			break;
		case S4:
			isMemref4(state, input);
			break;
		case S5:
			isMemref5(state, input);
			break;
		case S6:
			isMemref6(state, input);
			break;
		default:
			state = MEMREF_ERROR;
		};

	} while (state != MEMREF_ERROR && state != MEMREF_HALT);
	return state != MEMREF_ERROR;
}

void parser::isMemref0(memrefType & state, testTokenType input)
{
	//assert(state == S0);
	switch (input) {
	case TEST_LABEL:
		state = MEMREF_S1;
		break;
	case TEST_IMMID:
		state = MEMREF_S2;
		break;
	case REGISTER:
		state = MEMREF_S3;
		break;
	case OFFSET:
		state = MEMREF_S4;
		break;
	case TEST_OPEN:
		state = MEMREF_S5;
		break;

	default:
		state = MEMREF_ERROR;
		break;
	};
}

void parser::isMemref1(memrefType & state, testTokenType input)
{
	//assert(state == S1);
	switch (input) {
	case TEST_CLOSE:
		state = MEMREF_S6;
		break;
	case TEST_EOL:
		state = MEMREF_HALT;
		break;
	default:
		state = MEMREF_ERROR;
		break;
	};
}

void parser::isMemref2(memrefType & state, testTokenType input)
{
	//assert(state == S2);
	switch (input) {
	case TEST_CLOSE:
		state = MEMREF_S6;
		break;
	case TEST_EOL:
		state = MEMREF_HALT;
		break;
	default:
		state = MEMREF_ERROR;
		break;
	};
}

void parser::isMemref3(memrefType & state, testTokenType input)
{
	//assert(state == S3);
	switch (input) {
	case TEST_CLOSE:
		state = MEMREF_S6;
		break;
	case TEST_EOL:
		state = MEMREF_HALT;
		break;
	default:
		state = MEMREF_ERROR;
		break;
	};
}

void parser::isMemref4(memrefType & state, testTokenType input)
{
	//assert(state == S4);
	switch (input) {
	case TEST_OPEN:
		state = MEMREF_S5;
		break;
	case TEST_EOL:
		state = MEMREF_HALT;
		break;
	default:
		state = MEMREF_ERROR;
		break;
	};
}

void parser::isMemref5(memrefType & state, testTokenType input)
{
	//assert(state == S5);
	switch (input) {
	case TEST_LABEL:
		state = MEMREF_S1;
		break;
	case TEST_IMMID:
		state = MEMREF_S2;
		break;
	case REGISTER:
		state = MEMREF_S3;
		break;
	default:
		state = MEMREF_ERROR;
		break;
	};
}

void parser::isMemref6(memrefType & state, testTokenType input)
{
	
	switch (input) {
	case TEST_EOL:
		state = MEMREF_HALT;
		break;
	default:
		state = MEMREF_ERROR;
		break;
	};
}


bool parser::isNOP(TokenList & alist_text)
{

	auto alist = alist_text;
	//auto it = alist.begin();
	if (alist.begin()->value() != "nop") {
		return false;
	}
	alist.pop_front();
	if (alist.begin()->type() != EOL) {
		return false;
	}
	instructValues.op = NOP;
	instructions.emplace_back(instructValues);
	textLine++;
	return true;
}

bool parser::isLdword(TokenList& alist_text)
{
	auto alist = alist_text;
	auto alist1 = alist_text;
	//auto it = alist.begin();
	if (alist.begin()->value() != "lw") {
		return false;
	}
	alist.pop_front();
	if (!isRegister(*alist.begin())) {
		return false;
	}
	alist.pop_front();
	if (alist.begin()->type() != SEP)
		return false;
	alist.pop_front();
	if (!(isMemref(alist.begin(),alist.end())))
		return false;

	instructValues.op = LoadWord;
	alist1.pop_front();
	alist1.pop_back();
	arguments.clear();
	string str;
	for (TokenList::iterator it = alist1.begin(); it != alist1.end(); it++) {
		arguments.emplace_back(*it);
	}
	instructValues.args = arguments;
	instructions.emplace_back(instructValues);
	textLine++;
	return true;
}

bool parser::isLdhalf(TokenList &alist_text)
{
	auto alist = alist_text;
	auto it = alist.begin();
	if (it->value() != "lh") {
		return false;
	}
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!(isMemref(it, alist.end())))
		return false;
	return true;
}

bool parser::isLdbyte(TokenList &alist_text)
{
	auto alist = alist_text;
	auto it = alist.begin();
	if (it->value() != "lb") {
		return false;
	}
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;

	if (!(isMemref(it, alist.end())))
		return false;
	return true;
}

bool parser::isLdimm(TokenList &alist_text)
{
	auto alist = alist_text;
	auto it = alist.begin();
	if (it->value() != "li") {
		return false;
	}
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!(isImmid(*it)))
		return false;
	it++;
	if (it->type() != EOL)
		return false;
	auto alist1 = alist_text;
	instructValues.op = LoadImmid;
	alist1.pop_front();
	alist1.pop_back();
	arguments.clear();
	string str;
	for (TokenList::iterator it = alist1.begin(); it != alist1.end(); it++) {
		arguments.emplace_back(*it);
	}
	instructValues.args = arguments;
	instructions.emplace_back(instructValues);
	textLine++;
	return true;
}

bool parser::isLdadd(TokenList &alist_text)
{
	auto alist = alist_text;
	auto it = alist.begin();
	if (it->value() != "la") {
		return false;
	}
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!(isMemref(it, alist.end())))
		return false;
	auto alist1 = alist_text;
	instructValues.op = LoadAdd;
	alist1.pop_front();
	alist1.pop_back();
	arguments.clear();
	string str;
	for (TokenList::iterator it = alist1.begin(); it != alist1.end(); it++) {
		arguments.emplace_back(*it);
	}
	instructValues.args = arguments;
	instructions.emplace_back(instructValues);
	textLine++;
	return true;
}

bool parser::isLdsw(TokenList & alist_text)
{
	auto alist = alist_text;
	auto it = alist.begin();
	if (it->value() != "sw") {
		return false;
	}
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!(isMemref(it, alist.end())))
		return false;
	auto alist1 = alist_text;
	instructValues.op = StoreWord;
	alist1.pop_front();
	alist1.pop_back();
	arguments.clear();
	string str;
	for (TokenList::iterator it = alist1.begin(); it != alist1.end(); it++) {
		arguments.emplace_back(*it);
	}
	instructValues.args = arguments;
	instructions.emplace_back(instructValues);
	textLine++;
	return true;
}

bool parser::isLdsh(TokenList & alist_text)
{
	auto alist = alist_text;
	auto it = alist.begin();
	if (it->value() != "sh") {
		return false;
	}
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!(isMemref(it, alist.end())))
		return false;
	return true;
}

bool parser::isLdsb(TokenList & alist_text)
{
	auto alist = alist_text;
	auto it = alist.begin();
	if (it->value() != "sb") {
		return false;
	}
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!(isMemref(it, alist.end())))
		return false;
	return true;
}

bool parser::isLdmove(TokenList & alist_text)
{
	auto alist = alist_text;
	auto it = alist.begin();
	if (it->value() != "move") {
		return false;
	}
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!isRegister(*it))
		return false;
	auto alist1 = alist_text;
	instructValues.op = Move;
	alist1.pop_front();
	alist1.pop_back();
	arguments.clear();
	string str;
	for (TokenList::iterator it = alist1.begin(); it != alist1.end(); it++) {
		arguments.emplace_back(*it);
	}
	instructValues.args = arguments;
	instructions.emplace_back(instructValues);
	textLine++;
	return true;
}

bool parser::isLdmfhi(TokenList & alist_text)
{
	auto alist = alist_text;
	auto it = alist.begin();
	if (it->value() != "mfhi") {
		return false;
	}
	it++;
	if (!isRegister(*it)) 
		return false;
	auto alist1 = alist_text;
	instructValues.op = MoveFromHi;
	alist1.pop_front();
	alist1.pop_back();
	arguments.clear();
	string str;
	for (TokenList::iterator it = alist1.begin(); it != alist1.end(); it++) {
		arguments.emplace_back(*it);
	}
	instructValues.args = arguments;
	instructions.emplace_back(instructValues);
	textLine++;
	return true;
}

bool parser::isLdmflo(TokenList & alist_text)
{
	auto alist = alist_text;
	auto it = alist.begin();
	if (it->value() != "mflo") {
		return false;
	}
	it++;
	if (!isRegister(*it))
		return false;
	auto alist1 = alist_text;
	instructValues.op = MoveFromLo;
	alist1.pop_front();
	alist1.pop_back();
	arguments.clear();
	string str;
	for (TokenList::iterator it = alist1.begin(); it != alist1.end(); it++) {
		arguments.emplace_back(*it);
	}
	instructValues.args = arguments;
	instructions.emplace_back(instructValues);
	textLine++;
	return true;
}

bool parser::isLdmthi(TokenList & alist_text)
{
	auto alist = alist_text;
	auto it = alist.begin();
	if (it->value() != "mthi") {
		return false;
	}
	it++;
	if (!isRegister(*it))
		return false;
	return true;
}

bool parser::isLdmtlo(TokenList & alist_text)
{
	auto alist = alist_text;
	auto it = alist.begin();
	if (it->value() != "mtlo") {
		return false;
	}
	it++;
	if (!isRegister(*it))
		return false;
	return true;
}

bool parser::isadd(TokenList & alist_text)
{
	auto alist = alist_text;
	auto it = alist.begin();
	if (it->value() != "add") {
		return false;
	}
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!(isSource(*it)))
		return false;
	auto alist1 = alist_text;
	instructValues.op = AddS;
	alist1.pop_front();
	alist1.pop_back();
	arguments.clear();
	string str;
	for (TokenList::iterator it = alist1.begin(); it != alist1.end(); it++) {
		arguments.emplace_back(*it);
	}
	instructValues.args = arguments;
	instructions.emplace_back(instructValues);
	textLine++;
	return true;
}

bool parser::isaddu(TokenList & alist_text)
{
	auto alist = alist_text;
	auto it = alist.begin();
	if (it->value() != "addu") {
		return false;
	}
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!(isSource(*it)))
		return false;
	auto alist1 = alist_text;
	instructValues.op = AddU;
	alist1.pop_front();
	alist1.pop_back();
	arguments.clear();
	string str;
	for (TokenList::iterator it = alist1.begin(); it != alist1.end(); it++) {
		arguments.emplace_back(*it);
	}
	instructValues.args = arguments;
	instructions.emplace_back(instructValues);
	textLine++;
	return true;
}

bool parser::issub(TokenList & alist_text)
{
	auto alist = alist_text;
	auto it = alist.begin();
	if (it->value() != "sub") {
		return false;
	}
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!(isSource(*it)))
		return false;
	auto alist1 = alist_text;
	instructValues.op = SubS;
	alist1.pop_front();
	alist1.pop_back();
	arguments.clear();
	string str;
	for (TokenList::iterator it = alist1.begin(); it != alist1.end(); it++) {
		arguments.emplace_back(*it);
	}
	instructValues.args = arguments;
	instructions.emplace_back(instructValues);
	textLine++;
	return true;
}

bool parser::issubu(TokenList & alist_text)
{
	auto alist = alist_text;
	auto it = alist.begin();
	if (it->value() != "subu") {
		return false;
	}
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!(isSource(*it)))
		return false;
	auto alist1 = alist_text;
	instructValues.op = SubU;
	alist1.pop_front();
	alist1.pop_back();
	arguments.clear();
	string str;
	for (TokenList::iterator it = alist1.begin(); it != alist1.end(); it++) {
		arguments.emplace_back(*it);
	}
	instructValues.args = arguments;
	instructions.emplace_back(instructValues);
	textLine++;
	return true;
}

bool parser::ismul(TokenList & alist_text)
{
	auto alist = alist_text;
	auto it = alist.begin();
	if (it->value() != "mul") {
		return false;
	}
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!(isSource(*it)))
		return false;
	return true;
}

bool parser::ismulo(TokenList & alist_text)
{
	auto alist = alist_text;
	auto it = alist.begin();
	if (it->value() != "mulo") {
		return false;
	}
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!(isSource(*it)))
		return false;
	return true;
}

bool parser::ismulou(TokenList & alist_text)
{
	auto alist = alist_text;
	auto it = alist.begin();
	if (it->value() != "mulou") {
		return false;
	}
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!(isSource(*it)))
		return false;
	return true;
}

bool parser::ismult(TokenList & alist_text)
{
	auto alist = alist_text;
	auto it = alist.begin();
	if (it->value() != "mult") {
		return false;
	}
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	auto alist1 = alist_text;
	instructValues.op = MultS;
	alist1.pop_front();
	alist1.pop_back();
	arguments.clear();
	string str;
	for (TokenList::iterator it = alist1.begin(); it != alist1.end(); it++) {
		arguments.emplace_back(*it);
	}
	instructValues.args = arguments;
	instructions.emplace_back(instructValues);
	textLine++;
	return true;
}

bool parser::ismultu(TokenList & alist_text)
{
	auto alist = alist_text;
	auto it = alist.begin();
	if (it->value() != "multu") {
		return false;
	}
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	auto alist1 = alist_text;
	instructValues.op =MultU;
	alist1.pop_front();
	alist1.pop_back();
	arguments.clear();
	string str;
	for (TokenList::iterator it = alist1.begin(); it != alist1.end(); it++) {
		arguments.emplace_back(*it);
	}
	instructValues.args = arguments;
	instructions.emplace_back(instructValues);
	textLine++;
	return true;
}

bool parser::isdiv_3(TokenList & alist_text)
{
	auto alist = alist_text;
	auto it = alist.begin();
	if (it->value() != "div") {
		return false;
	}
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!(isSource(*it)))
		return false;
	return true;
}

bool parser::isdivu_3(TokenList & alist_text)
{
	auto alist = alist_text;
	auto it = alist.begin();
	if (it->value() != "divu") {
		return false;
	}
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!(isSource(*it)))
		return false;
	return true;
}

bool parser::isdiv_2(TokenList & alist_text)
{
	auto alist = alist_text;
	auto it = alist.begin();
	if (it->value() != "div") {
		return false;
	}
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	auto alist1 = alist_text;
	instructValues.op = Div2S;
	alist1.pop_front();
	alist1.pop_back();
	arguments.clear();
	string str;
	for (TokenList::iterator it = alist1.begin(); it != alist1.end(); it++) {
		arguments.emplace_back(*it);
	}
	instructValues.args = arguments;
	instructions.emplace_back(instructValues);
	textLine++;
	return true;
}

bool parser::isdivu_2(TokenList & alist_text)
{
	auto alist = alist_text;
	auto it = alist.begin();
	if (it->value() != "divu") {
		return false;
	}
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	auto alist1 = alist_text;
	instructValues.op = Div2U;
	alist1.pop_front();
	alist1.pop_back();
	arguments.clear();
	string str;
	for (TokenList::iterator it = alist1.begin(); it != alist1.end(); it++) {
		arguments.emplace_back(*it);
	}
	instructValues.args = arguments;
	instructions.emplace_back(instructValues);
	textLine++;
	return true;
}

bool parser::isrem(TokenList & alist_text)
{
	auto alist = alist_text;
	auto it = alist.begin();
	if (it->value() != "rem") {
		return false;
	}
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!(isSource(*it)))
		return false;
	return true;
}

bool parser::isremu(TokenList & alist_text)
{
	auto alist = alist_text;
	auto it = alist.begin();
	if (it->value() != "remu") {
		return false;
	}
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!(isSource(*it)))
		return false;
	return true;
}

bool parser::isabs(TokenList & alist_text)
{
	auto alist = alist_text;
	auto it = alist.begin();
	if (it->value() != "abs") {
		return false;
	}
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	return true;
}

bool parser::isneg(TokenList & alist_text)
{
	auto alist = alist_text;
	auto it = alist.begin();
	if (it->value() != "neg") {
		return false;
	}
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	return true;
}

bool parser::isnegu(TokenList & alist_text)
{
	auto alist = alist_text;
	auto it = alist.begin();
	if (it->value() != "negu") {
		return false;
	}
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	return true;
}

bool parser::isand(TokenList & alist_text)
{
	auto alist = alist_text;
	auto it = alist.begin();
	if (it->value() != "and") {
		return false;
	}
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!(isSource(*it)))
		return false;
	auto alist1 = alist_text;
	instructValues.op = And;
	alist1.pop_front();
	alist1.pop_back();
	arguments.clear();
	string str;
	for (TokenList::iterator it = alist1.begin(); it != alist1.end(); it++) {
		arguments.emplace_back(*it);
	}
	instructValues.args = arguments;
	instructions.emplace_back(instructValues);
	textLine++;
	return true;
}

bool parser::isnor(TokenList & alist_text)
{
	size_t count = 0;
	for (auto it = alist_text.begin(); it != alist_text.end(); ++it) {
		count++;
	}
	if (count > 7) {
		return false;
	}
	auto alist = alist_text;
	auto it = alist.begin();
	if (it->value() != "nor") {
		return false;
	}
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!(isSource(*it)))
		return false;
	auto alist1 = alist_text;
	instructValues.op = Nor;
	alist1.pop_front();
	alist1.pop_back();
	arguments.clear();
	string str;
	for (TokenList::iterator it = alist1.begin(); it != alist1.end(); it++) {
		arguments.emplace_back(*it);
	}
	instructValues.args = arguments;
	instructions.emplace_back(instructValues);
	textLine++;
	return true;
}

bool parser::isnot(TokenList & alist_text)
{
	auto alist = alist_text;
	auto it = alist.begin();
	if (it->value() != "not") {
		return false;
	}
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!(isSource(*it)))
		return false;
	auto alist1 = alist_text;
	instructValues.op = Not;
	alist1.pop_front();
	alist1.pop_back();
	arguments.clear();
	string str;
	for (TokenList::iterator it = alist1.begin(); it != alist1.end(); it++) {
		arguments.emplace_back(*it);
	}
	instructValues.args = arguments;
	instructions.emplace_back(instructValues);
	textLine++;
	return true;
}

bool parser::isor(TokenList & alist_text)
{
	auto alist = alist_text;
	auto it = alist.begin();
	if (it->value() != "or") {
		return false;
	}
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!(isSource(*it)))
		return false;
	auto alist1 = alist_text;
	instructValues.op = Or;
	alist1.pop_front();
	alist1.pop_back();
	arguments.clear();
	string str;
	for (TokenList::iterator it = alist1.begin(); it != alist1.end(); it++) {
		arguments.emplace_back(*it);
	}
	instructValues.args = arguments;
	instructions.emplace_back(instructValues);
	textLine++;
	return true;
}

bool parser::isxor(TokenList & alist_text)
{
	auto alist = alist_text;
	auto it = alist.begin();
	if (it->value() != "xor") {
		return false;
	}
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!(isSource(*it)))
		return false;
	auto alist1 = alist_text;
	instructValues.op = Xor;
	alist1.pop_front();
	alist1.pop_back();
	arguments.clear();
	string str;
	for (TokenList::iterator it = alist1.begin(); it != alist1.end(); it++) {
		arguments.emplace_back(*it);
	}
	instructValues.args = arguments;
	instructions.emplace_back(instructValues);
	textLine++;
	return true;
}

bool parser::isjump(TokenList & alist_text)
{
	auto alist = alist_text;
	auto it = alist.begin();
	if ((it->value() != "j")) {
		it++;
		if (it->value() != "j") {
			return false;
		}
		it++;
		if (!(is_text_label(*it))) {//needtobechanged
			return false;
		}
		auto alist1 = alist_text;
		instructValues.op = Jump;
		alist1.pop_front();
		alist1.pop_front();
		alist1.pop_back();
		arguments.clear();
		string str;
		for (TokenList::iterator it = alist1.begin(); it != alist1.end(); it++) {
			arguments.emplace_back(*it);
		}
		instructValues.args = arguments;
		instructions.emplace_back(instructValues);
		textLine++;
		return true;
	}
	else {
		if (it->value() != "j") {
			return false;
		}
		it++;
		if (!(is_text_label(*it))) {//needtobechanged
			return false;
		}
		auto alist1 = alist_text;
		instructValues.op = Jump;
		alist1.pop_front();
		alist1.pop_back();
		arguments.clear();
		string str;
		for (TokenList::iterator it = alist1.begin(); it != alist1.end(); it++) {
			arguments.emplace_back(*it);
		}
		instructValues.args = arguments;
		instructions.emplace_back(instructValues);
		textLine++;
		return true;
	}
	
}

bool parser::isbeq(TokenList & alist_text)
{

	auto alist = alist_text;
	auto it = alist.begin();
	if (it->value() != "beq") {
		return false;
	}
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!(isSource(*it)))
		return false;
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!(is_text_label(*it)))
		return false;
	auto alist1 = alist_text;
	instructValues.op = BranchEq;
	alist1.pop_front();
	alist1.pop_back();
	arguments.clear();
	string str;
	for (TokenList::iterator it = alist1.begin(); it != alist1.end(); it++) {
		arguments.emplace_back(*it);
	}
	instructValues.args = arguments;
	instructions.emplace_back(instructValues);
	textLine++;
	return true;
}

bool parser::isbne(TokenList & alist_text)
{
	auto alist = alist_text;
	auto it = alist.begin();
	if (it->value() != "bne") {
		return false;
	}
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!(isSource(*it)))
		return false;
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!(is_text_label(*it)))
		return false;
	auto alist1 = alist_text;
	instructValues.op = BranchNe;
	alist1.pop_front();
	alist1.pop_back();
	arguments.clear();
	string str;
	for (TokenList::iterator it = alist1.begin(); it != alist1.end(); it++) {
		arguments.emplace_back(*it);
	}
	instructValues.args = arguments;
	instructions.emplace_back(instructValues);
	textLine++;
	return true;
}

bool parser::isblt(TokenList & alist_text)
{
	auto alist = alist_text;
	auto it = alist.begin();
	if (it->value() != "blt") {
		return false;
	}
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!(isSource(*it)))
		return false;
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!(is_text_label(*it)))
		return false;
	auto alist1 = alist_text;
	instructValues.op = BranchLt;
	alist1.pop_front();
	alist1.pop_back();
	arguments.clear();
	string str;
	for (TokenList::iterator it = alist1.begin(); it != alist1.end(); it++) {
		arguments.emplace_back(*it);
	}
	instructValues.args = arguments;
	instructions.emplace_back(instructValues);
	textLine++;
	return true;
}

bool parser::isble(TokenList & alist_text)
{
	auto alist = alist_text;
	auto it = alist.begin();
	if (it->value() != "ble") {
		return false;
	}
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!(isSource(*it)))
		return false;
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!(is_text_label(*it)))
		return false;
	auto alist1 = alist_text;
	instructValues.op = BranchLe;
	alist1.pop_front();
	alist1.pop_back();
	arguments.clear();
	string str;
	for (TokenList::iterator it = alist1.begin(); it != alist1.end(); it++) {
		arguments.emplace_back(*it);
	}
	instructValues.args = arguments;
	instructions.emplace_back(instructValues);
	textLine++;
	return true;
}

bool parser::isbgt(TokenList & alist_text)
{
	auto alist = alist_text;
	auto it = alist.begin();
	if (it->value() != "bgt") {
		return false;
	}
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!(isSource(*it)))
		return false;
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!(is_text_label(*it)))
		return false;
	auto alist1 = alist_text;
	instructValues.op = BranchGt;
	alist1.pop_front();
	alist1.pop_back();
	arguments.clear();
	string str;
	for (TokenList::iterator it = alist1.begin(); it != alist1.end(); it++) {
		arguments.emplace_back(*it);
	}
	instructValues.args = arguments;
	instructions.emplace_back(instructValues);
	textLine++;
	return true;
}

bool parser::isbge(TokenList & alist_text)
{
	auto alist = alist_text;
	auto it = alist.begin();
	if (it->value() != "bge") {
		return false;
	}
	it++;
	if (!isRegister(*it)) {
		return false;
	}
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!(isSource(*it)))
		return false;
	it++;
	if (it->type() != SEP)
		return false;
	it++;
	if (!(is_text_label(*it)))
		return false;
	auto alist1 = alist_text;
	instructValues.op = BranchGe;
	alist1.pop_front();
	alist1.pop_back();
	arguments.clear();
	string str;
	for (TokenList::iterator it = alist1.begin(); it != alist1.end(); it++) {
		arguments.emplace_back(*it);
	}
	instructValues.args = arguments;
	instructions.emplace_back(instructValues);
	textLine++;
	return true;
}

bool parser::isInt(Token atoken)//tobecompleted
{
	
	string a = atoken.value();
	for (list<string>::iterator it = constList.begin(); it != constList.end(); it++) {
		if (a == *it) {
			return true;
		}
	}
	if (a.at(0) == '-' || a.at(0) == '+') {
		for (string::iterator it = next(a.begin()); it !=a.end(); it++) {
			if (!isDigit(*it)) {
				return false;
			}
		}
	}
	else {
		for (string::iterator it = a.begin(); it != a.end(); it++) {
			if (!isDigit(*it)) {
				return false;
			}
		}
	}
	//if (isWord) {
	//	int32_t b;
	//	/*if (!(b = stoi(a))) {
	//		return false;
	//	}*/
	//}
	//else if (isHalf) {
	//	int16_t b;
	//	/*if (!(b = stoi(a))) {
	//		return false;
	//	}*/
	//}
	//else if (isByte) {
	//	int8_t b;
	//	/*if (!(b = stoi(a))) {
	//		return false;
	//	}*/
	//}
	//else if (isSpace) {
	//	uint32_t b;
	//	/*if (!(b = stoi(a))) {
	//		return false;
	//	}*/
	//}
	return true;
}

bool parser::isString_LO(Token atoken)
{
	string a = atoken.value();
	if ((a!= ".ascii" && a!= ".asciiz"))
		return false;
	else
		return true;
}

bool parser::isInt_LO(Token atoken)
{
	string a =atoken.value();
	if (a == ".word") {
		//isWord = true;
		//isHalf = false;
		//isByte = false;
		//isSpace = false;
		return true;
	}
	else if (a == ".half") {
		//isHalf = true;
		//isWord = false;
		//isByte = false;
		//isSpace = false;

		return true;
	}
	else if (a == ".byte") {
		//isByte = true;
		//isWord = false;
		//isHalf = false;
		//isSpace = false;
		return true;
	}
	else if (a == ".space") {
		return true;
	}
	else
		return false;
}

bool parser::isLayout()
{
	layoutType state = S0;
	auto it = declarList.begin();
	do {
		myTokenType input = classify(it, declarList.end());
		if (it!=declarList.end()) {
			//declarList.pop_front();
			layoutCount++;
			++it;
		}
		switch (state) {
		case S0:
			isLayout0(state, input);
			break;
		case S1:
			isLayout1(state, input);
			break;
		case S2:
			isLayout2(state, input);
			break;
		case S3:
			isLayout3(state, input);
			break;
		case S4:
			isLayout4(state, input);
			break;
		case S5:
			isLayout5(state, input);
			break;
		case S6:
			isLayout6(state, input);
			break;
		case S7:
			isLayout7(state, input);
			break;
		case S8:
			isLayout8(state, input);
			break;
		default:
			state = LAYOUT_ERROR;
		};

	} while (state != LAYOUT_ERROR && state != LAYOUT_HALT);
	return state!= LAYOUT_ERROR;
}

bool parser::isLabel()
{
if (isAl_di_col()) {
	TokenList alist = declarList;
	string a = alist.begin()->value();
	string b;
	uint8_t data;
	bool isNegtive = false;
	if (a == ".space" ) {
		alist.pop_front();
		data = stoi(alist.begin()->value());
		for (size_t i = 0; i != data; i++) {
			memoryData.push_back(0);
			var++;
		}
	}
	else {
		alist.pop_front();
		a.pop_back();
		size_t preData;
		uint8_t numPos0 = 0;
		uint8_t numPos1 = 0;
		uint8_t numPos2 = 0;
		uint8_t numPos3 = 0;
		memWordType wordType = Word_pos3;
		if (alist.begin()->value() == ".word") {
			alist.pop_front();
			b = alist.begin()->value();
			if (b.at(0) == '-') {//tobechanged for negtive value
				isNegtive = true;
			}
			preData = stoi(alist.begin()->value());
			if (isNegtive) {
				preData = -1 * preData;
			}
			while (preData > 255) {
				switch (wordType) {
				case Word_pos3:
					if (preData > 16777215) {
						preData = preData - 16777216;
						numPos3++;
						wordType = Word_pos3;
					}
					else {
						wordType = Word_pos2;
					}
					break;
				case Word_pos2:
					if (preData > 65535) {
						preData = preData - 65536;
						numPos2++;
						wordType = Word_pos2;
					}
					else {
						wordType = Word_pos1;
					}
					break;
				case Word_pos1:
					if (preData > 255) {
						preData = preData - 256;
						numPos1++;
						wordType = Word_pos1;
					}
					break;
				};
			}
			numPos0 = uint8_t(preData);
			if (isNegtive) {
				numPos0 = ~numPos0;
				numPos0++;
				numPos1 = ~numPos1;
				numPos2 = ~numPos2;
				numPos3 = ~numPos3;
			}

			memoryData.emplace_back(numPos0);
			memoryData.emplace_back(numPos1);
			memoryData.emplace_back(numPos2);
			memoryData.emplace_back(numPos3);

			DataLabel.insert({ a, var });
			var = var + 4;
		}
		else if (alist.begin()->value() == ".half") {
			alist.pop_front();
			data = stoi(alist.begin()->value());
			DataLabel.insert({ a, var });
			memoryData.emplace_back(data);
			memoryData.emplace_back(0);
			var = var + 2;
		}
		else if (alist.begin()->value() == ".byte") {
			alist.pop_front();
			data = stoi(alist.begin()->value());
			DataLabel.insert({ a, var });
			memoryData.emplace_back(data);
			var = var + 1;
		}
		else if (alist.begin()->value() == ".space") {
			alist.pop_front();
			data = stoi(alist.begin()->value());
			DataLabel.insert({ a, var });
			for (size_t i = 0; i != data; i++) {
				memoryData.emplace_back(0);
				var++;
			}
			
		}

	}
	
	return true;
	//labelList.emplace_back(*declarList.begin());
	
}
return false;
}

bool parser::isConst()
{
	constType state = CONST_S0;
	auto it = declarList.begin();
	do {
		myTokenType input = classify(it, declarList.end());
		if (it != declarList.end()) {
			//declarList.pop_front();
			constCount++;
			++it;
		}
		switch (state) {
		case S0:
			isConstant0(state, input);
			break;
		case S1:
			isConstant1(state, input);
			break;
		case S2:
			isConstant2(state, input);
			break;
		case S3:
			isConstant3(state, input);
			break;
		default:
			state = CONST_ERROR;
		};

	} while (state != CONST_ERROR && state != CONST_HALT);
	return state != CONST_ERROR;
}

bool parser::isDeclara(TokenList alist)
{
	declarList = alist;
	if (declarList.back().type() != EOL) {
		return false;
	}
	declarList.pop_back();
	declarType state = DECLAR_S0;
	//TokenList changed = alist;
	do {
		myDeclarSubType input = declar_classify(declarList.begin(), declarList.end());
		if (!declarList.empty()) {
			declarList.pop_front();
		}
		switch (state) {
		case S0:
			isDeclar0(state, input);
			break;
		case S1:
			isDeclar1(state, input);
			break;
		case S2:
			isDeclar2(state, input);
			break;
		case S3:
			isDeclar3(state, input);
			break;
			//case S4:
			//	isDeclar4(state, input);
			//	break;
		default:
			state = DECLAR_ERROR;
		};

	} while (state != DECLAR_ERROR && state != DECLAR_HALT);
	return state != DECLAR_ERROR;
}

bool parser::parseToken()
{
	TokenList alist;
	string str;
	for (lexit = mylist.begin(); lexit != mylist.end(); lexit++) {
		if (lexit->type() == ERROR) {
			getlexer_error();
			return false;
		}
	}
	str = myit->value();
	myit++;
	if (!myit->type() == EOL) {
		return false;
	}
	myit++;
	while (myit != mylist.end()) {
		if (myit->value() == ".text") {
			alist.clear();
			break;
		}
		if (myit->type() != EOL) {
			alist.emplace_back(*myit);
		}
		else {
			alist.emplace_back(*myit);
			if (alist.begin()->value() == ".space") {
				alist.pop_front();
				uint8_t data = stoi(alist.begin()->value());
				for (size_t i = 0; i < data; i++) {
					memoryData.emplace_back(0);
					var++;
				}
			}
			else  {
				if (!isDeclara(alist))
					return false;
			}
			alist.clear();
		}
		myit++;
	}
	
	if (myit->value() == ".text") {
		myit++;
		myit++;
		for (auto it = myit; it != mylist.end(); it++) {
			if (it->type() != EOL) {
				alist.emplace_back(*it);
			}
			else {
				alist.emplace_back(*it);
				textLine++;
				gettextLabel(alist);
				alist.clear();
			}
			
		}
		while (myit != mylist.end()) {

			if (myit->type() != EOL) {
				alist.emplace_back(*myit);
			}
			else {
				alist.emplace_back(*myit);
				if (!isInstruct(alist)) {
					//return get_error();
					return false;
				}
				alist.clear();
			}
			myit++;
		}

	}
	return true;
}

void parser::isLayout0(layoutType & state, myTokenType input)
{
	//assert(state == S0);
	switch (input) {
	case STRING_INT_LO:
		state = S1;
		break;
	case STRING_LAYOUT:
		state = S5;
		break;
	default:
		state = LAYOUT_ERROR;
		break;
	};

}

void parser::isLayout1(layoutType & state, myTokenType input)
{
	//assert(state == S1);
	switch (input) {
	case STRING_INT:
		state = S2;
		break;
	default:
		state = LAYOUT_ERROR;
		break;
	};

}

void parser::isLayout2(layoutType & state, myTokenType input)
{
	//assert(state == S2);
	switch (input) {
	case EOT:
		state = LAYOUT_HALT;
		break;
	case SEP:
		state = S3;
		break;
	default:
		state = LAYOUT_ERROR;
		break;
	};

}

void parser::isLayout3(layoutType & state, myTokenType input)
{
	//assert(state == S3);
	switch (input)
	{
	case STRING_INT:
		state = S4;
		break;
	default:
		state = LAYOUT_ERROR;
		break;
	}
}

void parser::isLayout4(layoutType & state, myTokenType input)
{
	//assert(state == S4);
	switch (input)
	{
	case EOT:
		state = LAYOUT_HALT;
		break;
	case SEP:
		state = S3;
		break;
	default:
		state = LAYOUT_ERROR;
		break;
	}
}

void parser::isLayout5(layoutType & state, myTokenType input)
{
	//assert(state == S5);
	switch (input)
	{
	case STRING_DEL:
		state = S6;
		break;

	default:
		state = LAYOUT_ERROR;
		break;
	}

}

void parser::isLayout6(layoutType & state, myTokenType input)
{
	//assert(state == S6);
	switch (input)
	{
	case STRING_STRING:
		state = S7;
		break;
	case STRING_AL_DI:
		state = S7;
		break;
	case STRING_AL_DI_COL:
		state = S7;
		break;
	default:
		state = LAYOUT_ERROR;
		break;
	}
}

void parser::isLayout7(layoutType & state, myTokenType input)
{
	//assert(state == S7);
	switch (input)
	{
	case STRING_DEL:
		state = S8;
		break;
	default:
		state = LAYOUT_ERROR;
		break;
	}
}

void parser::isLayout8(layoutType & state, myTokenType input)
{
	//assert(state == S8);
	switch (input)
	{
	case EOT:
		state = LAYOUT_HALT;
		break;
	default:
		state = LAYOUT_ERROR;
		break;
	}
}

void parser::isLabel0(labelType & state, myTokenType input)
{
	//assert(state == S0);
	switch (input) {
	case STRING_AL_DI_COL:
		state = LABEL_S1;
		break;
	default:
		state = LABEL_ERROR;
		break;
	};

}

void parser::isLabel1(labelType & state, myTokenType input)
{
	//assert(state == S1);
	switch (input) {
	case EOT:
		state = LABEL_HALT;
		break;

	default:
		state = LABEL_ERROR;
		break;
	};

}

void parser::isConstant0(constType & state, myTokenType input)
{
	//assert(state == S0);
	switch (input) {
	case STRING_AL_DI:
		state = CONST_S1;
		break;
	default:
		state = CONST_ERROR;
		break;
	};
}

void parser::isConstant1(constType & state, myTokenType input)
{
	//assert(state == S1);
	switch (input) {
	case EQUAL:
		state = CONST_S2;
		break;
	default:
		state = CONST_ERROR;
		break;
	};
}

void parser::isConstant2(constType & state, myTokenType input)
{
	//assert(state == S2);
	switch (input) {
	case STRING_INT:

		state = CONST_S3;
		break;
	default:
		state = CONST_ERROR;
		break;
	};
}

void parser::isConstant3(constType & state, myTokenType input)
{
	//assert(state == S3);
	switch (input) {
	case EOT:
		state = CONST_HALT;
		break;
	case EOL:
		state = CONST_HALT;
		break;
	default:
		state = CONST_ERROR;
		break;
	};
}

void parser::isDeclar0(declarType & state, myDeclarSubType input )
{
	//assert(state == S0);
	switch (input) {
	case CONST:
		state = DECLAR_S1;
		break;
	case LABEL:
		state = DECLAR_S2;
		break;
	case LAYOUT:
		state = DECLAR_S2;
		break;
	default:
		state = DECLAR_ERROR;
		break;
	};
}

void parser::isDeclar1(declarType & state, myDeclarSubType input)
{
	//assert(state == S1);
	switch (input) {
	case DECLAR_EOT:
		state = DECLAR_HALT;
		break;
	default:
		state = DECLAR_ERROR;
		break;
	};
}

void parser::isDeclar2(declarType & state, myDeclarSubType input)
{
	//assert(state == S2);
	switch (input) {
	case DECLAR_EOT:
		state = DECLAR_HALT;
		break;
	case LAYOUT:
		state = DECLAR_S3;
		break;
	default:
		state = DECLAR_ERROR;
		break;
	};
}

void parser::isDeclar3(declarType & state, myDeclarSubType input)
{
	//assert(state == S3);
	switch (input) {
	case DECLAR_EOT:
		state = DECLAR_HALT;
		break;
	default:
		state = DECLAR_ERROR;
		break;
	};
}

void parser::isDeclar4(declarType & state, myDeclarSubType input)
{
	//assert(state == S4);
	switch (input) {
	case DECLAR_EOL:
		state = DECLAR_S2;
		break;
	default:
		state = DECLAR_ERROR;
		break;
	};
}




myTokenType parser::classify(TokenList::const_iterator it, TokenList::const_iterator end)
{
	myTokenType inputvalue = EOT;

	if (it == end) {
		inputvalue = EOT;
	}
	else if (it->type() == EOL) {
		inputvalue = MY_EOL;
	}
	else if (it->type() == OPEN_PAREN) {
		inputvalue = MY_OPEN;
	}
	else if (it->type() == CLOSE_PAREN) {
		inputvalue = MY_CLOSE;
	}
	else if (it->type() == STRING_DELIM) {
		inputvalue = STRING_DEL;
	}
	else if (it->type() == EQUAL) {
		inputvalue = MY_EQUAL;
	}
	else if (it->type() == SEP) {
		inputvalue = MY_SEP;
	}
	else if ((it->type() == STRING) && isString_LO(*it)) {
		inputvalue = STRING_LAYOUT;
	}
	else if ((it->type() == STRING) && isInt_LO(*it)) {
		inputvalue = STRING_INT_LO;
	}
	else if ((it->type() == STRING) && isInt(*it)) {
		inputvalue = STRING_INT;
	}
	else if ((it->type() == STRING) && isAl_di(*it)) {
		inputvalue = STRING_AL_DI;
	}
	/*else if ((it->type() == STRING) && isAl_di_col()) {
		inputvalue = STRING_AL_DI_COL;
	}*/
	else if ((it->type() == STRING) && isChar(*it)) {
		inputvalue = STRING_STRING;
	}
	return inputvalue;
}

myDeclarSubType parser::declar_classify(TokenList::const_iterator it, TokenList::const_iterator end)
{
	immidiate imm;
	myDeclarSubType inputvalue = DECLAR_EOT;
	if (it == end) {
		inputvalue = DECLAR_EOT;
	}
	else if ((it->type() == EOL)) {
		inputvalue = DECLAR_EOL;
	}
	else if (isLabel()) {
		inputvalue = LABEL;
	}
	else if (isConst()) {
		inputvalue = CONST;
		for (size_t i = 0; i < constCount; i++) {
			if (i == 0) {
				imm.name = declarList.begin()->value();
				constList.emplace_back(declarList.begin()->value());
			}
			if (i == 2) {
				imm.value = stoi(declarList.begin()->value());
				immidiateLabels.emplace_back(imm);
			}
			declarList.pop_front();
		}
		
	}
	else if (isLayout()) {
		inputvalue = LAYOUT;
		for (size_t i = 0; i < layoutCount; i++) {
			declarList.pop_front();
		}
		
	}
	else if(it!=end){
		inputvalue = DECLAR_EOL;
	}
	constCount = 0;
	layoutCount = 0;
	return inputvalue;
}

void parser::gettextLabel(TokenList alist)//not right
{
	bool isLabel = true;
	immidiate mylabels;
	auto it = alist.begin();
	string astr = it->value();
	char a = astr.at(0);//first character
	char b = astr.back();//the last character
	string c = it->value();
	string d;
	if (a < 'A' || (a > 'Z' && a < '_') || (a > '_' && a < 'a') || (a > 'z')) {
		isLabel = false;
	}
	if (b != ':') {
		isLabel = false;
	}
	astr.pop_back();
	for (std::string::iterator it = astr.begin(); it != astr.end(); ++it) {
		if (*it < '0' || (*it > '9' && *it < 'A') || (*it > 'Z' && *it < '_') || (*it > '_' && *it < 'a') || (*it > 'z') || *it == ':') {
			isLabel = false;

		}
	}

	for (std::string::iterator it = c.begin(); it != c.end() - 1; ++it) {
		d = d + *it;
	}
	mylabels.name = d;
	//textLine--;

	it++;
	if (it->type() == EOL && isLabel == true) {
		it--;
		textLabels.emplace_back(*it);
		it++;
		labelList.emplace_back(d);
		textLine--;
		mylabels.value = textLine;
		mainVec.emplace_back(mylabels);
		

	}
	else if (isTextLabel(alist)) {
		labelList.emplace_back(d);

	}
		
}

bool parser::isRegister(Token atoken)
{
	string a = atoken.value();
	if (a.at(0) != '$') {
		return false;
	}
	if (a == "$zero") {
		return true;
	}
	if (a.size() == 2) {
		if (a.at(1)>'9' || a.at(1)<'0') {
			return false;
		}
	}
	else if (a.size() == 3) {
		if (a.at(1) == 't') {
			if (a.at(2) > '9' || a.at(2) < '0')
				return false;
		}
		else if (a.at(1) == 'v') {
			if (a.at(2) > '1' || a.at(2) < '0')
				return false;
		}
		else if (a.at(1) == 'a') {
			if (a.at(2) == 't')
				return true;
			else if (a.at(2) > '3' || a.at(2) < '0')
				return false;
		}
		else if (a.at(1) == 's') {
			if (a.at(2) == 'p')
				return true;
			else if (a.at(2) > '7' || a.at(2) < '0')
				return false;
		}
		else if (a.at(1) == 'k') {
			if (a.at(2) > '1' || a.at(2) < '0')
				return false;
		}
		else if (a.at(1) == 'g') {
			if (a.at(2)!='p')
				return false;
		}
		else if (a.at(1) == 'f') {
			if (a.at(2) != 'p')
				return false;
		}
		else if (a.at(1) == 'r') {
			if (a.at(2) != 'a')
				return false;
		}
		else if (a.at(1) > '3' || a.at(1) < '1') {
			return false;
		}
		else if (a.at(1) == '3') {
			if (a.at(2) > '1' || a.at(2) < '0') {
				return false;
			}
		}
		else if (a.at(1) < '3') {
			if (a.at(2) > '9' || a.at(2) < '0')
				return false;
		}

	}
	return true;
}

bool parser::isLabel_fromData(Token atoken)
{
	//string a = atoken.value();

	for (auto it = variables.begin(); it != variables.end(); it++) {
		if (*it == atoken.value()) {
			return true;
		}

	}
	return false;
}

bool parser::isTextLabel(TokenList alist)
{
	auto it =alist.begin();
	string astr = it->value();
	char a = astr.at(0);//first character
	char b = astr.back();//the last character
	string c = it->value();
	string d;
	if (a < 'A' || (a > 'Z' && a < '_') || (a > '_' && a < 'a') || (a > 'z')) {
		return false;
	}
	if (b != ':') {
		return false;
	}
	astr.pop_back();
	for (std::string::iterator it = astr.begin(); it != astr.end(); ++it) {
		if (*it < '0' || (*it > '9' && *it < 'A') || (*it > 'Z' && *it < '_') || (*it > '_' && *it < 'a') || (*it > 'z') || *it == ':') {
			return false;

		}
	}
	it++;
	if (it->type() == EOL) {
		return true;
	}
	else {
		it++;
		if (it->value() == "nop") {
			it++;
			if (it->type() == EOL) {
				return true;
			}
			else
				return false;
		}


	}
	return true;
}

bool parser::is_text_label(Token atoken)
{
	
	for (auto it = labelList.begin(); it != labelList.end(); it++) {
		if (*it == atoken.value()) {
			return true;
		}
	}
	return false;
}

bool parser::isLexerError()
{
	return isLexErr;
}


bool parser::isChar(Token atoken)
{
	string a = atoken.value();
	for (string::iterator it = a.begin(); it != a.end(); it++) {
		if (!isprint(static_cast<unsigned char>(*it))) {
			return false;
		}
	}
	return true;
}

bool parser::isAl_di(Token atoken)
{
	string astr = atoken.value();
	char a = astr.at(0);
	if (a < 'A' || (a > 'Z' && a < '_') || (a > '_' && a < 'a') || (a > 'z')) {
		return false;
	}
	for (std::string::iterator it = astr.begin(); it != astr.end(); ++it) {
		if (*it < '0' || (*it > '9' && *it < 'A') || (*it > 'Z' && *it < '_') || (*it > '_' && *it < 'a') || (*it > 'z')) {
			return false;
		}
	}
	return true;
}

bool parser::isAl_di_col()
{
	auto it = declarList.begin();
	string astr = it->value();
	char a = astr.at(0);//first character
	char b = astr.back();//the last character
	string c = it->value();
	string d;
	if (a < 'A' || (a > 'Z' && a < '_') || (a > '_' && a < 'a') || (a > 'z')) {
		return false;
	}
	if (b != ':') {
		return false;
	}
	astr.pop_back();
	for (std::string::iterator it = astr.begin(); it != astr.end(); ++it) {
		if (*it < '0' || (*it > '9' && *it < 'A') || (*it > 'Z' && *it < '_') || (*it > '_' && *it < 'a') || (*it > 'z') || *it == ':') {
			return false;

		}
	}

	for (std::string::iterator it = c.begin(); it != c.end() - 1; ++it) {
		d = d + *it;
	}
	variables.emplace_back(d);
	return true;
}

bool parser::isDigit(char & str)
{
	if (str<'0' || str>'9')
		return false;
	return true;
}

bool parser::isImmid(Token atoken)
{
	bool isconst = false;
	for (auto it = constList.begin(); it != constList.end(); it++) {
		if (*it == atoken.value()) {
			isconst = true;
		}
	}
	if (isconst) {
		return true;
	}
	string a = atoken.value();
	for (string::iterator it = a.begin(); it != a.end(); it++) {
		if (*it<'0' || *it>'9')
			return false;
	}
	return true;
}

bool parser::isSource(Token atoken)
{
	bool isconst = false;
	for (auto it = constList.begin(); it != constList.end(); it++){
		if (*it == atoken.value()) {
			isconst = true;
		}
	}
	if (isRegister(atoken)) {
		return true;
	}
	else {
		if (isconst) {
			return true;
		}
		else {
			if(isImmid(atoken)){
				return true;
			}
		}
	}
	return false;
}
