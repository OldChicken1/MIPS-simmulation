#ifndef PARSER_HPP
#define PARSER_HPP

#include "lexer.hpp"
#include <list>
#include <string>
#include <unordered_map>
#include <vector>
#include <array>
using namespace std;
// define the parser module here

enum memWordType {
	Word_pos0,
	Word_pos1,
	Word_pos2,
	Word_pos3
};
enum myTokenType {
	MY_EOL,
	MY_SEP,
	MY_EQUAL,
	MY_OPEN,
	MY_CLOSE,
	STRING_AL_DI,
	STRING_AL_DI_COL,
	STRING_DEL,
	STRING_STRING,
	STRING_LAYOUT,
	STRING_INT_LO,
	STRING_INT,
	STRING_CONST,
	EOT
};
enum testTokenType {
	REGISTER,
	OFFSET,
	TEST_EOL,
	TEST_SEP,
	TEST_EQUAL,
	TEST_OPEN,
	TEST_CLOSE,
	TEST_LABEL,
	TEST_SOURCE,
	TEST_IMMID,
	TEST_EOT
};
enum memrefType {
	MEMREF_S0,
	MEMREF_S1,
	MEMREF_S2,
	MEMREF_S3,
	MEMREF_S4,
	MEMREF_S5,
	MEMREF_S6,
	MEMREF_S7,
	MEMREF_HALT,
	MEMREF_ERROR
};
enum myDeclarSubType {
	LAYOUT,
	LABEL,
	CONST,
	DECLAR_EOT,
	DECLAR_EOL
};
enum layoutType {
	S0,
	S1,
	S2,
	S3,
	S4,
	S5,
	S6,
	S7,
	S8,
	LAYOUT_HALT,
	LAYOUT_ERROR
};
enum labelType {
	LABEL_S0,
	LABEL_S1,
//	LABEL_S2,
//	LABEL_S3,
//	LABEL_S4,
	LABEL_HALT,
	LABEL_ERROR
};
enum constType {
	CONST_S0,
	CONST_S1,
	CONST_S2,
	CONST_S3,
//	CONST_S4,
//	CONST_S5,
//	CONST_S6,
	CONST_HALT,
	CONST_ERROR
};
enum declarType {
	DECLAR_S0,
	DECLAR_S1,
	DECLAR_S2,
	DECLAR_S3,
	DECLAR_S4,
	DECLAR_HALT,
	DECLAR_ERROR

};
enum operation {
	LoadWord,
	LoadImmid,
	LoadAdd,
	StoreWord,
	Move,
	MoveFromHi,
	MoveFromLo,
	AddS,
	AddU,
	SubS,
	SubU,
	MultS,
	MultU,
	Div2S,
	Div2U,
	And,
	Nor,
	Not,
	Or,
	Xor,
	Jump,
	BranchEq,
	BranchNe,
	BranchLt,
	BranchLe,
	BranchGt,
	BranchGe,
	NOP,
	NONE
};
typedef std::vector<Token> Argument;
struct instruction {
	operation op;
	Argument args;
	size_t line;
};
struct immidiate {
	string name;
	size_t value;
};

typedef std::unordered_map<std::string, size_t> LabelMapType;
typedef std::vector<Token> TextLabelType;
typedef std::vector<uint8_t> MemoryType;
typedef std::vector<instruction> instructionSet;
typedef std::vector<immidiate> immidiateSet;
//template <typename T>
class parser {
public:
	parser(TokenList alist);
	~parser();
	size_t getFirstTextLine();
	void getInstructionLine(std::string &inputfile);
	const string get_error();
	const string getlexer_error();
	MemoryType getMemory();
	LabelMapType getLabelMap();
	instructionSet getInstructions();
	immidiateSet getImmidiates();
	TextLabelType getTextLabels();
	vector<immidiate> getMainLabels();
	bool isInstruct(TokenList&alist);

	testTokenType test_classify(TokenList::const_iterator it, TokenList::const_iterator end);
	bool isMemref(TokenList::const_iterator it, TokenList::const_iterator end);
	void isMemref0(memrefType & state, testTokenType input);
	void isMemref1(memrefType & state, testTokenType input);
	void isMemref2(memrefType & state, testTokenType input);
	void isMemref3(memrefType & state, testTokenType input);
	void isMemref4(memrefType & state, testTokenType input);
	void isMemref5(memrefType & state, testTokenType input);
	void isMemref6(memrefType & state, testTokenType input);

	//text
	bool isNOP(TokenList &alist_text);//add
	bool isLdword(TokenList &alist_text);//add
	bool isLdhalf(TokenList &alist_text);
	bool isLdbyte(TokenList &alist_text);
	bool isLdimm(TokenList &alist_text);//add
	bool isLdadd(TokenList &alist_text);//add
	bool isLdsw(TokenList &alist_text);//add
	bool isLdsh(TokenList &alist_text);
	bool isLdsb(TokenList &alist_text);
	bool isLdmove(TokenList &alist_text);//add
	bool isLdmfhi(TokenList &alist_text);//add
	bool isLdmflo(TokenList &alist_text);//add
	bool isLdmthi(TokenList &alist_text);
	bool isLdmtlo(TokenList &alist_text);
	bool isadd(TokenList &alist_text);//add
	bool isaddu(TokenList &alist_text);//add
	bool issub(TokenList &alist_text);//add
	bool issubu(TokenList &alist_text);//add
	bool ismul(TokenList &alist_text);
	bool ismulo(TokenList &alist_text);
	bool ismulou(TokenList &alist_text);
	bool ismult(TokenList &alist_text);//add
	bool ismultu(TokenList &alist_text);//add
	bool isdiv_3(TokenList &alist_text);
	bool isdivu_3(TokenList &alist_text);
	bool isdiv_2(TokenList &alist_text);//add
	bool isdivu_2(TokenList &alist_text);//add
	bool isrem(TokenList &alist_text);
	bool isremu(TokenList &alist_text);
	bool isabs(TokenList &alist_text);
	bool isneg(TokenList &alist_text);
	bool isnegu(TokenList &alist_text);
	bool isand(TokenList &alist_text);//add
	bool isnor(TokenList &alist_text);//add
	bool isnot(TokenList &alist_text);//add
	bool isor(TokenList &alist_text);//add
	bool isxor(TokenList &alist_text);//add
	bool isjump(TokenList &alist_text);//add
	bool isbeq(TokenList &alist_text);//add
	bool isbne(TokenList &alist_text);//add
	bool isblt(TokenList &alist_text);//add
	bool isble(TokenList &alist_text);//add
	bool isbgt(TokenList &alist_text);//add
	bool isbge(TokenList &alist_text);//add
	//data
	bool isInt(Token atoken);
	bool isString_LO(Token atoken);
	bool isInt_LO(Token atoken);
	bool isLayout();
	bool isLabel();
	bool isConst();
	bool isDeclara(TokenList alist);
	bool parseToken();
	void isLayout0(layoutType & state, myTokenType input);
	void isLayout1(layoutType & state, myTokenType input);
	void isLayout2(layoutType & state, myTokenType input);
	void isLayout3(layoutType & state, myTokenType input);
	void isLayout4(layoutType & state, myTokenType input);
	void isLayout5(layoutType & state, myTokenType input);
	void isLayout6(layoutType & state, myTokenType input);
	void isLayout7(layoutType & state, myTokenType input);
	void isLayout8(layoutType & state, myTokenType input);

	void isLabel0(labelType & state, myTokenType input);
	void isLabel1(labelType & state, myTokenType input);
	void isConstant0(constType & state, myTokenType input);
	void isConstant1(constType & state, myTokenType input);
	void isConstant2(constType & state, myTokenType input);
	void isConstant3(constType & state, myTokenType input);
	void isDeclar0(declarType & state, myDeclarSubType input);
	void isDeclar1(declarType & state, myDeclarSubType input);
	void isDeclar2(declarType & state, myDeclarSubType input);
	void isDeclar3(declarType & state, myDeclarSubType input);
	void isDeclar4(declarType & state, myDeclarSubType input);
	myTokenType classify(TokenList::const_iterator it, TokenList::const_iterator end);
	myDeclarSubType declar_classify(TokenList::const_iterator it, TokenList::const_iterator end);
	void gettextLabel(TokenList alist);
	bool isRegister(Token atoken);
	bool isLabel_fromData(Token  atoken);
	bool isTextLabel(TokenList alist);
	bool is_text_label(Token atoken);
	bool isLexerError();
	
private:
	size_t firstLine;
	Argument arguments;
	size_t instructLine;
	instruction instructValues;
	size_t var = 0;
	TokenList mylist;
	TokenList::iterator myit;
	TokenList::iterator lexit;
	TokenList declarList;
	//TokenList instructList;
	//TokenList labelList;
	list<string> variables;
	list<string> constList;
	list<string> labelList;
	LabelMapType DataLabel;
	size_t textLine;
	size_t documentLine;
	vector<immidiate> mainVec;
	MemoryType memoryData;
	instructionSet instructions;
	immidiateSet immidiateLabels;
	TextLabelType textLabels;
	size_t layoutCount;
	size_t constCount;
	size_t memrefCount;
	bool isLexErr;
	bool isChar(Token atoken);
	bool isAl_di(Token atoken);
	bool isAl_di_col();
	bool isDigit(char & str);
	bool isImmid(Token atoken);
	bool isSource(Token atoken);
};



#endif
