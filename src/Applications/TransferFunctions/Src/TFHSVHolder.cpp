#include "TFHSVHolder.h"

#include <QtGui/QColor>
#include <QtGui/QPainter>

namespace M4D {
namespace GUI {

TFHSVHolder::TFHSVHolder(QWidget* window): colorBarWidth_(10), painterMargin_(5){

	setParent(window);
	type_ = TFHOLDER_HSV;
	painterLeftTop_ = TFPaintingPoint(painterLeftTop_.x + colorBarWidth_,painterLeftTop_.y);
}

TFHSVHolder::~TFHSVHolder(){}

void TFHSVHolder::setUp(QWidget *parent, const QRect& rect){

	painter_.setUp(this);
	size_changed(rect);
	setParent(parent);
	show();
}

void TFHSVHolder::paintEvent(QPaintEvent *e){

	QPainter colorBarPainter(this);
	int colorBarBegin = painterLeftTop_.y + painter_.height() - painterMargin_;
	float range = painter_.height() - 2*painterMargin_;
	tfAssert(range>0);
	for(int i = 0; i < range; ++i)
	{
		QColor color;
		color.setHsvF(i/range, 1, 1);		

		int currentLvl = colorBarBegin - i;

		colorBarPainter.setPen(color);
		colorBarPainter.drawLine(painterLeftTop_.x - colorBarWidth_, currentLvl, painterLeftTop_.x, currentLvl);
	}
}

void TFHSVHolder::updateFunction_(){

	if(!painter_.changed()) return;

	calculate_(painter_.getView(), function_.getColorMap());
}

void TFHSVHolder::updatePainter_(){
	
	calculate_(function_.getColorMap(), painter_.getView());
}

void TFHSVHolder::resizePainter_(const QRect& rect){

	updateFunction_();

	painter_.resize(rect);
	
	updatePainter_();
}

TFAbstractFunction* TFHSVHolder::getFunction_(){

	return &function_;
}

} // namespace GUI
} // namespace M4D
