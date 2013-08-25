#include <QApplication>
#include <QPlastiqueStyle>
#include <QPalette>
#include "Julia.h"

int main(int argc, char *argv[])
{
	QApplication app(argc,argv);
	QPlastiqueStyle *style = new QPlastiqueStyle;
	app.setStyle(style);
	app.setWindowIcon(QIcon(":/julia.png"));
	QPalette pal;
	pal.setColor(QPalette::Active, QPalette::Window, Qt::black);
	pal.setColor(QPalette::Active, QPalette::WindowText, Qt::darkGray);
	pal.setColor(QPalette::Active, QPalette::Base, Qt::lightGray);
	pal.setColor(QPalette::Active, QPalette::Text, Qt::black);
	pal.setColor(QPalette::Active, QPalette::Button, QColor(30,30,30));
	pal.setColor(QPalette::Active, QPalette::ButtonText, Qt::gray);
	pal.setColor(QPalette::Active, QPalette::Highlight, QColor(150,150,150));
	pal.setColor(QPalette::Active, QPalette::HighlightedText, Qt::black);
	pal.setColor(QPalette::Active, QPalette::BrightText, Qt::white);
	
	pal.setColor(QPalette::Disabled, QPalette::Window, Qt::black);
	pal.setColor(QPalette::Disabled, QPalette::WindowText, Qt::lightGray);
	pal.setColor(QPalette::Disabled, QPalette::Base, Qt::darkGray);
	pal.setColor(QPalette::Disabled, QPalette::Text, QColor(20,20,20));
	pal.setColor(QPalette::Disabled, QPalette::Button, Qt::black);
	pal.setColor(QPalette::Disabled, QPalette::ButtonText, Qt::darkGray);
	pal.setColor(QPalette::Disabled, QPalette::Highlight, QColor(150,150,150));
	pal.setColor(QPalette::Disabled, QPalette::HighlightedText, Qt::black);
	pal.setColor(QPalette::Disabled, QPalette::BrightText, Qt::white);
	
	pal.setColor(QPalette::Inactive, QPalette::Window, Qt::black);
	pal.setColor(QPalette::Inactive, QPalette::WindowText, Qt::darkGray);
	pal.setColor(QPalette::Inactive, QPalette::Base, Qt::lightGray);
	pal.setColor(QPalette::Inactive, QPalette::Text, Qt::black);
	pal.setColor(QPalette::Inactive, QPalette::Button, QColor(30,30,30));
	pal.setColor(QPalette::Inactive, QPalette::ButtonText, Qt::gray);
	pal.setColor(QPalette::Inactive, QPalette::Highlight, QColor(150,150,150));
	pal.setColor(QPalette::Inactive, QPalette::HighlightedText, Qt::black);
	pal.setColor(QPalette::Inactive, QPalette::BrightText, Qt::white);
	app.setPalette(pal);
	Julia *window = new Julia;
	window->show();
	return app.exec();
}
