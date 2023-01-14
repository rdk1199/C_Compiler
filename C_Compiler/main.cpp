
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
#include <chrono>

#include "CSyntax.h"
#include "TableDrivenScanner.h"
#include "Preprocessor.h"
#include "ExpressionGrammar.h"
#include "C_Grammar.h"
#include "TableParser.h"

using std::cout;
using std::endl;
using std::vector;
using std::map;

int main()
{
	cout << std::boolalpha;

	//C_grammar.LR1_prepare();

	
	cout << "loading non-optimized tables" << endl;

	auto start = std::chrono::system_clock::now();

	

	std::pair<map<int, map<int, Action>>, map<int, map<int, int>>> C_tables =  get_tables_from_file("C_grammar_tables.txt");


	auto end = std::chrono::system_clock::now();


	std::chrono::duration<double> elapsed_seconds = end - start;
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);

	cout << "non-optimized load took: " << elapsed_seconds.count() << "s" << endl;

	/*
	C_grammar.set_actions(C_tables.first);
	C_grammar.set_go_to(C_tables.second);
	*/

	cout << "loading optimized tables" << endl;

	auto o_start = std::chrono::system_clock::now();



	std::pair<map<int, map<int, Action>>, map<int, map<int, int>>> new_C_tables = get_tables_from_file("opt_C_grammar_tables.txt");


	auto o_end = std::chrono::system_clock::now();


	std::chrono::duration<double> o_elapsed_seconds = o_end - o_start;
	std::time_t o_end_time = std::chrono::system_clock::to_time_t(o_end);

	cout << "optimized load took: " << o_elapsed_seconds.count() << "s" << endl;



	//C_grammar.write_LR1_tables_to_file("opt_C_grammar_tables.txt");



	/*

	TableDrivenScanner test_scanner("C:\\Users\\rdk11\\Desktop\\CompilerFiles\\ParseTestFile.txt", C_tokenizer, 1000);
	vector<PPWord> scanned_tokens = test_scanner.scan();
	
	Preprocessor macro_preprocessor(scanned_tokens, "C:\\Users\\rdk11\\Desktop\\CompilerFiles\\h_headers", "C:\\Users\\rdk11\\Desktop\\CompilerFiles\\q_headers");

	vector<Word> processed_tokens = Preprocessor::convert_to_tokens(macro_preprocessor.preprocess());

	ParseTree test_parse_tree = C_grammar.LR1_C_parse(processed_tokens);

	cout << "parse success!" << endl;

	cout << "parse tree:";
	test_parse_tree.print_leaves();
	
	cout << '\a' << endl;

	*/
}





























