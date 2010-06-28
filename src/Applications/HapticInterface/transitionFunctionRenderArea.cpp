#include <QtGui>

#include "transitionFunctionRenderArea.h"
#include <sstream>

transitionFunctionRenderAreaWidget::transitionFunctionRenderAreaWidget(transitionFunction* functionData, QWidget *parent)
    : QWidget(parent)
{
   this->functionData = functionData;
   setMouseTracking(true);

    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);

	Qt::PenStyle style = Qt::PenStyle(Qt::PenStyle::SolidLine);
	Qt::PenCapStyle cap = Qt::PenCapStyle(Qt::PenCapStyle::FlatCap);
	Qt::PenJoinStyle join = Qt::PenJoinStyle(Qt::PenJoinStyle::MiterJoin);
	pen = QPen(Qt::black, 1, style, cap, join);
}

QSize transitionFunctionRenderAreaWidget::minimumSizeHint() const
{
    return QSize(100, 100);
}

QSize transitionFunctionRenderAreaWidget::sizeHint() const
{
    return QSize(800, 200);
}

void transitionFunctionRenderAreaWidget::paintEvent(QPaintEvent * /* event */)
{
    double xstep = (double)(width() - 2) / (double)(functionData->GetMaxPoint() - functionData->GetMinPoint());
	
	QPainterPath path;
    path.moveTo((int)(xstep * (double)functionData->GetMinPoint()) + 1,height() - ((int)((double)(height()-2) * functionData->GetValueOfMinPoint()) + 1));
    for (size_t i = 1; i < functionData->size(); ++i)
	{
		path.lineTo(QPoint((int)(xstep * (double)functionData->GetPointOnPosition(i)) + 1, height()-((int)((double)(height()-2) * functionData->GetValueOnPoint(functionData->GetPointOnPosition(i))) + 1)));
	}

    QPainter painter(this);
    painter.setPen(pen);
	painter.setBrush(Qt::NoBrush);
    painter.save();
	painter.drawPath(path);

	painter.restore();
                
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);
	painter.save();
    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));

	painter.restore();

	painter.setPen(pen);
	painter.setBrush(Qt::NoBrush);
	painter.save();
	painter.drawText(1, height() - 3, tr("0"));

	painter.restore();

	painter.setPen(pen);
		painter.setBrush(Qt::NoBrush);
	painter.save();
	painter.drawText(1, 11, tr("1.0"));

	painter.restore();

	painter.setPen(pen);
		painter.setBrush(Qt::NoBrush);
	painter.save();
	std::stringstream s;
	s << functionData->GetMaxPoint();
	painter.drawText(width() - 26, height() - 3, tr(s.str().c_str()));

}

void transitionFunctionRenderAreaWidget::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) 
	{
		double x = (double)event->pos().x() / (double)width();
		double y = ((double)height() - (double)event->pos().y()) / (double)height();
		emit addPointSignal(x, y);
	}
}

void transitionFunctionRenderAreaWidget::mouseMoveEvent( QMouseEvent * event )
{
	double x = (double)event->pos().x() / (double)width();
	double y = ((double)height() - (double)event->pos().y()) / (double)height();
	emit mouseCoordinatesChangedSignal(x, y);
}