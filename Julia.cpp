#include <QtGui>
#include "Julia.h"
#include "colorizer.h"


JuliaWidget::JuliaWidget(QWidget *parent):QWidget(parent)
{
	color = new colorizer;
	color->setWindowFlags(Qt::FramelessWindowHint | Qt::SubWindow | Qt::WindowStaysOnTopHint);
	color->show();
	setFixedSize(QSize(800,600));
	image = QPixmap(800,600);
	draw = true;
	save0 = false;
	fileName0 = "";
	resolution = 480000;
	calc(-0.025f, -0.71f);
	press_x = 0;
	press_y = 0;
	release_x = 0;
	release_y = 0;
	connect(color, SIGNAL(changed()), this, SLOT(updater()));
}

void JuliaWidget::updater()
{
	draw = true;
	update();
}

void JuliaWidget::in()
{
	setCursor(Qt::BusyCursor);
	calc(c_real, c_imag, x0 + (width0-width0/1.5)/2, y0 - (height0-height0/1.5)/2, width0/1.5, height0/1.5, apfel0);
	setCursor(Qt::ArrowCursor);
}

void JuliaWidget::out()
{
	setCursor(Qt::BusyCursor);
	calc(c_real, c_imag, x0 - (width0*1.5-width0)/2, y0 + (height0*1.5-height0)/2, width0*1.5, height0*1.5, apfel0);
	setCursor(Qt::ArrowCursor);
}

void JuliaWidget::calc(long double real_c, long double imaginary_c, long double x, long double y, long double width, long double height, bool apfelmann)
{
	if(resolution == 480000)
	{
		resolution_x = 800;
		resolution_y = 600;
	}
	else if(resolution == 1920000)
	{
		resolution_x = 1600;
		resolution_y = 1200;
	}
	x0 = x;
	y0 = y;
	width0 = width;
	height0 = height;
	apfel0 = apfelmann;
	int iterations = 0;
	int x_round = 0;
	int y_round = 0;
	int bit;
	c_real = real_c;
	c_imag = imaginary_c;
	for(double i = y0; i > y0 - height0; i -= height0/resolution_y)
	{
		for(double j = x0; j < x0 + width0; j += width0/resolution_x)
		{
			z_real = j;
			z_imag = i;
			if(apfelmann)
			{
				c_real = j;
				c_imag = i;
			}
			while(iterations < 100 && ((z_real * z_real) + (z_imag * z_imag)) < 4.0f)
			{
				temp = z_real * z_imag;
				z_real = z_real * z_real - z_imag * z_imag + c_real;
				z_imag = temp + temp + c_imag;
				++iterations;
			}
			bit = y_round*resolution_x+x_round;
			if(bit >= 0 && bit < resolution)
				col[bit] = (char)iterations;
				
			
			iterations = 0;
			++x_round;
		}
		x_round = 0;
		++y_round;
	}
	draw = true;
	update();
}

void JuliaWidget::save()
{
	fileName0 = QFileDialog::getSaveFileName(0, "Datei speichern unter", "C:\\", "Joint Photographic Experts Group (*.jpg);;Portable Network Graphics (*.png);;Windows-Bitmap (*.bmp)");
	if(fileName0 != QString(""))
	{		
		resolution = 1920000;
		save0 = true;
		calc(c_real, c_imag, x0, y0, width0, height0, apfel0);
	}
}

void JuliaWidget::paintEvent(QPaintEvent *event)
{
	unsigned int colorMap[100];
	color->update();
	for(int i = 0; i < 100; i++)
		colorMap[i] = color->getColors().value[i];
	if(save0)
	{
		setCursor(Qt::WaitCursor);
		QImage saveImg(QSize(1600,1200),QImage::Format_ARGB32);
		QPainter painter(&saveImg);
		painter.setBackgroundMode(Qt::TransparentMode);
		painter.setBrush(Qt::white);
		painter.drawRect(0,0, 1600, 1200);
		painter.setBrush(Qt::NoBrush);
		for(int i = 0; i < 1920000; i++)
		{
			if((int)col[i] == 100)
			{
				painter.setPen(QColor(0,0,0));
				painter.drawPoint((i% 1600) , (i/ 1600));
			}
			else
			{
				painter.setPen(QColor(colorMap[(int)col[i]]));
				painter.drawPoint((i% 1600) , (i/ 1600));
			}
		}
		saveImg.save(fileName0, 0, 100);
		fileName0 = "";
		save0 = false;
		resolution = 480000;
		calc(c_real, c_imag, x0, y0, width0, height0, apfel0);
		setCursor(Qt::ArrowCursor);
		return;
	}
	
	if(draw)
	{
		draw = false;
		QPainter painter(&image);
		painter.setWindow(0, 0, resolution_x, resolution_y);
		painter.setBackgroundMode(Qt::TransparentMode);
		painter.setBrush(Qt::white);
		painter.drawRect(0,0, resolution_x, resolution_y);
		painter.setBrush(Qt::NoBrush);
		for(int i = 0; i < resolution; i++)
		{
			if((int)col[i] == 100)
			{
				painter.setPen(QColor(0,0,0));
				painter.drawPoint((i% resolution_x) , (i/ resolution_x));
			}
			else
			{
				painter.setPen(QColor(colorMap[(int)col[i]]));
				painter.drawPoint((i% resolution_x) , (i/ resolution_x));
			}
		}
	}
	QPainter glob(this);
	glob.drawPixmap(QRect(0,0,800,600), image, QRect(0,0,800,600));

	
}

void JuliaWidget::mousePressEvent(QMouseEvent *event)
{
	if(event->button() == Qt::LeftButton)
	{
		press_x = event->globalX();
		press_y = event->globalY();
		setCursor(Qt::SizeAllCursor);
	}
}

void JuliaWidget::mouseReleaseEvent(QMouseEvent *event)
{
	if(event->button() == Qt::LeftButton)
	{
		setCursor(Qt::ArrowCursor);
		release_x = event->globalX();
		release_y = event->globalY();
		if(press_x - release_x != 0 || press_y - release_y != 0)
		{
			setCursor(Qt::BusyCursor);
			calc(c_real, c_imag, ((long double) press_x - (long double) release_x) * width0/(long double)this->width() + x0, ((long double)release_y - (long double)press_y) * height0/(long double)this->height() + y0, width0, height0, apfel0);
			setCursor(Qt::ArrowCursor);
		}
	}
}

void JuliaWidget::wheelEvent(QWheelEvent *event)
{
	if(event->delta() / 120 == 1)
	{
		setCursor(Qt::BusyCursor);
		calc(c_real, c_imag, x0 + (width0-width0/1.5)/2, y0 - (height0-height0/1.5)/2, width0/1.5, height0/1.5, apfel0);
		setCursor(Qt::ArrowCursor);
	}
	if(event->delta() /120 == -1)
	{
		setCursor(Qt::BusyCursor);
		calc(c_real, c_imag, x0 - (width0*1.5-width0)/2, y0 + (height0*1.5-height0)/2, width0*1.5, height0*1.5, apfel0);
		setCursor(Qt::ArrowCursor);
	}
}


void JuliaWidget::closeEvent(QCloseEvent *event)
{
	QSettings settings(QSettings::IniFormat, QSettings::UserScope, "$h@d0WFoXx","Julia");
    settings.setValue("colorizer/geometry", color->saveGeometry());
    QWidget::closeEvent(event);
}


Julia::Julia(QWidget *parent):QWidget(parent)
{
	
	setWindowIcon(QIcon(":/julia.png"));
	QDesktopWidget *desktop = new QDesktopWidget;
	QSettings settings(QSettings::IniFormat, QSettings::UserScope, "$h@d0WFoXx","Julia");
	move(desktop->screenGeometry().width()/2 - 400,desktop->screenGeometry().height()/2-305);
	restoreGeometry(settings.value("Julia/geometry").toByteArray());
	setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
	QPushButton *saver = new QPushButton("Speichern");
	saver->setFont(QFont(font().family(), font().pointSize()+2, QFont::Bold));
	QPushButton *zoom_in = new QPushButton;
	QPushButton *zoom_out = new QPushButton;
	zoom_in->setText("+");
	zoom_out->setText("-");
	zoom_in->move(600-zoom_in->width(), 0);
	zoom_out->move(600-zoom_out->width(), zoom_in->height()*3/4);
	re = new QLineEdit("-0.025");
	re->setFocus(Qt::OtherFocusReason);
	im = new QLineEdit("-0.71");
	rea = -0.025;
	img = -0.71;
	QPushButton *OK = new QPushButton("Zeichne");
	OK->setDefault(true);
	OK->setFont(QFont(font().family(), font().pointSize()+2, QFont::Bold));
	QLabel *caption1 = new QLabel("<h3>Startwert c</h3>");
	QLabel *caption2 = new QLabel("<h3>Zoom</h3>");
	labimag = new QLabel("Imaginär-Teil:");
	labreal = new QLabel("Real-Teil:");
	tool = new QDialog;
	QHBoxLayout *zoomLayout = new QHBoxLayout;
	zoomLayout->addWidget(zoom_in);
	zoomLayout->addWidget(zoom_out);
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(caption1);
	mainLayout->addWidget(labreal);
	mainLayout->addWidget(re);
	mainLayout->addWidget(labimag);
	mainLayout->addWidget(im);
	mainLayout->addWidget(caption2);
	mainLayout->addLayout(zoomLayout);
	mainLayout->addWidget(OK);
	mainLayout->addWidget(saver);
	tool->setLayout(mainLayout);
	tool->setWindowFlags(Qt::SubWindow | Qt::WindowStaysOnTopHint);
	tool->show();
	tool->move(desktop->screenGeometry().width()/2 - 390, desktop->screenGeometry().height()/2-270);
	tool->restoreGeometry(settings.value("tool/geometry").toByteArray());
	
	julia = new JuliaWidget;
	QHBoxLayout *main = new QHBoxLayout;
	main->addWidget(julia);
	main->setSpacing(0);
	main->setContentsMargins (0, 0, 0, 0);
	setLayout(main);
	
	connect(OK, SIGNAL(clicked()), this, SLOT(draw()));
	connect(zoom_in, SIGNAL(clicked()), julia, SLOT(in()));
	connect(zoom_out, SIGNAL(clicked()), julia, SLOT(out()));
	connect(saver, SIGNAL(clicked()), julia, SLOT(save()));
}

void Julia::draw()
{
	if(im->text() == QString("Apfelmann") || re->text() == QString("Apfelmann"))
	{
		setCursor(Qt::BusyCursor);
		julia->calc(0,0,-2.0,1.5,4.0,3.0,true);
		setCursor(Qt::ArrowCursor);
	}
	else
	{
		setCursor(Qt::BusyCursor);
		img = im->text().toDouble();
		rea = re->text().toDouble();
		julia->calc(re->text().toDouble(), im->text().toDouble(), -2.0,1.5,4.0,3.0);
		setCursor(Qt::ArrowCursor);
	}
}

void Julia::closeEvent(QCloseEvent *event)
{
	QSettings settings(QSettings::IniFormat, QSettings::UserScope, "$h@d0WFoXx","Julia");
    settings.setValue("Julia/geometry", saveGeometry());
	settings.setValue("tool/geometry", tool->saveGeometry());
	julia->close();
    QWidget::closeEvent(event);
}
