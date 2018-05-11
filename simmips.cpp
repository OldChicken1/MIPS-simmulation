// implement entry point for simmips here
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <QApplication>
#include "virtualMachine.hpp"
#include "virtual_machine_gui.hpp"
#include <qstring.h>
using namespace std;
int main(int argc, char *argv[])
{
	vector<std::string> args;
	for (auto i = 0; i < argc; ++i) {
		args.push_back(argv[i]);
	}
	if (args.size()<2 || args.size()>3) {
		std::cerr << "Error: " << "invalid file name." << std::endl;
	}
	if (args.size() == 3) {
		if (args[1] == "--gui") {
			string a = args[2];
			QApplication app(argc, argv);
			VirtualMachineGUI myGui;
			myGui.load(QString::fromStdString(args[2]));
			myGui.show();
			return app.exec();
		}
	}
		
	//if (1) {
	//string a = "test20.asm";
	//QApplication app(argc, argv);
	//VirtualMachineGUI myGui;
	//myGui.load(QString::fromStdString(a));
	//myGui.show();
	//return app.exec();
	//}
	else {
		ifstream inputfile(args[1]);
		//ifstream inputfile("test20.asm");

		//string a = "test20.asm";
		string a = args[1];
		if (!inputfile.good()) {
			std::cerr << "Error: " << "invalid file name." << std::endl;
		}
		TokenList mylist = tokenize(inputfile);

		parser myparser(mylist);

		if (!myparser.parseToken()) {
			if (myparser.isLexerError()) {
				cerr << myparser.getlexer_error() << std::endl;
				return EXIT_FAILURE;
			}
			cerr << myparser.get_error() << std::endl;
			return EXIT_FAILURE;
		}
		myparser.getInstructionLine(a);
		VirtualMachine myvirtual;
		myvirtual.setValues(myparser.getLabelMap(), myparser.getMemory(),
			myparser.getInstructions(), myparser.getImmidiates(), myparser.getTextLabels(), myparser.getMainLabels(),
			myparser.getFirstTextLine());
		cout << "simmips> ";
		string input;
		bool isRunning = false;
		bool isThreadUp = false;
		getline(cin, input);
		MessageQueue myqueue;
		bool whileDeter = true;
		while (whileDeter) {
			string print = input.substr(0, 7);
			if (isRunning==true) {
				if (input == "break") {
					myqueue.push(input);
					isRunning = false;
				}
				else {
					cout << "Error: simulation running. Type break to halt." << endl;
				}
			}
			else if (print == "print $") {
				string registerPrint = input.substr(6, 12);
				myvirtual.getRegister(registerPrint);
				if (myvirtual.isError()) {
					cerr << "Error: Invalid register" << endl;
				}
				else {
					cout << "0x";
					cout << std::setfill('0') << std::setw(8) << std::hex << myvirtual.printRegisters(registerPrint) << '\n';
				}

			}
			else if (print == "print &") {
				string address = input.substr(7, 18);
				istringstream buffer{ address };

				uint32_t memoryadd;
				buffer >> std::hex >> memoryadd;
				cout << "0x";
				cout << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(myvirtual.getMemoryAdd(memoryadd)) << '\n';
			}
			else if (input == "step") {
				if (myvirtual.getPc() < myvirtual.getInstructionSize()) {
					myvirtual.pcSimulate();
				}
				cout << "0x";
				cout << std::setfill('0') << std::setw(8) << std::hex << myvirtual.printRegisters("$pc") << '\n';
			}
			else if (input == "status") {
			}
			else if (input == "run") {
				if (isThreadUp == false) {
					myqueue.push(input);
					myvirtual.threadSet(&myqueue);
					isRunning = true;
					isThreadUp = true;
				}
				else {
					myqueue.push(input);
					isRunning = true;
				}
				
			}
			else {
				cerr << "Error: unknown command." << std::endl;
			}
			cout << "simmips> ";
			getline(cin, input);
			if (input == "quit" && isRunning!=true) {
				whileDeter = false;
				myqueue.push(input);
			}
		}
		if (isThreadUp) {
			myvirtual.breakRun();
		}
		
		return EXIT_SUCCESS;
		

	}


}
