#include "TFGrayscaleAlphaPainter.h"

namespace M4D {
namespace GUI {

TFGrayscaleAlphaPainter::TFGrayscaleAlphaPainter(bool drawAlpha):
	margin_(5),
	spacing_(5),
	colorBarSize_(10),
	background_(Qt::black),
	gray_(Qt::lightGray),
	alpha_(Qt::yellow),
	drawAlpha_(drawAlpha){
}

TFGrayscaleAlphaPainter::~TFGrayscaleAlphaPainter(){}

void TFGrayscaleAlphaPainter::setArea(QRect area){
	
	area_ = area;

	backgroundArea_= QRect(
		0,
		0,
		area_.width(),
		area_.height() - colorBarSize_ - spacing_);

	inputArea_= QRect(
		backgroundArea_.x() + margin_,
		backgroundArea_.y() + margin_,
		backgroundArea_.width() - 2*margin_,
		backgroundArea_.height() - 2*margin_);
	
	bottomBarArea_= QRect(
		inputArea_.x() - 1,
		area_.height() - colorBarSize_ - 1,
		inputArea_.width() + 2,
		colorBarSize_ + 2);
}

QRect TFGrayscaleAlphaPainter::getInputArea(){

	return QRect(area_.x() + inputArea_.x(), area_.y() + inputArea_.y(),
		inputArea_.width(), inputArea_.height());
}

void TFGrayscaleAlphaPainter::drawBackground_(QPainter* drawer){

	drawer->fillRect(backgroundArea_, QBrush(background_));

	drawer->fillRect(bottomBarArea_, QBrush(background_));
}

void TFGrayscaleAlphaPainter::drawData(QPainter* drawer, TFWorkCopy::Ptr workCopy){

	tfAssert(workCopy->size() == inputArea_.width());
	
	if(workCopy->changed())
	{
		dBuffer_ = QPixmap(area_.width(), area_.height());

		QPainter dbPainter(&dBuffer_);
		//dbPainter.setBackgroundMode(Qt::TransparentMode);
		
		drawBackground_(&dbPainter);

		dbPainter.setClipRect(inputArea_.x(), inputArea_.y(),
			inputArea_.width() + 1, inputArea_.height() + 1);

		TFPaintingPoint origin(inputArea_.x(), inputArea_.y());
		
		TFColor tfColor;
		QColor qColor;
		for(int i = 0; i < inputArea_.width() - 1; ++i)
		{
			if(drawAlpha_)
			{
				//alpha
				dbPainter.setPen(alpha_);
				dbPainter.drawLine(origin.x + i, origin.y + (1 - workCopy->getAlpha(i))*inputArea_.height(),
					origin.x + i + 1, origin.y + (1 - workCopy->getAlpha(i+1))*inputArea_.height());
			}
			//gray
			dbPainter.setPen(gray_);	
			dbPainter.drawLine(origin.x + i, origin.y + (1 - workCopy->getComponent1(i))*inputArea_.height(),
				origin.x + i + 1, origin.y + (1 - workCopy->getComponent1(i+1))*inputArea_.height());

			//TODO draw histogram if enabled
		}

		dbPainter.setClipRect(bottomBarArea_.x() + 1, bottomBarArea_.y() + 1,
			bottomBarArea_.width() + 1, bottomBarArea_.height());

		for(int i = 0; i < inputArea_.width(); ++i)
		{
			//bottom bar
			tfColor = workCopy->getColor(i);
			qColor.setRgbF(tfColor.component1, tfColor.component2, tfColor.component3, tfColor.alpha);
			dbPainter.setPen(qColor);

			dbPainter.drawLine(bottomBarArea_.x() + i + 1, bottomBarArea_.y(),
				bottomBarArea_.x() + i + 1, bottomBarArea_.y() + bottomBarArea_.height() - 1);
		}
	}

	drawer->drawPixmap(area_.x(), area_.y(), dBuffer_);
}

} // namespace GUI
} // namespace M4D
