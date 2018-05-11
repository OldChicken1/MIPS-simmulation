#include "virtual_machine_gui.hpp"

// TODO implement the GUI class

VirtualMachineGUI::VirtualMachineGUI()
{
	QGridLayout *layout = new QGridLayout;
	text = new QPlainTextEdit;
	registers = new QTableView;
	memory = new QTableView;
	status = new QLineEdit;
	step = new QPushButton("Step");
	run = new QPushButton("Run");
	runBreak = new QPushButton("Break");
	statusShow = new QLabel("status:");

	regModel = new QStandardItemModel(35, 3);
	memModel = new QStandardItemModel(512, 2);
	background.setBackground(QColor("yellow"));
	white.setBackground(QColor("white"));


	status->setReadOnly(true);
	runBreak->setDisabled(true);
	registers->setModel(regModel);
	regModel->setHeaderData(0, Qt::Horizontal, QObject::tr("Number"));
	regModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Alias"));
	regModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Value(Hex)"));
	registers->verticalHeader()->setVisible(false);
	memory->setModel(memModel);
	memModel->setHeaderData(0, Qt::Horizontal, QObject::tr("Address(Hex)"));
	memModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Value(Hex)"));
	memory->verticalHeader()->setVisible(false);

	text->setObjectName("text");
	registers->setObjectName("registers");
	memory->setObjectName("memory");
	status->setObjectName("status");
	step->setObjectName("step");
	run->setObjectName("run");
	runBreak->setObjectName("break");
	text->setLineWrapMode(QPlainTextEdit::NoWrap);

	layout->addWidget(text, 0, 0);
	layout->addWidget(registers, 0, 1);
	layout->addWidget(memory, 0, 2);
	layout->addWidget(status, 1, 1, 1, 3);
	layout->addWidget(statusShow, 1, 0);
	layout->addWidget(step, 2, 0, 1, 1);
	layout->addWidget(run, 2, 1, 1, 1);
	layout->addWidget(runBreak, 2, 2, 1, 1);
	setLayout(layout);

	QObject::connect(step, SIGNAL(clicked()), this, SLOT(changeReg()));
	QObject::connect(run, SIGNAL(clicked()), this, SLOT(setUpThread()));
	QObject::connect(runBreak, SIGNAL(clicked()), this, SLOT(runStop()));
}


VirtualMachineGUI::~VirtualMachineGUI()
{
}

void VirtualMachineGUI::load(QString filename)
{
	QFile inputfile(filename);
	if (!inputfile.open(QIODevice::ReadOnly)) {
		isFileOk = false;
		status->setText("Error: Invalid input file");
		step->setDisabled(true);
		run->setDisabled(true);
	}
	else {
		string afilename = filename.toStdString();
		std::ifstream inputfile1(afilename);
		TokenList mylist = tokenize(inputfile1);
		parser myparser(mylist);
		myparser.parseToken();
		myparser.getInstructionLine(afilename);
		myvirtual.setValues(myparser.getLabelMap(), myparser.getMemory(),
			myparser.getInstructions(), myparser.getImmidiates(), myparser.getTextLabels(), myparser.getMainLabels(), myparser.getFirstTextLine());
		GuiInstructions = myvirtual.getInstuctions();
		beginLine = myvirtual.getfirstTextLine();
		text->setPlainText(inputfile.readAll());
		status->setText("Ok");
	}
	

	

	if (isFileOk) {
		cursor = text->textCursor();
		for (size_t index = 0; index != beginLine; index++) {
			cursor.movePosition(QTextCursor::Down);
		}
		text->setTextCursor(cursor);
		cursor.setBlockFormat(background);
		setregister();
	}
	
}

void VirtualMachineGUI::setregister()
{
	regModel->setItem(0, 1, new QStandardItem("$pc"));
	regModel->setItem(1, 1, new QStandardItem("$hi"));
	regModel->setItem(2, 1, new QStandardItem("$lo"));
	regModel->setItem(3, 0, new QStandardItem("$0"));
	regModel->setItem(4, 0, new QStandardItem("$1"));
	regModel->setItem(5, 0, new QStandardItem("$2"));
	regModel->setItem(6, 0, new QStandardItem("$3"));
	regModel->setItem(7, 0, new QStandardItem("$4"));
	regModel->setItem(8, 0, new QStandardItem("$5"));
	regModel->setItem(9, 0, new QStandardItem("$6"));
	regModel->setItem(10, 0, new QStandardItem("$7"));
	regModel->setItem(11, 0, new QStandardItem("$8"));
	regModel->setItem(12, 0, new QStandardItem("$9"));
	regModel->setItem(13, 0, new QStandardItem("$10"));
	regModel->setItem(14, 0, new QStandardItem("$11"));
	regModel->setItem(15, 0, new QStandardItem("$12"));
	regModel->setItem(16, 0, new QStandardItem("$13"));
	regModel->setItem(17, 0, new QStandardItem("$14"));
	regModel->setItem(18, 0, new QStandardItem("$15"));
	regModel->setItem(19, 0, new QStandardItem("$16"));
	regModel->setItem(20, 0, new QStandardItem("$17"));
	regModel->setItem(21, 0, new QStandardItem("$18"));
	regModel->setItem(22, 0, new QStandardItem("$19"));
	regModel->setItem(23, 0, new QStandardItem("$20"));
	regModel->setItem(24, 0, new QStandardItem("$21"));
	regModel->setItem(25, 0, new QStandardItem("$22"));
	regModel->setItem(26, 0, new QStandardItem("$23"));
	regModel->setItem(27, 0, new QStandardItem("$24"));
	regModel->setItem(28, 0, new QStandardItem("$25"));
	regModel->setItem(29, 0, new QStandardItem("$26"));
	regModel->setItem(30, 0, new QStandardItem("$27"));
	regModel->setItem(31, 0, new QStandardItem("$28"));
	regModel->setItem(32, 0, new QStandardItem("$29"));
	regModel->setItem(33, 0, new QStandardItem("$31"));
	regModel->setItem(34, 0, new QStandardItem("$32"));
	regModel->setItem(3, 1, new QStandardItem("$zero"));
	regModel->setItem(4, 1, new QStandardItem("$at"));
	regModel->setItem(5, 1, new QStandardItem("$v0"));
	regModel->setItem(6, 1, new QStandardItem("$v1"));
	regModel->setItem(7, 1, new QStandardItem("$a0"));
	regModel->setItem(8, 1, new QStandardItem("$a1"));
	regModel->setItem(9, 1, new QStandardItem("$a2"));
	regModel->setItem(10, 1, new QStandardItem("$a3"));
	regModel->setItem(11, 1, new QStandardItem("$t0"));
	regModel->setItem(12, 1, new QStandardItem("$t1"));
	regModel->setItem(13, 1, new QStandardItem("$t2"));
	regModel->setItem(14, 1, new QStandardItem("$t3"));
	regModel->setItem(15, 1, new QStandardItem("$t4"));
	regModel->setItem(16, 1, new QStandardItem("$t5"));
	regModel->setItem(17, 1, new QStandardItem("$t6"));
	regModel->setItem(18, 1, new QStandardItem("$t7"));
	regModel->setItem(19, 1, new QStandardItem("$s0"));
	regModel->setItem(20, 1, new QStandardItem("$s1"));
	regModel->setItem(21, 1, new QStandardItem("$s2"));
	regModel->setItem(22, 1, new QStandardItem("$s3"));
	regModel->setItem(23, 1, new QStandardItem("$s4"));
	regModel->setItem(24, 1, new QStandardItem("$s5"));
	regModel->setItem(25, 1, new QStandardItem("$s6"));
	regModel->setItem(26, 1, new QStandardItem("$s7"));
	regModel->setItem(27, 1, new QStandardItem("$t8"));
	regModel->setItem(28, 1, new QStandardItem("$t9"));
	regModel->setItem(29, 1, new QStandardItem("$k0"));
	regModel->setItem(30, 1, new QStandardItem("$k1"));
	regModel->setItem(31, 1, new QStandardItem("$gp"));
	regModel->setItem(32, 1, new QStandardItem("$sp"));
	regModel->setItem(33, 1, new QStandardItem("$fp"));
	regModel->setItem(34, 1, new QStandardItem("$ra"));
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

	int32_t memAddress = 0;
	for (size_t i = 0; i < 512; i++) {
		ss << "0x" << std::setfill('0') << std::setw(8) << std::hex << memAddress;
		ss >> registerValue;
		memModel->setItem(i, 0, new QStandardItem(QString::fromStdString(registerValue)));
		memAddress++;
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

void VirtualMachineGUI::closeEvent(QCloseEvent * event)
{
	if (isThreadUp == true) {
		mq.push("quit");
		myvirtual.breakRun();
	}
	event->accept();
}

void VirtualMachineGUI::update()
{
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
	if (myvirtual.getPc() != myvirtual.getInstructionSize()) {
		cursor.setBlockFormat(white);
		size_t num = GuiInstructions[myvirtual.getPc()].line;
		cursor.movePosition(QTextCursor::Start);
		for (size_t index = 0; index != num; index++) {
			cursor.movePosition(QTextCursor::Down);
		}
		cursor.setBlockFormat(background);
	}
	else {
		cursor.setBlockFormat(white);
		size_t num = GuiInstructions[myvirtual.getPc() - 1].line;
		cursor.movePosition(QTextCursor::Start);
		for (size_t index = 0; index != num; index++) {
			cursor.movePosition(QTextCursor::Down);
		}
		cursor.setBlockFormat(background);
	}
}

bool VirtualMachineGUI::setMemory(MemorySet & mem)
{
	for (size_t index = 0; index < 513; index++) {
		memModel->setItem(index, 0, new QStandardItem(std::size_t(0)));
	}
	return false;
}

