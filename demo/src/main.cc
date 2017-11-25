//! @file       main.cc
//! @brief      Main entry point for CLarity demo
//!
//! @author     Jeffrey Wallace
//! @copyright  MIT


// CLarity Imports

// Standard Imports

// Third-Party Imports
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QWidget>


int main(int argc, char ** argv)
{
    QApplication app(argc, argv);

    // Set Style
   	QFile f(":qdarkstyle/style.qss");
	if (!f.exists()) {
		printf("Unable to set stylesheet, file not found\n");
	} else {
		f.open(QFile::ReadOnly | QFile::Text);
		QTextStream ts(&f);
		app.setStyleSheet(ts.readAll());
	} 

    QWidget widget;
    widget.resize(300, 200);
    widget.setWindowTitle("CLarity Demo");
    widget.show();


    return app.exec();
}
