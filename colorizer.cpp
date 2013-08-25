#include <QtGui>
#include "colorizer.h"

check::check(QWidget *parent):QWidget(parent)
{
	parent0 = parent;
	setFixedSize(10,10);
	col = QColor(Qt::white);
	fixed0 = false;
}

void check::setFixed(bool fixed)
{
	fixed0 = fixed;
}

void check::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setBrush(col);
	if(fixed0)
	{
		painter.setPen(QPen(Qt::black, 2));
		QPolygon poly;
		poly << QPoint(1,5) << QPoint(9,1) << QPoint(9,9);
		painter.drawPolygon(poly);
	}
	else
	{
		painter.setPen(Qt::black);
		QPolygon poly;
		poly << QPoint(0,5) << QPoint(10,0) << QPoint(10,10);
		painter.drawPolygon(poly);
	}
}

void check::mousePressEvent(QMouseEvent *event)
{
	if(event->button() == Qt::LeftButton)
	{
		if(!fixed0)
		{
			cur_y = event->globalY();
			update();
			emit onClick();
		}
		else
		{
			emit onClick();
		}
	}
	else if(event->button() == Qt::RightButton)
	{
		QColor temp;
		temp = QColorDialog::getColor(col,parent0, "Farbe auswählen");
		if(temp.isValid())
		{
			col = temp;
			update();
			emit changed();
		}
	}
}

void check::mouseReleaseEvent(QMouseEvent *event)
{
	if(!fixed0)
	{
	if(event->button() == Qt::LeftButton && this->y() + event->globalY()-cur_y < 200 && this->y() + event->globalY()-cur_y > 0 )
	{
		this->move(this->x(),this->y() + event->globalY()-cur_y);
		cur_y = event->globalY();
		emit posChanged(this->y() + event->globalY()-cur_y);
		emit changed();
	}
	}
}

void check::mouseMoveEvent(QMouseEvent *event)
{
	if(!fixed0)
	{
	if(this->y() + event->globalY()-cur_y < 200 && this->y() + event->globalY()-cur_y > 0 )
	{
		this->move(this->x(),this->y() + event->globalY()-cur_y);
		cur_y = event->globalY();
		emit posChanged(this->y() + event->globalY()-cur_y);
	}
	}
}

void check::mouseDoubleClickEvent(QMouseEvent *event)
{
	if(event->button() == Qt::LeftButton && !fixed0)
	{
		emit deleted(this);
	}
}

QColor check::getColor()
{
	return col;
}

void check::setColor(QColor color)
{
	col = color;
	update();
}

colorizer::colorizer(QWidget *parent):QWidget(parent)
{
	QSettings settings(QSettings::IniFormat, QSettings::UserScope, "$h@d0WFoXx","Julia");
	QDesktopWidget *desktop = new QDesktopWidget;
	move(desktop->screenGeometry().width()/2 - 390, desktop->screenGeometry().height()/2 + 100);
	restoreGeometry(settings.value("colorizer/geometry").toByteArray());
	img = QImage(QSize(40,210), QImage::Format_RGB32);
	inMotion = false;
	ticks.append(new check(this));
	ticks[0]->move(30,0);
	ticks[0]->setFixed(true);
	
	ticks.append(new check(this));
	ticks[1]->move(30,200);
	ticks[1]->setColor(QColor(Qt::blue));
	ticks[1]->setFixed(true);
	
	setFixedSize(40,210);
	connect(ticks[0], SIGNAL(onClick()), this, SLOT(newTick()));
	connect(ticks[0], SIGNAL(changed()), this, SIGNAL(changed()));
	connect(ticks[1], SIGNAL(changed()), this, SIGNAL(changed()));

}

void colorizer::newTick()
{
	if(ticks.count() < 50)
	{
		ticks.append(new check(this));
		ticks.last()->setColor(QColor(Qt::red));
		ticks.last()->move(30,90);
		ticks.last()->show();
		update();
		connect(ticks.last(), SIGNAL(posChanged(int)), this, SLOT(update()));
		connect(ticks.last(), SIGNAL(changed()), this, SIGNAL(changed()));
		connect(ticks.last(), SIGNAL(deleted(check *)), this, SLOT(delTick(check *)));
		emit changed();
	}
}

void colorizer::delTick(check *tick)
{
	ticks.removeOne(tick);
	delete(tick);
	update();
	emit changed();
}

struct ColorInt colorizer::getColors()
{
	struct ColorInt ret;
	for(int i = 0; i < 100; i++)
	{
		ret.value[i] = img.pixel(QPoint(15, 2*i+5));
	}
	return ret;
}

void colorizer::paintEvent(QPaintEvent *event)
{
	QPainter painter(&img);
	
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setBrush(Qt::white);
	painter.setPen(Qt::red);
	painter.drawRect(0,0,40,210);
	
	colorAt.clear();
	
	for(int i = 0; i < ticks.count(); i++)
	{
		colorAt[ticks[i]->y()] = ticks[i]->getColor();
	}
	
	QLinearGradient linearGrad(QPointF(0, 0), QPointF(0, 200));
	for(int i = 0; i < colorAt.count(); i++)
	{
		linearGrad.setColorAt((double)colorAt.keys().at(i) / 200, colorAt.values().at(i));
	}
	painter.setBrush(linearGrad);
	painter.setPen(Qt::NoPen);
	painter.drawRect(5,5,25,200);
	QPainter glob(this);
	/*glob.setBrush(Qt::white);
	glob.setPen(Qt::white);
	glob.drawRect(0,0,40,210);*/
	glob.drawImage(QRectF(0,0,40,210), img, QRectF(0,0,40,210));
}

void colorizer::mousePressEvent(QMouseEvent *event)
{
	if(event->button() == Qt::LeftButton)
	{
		setCursor(Qt::SizeAllCursor);
		press_x = event->globalX();
		press_y = event->globalY();
		inMotion = true;
	}
}

void colorizer::mouseReleaseEvent(QMouseEvent *event)
{
	if(event->button() == Qt::LeftButton)
	{
		release_x = event->globalX();
		release_y = event->globalY();
		if(release_x - press_x != 0 || release_y - press_y != 0)
		{
			move(this->x()+release_x-press_x, this->y() + release_y-press_y);
		}
		setCursor(Qt::ArrowCursor);
		inMotion = false;
	}
}

void colorizer::mouseMoveEvent(QMouseEvent *event)
{
	if(inMotion)
	{
		release_x = event->globalX();
		release_y = event->globalY();
		if(release_x - press_x != 0 || release_y - press_y != 0)
		{
			move(this->x()+release_x-press_x, this->y() + release_y-press_y);
		}
		press_x = release_x;
		press_y = release_y;
	}
}

