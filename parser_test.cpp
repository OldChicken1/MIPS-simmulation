#include "catch.hpp"
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include "parser.hpp"
#include "virtualMachine.hpp"
#include <chrono>
#include <random>

using namespace std;
// put your parser tests here

TEST_CASE("test1", "[parser]") {
	{
		std::string input_true("var1=5");
		std::string input_false("1var:");
		std::istringstream iss1(input_true);
		std::istringstream iss2(input_false);
		TokenList alist1 = tokenize(iss1);
		TokenList alist2 = tokenize(iss2);
		parser parser1(tokenize(iss1));
		parser parser2(tokenize(iss2));
		REQUIRE(parser1.isDeclara(alist1) == true);
		REQUIRE(parser2.isDeclara(alist2) == false);
	}
}
TEST_CASE("test2", "[parser]") {
	{
		std::string input_true("var6:	.ascii \"hello\"");
		std::string input_false("var6:	.ascii  ");
		std::istringstream iss1(input_true);
		std::istringstream iss2(input_false);
		TokenList alist1 = tokenize(iss1);
		TokenList alist2 = tokenize(iss2);
		parser parser1(tokenize(iss1));
		parser parser2(tokenize(iss2));
		REQUIRE(parser1.isDeclara(alist1) == true);
		REQUIRE(parser2.isDeclara(alist2) == false);
	}
}

TEST_CASE("test3", "[parser]") {
	{
		std::string input_true("var6: .byte 250");
		std::string input_false("var6:	.word 5gfsfg");
		std::istringstream iss1(input_true);
		std::istringstream iss2(input_false);
		TokenList alist1 = tokenize(iss1);
		TokenList alist2 = tokenize(iss2);
		parser parser1(tokenize(iss1));
		parser parser2(tokenize(iss2));
		REQUIRE(parser1.isDeclara(alist1) == true);
		REQUIRE(parser2.isDeclara(alist2) == false);
	}
}
TEST_CASE("lw_test0", "[parser]") {
	{
		std::string input_true1("x:	.word 1");
		std::string input_true2("lw $t0, x");
		std::istringstream iss1(input_true1);
		std::istringstream iss2(input_true2);
		TokenList alist1 = tokenize(iss1);
		TokenList alist2 = tokenize(iss2);
		parser parser1(alist1);
		parser1.isDeclara(alist1);
		REQUIRE(parser1.isLdword(alist2) == true);
	}
}

TEST_CASE("lh_test", "[parser]") {
	{
		std::string input_true1("x:	.word 1");
		std::string input_true2("lh $t0, x");
		std::istringstream iss1(input_true1);
		std::istringstream iss2(input_true2);
		TokenList alist1 = tokenize(iss1);
		TokenList alist2 = tokenize(iss2);
		parser parser1(alist1);
		parser1.isDeclara(alist1);
		REQUIRE(parser1.isLdhalf(alist2) == true);
	}
}

TEST_CASE("lb_test", "[parser]") {
	{
		std::string input_true1("x:	.word 1");
		std::string input_true2("lb $t0, x");
		std::istringstream iss1(input_true1);
		std::istringstream iss2(input_true2);
		TokenList alist1 = tokenize(iss1);
		TokenList alist2 = tokenize(iss2);
		parser parser1(alist1);
		parser1.isDeclara(alist1);
		REQUIRE(parser1.isLdbyte(alist2) == true);
	}
}


TEST_CASE("li_test", "[parser]") {
	{
		std::string input_true1("x:	.word 1");
		std::string input_true2("li $t0, $t1");
		std::istringstream iss1(input_true1);
		std::istringstream iss2(input_true2);
		TokenList alist1 = tokenize(iss1);
		TokenList alist2 = tokenize(iss2);
		parser parser1(alist2);
		//parser1.isDeclara(alist1);
		REQUIRE(parser1.isLdimm(alist2) == false);
	}
}
TEST_CASE("la_test", "[parser]") {
	{
		std::string input_true1("y:	.word 1");
		std::string input_true2("la $t0, y");
		std::istringstream iss1(input_true1);
		std::istringstream iss2(input_true2);
		TokenList alist1 = tokenize(iss1);
		TokenList alist2 = tokenize(iss2);
		parser parser1(alist1);
		parser1.isDeclara(alist1);
		REQUIRE(parser1.isLdadd(alist2) == true);
	}
}

TEST_CASE("sw_test", "[parser]") {
	{
		std::string input_true1("y:	.word 1");
		std::string input_true2("sw $t0, $zero");
		std::istringstream iss1(input_true1);
		std::istringstream iss2(input_true2);
		//TokenList alist1 = tokenize(iss1);
		TokenList alist2 = tokenize(iss2);
		parser parser1(alist2);
		//parser1.isDeclara(alist1);
		REQUIRE(parser1.isLdsw(alist2) == true);
	}
}
TEST_CASE("sh_test", "[parser]") {
	{
		std::string input_true1("y:	.word 1");
		std::string input_true2("sh $t0, y");
		std::istringstream iss1(input_true1);
		std::istringstream iss2(input_true2);
		TokenList alist1 = tokenize(iss1);
		TokenList alist2 = tokenize(iss2);
		parser parser1(alist1);
		parser1.isDeclara(alist1);
		REQUIRE(parser1.isLdsh(alist2) == true);
	}
}
TEST_CASE("sb_test", "[parser]") {
	{
		std::string input_true1("y:	.word 1");
		std::string input_true2("sb $t0, y");
		std::istringstream iss1(input_true1);
		std::istringstream iss2(input_true2);
		TokenList alist1 = tokenize(iss1);
		TokenList alist2 = tokenize(iss2);
		parser parser1(alist1);
		parser1.isDeclara(alist1);
		REQUIRE(parser1.isLdsb(alist2) == true);
	}
}
TEST_CASE("move_test", "[parser]") {
	{
		//std::string input_true1("y:	.word 1");
		std::string input_true2("move $t0, $t1");
		//std::istringstream iss1(input_true1);
		std::istringstream iss2(input_true2);
		//TokenList alist1 = tokenize(iss1);
		TokenList alist2 = tokenize(iss2);
		parser parser2(alist2);
		//parser1.isDeclara(alist1);
		REQUIRE(parser2.isLdmove(alist2) == true);
	}
}
TEST_CASE("mfhi_test", "[parser]") {
	{
		std::string input_true1("mfhi $t3");
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser parser1(alist1);
		parser1.isDeclara(alist1);
		REQUIRE(parser1.isLdmfhi(alist1) == true);
	}
}
TEST_CASE("mflo_test", "[parser]") {
	{
		std::string input_true1("mflo $t3");
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser parser1(alist1);
		parser1.isDeclara(alist1);
		REQUIRE(parser1.isLdmflo(alist1) == true);
	}
}
TEST_CASE("mtlo_test", "[parser]") {
	{
		std::string input_true1("mtlo $t3");
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser parser1(alist1);
		parser1.isDeclara(alist1);
		REQUIRE(parser1.isLdmtlo(alist1) == true);
	}
}

TEST_CASE("add_test", "[parser]") {
	{
		std::string input_true1("add $t2, $t2, $32");
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser parser1(alist1);
		parser1.isDeclara(alist1);
		REQUIRE(parser1.isadd(alist1) == false);
	}
}
TEST_CASE("addu_test", "[parser]") {
	{
		std::string input_true1("addu $t3, $t2, 5");
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser parser1(alist1);
		parser1.isDeclara(alist1);
		REQUIRE(parser1.isaddu(alist1) == true);
	}
}
TEST_CASE("sub_test", "[parser]") {
	{
		std::string input_true1("sub $t3, $t2, 5");
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser parser1(alist1);
		parser1.isDeclara(alist1);
		REQUIRE(parser1.issub(alist1) == true);
	}
}
TEST_CASE("subu_test", "[parser]") {
	{
		std::string input_true1("subu $t3, $t2, 5");
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser parser1(alist1);
		parser1.isDeclara(alist1);
		REQUIRE(parser1.issubu(alist1) == true);
	}
}
TEST_CASE("mul_test", "[parser]") {
	{
		std::string input_true1("mul $t3, $t2, 5");
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser parser1(alist1);
		parser1.isDeclara(alist1);
		REQUIRE(parser1.ismul(alist1) == true);
	}
}
TEST_CASE("mulo_test", "[parser]") {
	{
		std::string input_true1("mulo $t3, $t2, 5");
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser parser1(alist1);
		parser1.isDeclara(alist1);
		REQUIRE(parser1.ismulo(alist1) == true);
	}
}
TEST_CASE("mulou_test", "[parser]") {
	{
		std::string input_true1("mulou $t3, $t2, 5");
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser parser1(alist1);
		parser1.isDeclara(alist1);
		REQUIRE(parser1.ismulou(alist1) == true);
	}
}
TEST_CASE("mult_test", "[parser]") {
	{
		std::string input_true1("mult $t3, $t2");
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser parser1(alist1);
		parser1.isDeclara(alist1);
		REQUIRE(parser1.ismult(alist1) == true);
	}
}

TEST_CASE("multu_test", "[parser]") {
	{
		std::string input_true1("multu $t3, $t2");
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser parser1(alist1);
		parser1.isDeclara(alist1);
		REQUIRE(parser1.ismultu(alist1) == true);
	}
}
TEST_CASE("div3_test", "[parser]") {
	{
		std::string input_true1("div $t3, $t2, $t1");
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser parser1(alist1);
		parser1.isDeclara(alist1);
		REQUIRE(parser1.isdiv_3(alist1) == true);
	}
}
TEST_CASE("divu3_test", "[parser]") {
	{
		std::string input_true1("divu $t3, $t2, $t1");
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser parser1(alist1);
		parser1.isDeclara(alist1);
		REQUIRE(parser1.isdivu_3(alist1) == true);
	}
}
TEST_CASE("div2_test", "[parser]") {
	{
		std::string input_true1("div $t3, $t2");
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser parser1(alist1);
		parser1.isDeclara(alist1);
		REQUIRE(parser1.isdiv_2(alist1) == true);
	}
}
TEST_CASE("divu2_test", "[parser]") {
	{
		std::string input_true1("divu $t3, $t2");
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser parser1(alist1);
		parser1.isDeclara(alist1);
		REQUIRE(parser1.isdivu_2(alist1) == true);
	}
}
TEST_CASE("rem_test", "[parser]") {
	{
		std::string input_true1("rem $t3, $t2, $t1");
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser parser1(alist1);
		parser1.isDeclara(alist1);
		REQUIRE(parser1.isrem(alist1) == true);
	}
}
TEST_CASE("remu_test", "[parser]") {
	{
		std::string input_true1("remu $t3, $t2, $t1");
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser parser1(alist1);
		parser1.isDeclara(alist1);
		REQUIRE(parser1.isremu(alist1) == true);
	}
}
TEST_CASE("abs_test", "[parser]") {
	{
		std::string input_true1("abs $t3, $t2");
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser parser1(alist1);
		parser1.isDeclara(alist1);
		REQUIRE(parser1.isabs(alist1) == true);
	}
}
TEST_CASE("neg_test", "[parser]") {
	{
		std::string input_true1("neg $t3, $t2");
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser parser1(alist1);
		parser1.isDeclara(alist1);
		REQUIRE(parser1.isneg(alist1) == true);
	}
}
TEST_CASE("negu_test", "[parser]") {
	{
		std::string input_true1("negu $t3, $t2");
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser parser1(alist1);
		parser1.isDeclara(alist1);
		REQUIRE(parser1.isnegu(alist1) == true);
	}
}
TEST_CASE("and_test", "[parser]") {
	{
		std::string input_true1("and $t3, $t2, $t1");
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser parser1(alist1);
		parser1.isDeclara(alist1);
		REQUIRE(parser1.isand(alist1) == true);
	}
}
TEST_CASE("nor_test", "[parser]") {
	{
		std::string input_true1("nor $t3, $t2, $t1");
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser parser1(alist1);
		parser1.isDeclara(alist1);
		REQUIRE(parser1.isnor(alist1) == true);
	}
}
TEST_CASE("nor_test1", "[parser]") {
	{
		std::string input_true1("nor $t3, $t2, $t1, $k1");
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser parser1(alist1);
		parser1.isDeclara(alist1);
		REQUIRE(parser1.isnor(alist1) == false);
	}
}
TEST_CASE("nor_test2", "[parser]") {
	{
		std::string input_true1("nor $t3, $t22 $t1");
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser parser1(alist1);
		parser1.isDeclara(alist1);
		REQUIRE(parser1.isnor(alist1) == false);
	}
}
TEST_CASE("nor_test3", "[parser]") {
	{
		std::string input_true1("nor $t29, $t2 $t1");
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser parser1(alist1);
		parser1.isDeclara(alist1);
		REQUIRE(parser1.isnor(alist1) == false);
	}
}
TEST_CASE("not_test", "[parser]") {
	{
		std::string input_true1("not $t3, $t2");
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser parser1(alist1);
		parser1.isDeclara(alist1);
		REQUIRE(parser1.isnot(alist1) == true);
	}
}
TEST_CASE("or_test", "[parser]") {
	{
		std::string input_true1("or $t3, $t2, $t1");
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser parser1(alist1);
		parser1.isDeclara(alist1);
		REQUIRE(parser1.isor(alist1) == true);
	}
}
TEST_CASE("or_test1", "[parser]") {
	{
		std::string input_true1("or $t31 $t2 $t1 $s1 $t1");
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser parser1(alist1);
		parser1.isDeclara(alist1);
		REQUIRE(parser1.isor(alist1) == false);
	}
}
TEST_CASE("or_test2", "[parser]") {
	{
		std::string input_true1("or $t333 $t22, $t1");
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser parser1(alist1);
		parser1.isDeclara(alist1);
		REQUIRE(parser1.isor(alist1) == false);
	}
}
TEST_CASE("or_test3", "[parser]") {
	{
		std::string input_true1("or $t18, $t2 $t1");
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser parser1(alist1);
		parser1.isDeclara(alist1);
		REQUIRE(parser1.isor(alist1) == false);
	}
}
TEST_CASE("xor_test", "[parser]") {
	{
		std::string input_true1("xor $t3, $t2, $t1");
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser parser1(alist1);
		parser1.isDeclara(alist1);
		REQUIRE(parser1.isxor(alist1) == true);
	}
}
TEST_CASE("jump_test", "[parser]") {
	{
		std::string input = R"(
	.data
str:	.ascii	"hello"

	.text
main:
	lb $t8, str
	la $t0, str
	lb $t1, ($t0)
	lb $t2, 1($t0)
	lb $t3, 2($t0)
	lb $t4, 3($t0)
	lb $t5, 4($t0)
	
end:	nop)";
		std::string input_true1("j end");
		std::istringstream iss1(input);
		std::istringstream iss2(input_true1);
		TokenList alist1 = tokenize(iss1);
		TokenList alist2 = tokenize(iss2);
		parser parser1(alist1);
		parser1.parseToken();
		REQUIRE(parser1.isjump(alist2) == true);
	}
}

TEST_CASE("beq_test", "[parser]") {
	{
		std::string input_true1 = R"(
	.data
str:	.ascii	"hello"

	.text
main:
	lb $t8, str
	la $t0, str
	lb $t1, ($t0)
	lb $t2, 1($t0)
	lb $t3, 2($t0)
	lb $t4, 3($t0)
	lb $t5, 4($t0)
	
end:	nop)";
		std::string input_true2("beq $t0, 5, main");
		std::istringstream iss1(input_true1);
		std::istringstream iss2(input_true2);
		TokenList alist1 = tokenize(iss1);
		TokenList alist2 = tokenize(iss2);
		parser parser1(alist1);
		parser1.parseToken();
		REQUIRE(parser1.isbeq(alist2) == true);
	}
}

TEST_CASE("bne_test", "[parser]") {
	{
		std::string input_true1 = R"(
	.data
str:	.ascii	"hello"

	.text
main:
	lb $t8, str
	la $t0, str
	lb $t1, ($t0)
	lb $t2, 1($t0)
	lb $t3, 2($t0)
	lb $t4, 3($t0)
	lb $t5, 4($t0)
	
end:	nop)";
		std::string input_true2("bne $t0, 5, end");
		std::istringstream iss1(input_true1);
		std::istringstream iss2(input_true2);
		TokenList alist1 = tokenize(iss1);
		TokenList alist2 = tokenize(iss2);
		parser parser1(alist1);
		parser1.parseToken();
		REQUIRE(parser1.isbne(alist2) == true);
	}
}
TEST_CASE("bne_test1", "[parser]") {
	{
		std::string input_true1 = R"(
	.data
str:	.ascii	"hello"

	.text
main:
	lb $t8, str
	la $t0, str
	lb $t1, ($t0)
	lb $t2, 1($t0)
	lb $t3, 2($t0)
	lb $t4, 3($t0)
	lb $t5, 4($t0)
	
end:	nop)";
		std::string input_true2("bne $t, 5 end");
		std::istringstream iss1(input_true1);
		std::istringstream iss2(input_true2);
		TokenList alist1 = tokenize(iss1);
		TokenList alist2 = tokenize(iss2);
		parser parser1(alist1);
		parser1.parseToken();
		REQUIRE(parser1.isbne(alist2) == false);
	}
}

TEST_CASE("bne_test3", "[parser]") {
	{
		std::string input_true1 = R"(
	.data
str:	.ascii	"hello"

	.text
main:
	lb $t8, str
	la $t0, str
	lb $t1, ($t0)
	lb $t2, 1($t0)
	lb $t3, 2($t0)
	lb $t4, 3($t0)
	lb $t5, 4($t0)
	
end:	nop)";
		std::string input_true2("bne $t0 5, end");
		std::istringstream iss1(input_true1);
		std::istringstream iss2(input_true2);
		TokenList alist1 = tokenize(iss1);
		TokenList alist2 = tokenize(iss2);
		parser parser1(alist1);
		parser1.parseToken();
		REQUIRE(parser1.isbne(alist2) == false);
	}
}
TEST_CASE("blt_test", "[parser]") {
	{
		std::string input_true1 = R"(
	.data
str:	.ascii	"hello"

	.text
main:
	lb $t8, str
	la $t0, str
	lb $t1, ($t0)
	lb $t2, 1($t0)
	lb $t3, 2($t0)
	lb $t4, 3($t0)
	lb $t5, 4($t0)
	
end:	nop)";
		std::string input_true2("blt $t1, 5, end");
		std::istringstream iss1(input_true1);
		std::istringstream iss2(input_true2);
		TokenList alist1 = tokenize(iss1);
		TokenList alist2 = tokenize(iss2);
		parser parser1(alist1);
		parser1.parseToken();
		REQUIRE(parser1.isblt(alist2) == true);
	}
}
TEST_CASE("blt_test1", "[parser]") {
	{
		std::string input_true1("blt $t0, 5, end, end");
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser parser1(alist1);
		parser1.isDeclara(alist1);
		REQUIRE(parser1.isblt(alist1) == false);
	}
}
TEST_CASE("blt_test2", "[parser]") {
	{
		std::string input_true1("blt $t0, 5 end");
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser parser1(alist1);
		parser1.isDeclara(alist1);
		REQUIRE(parser1.isblt(alist1) == false);
	}
}
TEST_CASE("blt_test3", "[parser]") {
	{
		std::string input_true1("blt $t0 5, end");
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser parser1(alist1);
		parser1.isDeclara(alist1);
		REQUIRE(parser1.isblt(alist1) == false);
	}
}
TEST_CASE("ble_test", "[parser]") {
	{
		std::string input_true1 = R"(
	.data
str:	.ascii	"hello"

	.text
main:
	lb $t8, str
	la $t0, str
	lb $t1, ($t0)
	lb $t2, 1($t0)
	lb $t3, 2($t0)
	lb $t4, 3($t0)
	lb $t5, 4($t0)
	
end:	nop)";
		std::string input_true2("ble $t0, 5, main");
		std::istringstream iss1(input_true1);
		std::istringstream iss2(input_true2);
		TokenList alist1 = tokenize(iss1);
		TokenList alist2 = tokenize(iss2);
		parser parser1(alist1);
		parser1.parseToken();
		REQUIRE(parser1.isble(alist2) == true);
	}
}
TEST_CASE("ble_test1", "[parser]") {
	{
		std::string input_true1("ble $t0, 5, end, end");
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser parser1(alist1);
		parser1.isDeclara(alist1);
		REQUIRE(parser1.isble(alist1) == false);
	}
}
TEST_CASE("ble_test2", "[parser]") {
	{
		std::string input_true1("ble $t0, 5 end");
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser parser1(alist1);
		parser1.isDeclara(alist1);
		REQUIRE(parser1.isble(alist1) == false);
	}
}
TEST_CASE("ble_test3", "[parser]") {
	{
		std::string input_true1("ble $t0 5, end");
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser parser1(alist1);
		parser1.isDeclara(alist1);
		REQUIRE(parser1.isble(alist1) == false);
	}
}
TEST_CASE("bgt_test", "[parser]") {
	{
		std::string input_true1 = R"(
	.data
str:	.ascii	"hello"

	.text
main:
	lb $t8, str
	la $t0, str
	lb $t1, ($t0)
	lb $t2, 1($t0)
	lb $t3, 2($t0)
	lb $t4, 3($t0)
	lb $t5, 4($t0)
	
end:	nop)";
		std::string input_true2("bgt $t0, 5, main");
		std::istringstream iss1(input_true1);
		std::istringstream iss2(input_true2);
		TokenList alist1 = tokenize(iss1);
		TokenList alist2 = tokenize(iss2);
		parser parser1(alist1);
		parser1.parseToken();
		REQUIRE(parser1.isbgt(alist2) == true);
	}
}
TEST_CASE("bgt_test1", "[parser]") {
	{
		std::string input_true1("bgt $t0, 5, end, end");
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser parser1(alist1);
		parser1.isDeclara(alist1);
		REQUIRE(parser1.isbgt(alist1) == false);
	}
}
TEST_CASE("bgt_test2", "[parser]") {
	{
		std::string input_true1("bgt $t0 5, end");
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser parser1(alist1);
		parser1.isDeclara(alist1);
		REQUIRE(parser1.isbgt(alist1) == false);
	}
}
TEST_CASE("bgt_test3", "[parser]") {
	{
		std::string input_true1("bgt $t0, 5 end");
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser parser1(alist1);
		parser1.isDeclara(alist1);
		REQUIRE(parser1.isbgt(alist1) == false);
	}
}
TEST_CASE("bge_test", "[parser]") {
	{
		std::string input_true1 = R"(
	.data
str:	.ascii	"hello"

	.text
main:
	lb $t8, str
	la $t0, str
	lb $t1, ($t0)
	lb $t2, 1($t0)
	lb $t3, 2($t0)
	lb $t4, 3($t0)
	lb $t5, 4($t0)
	
end:	nop)";
		std::string input_true2("bge $t0, 5, end");
		std::istringstream iss1(input_true1);
		std::istringstream iss2(input_true2);
		TokenList alist1 = tokenize(iss1);
		TokenList alist2 = tokenize(iss2);
		parser parser1(alist1);
		parser1.parseToken();
		REQUIRE(parser1.isbge(alist2) == true);
	}
}
TEST_CASE("virtualMachine test", "[vm]") {
	{
		std::string input_true1 = R"(
        .data
        .space 8
var1:   .word 1
var2:   .word -2

        .text
main:
     	la $t0, var1

	lw $t1, 0
	lw $t2, $t0
	lw $t3, 4($t0)
	lw $t4, 4(var1)
	lw $t5, var2)";
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser myparser(alist1);
		myparser.parseToken();
		VirtualMachine myvirtual;
		myvirtual.setValues(myparser.getLabelMap(), myparser.getMemory(),
			myparser.getInstructions(), myparser.getImmidiates(), myparser.getTextLabels(), myparser.getMainLabels(),
			myparser.getFirstTextLine());
		REQUIRE(myvirtual.getPc()==0);
		myvirtual.pcSimulate();
		REQUIRE(myvirtual.getPc() == 1);
		REQUIRE(myvirtual.getRegister("$t0") != 0);
		myvirtual.pcSimulate();
		myvirtual.pcSimulate();
		myvirtual.pcSimulate();
		REQUIRE(myvirtual.getPc() != 1);
	}
	
}
TEST_CASE("virtualMachine test2", "[vm]") {
	{
		std::string input_true1 = R"(
        .data
r1:     .space 4
r2:     .space 12
r3:     .space 4
var:    .word 7

        .text
main:
        la $t0, r2
     	lw $t1, var

	sw $t1, 0
	sw $t1, $t0
	sw $t1, 4($t0)
	sw $t1, 8(r2)
	sw $t1, r3)";
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser myparser(alist1);
		myparser.parseToken();
		VirtualMachine myvirtual;
		myvirtual.setValues(myparser.getLabelMap(), myparser.getMemory(),
			myparser.getInstructions(), myparser.getImmidiates(), myparser.getTextLabels(), myparser.getMainLabels(),
			myparser.getFirstTextLine());
		REQUIRE(myvirtual.getPc() == 0);
		REQUIRE(myvirtual.getInstructionSize() == 7);
		REQUIRE(myvirtual.getRegister("$lo") == RegisterLo);
		myvirtual.pcSimulate();
		REQUIRE(myvirtual.getPc() == 1);
		REQUIRE(myvirtual.getRegister("$t0") != 0);
		REQUIRE(myvirtual.getMemoryAdd(0) == 0);
		myvirtual.pcSimulate();
		myvirtual.pcSimulate();
		myvirtual.pcSimulate();
		REQUIRE(myvirtual.getPc() != 1);
	}

}
typedef int Messages;
typedef message_queue<Messages> MessageQueues;

// for generating random sleep times

class Consumer
{
public:

	Consumer(MessageQueues *messageQueuePtr, int identifier)
	{
		mq = messageQueuePtr;
		id = identifier;
	}

	void operator()() const
	{
		for (int i = 0; i < 5; ++i) {
			Messages m;
			mq->wait_and_pop(m);
		}
		for (int i = 0; i < 5; ++i) {
			Messages m;
			mq->try_pop(m);
		}
	}

private:

	int id;
	MessageQueues * mq;
};

class Producer
{
public:

	Producer(MessageQueues *messageQueuePtr)
	{
		mq = messageQueuePtr;
	}

	void operator()() const
	{
		for (int i = 0; i < 10; ++i) {
			mq->push(i);
		}
	}

private:
	MessageQueues * mq;
};

TEST_CASE("virtualMachine(message queue)", "[mq]") {
	MessageQueues mq;

	Consumer c1(&mq, 1);
	std::thread consumer_th1(c1);

	Producer p1(&mq);
	std::thread producer_th1(p1);

	consumer_th1.join();
	producer_th1.join();

}

TEST_CASE("virtualMachine test3", "[vm]") {
	{
		std::string input_true1 = R"(
        .data
var0:   .word 0
var1:   .word 1
var2:   .word 2
var3:   .word 3
        .text
main:
        lw $t0, var0
        lw $t1, var1
        lw $t2, var2
        lw $t3, var3

        beq $t0, $t1, check1
        beq $t0, $t0, check1
        nop
check1:
        bne $t0, $t0, check2
        bne $t0, $t1, check2
        nop
check2:
        bgt $t0, $t0, check3
        bgt $t3, $t1, check3
        nop
check3:
        bge $t0, $t1, check4
        bge $t3, $t2, check4
        nop
check4:
        blt $t3, $t1, check5
        blt $t1, $t3, check5
        nop
check5:
        ble $t3, $t1, check6
        ble $t3, $t3, check6
        nop
check6:
        nop
        j check6)";
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser myparser(alist1);
		myparser.parseToken();
		VirtualMachine myvirtual;
		myvirtual.setValues(myparser.getLabelMap(), myparser.getMemory(),
			myparser.getInstructions(), myparser.getImmidiates(), myparser.getTextLabels(), myparser.getMainLabels(),
			myparser.getFirstTextLine());
		REQUIRE(myvirtual.getPc() == 0);
		REQUIRE(myvirtual.getInstructionSize() == 24);
		for (size_t i = 0; i < 25; i++) {
			myvirtual.pcSimulate();
		}
	}

}

TEST_CASE("virtualMachine test4", "[vm]") {
	{
		std::string input_true1 = R"(
        .data
VALUE = -1234

        .text
main:
        li $t0, VALUE
        move $t1, $t0)";
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser myparser(alist1);
		myparser.parseToken();
		VirtualMachine myvirtual;
		myvirtual.setValues(myparser.getLabelMap(), myparser.getMemory(),
			myparser.getInstructions(), myparser.getImmidiates(), myparser.getTextLabels(), myparser.getMainLabels(),
			myparser.getFirstTextLine());
		REQUIRE(myvirtual.getPc() == 0);
		REQUIRE(myvirtual.getInstructionSize() == 2);
		for (size_t i = 0; i < 2; i++) {
			myvirtual.pcSimulate();
		}
		REQUIRE(myvirtual.getPc() == 2);
	}

}

TEST_CASE("virtualMachine test5", "[vm]") {
	{
		std::string input_true1 = R"(
        .data
VALUE = -1
var:    .word 1
        .text
main:
        lw $t0, var
        add $t1, $t0, VALUE
        addu $t2, $t1, $t0)";
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser myparser(alist1);
		myparser.parseToken();
		VirtualMachine myvirtual;
		myvirtual.setValues(myparser.getLabelMap(), myparser.getMemory(),
			myparser.getInstructions(), myparser.getImmidiates(), myparser.getTextLabels(), myparser.getMainLabels(),
			myparser.getFirstTextLine());
		REQUIRE(myvirtual.getPc() == 0);
		REQUIRE(myvirtual.getInstructionSize() == 3);
		for (size_t i = 0; i < 3; i++) {
			myvirtual.pcSimulate();
		}
		REQUIRE(myvirtual.getPc() == 3);
	}

}
TEST_CASE("virtualMachine test6", "[vm]") {
	{
		std::string input_true1 = R"(
        .data
VALUE = 2
var1:   .word 1
var2:   .word 12
var3:   .word -1
        .text
main:
        lw $t0, var1
        lw $t1, var2
        lw $t2, var3
        sub $t3, $t0, VALUE
        sub $t4, $t1, $t0 
        sub $t5, $t2, VALUE)";
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser myparser(alist1);
		myparser.parseToken();
		VirtualMachine myvirtual;
		myvirtual.setValues(myparser.getLabelMap(), myparser.getMemory(),
			myparser.getInstructions(), myparser.getImmidiates(), myparser.getTextLabels(), myparser.getMainLabels(),
			myparser.getFirstTextLine());
		REQUIRE(myvirtual.getPc() == 0);
		REQUIRE(myvirtual.getInstructionSize() == 6);
		for (size_t i = 0; i < 6; i++) {
			myvirtual.pcSimulate();
		}
		REQUIRE(myvirtual.getPc() == 6);
	}

}
TEST_CASE("virtualMachine test7", "[vm]") {
	{
		std::string input_true1 = R"(
        .data
VALUE = 2
var1:   .word 1
var2:   .word 12
var3:   .word -1
        .text
main:
        lw $t0, var1
        lw $t1, var2
        lw $t2, var3
        subu $t3, $t0, VALUE
        subu $t4, $t1, $t0
        subu $t5, $t2, VALUE)";
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser myparser(alist1);
		myparser.parseToken();
		VirtualMachine myvirtual;
		myvirtual.setValues(myparser.getLabelMap(), myparser.getMemory(),
			myparser.getInstructions(), myparser.getImmidiates(), myparser.getTextLabels(), myparser.getMainLabels(),
			myparser.getFirstTextLine());
		REQUIRE(myvirtual.getPc() == 0);
		REQUIRE(myvirtual.getInstructionSize() == 6);
		for (size_t i = 0; i < 6; i++) {
			myvirtual.pcSimulate();
		}
		REQUIRE(myvirtual.getPc() == 6);
	}

}
TEST_CASE("virtualMachine test8", "[vm]") {
	{
		std::string input_true1 = R"(
        .data
VALUE = 4
VALUE2 = -4
var1:   .word 2
var2:   .word -2
var3:   .word 1073741824
        .text
main:
        lw $t0, var1
        li $t1, VALUE
        mult $t0, $t1
        mflo $t8
        mfhi $t9

        lw $t0, var2
        lw $t1, var1
        mult $t0, $t1
        mflo $t8
        mfhi $t9

        lw $t0, var3
        li $t1, VALUE
        mult $t0, $t1
        mflo $t8
        mfhi $t9

        lw $t0, var3
        li $t1, VALUE2
        mult $t0, $t1
        mflo $t8
        mfhi $t9)";
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser myparser(alist1);
		myparser.parseToken();
		VirtualMachine myvirtual;
		myvirtual.setValues(myparser.getLabelMap(), myparser.getMemory(),
			myparser.getInstructions(), myparser.getImmidiates(), myparser.getTextLabels(), myparser.getMainLabels(),
			myparser.getFirstTextLine());
		REQUIRE(myvirtual.getPc() == 0);
		REQUIRE(myvirtual.getInstructionSize() == 20);
		for (size_t i = 0; i < 20; i++) {
			myvirtual.pcSimulate();
		}
		REQUIRE(myvirtual.getPc() == 20);
	}

}
TEST_CASE("virtualMachine test9", "[vm]") {
	{
		std::string input_true1 = R"(
        .data
VALUE = 4
VALUE2 = -4
var1:   .word 2
var2:   .word -2
var3:   .word 1073741824
        .text
main:
                lw $t0, var1
        li $t1, VALUE
        div $t0, $t1
        mflo $t8
        mfhi $t9

        lw $t0, var2
        lw $t1, var1
        div $t0, $t1
        mflo $t8
        mfhi $t9

        lw $t0, var3
        li $t1, VALUE
        div $t0, $t1
        mflo $t8
        mfhi $t9

        lw $t0, var3
        li $t1, VALUE2
        div $t0, $t1
        mflo $t8
        mfhi $t9
        div $t0, $0)";
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser myparser(alist1);
		myparser.parseToken();
		VirtualMachine myvirtual;
		myvirtual.setValues(myparser.getLabelMap(), myparser.getMemory(),
			myparser.getInstructions(), myparser.getImmidiates(), myparser.getTextLabels(), myparser.getMainLabels(),
			myparser.getFirstTextLine());
		REQUIRE(myvirtual.getPc() == 0);
		REQUIRE(myvirtual.getInstructionSize() == 21);
		for (size_t i = 0; i < 21; i++) {
			myvirtual.pcSimulate();
		}
		REQUIRE(myvirtual.getPc() == 21);
		REQUIRE(myvirtual.getRegister("$t8") == 24);
		REQUIRE(myvirtual.getRegister("$t9") == 25);
	}

}
TEST_CASE("virtualMachine test11", "[vm]") {
	{
		std::string input_true1 = R"(
        .data
VALUE = 3
var1:   .word 12
var2:   .word 10
        .text
main:
        lw $t0, var1
        lw $t1, var2
        and $t2, $t0, $t1  
        and $t2, $t0, VALUE
        nor $t2, $t0, $t1  
        nor $t2, $t0, VALUE
        or $t2, $t0, $t1  
		or $t2, $t0, VALUE
		xor $t2, $t0, VALUE
		xor $t2, $t0, $t1 
		not $t2, $t0
        not $t2, $t1
		not $t2, VALUE)";
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser myparser(alist1);
		myparser.parseToken();
		VirtualMachine myvirtual;
		myvirtual.setValues(myparser.getLabelMap(), myparser.getMemory(),
			myparser.getInstructions(), myparser.getImmidiates(), myparser.getTextLabels(), myparser.getMainLabels(),
			myparser.getFirstTextLine());
		REQUIRE(myvirtual.getPc() == 0);
		REQUIRE(myvirtual.getInstructionSize() == 13);
		for (size_t i = 0; i < 13; i++) {
			myvirtual.pcSimulate();
		}
		REQUIRE(myvirtual.getPc() == 13);
	}

}
TEST_CASE("virtualMachine test12", "[vm]") {
	{
		std::string input_true1 = R"(
        .data
r1:     .space 4
r2:     .space 12
r3:     .space 4
var:    .word 7

        .text
main:
        la $t0, r2
     	lw $t1, var

	sw $t1, 0
	sw $t1, $t0
	sw $t1, 4($t0)
	sw $t1, 8(r2)
	sw $t1, r3)";
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser myparser(alist1);
		myparser.parseToken();
		VirtualMachine myvirtual;
		myvirtual.setValues(myparser.getLabelMap(), myparser.getMemory(),
			myparser.getInstructions(), myparser.getImmidiates(), myparser.getTextLabels(), myparser.getMainLabels(),
			myparser.getFirstTextLine());
		REQUIRE(myvirtual.getPc() == 0);
		REQUIRE(myvirtual.getInstructionSize() == 7);
		for (size_t i = 0; i < 7; i++) {
			myvirtual.pcSimulate();
		}
		REQUIRE(myvirtual.getPc() == 7);
	}

}
TEST_CASE("virtualMachine test13", "[vm]") {
	{
		std::string input_true1 = R"(
        .data
        .space 8
var1:   .word 1
var2:   .word -2

        .text
main:
     	la $t0, var1

	lw $t1, 0
	lw $t2, $t0
	lw $t3, 4($t0)
	lw $t4, 4(var1)
	lw $t5, var2
	
)";
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser myparser(alist1);
		myparser.parseToken();
		VirtualMachine myvirtual;
		myvirtual.setValues(myparser.getLabelMap(), myparser.getMemory(),
			myparser.getInstructions(), myparser.getImmidiates(), myparser.getTextLabels(), myparser.getMainLabels(),
			myparser.getFirstTextLine());
		REQUIRE(myvirtual.getPc() == 0);
		REQUIRE(myvirtual.getInstructionSize() == 6);
		for (size_t i = 0; i < 6; i++) {
			myvirtual.pcSimulate();
		}
		REQUIRE(myvirtual.getPc() == 6);
	}

}
TEST_CASE("virtualMachine test14", "[vm]") {
	{
		std::string input_true1 = R"(
        .data
        .space 8
var1:   .word 1
var2:   .word -2

        .text
main:
     	la $t0, var1
	
)";
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser myparser(alist1);
		myparser.parseToken();
		VirtualMachine myvirtual;
		myvirtual.setValues(myparser.getLabelMap(), myparser.getMemory(),
			myparser.getInstructions(), myparser.getImmidiates(), myparser.getTextLabels(), myparser.getMainLabels(),
			myparser.getFirstTextLine());
		REQUIRE(myvirtual.getPc() == 0);
		myvirtual.getRegister("zz");
		REQUIRE(myvirtual.isError()==true);
		REQUIRE(myvirtual.getPc() == 0);
	}

}

TEST_CASE("virtualMachine test15", "[vm]") {
	{
		std::string input_true1 = R"(
        .data
VALUE = 4
var1:   .word 2
var2:   .word 1073741824
        .text
main:
        lw $t0, var1
        li $t1, VALUE
        mult $t0, $t1
        mflo $t8
        mfhi $t9

        lw $t0, var2
        li $t1, VALUE
        mult $t0, $t1
        mflo $t8
        mfhi $t9
	
)";
		std::istringstream iss1(input_true1);
		TokenList alist1 = tokenize(iss1);
		parser myparser(alist1);
		myparser.parseToken();
		VirtualMachine myvirtual;
		myvirtual.setValues(myparser.getLabelMap(), myparser.getMemory(),
			myparser.getInstructions(), myparser.getImmidiates(), myparser.getTextLabels(), myparser.getMainLabels(),
			myparser.getFirstTextLine());
		REQUIRE(myvirtual.getPc() == 0);
		REQUIRE(myvirtual.getInstructionSize() == 10);
		for (size_t i = 0; i < 10; i++) {
			myvirtual.pcSimulate();
		}
		REQUIRE(myvirtual.getPc() == 10);
	}

}
