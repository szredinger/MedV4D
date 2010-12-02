#include <TFAbstractHolder.h>

namespace M4D {
namespace GUI {

TFAbstractHolder::TFAbstractHolder():
	type_(TFHOLDER_UNKNOWN),
	basicTools_(new Ui::TFAbstractHolder),
	button_(NULL),
	setup_(false),
	released_(false),
	painterLeftTop_(25,50),
	painterRightBottom_(25,25),
	colorBarSize_(10),
	index_(0){

	basicTools_->setupUi(this);
}

TFAbstractHolder::TFAbstractHolder(QWidget* parent):
	QWidget(parent),
	type_(TFHOLDER_UNKNOWN),
	basicTools_(new Ui::TFAbstractHolder),
	button_(NULL),
	setup_(false),
	released_(false),
	painterLeftTop_(25,50),
	painterRightBottom_(25,25),
	colorBarSize_(10),
	index_(0){

	basicTools_->setupUi(this);
}

TFAbstractHolder::~TFAbstractHolder(){

	if(basicTools_) delete basicTools_;
	if(button_) delete button_;
}

bool TFAbstractHolder::connectToTFWindow(QObject *tfWindow){
	
	bool resizeConnected = QObject::connect( tfWindow, SIGNAL(ResizeHolder(const TFSize&, const QRect)), this, SLOT(size_changed(const TFSize&, const QRect&)));
	tfAssert(resizeConnected);

	bool activateConnected = QObject::connect(this, SIGNAL( ActivateHolder(const TFSize&) ), tfWindow, SLOT( change_activeHolder(const TFSize&) ));
	tfAssert(activateConnected);
	
	bool paletteConnected = QObject::connect( button_, SIGNAL(TFPaletteSignal(const TFSize&)), tfWindow, SLOT(change_activeHolder(const TFSize&)));
	tfAssert(paletteConnected);

	bool releaseConnected = QObject::connect( this, SIGNAL(ReleaseHolder()), tfWindow, SLOT(release_triggered()));
	tfAssert(releaseConnected);

	bool closeConnected = QObject::connect( this, SIGNAL(CloseHolder()), tfWindow, SLOT(close_triggered()));
	tfAssert(closeConnected);

	return resizeConnected &&
		activateConnected &&
		paletteConnected &&
		releaseConnected &&
		closeConnected;
}

void TFAbstractHolder::createPaletteButton(QWidget *parent){

	button_ = new TFPaletteButton(parent, index_);
}

TFHolderType TFAbstractHolder::getType() const{

	return type_;
}

TFPaletteButton* TFAbstractHolder::getButton() const{

	return button_;
}

void TFAbstractHolder::changeIndex(const TFSize &index){

	index_ = index;
	button_->changeIndex(index_);
}

bool TFAbstractHolder::isReleased(){

	return released_;
}

void TFAbstractHolder::setReleased(const bool &released){

	released_ = released;
	button_->setVisible(!released);
	if(released_) basicTools_->releaseButton->setText("Join");
	else basicTools_->releaseButton->setText("Release");
}

void TFAbstractHolder::save(){

	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Save Transfer Function"),
		QDir::currentPath(),
		tr("TF Files (*.tf)"));

	if (fileName.isEmpty()) return;

	QFile file(fileName);
	if (!file.open(QFile::WriteOnly | QFile::Text))
	{
		QMessageBox::warning(this, tr("Transfer Functions"),
			tr("Cannot write file %1:\n%2.")
			.arg(fileName)
			.arg(file.errorString()));
		return;
	}

	save_(file);

	file.close();
}

void TFAbstractHolder::mousePressEvent(QMouseEvent* e){

	emit ActivateHolder(index_);
	QWidget::mousePressEvent(e);
}

void TFAbstractHolder::keyPressEvent(QKeyEvent* e){

	emit ActivateHolder(index_);
	QWidget::keyPressEvent(e);
}

void TFAbstractHolder::paintEvent(QPaintEvent *e){}

void TFAbstractHolder::resizeEvent(QResizeEvent *e){

	size_changed(index_, rect());
}

void TFAbstractHolder::size_changed(const TFSize& index, const QRect& rect){

	if(index != index_) return;
	
	setGeometry(rect);

	basicTools_->closeButton->move(
		rect.width() - basicTools_->closeButton->width() - painterRightBottom_.x,
		(painterLeftTop_.y - basicTools_->closeButton->height())/2 );
	basicTools_->releaseButton->move(
		basicTools_->closeButton->x() - basicTools_->releaseButton->width() - painterRightBottom_.x,
		(painterLeftTop_.y - basicTools_->releaseButton->height())/2 );

	
	int newWidth = rect.width() - (painterLeftTop_.x + painterRightBottom_.x);
	int newHeight = rect.height() - (painterLeftTop_.y + painterRightBottom_.y);
	QRect newRect(painterLeftTop_.x, painterLeftTop_.y, newWidth, newHeight);
	
	resizePainter_(newRect);
}

void TFAbstractHolder::on_closeButton_clicked(){

	emit CloseHolder();
}

void TFAbstractHolder::on_releaseButton_clicked(){

	emit ReleaseHolder();
}

void TFAbstractHolder::save_(QFile &file){
	
	updateFunction_();

	 TFXmlWriter writer;
	 writer.write(&file, getFunction_(), getType());
	 //writer.writeTestData(&file);	//testing
}

bool TFAbstractHolder::load_(QFile &file){
	
	TFXmlReader reader;

	bool error = false;

	//reader.readTestData(&function_);	//testing
	reader.read(&file, getFunction_(), error);

	if (error || reader.error())
	{
		return false;
	}

	updatePainter_();
	
	return true;
}

void TFAbstractHolder::calculate_(const TFColorMapPtr input, TFColorMapPtr output){

	if(!(input && output)) tfAbort("calculation error");
	if(output->begin() == output->end())
	{
		tfAssert(!"empty output for calculation");
		return;
	}
	if(input->begin() == input->end())
	{
		tfAssert(!"empty input for calculation");
		return;
	}

	TFSize inputSize = input->size();
	TFSize outputSize = output->size();
	float ratio = inputSize/(float)outputSize;

	if(ratio > 1)
	{
		float inOutCorrection = ratio;
		int inOutRatio =  (int)(inOutCorrection);	//how many input values are used for computing 1 output values
		inOutCorrection -= inOutRatio;
		float corrStep = inOutCorrection;

		TFColorMapIt outIt = output->begin();

		TFColorMap::const_iterator inBegin = input->begin();
		TFColorMap::const_iterator inEnd = input->end();

		for(TFColorMap::const_iterator it = inBegin; it != inEnd; ++it)
		{
			TFColor computedValue(0,0,0,0);
			TFSize valueCount = inOutRatio + (int)inOutCorrection;
			for(TFSize i = 0; i < valueCount; ++i)
			{
				if(it == inEnd) return;		//TODO fail

				computedValue.component1 += it->component1;
				computedValue.component2 += it->component2;
				computedValue.component3 += it->component3;
				computedValue.alpha += it->alpha;

				if(i < (valueCount-1)) ++it;
			}
			inOutCorrection -= (int)inOutCorrection;
			inOutCorrection += corrStep;

			tfAssert(outIt != output->end());

			computedValue.component1 = computedValue.component1/valueCount;
			computedValue.component2 = computedValue.component2/valueCount;
			computedValue.component3 = computedValue.component3/valueCount;
			computedValue.alpha = computedValue.alpha/valueCount;

			*outIt = computedValue;
			++outIt;
		}
	}
	else
	{
		float outInCorrection = outputSize/(float)inputSize;
		int outInRatio = (int)(outInCorrection);	//how many input values are used for computing 1 output values
		outInCorrection -= outInRatio;
		float corrStep = outInCorrection;

		TFColorMapIt outIt = output->begin();

		TFColorMap::const_iterator inBegin = input->begin();
		TFColorMap::const_iterator inEnd = input->end();

		for(TFColorMap::const_iterator it = inBegin; it != inEnd; ++it)
		{
			TFSize valueCount = outInRatio + (int)outInCorrection;
			for(TFSize i = 0; i < valueCount; ++i)
			{
				tfAssert(outIt != output->end());
				*outIt = *it;
				++outIt;
			}
			outInCorrection -= (int)outInCorrection;
			outInCorrection += corrStep;
		}
	}
}

} // namespace GUI
} // namespace M4D