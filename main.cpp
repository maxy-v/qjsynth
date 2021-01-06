#include <QApplication>
#include "MainWidget.h"
#include "QtDarkStyle/DarkStyle.h"
																	
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	DarkStyle::apply();
	MainWidget w;
	w.show();
	return a.exec();
}
