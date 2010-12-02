#ifndef TF_GRAYSCALEALPHA_PAINTER
#define TF_GRAYSCALEALPHA_PAINTER

#include <TFAbstractPainter.h>

namespace M4D {
namespace GUI {

class TFGrayscaleAlphaPainter: public TFAbstractPainter{

public:

	TFGrayscaleAlphaPainter();
	~TFGrayscaleAlphaPainter();

	void setUp(QWidget *parent);
	void setUp(QWidget *parent, int margin);

protected:

	void paintEvent(QPaintEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);

	void addPoint_(TFPaintingPoint point);
	void correctView_();

private:

	enum ActiveView{
		ACTIVE_GRAYSCALE,
		ACTIVE_ALPHA
	};

	ActiveView activeView_;
	
	void paintCurves_(QPainter *drawer);
};

} // namespace GUI
} // namespace M4D

#endif //TF_GRAYSCALEALPHA_PAINTER