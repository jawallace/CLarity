//! @file       main.cc
//! @brief      Main entry point for CLarity demo
//!
//! @author     Jeffrey Wallace
//! @copyright  MIT

// CLarity Imports
#include "terrain_viewer.h"
#include "range_viewer.h"
#include "cl_utils.h"

// Standard Imports

// Third-Party Imports
#include "cl.hpp"
#include <QApplication>
#include <QFile>
#include <QHBoxLayout>
#include <QTextStream>
#include <QWidget>


namespace cd = clarity::demo;

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

    std::shared_ptr<cl::Context> ctx = clarity::get_context();

    QWidget w;
    w.setWindowTitle("CLarity Demo");
    
    QHBoxLayout * layout = new QHBoxLayout;
    cd::Terrain_Viewer terrain_viewer(ctx);
    layout->addWidget(&terrain_viewer);
    
    cd::Range_Viewer range_viewer(ctx);
    layout->addWidget(&range_viewer);

    w.setLayout(layout);
    w.show();

    return app.exec();
}
