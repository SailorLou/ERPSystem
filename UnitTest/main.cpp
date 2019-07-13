#include <QtTest>
//#include <QtWidgets/QApplication>
#include <iostream>
#include <QList>


int main(int argc, char *argv[])
{

	QApplication a(argc, argv);

	bool enableXUnitXML = false;
	bool enableOutput = false;
	int outputIndex = -1;

	// run specific case
	QString currentTestClassName;

	QStringList stringList = a.arguments();
	for(int i = 0; i < stringList.length(); i++) {
		QString param = stringList.at(i);
		if(stringList.at(i).contains("-xunitxml"))
			enableXUnitXML = true;

		if(stringList.at(i).contains("-o")) {
			enableOutput = true;
			outputIndex = i;
		}

		if(i > 0 && stringList.at(i).startsWith("Test")) {
			currentTestClassName = stringList.at(i);
			stringList.removeAt(i);
			break;
		}
	}

	// Create the tests
	QList<QObject*> tests;
	//tests.append(new TestAnnotationViewContainer());

	QStringList failedTestNames;
	for(int i = 0; i < tests.size(); ++i) {
		QObject* testObject = tests.at(i);
		QString testName = testObject->metaObject()->className();

		// replace the parameter to avoid the xml file was rewrite by last test case
		if(enableOutput && enableXUnitXML && outputIndex > 0 && outputIndex + 1 < stringList.size()) {
			stringList.replace(outputIndex + 1, testName + ".xml");
		}

		bool runThisCase = true;
		if(!currentTestClassName.isEmpty() && testName != currentTestClassName)
			runThisCase = false;

		if(runThisCase) {
			if(0 != QTest::qExec(testObject, stringList))
				failedTestNames.append(testObject->metaObject()->className());
			delete testObject;
		}
	}

	// Print a summary at the bottom
	std::cout << std::endl << "********* Summary *********" << std::endl;
	if(!failedTestNames.empty()) {
		std::cout << "There are " << failedTestNames.size() << " failed tests!" << std::endl;
		for(int i(0); i < failedTestNames.size(); ++i) {
			std::cout << "\t" << failedTestNames.at(i).toStdString() << std::endl;
		}
	} else {
		std::cout << "All tests passed." << std::endl;
	}
	std::cout << "***************************" << std::endl;
	system("pause");

	//return a.exec();
}
