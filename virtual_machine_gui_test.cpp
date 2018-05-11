#include <QTest>
#include <QPlainTextEdit>
#include <QTableView>
#include <QLineEdit>
#include <QPushButton>
#include <QSignalSpy>
#include "virtual_machine_gui.hpp"

#include "test_config.hpp"

class VirtualMachineGUITest : public QObject {
  Q_OBJECT

private slots:

  void initTestCase();
  void buttonTest();
  void buttonTest1();
  // TODO: implement additional tests here
  
private:
  VirtualMachineGUI widget;
};

// this section just verifies the object names and API
void VirtualMachineGUITest::initTestCase(){

  widget.load(QString::fromStdString(TEST_FILE_DIR + "/vm/test00.asm"));

  auto textWidget = widget.findChild<QPlainTextEdit *>("text");
  QVERIFY2(textWidget, "Could not find QPLainText widget");

  auto registerViewWidget = widget.findChild<QTableView *>("registers");
  QVERIFY2(registerViewWidget, "Could not find QTableView widget for registers");

  auto memoryViewWidget = widget.findChild<QTableView *>("memory");
  QVERIFY2(memoryViewWidget, "Could not find QTableView widget for memory");

  auto statusViewWidget = widget.findChild<QLineEdit *>("status");
  QVERIFY2(statusViewWidget, "Could not find QLineEdit widget for status");

  auto stepButtonWidget = widget.findChild<QPushButton *>("step");
  QVERIFY2(stepButtonWidget, "Could not find QTableView widget for memory");

}

void VirtualMachineGUITest::buttonTest() {
	widget.load(QString::fromStdString(TEST_FILE_DIR + "/vm/test01.asm"));
	auto stepButtonWidget = widget.findChild<QPushButton *>("step");
	QTest::mouseClick(stepButtonWidget, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(), 1);
	widget.update();
	//auto runButtonWidget = widget.findChild<QPushButton *>("run");
	//auto breakButtonWidget = widget.findChild<QPushButton *>("break");
	//QSignalSpy spy(runButtonWidget, SIGNAL(clicked()));
	//QSignalSpy spy1(breakButtonWidget, SIGNAL(clicked()));
	for (size_t i = 0; i < 6; i++) {
		widget.changeReg();
		//QTest::mouseClick(runButtonWidget, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(), 1);
	}

	//QTest::mouseClick(breakButtonWidget, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(), 1);
	
	//QTest::mouseClick(runButtonWidget, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(), 1);
	//QCOMPARE(spy.isValid(), true);
	//widget.setUpThread();
	//widget.runStop();
	//widget.closeEvent(QTest::QCloseE);
	//auto textWidget = widget.findChild<QPlainTextEdit *>("text");
	//QVERIFY(textWidget != nullptr);
	////QObject::connect(stepButtonWidget, SIGNAL(clicked()), this, SLOT(changeReg()));
	//QSignalSpy spy(textWidget, SIGNAL(stepButtonWidget->animateClick()));
	//QCOMPARE(spy.isValid(), true);
}
void VirtualMachineGUITest::buttonTest1() {
	widget.load(QString::fromStdString(TEST_FILE_DIR + "/vm/test18.asm"));
	auto stepButtonWidget = widget.findChild<QPushButton *>("step");
	QTest::mouseClick(stepButtonWidget, Qt::LeftButton, Qt::KeyboardModifiers(), QPoint(), 1);
	for (size_t i = 0; i < 12; i++) {
		widget.changeReg();
	}
}

QTEST_MAIN(VirtualMachineGUITest)
#include "virtual_machine_gui_test.moc"

