#ifndef VIRTUAL_MACHINE_GUI_HPP
#define VIRTUAL_MACHINE_GUI_HPP

#include <QString>
#include <QWidget>
#include <qlabel.h>
#include <QPlainTextEdit> 
#include <QHeaderView>
#include <QTableView>
#include <QLineEdit>
#include <QStandardItemModel>
#include <QPushButton>
#include <QLayout>
#include <token.hpp>
#include "parser.hpp"
#include "virtualMachine.hpp"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <QTextBlock>
#include <QTextEdit>

// TODO define the GUI class
class VirtualMachineGUI: public QWidget{
	Q_OBJECT

public:
	VirtualMachineGUI();
	~VirtualMachineGUI();
	void load(QString filename);
	void setregister();
	void closeEvent(QCloseEvent *event);
	void update();
	bool setMemory(MemorySet & mem);

public slots:
void changeReg() {
	if (myvirtual.getPc() == myvirtual.getInstructionSize()-1) {
		myvirtual.pcSimulate();
		
		if (myvirtual.getPc() != myvirtual.getInstructionSize()) {
			cursor.setBlockFormat(white);
			size_t num = GuiInstructions[myvirtual.getPc()].line;
			cursor.movePosition(QTextCursor::Start);
			for (size_t index = 0; index != num; index++) {
				cursor.movePosition(QTextCursor::Down);
			}
			cursor.setBlockFormat(background);
		}
		
		std::stringstream ss;
		string registerName;
		QString QregisterValue;
		string registerValue;
		for (size_t i = 0; i < 35; i++) {
			QregisterValue = regModel->item(i, 1)->text();
			registerName = QregisterValue.toStdString();
			ss << "0x" << std::setfill('0') << std::setw(8) << std::hex << myvirtual.printRegisters(registerName);
			ss >> registerValue;
			regModel->setItem(i, 2, new QStandardItem(QString::fromStdString(registerValue)));
			ss.clear();
		}
		string address;
		QString Qaddress;
		for (size_t i = 0; i < 512; i++) {
			Qaddress = memModel->item(i, 0)->text();
			address = Qaddress.toStdString();
			istringstream buffer{ address };
			uint32_t memoryadd;
			buffer >> std::hex >> memoryadd;
			ss << "0x";
			ss << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(myvirtual.getMemoryAdd(memoryadd));
			ss >> registerValue;
			memModel->setItem(i, 1, new QStandardItem(QString::fromStdString(registerValue)));
			ss.clear();
		}
	}
	else if (myvirtual.getPc() < myvirtual.getInstructionSize()-1) {
		myvirtual.pcSimulate();
		cursor.setBlockFormat(white);
		size_t num = GuiInstructions[myvirtual.getPc()].line;
		cursor.movePosition(QTextCursor::Start);
		for (size_t index = 0; index != num; index++) {
			cursor.movePosition(QTextCursor::Down);
		}
		cursor.setBlockFormat(background);
		std::stringstream ss;
		string registerName;
		QString QregisterValue;
		string registerValue;
		for (size_t i = 0; i < 35; i++) {
			QregisterValue = regModel->item(i, 1)->text();
			registerName = QregisterValue.toStdString();
			ss << "0x" << std::setfill('0') << std::setw(8) << std::hex << myvirtual.printRegisters(registerName);
			ss >> registerValue;
			regModel->setItem(i, 2, new QStandardItem(QString::fromStdString(registerValue)));
			ss.clear();
		}
		string address;
		QString Qaddress;
		for (size_t i = 0; i < 512; i++) {
			Qaddress = memModel->item(i, 0)->text();
			address = Qaddress.toStdString();
			istringstream buffer{ address };
			uint32_t memoryadd;
			buffer >> std::hex >> memoryadd;
			ss << "0x";
			ss << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(myvirtual.getMemoryAdd(memoryadd));
			ss >> registerValue;
			memModel->setItem(i, 1, new QStandardItem(QString::fromStdString(registerValue)));
			ss.clear();
		}
	}
	else {
		cursor.setBlockFormat(white);
		status->setText("Error: invalid Step(End of instructions)");
	}
}

	void setUpThread() {
		string m;
		mq.push("run");
		if (!isThreadUp) {
			myvirtual.threadSet(&mq);
			isThreadUp = true;
		}
		text->setDisabled(true);
		step->setDisabled(true);
		status->setDisabled(true);
		registers->setDisabled(true);
		memory->setDisabled(true);
		run->setDisabled(true);
		runBreak->setEnabled(true);
	}
	void runStop() {
		mq.push("break");
		text->setEnabled(true);
		step->setEnabled(true);
		status->setEnabled(true);
		registers->setEnabled(true);
		memory->setEnabled(true);
		run->setEnabled(true);
		runBreak->setDisabled(true);
		update();
		
	}
private:
	size_t beginLine;
	size_t currentLine;
	QPlainTextEdit *text;
	QTableView *registers;
	QTableView *memory;
	QLineEdit *status;
	QPushButton *step;
	QPushButton *run;
	QPushButton *runBreak;
	QLabel *statusShow;
	QStandardItemModel *regModel;
	QStandardItemModel *memModel;
	VirtualMachine myvirtual;
	bool isFileOk = true;
	bool isThreadUp = false;
	QTextCursor cursor;
	QTextBlockFormat background;
	QTextBlockFormat white;
	instructionSet GuiInstructions;
	MessageQueue mq;
};


#endif
