#include "TFPolygonModifier.h"

namespace M4D {
namespace GUI {

TFPolygonModifier::TFPolygonModifier(TFAbstractModifier::Type type, const TFSize domain):
	type_(type),
	tools_(new Ui::TFPolygonModifier),
	activeView_(Active1),
	inputHelper_(),
	leftMousePressed_(false),
	zoomMovement_(false),
	baseRadius_(50),
	topRadius_(20){

	workCopy_ = TFWorkCopy::Ptr(new TFWorkCopy(domain));

	toolsWidget_ = new QWidget();
	tools_->setupUi(toolsWidget_);

	tools_->maxZoomSpin->setValue((int)workCopy_->getMaxZoom());
	tools_->ratioValue->setText(QString::number(workCopy_->getZoom()));
	tools_->zoomXValue->setText(QString::number(domain/2));
	tools_->zoomYValue->setText(QString::number(0.5));

	bool changeViewConnected = QObject::connect(tools_->activeViewBox, SIGNAL(currentIndexChanged(int)),
		this, SLOT(activeView_changed(int)));
	tfAssert(changeViewConnected);
	bool histogramCheckConnected = QObject::connect(tools_->histogramCheck, SIGNAL(toggled(bool)),
		this, SLOT(histogram_check(bool)));
	tfAssert(histogramCheckConnected);

	bool maxZoomSpinConnected = QObject::connect( tools_->maxZoomSpin, SIGNAL(valueChanged(int)),
		this, SLOT(maxZoomSpin_changed(int)));
	tfAssert(maxZoomSpinConnected);

	bool topSpinConnected = QObject::connect(tools_->topSpin, SIGNAL(valueChanged(int)),
		this, SLOT(topSpin_changed(int)));
	tfAssert(topSpinConnected);
	bool bottomSpinConnected = QObject::connect(tools_->bottomSpin, SIGNAL(valueChanged(int)),
		this, SLOT(bottomSpin_changed(int)));
	tfAssert(bottomSpinConnected);

	switch(type_)
	{
		case TFModifierGrayscale:
		{
			tools_->activeViewBox->addItem(QObject::tr("gray"));
			break;
		}
		case TFModifierGrayscaleAlpha:
		{
			tools_->activeViewBox->addItem(QObject::tr("gray"));
			tools_->activeViewBox->addItem(QObject::tr("opacity"));
			break;
		}
		case TFModifierRGB:
		{
			tools_->activeViewBox->addItem(QObject::tr("red"));
			tools_->activeViewBox->addItem(QObject::tr("green"));
			tools_->activeViewBox->addItem(QObject::tr("blue"));
			break;
		}
		case TFModifierRGBa:
		{
			tools_->activeViewBox->addItem(QObject::tr("red"));
			tools_->activeViewBox->addItem(QObject::tr("green"));
			tools_->activeViewBox->addItem(QObject::tr("blue"));
			tools_->activeViewBox->addItem(QObject::tr("opacity"));
			break;
		}
		case TFModifierHSV:
		{
			tools_->activeViewBox->addItem(QObject::tr("hue"));
			tools_->activeViewBox->addItem(QObject::tr("saturation"));
			tools_->activeViewBox->addItem(QObject::tr("value"));
			break;
		}
		case TFModifierHSVa:
		{
			tools_->activeViewBox->addItem(QObject::tr("hue"));
			tools_->activeViewBox->addItem(QObject::tr("saturation"));
			tools_->activeViewBox->addItem(QObject::tr("value"));
			tools_->activeViewBox->addItem(QObject::tr("opacity"));
			break;
		}
	}
}

TFPolygonModifier::~TFPolygonModifier(){}

void TFPolygonModifier::histogram_check(bool enabled){

	workCopy_->setHistogramEnabled(enabled);
	emit RefreshView();
}

void TFPolygonModifier::activeView_changed(int index){

	switch(index)
	{
		case 0:
		{
			activeView_ = Active1;
			break;
		}
		case 1:
		{
			if(type_ == TFModifierGrayscaleAlpha) activeView_ = ActiveAlpha;
			else activeView_ = Active2;
			break;
		}
		case 2:
		{
			activeView_ = Active3;
			break;
		}
		case 3:
		{
			activeView_ = ActiveAlpha;
			break;
		}
		default:
		{
			tfAbort(!"Bad view selected.");
			break;
		}
	}
}

void TFPolygonModifier::topSpin_changed(int value){

	topRadius_ = value;
	if(baseRadius_ < topRadius_) tools_->bottomSpin->setValue(topRadius_);
}

void TFPolygonModifier::bottomSpin_changed(int value){

	baseRadius_ = value;
	if(baseRadius_ < topRadius_) tools_->topSpin->setValue(baseRadius_);
}

void TFPolygonModifier::maxZoomSpin_changed(int value){

	workCopy_->setMaxZoom(value);
}

void TFPolygonModifier::updateZoomTools_(){

	tools_->ratioValue->setText(QString::number(workCopy_->getZoom()));

	TFPoint<int,float> center = workCopy_->getZoomCenter();

	tools_->zoomXValue->setText(QString::number(center.x));
	tools_->zoomYValue->setText(QString::number(center.y));
}

void TFPolygonModifier::mousePress(const int x, const int y, MouseButton button){

	TFPaintingPoint relativePoint = getRelativePoint_(x,y);
	if(relativePoint == ignorePoint_) return;

	if(button == MouseButtonRight)
	{
		int nextIndex = (tools_->activeViewBox->currentIndex()+1) % tools_->activeViewBox->count();
		tools_->activeViewBox->setCurrentIndex(nextIndex);
	}
	if(button == MouseButtonLeft)
	{
		leftMousePressed_ = true;
		inputHelper_ = relativePoint;
	}
	if(button == MouseButtonMid)
	{
		zoomMovement_ = true;
		zoomMoveHelper_ = relativePoint;
	}

	emit RefreshView();
}

void TFPolygonModifier::mouseRelease(const int x, const int y){

	TFPaintingPoint relativePoint = getRelativePoint_(x, y, leftMousePressed_ || zoomMovement_);
	if(relativePoint == ignorePoint_) return;

	if(leftMousePressed_) addPolygon_(relativePoint);

	leftMousePressed_ = false;
	zoomMovement_ = false;

	emit RefreshView();
}

void TFPolygonModifier::mouseMove(const int x, const int y){

	TFPaintingPoint relativePoint = getRelativePoint_(x, y, leftMousePressed_ || zoomMovement_);
	if(relativePoint == ignorePoint_) return;

	if(leftMousePressed_)
	{
		for(;inputHelper_.x < relativePoint.x; ++inputHelper_.x)
		{
			addPoint_(inputHelper_.x - baseRadius_, 0);
		}

		addPolygon_(relativePoint);

		for(;inputHelper_.x > relativePoint.x; --inputHelper_.x)
		{
			addPoint_(inputHelper_.x + baseRadius_, 0);
		}
	}

	if(zoomMovement_)
	{
		workCopy_->move(zoomMoveHelper_.x - relativePoint.x, zoomMoveHelper_.y - relativePoint.y);
		zoomMoveHelper_ = relativePoint;
		updateZoomTools_();
	}

	emit RefreshView();
}

void TFPolygonModifier::mouseWheel(const int steps, const int x, const int y){

	TFPaintingPoint relativePoint = getRelativePoint_(x,y);
	if(relativePoint == ignorePoint_) return;

	if(steps > 0) workCopy_->zoomIn(steps, relativePoint.x, relativePoint.y);
	if(steps < 0) workCopy_->zoomOut(-steps, relativePoint.x, relativePoint.y);
	
	updateZoomTools_();
	emit RefreshView();
}

void TFPolygonModifier::addPolygon_(const TFPaintingPoint point){

	addLine_(point.x - baseRadius_, 0,	point.x - topRadius_, point.y);
	addLine_(point.x - topRadius_, point.y, point.x + topRadius_, point.y);
	addLine_(point.x + topRadius_, point.y, point.x + baseRadius_, 0);
}

void TFPolygonModifier::addPoint_(const int x, const int y){

	float yValue = y/(float)inputArea_.height();
	
	switch(activeView_)
	{
		case Active1:
		{
			workCopy_->setComponent1(x, yValue);
			if(type_ == TFModifierGrayscale ||
				type_ == TFModifierGrayscaleAlpha)
			{
				workCopy_->setComponent2(x, yValue);
				workCopy_->setComponent3(x, yValue);
			}
			break;
		}
		case Active2:
		{
			workCopy_->setComponent2(x, yValue);
			break;
		}
		case Active3:
		{
			workCopy_->setComponent3(x, yValue);
			break;
		}
		case ActiveAlpha:
		{
			workCopy_->setAlpha(x, yValue);
			break;
		}
	}
	++lastChange_;	
}

} // namespace GUI
} // namespace M4D
