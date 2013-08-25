#ifndef COLORIZER
#define COLORIZER
#include <QWidget>
#include <QHash>
#include <QList>
class QImage;

struct ColorInt
{
	unsigned int value[100];
};

class check: public QWidget
{
	Q_OBJECT
public:
	QWidget *parent0;
	check(QWidget *parent = 0);
	void setColor(QColor color);
	void setFixed(bool fixed);
	QColor getColor();
	
protected:
	int cur_y;
	QColor col;
	bool fixed0;
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
	
signals:
	void posChanged(int y);
	void onClick();
	void changed();
	void deleted(check *tick);
};

class colorizer: public QWidget
{
	Q_OBJECT
public:
	colorizer(QWidget *parent = 0);
	struct ColorInt getColors();
	
private:
	QImage img;
	check *tick[4];
	QHash<int, QColor> colorAt;
	QList<check *> ticks;
	int press_x, press_y, release_x, release_y;
	bool inMotion;
	
protected:
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	
private slots:
	void newTick();
	void delTick(check *tick);
	
signals:
	void changed();
};
#endif
