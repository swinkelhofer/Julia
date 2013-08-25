#ifndef JULIA
#define JULIA
#include <QWidget>
#include <QDialog>
class QLabel;
class QPushButton;
class QLineEdit;
class colorizer;

class JuliaWidget: public QWidget
{
	Q_OBJECT
public:
	JuliaWidget(QWidget *parent = 0);
	
private:
	QString fileName0;
	QPixmap image;
	colorizer *color;
	bool draw, apfel0, save0;
	long double x0,y0,width0,height0, c_real, c_imag, z_real, z_imag, temp;
	int press_x, press_y, release_x, release_y, resolution, resolution_x, resolution_y;
	char col[1920000];
	
public slots:
	void calc(long double real_c, long double imaginary_c, long double x = -2.0, long double y = 1.5, long double width = 4.0, long double height = 3.0, bool apfelmann = false);
	void in();
	void out();
	void updater();
	void save();
	
protected:
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);
	void closeEvent(QCloseEvent *event);
};

class Julia: public QWidget
{
	Q_OBJECT
public:
	Julia(QWidget *parent = 0);
	
private:
	QDialog *tool;
	QLineEdit *im, *re;
	QLabel *labimag, *labreal;
	JuliaWidget *julia;
	long double img, rea;
	
private slots:
	void draw();

protected:
	void closeEvent(QCloseEvent *event);
};
#endif
