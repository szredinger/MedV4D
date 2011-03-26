#include "TFCreator.h"

#include <TFBasicHolder.h>

#include <TFRGBaFunction.h>
#include <TFHSVaFunction.h>

#include <TFGrayscaleAlphaPainter.h>
#include <TFRGBaPainter.h>
#include <TFHSVaPainter.h>

#include <TFSimpleModifier.h>
#include <TFPolygonModifier.h>

namespace M4D {
namespace GUI {

template<TF::Size dim>
typename TFAbstractFunction<dim>::Ptr TFCreator::createFunction_(){

	switch(getStructure_().function)
	{
		case TF::Types::FunctionRGB:
		{
			return typename TFAbstractFunction<dim>::Ptr(new TFRGBaFunction<dim>(domain_));
		}
		case TF::Types::FunctionHSV:
		{
			return typename TFAbstractFunction<dim>::Ptr(new TFHSVaFunction<dim>(domain_));
		}
	}

	tfAssert(!"Unknown function");
	return typename TFAbstractFunction<dim>::Ptr(new TFRGBaFunction<dim>(domain_));	//default
}

template<TF::Size dim>
typename TFAbstractPainter<dim>::Ptr TFCreator::createPainter_(){

	switch(getStructure_().painter)
	{
		case TF::Types::PainterGrayscale:
		{
			return typename TFAbstractPainter<dim>::Ptr(new TFGrayscaleAlphaPainter(false));
		}
		case TF::Types::PainterGrayscaleAlpha:
		{
			return typename TFAbstractPainter<dim>::Ptr(new TFGrayscaleAlphaPainter(true));
		}
		case TF::Types::PainterRGB:
		{
			return typename TFAbstractPainter<dim>::Ptr(new TFRGBaPainter(false));
		}
		case TF::Types::PainterRGBa:
		{
			return typename TFAbstractPainter<dim>::Ptr(new TFRGBaPainter(true));
		}
		case TF::Types::PainterHSV:
		{
			return typename TFAbstractPainter<dim>::Ptr(new TFHSVaPainter(false));
		}
		case TF::Types::PainterHSVa:
		{
			return typename TFAbstractPainter<dim>::Ptr(new TFHSVaPainter(true));
		}
	}
	
	tfAssert(!"Unknown painter!");
	return typename TFAbstractPainter<dim>::Ptr(new TFRGBaPainter(true));
}

template<TF::Size dim>
typename TFAbstractModifier<dim>::Ptr TFCreator::createModifier_(typename TFWorkCopy<dim>::Ptr workCopy){

	switch(getStructure_().modifier)
	{
		case TF::Types::ModifierSimple:
		{
			switch(getStructure_().painter)
			{
				case TF::Types::PainterGrayscale:
				{
					return typename TFAbstractModifier<dim>::Ptr(
						new TFSimpleModifier(workCopy, TFSimpleModifier::Grayscale, false));
				}
				case TF::Types::PainterGrayscaleAlpha:
				{
					return typename TFAbstractModifier<dim>::Ptr(
						new TFSimpleModifier(workCopy, TFSimpleModifier::Grayscale, true));
				}
				case TF::Types::PainterRGB:
				{
					return typename TFAbstractModifier<dim>::Ptr(
						new TFSimpleModifier(workCopy, TFSimpleModifier::RGB, false));
				}
				case TF::Types::PainterRGBa:
				{
					return typename TFAbstractModifier<dim>::Ptr(
						new TFSimpleModifier(workCopy, TFSimpleModifier::RGB, true));
				}
				case TF::Types::PainterHSV:
				{
					return typename TFAbstractModifier<dim>::Ptr(
						new TFSimpleModifier(workCopy, TFSimpleModifier::HSV, false));
				}
				case TF::Types::PainterHSVa:
				{
					return typename TFAbstractModifier<dim>::Ptr(
						new TFSimpleModifier(workCopy, TFSimpleModifier::HSV, true));
				}
			}
		}
		case TF::Types::ModifierPolygon:
		{
			switch(getStructure_().painter)
			{
				case TF::Types::PainterGrayscale:
				{
					return typename TFAbstractModifier<dim>::Ptr(
						new TFPolygonModifier(workCopy, TFPolygonModifier::Grayscale, false));
				}
				case TF::Types::PainterGrayscaleAlpha:
				{
					return typename TFAbstractModifier<dim>::Ptr(
						new TFPolygonModifier(workCopy, TFPolygonModifier::Grayscale, true));
				}
				case TF::Types::PainterRGB:
				{
					return typename TFAbstractModifier<dim>::Ptr(
						new TFPolygonModifier(workCopy, TFPolygonModifier::RGB, false));
				}
				case TF::Types::PainterRGBa:
				{
					return typename TFAbstractModifier<dim>::Ptr(
						new TFPolygonModifier(workCopy, TFPolygonModifier::RGB, true));
				}
				case TF::Types::PainterHSV:
				{
					return typename TFAbstractModifier<dim>::Ptr(
						new TFPolygonModifier(workCopy, TFPolygonModifier::HSV, false));
				}
				case TF::Types::PainterHSVa:
				{
					return typename TFAbstractModifier<dim>::Ptr(
						new TFPolygonModifier(workCopy, TFPolygonModifier::HSV, true));
				}
			}
		}
	}

	tfAssert(!"Unknown modifier!");
	return typename TFAbstractModifier<dim>::Ptr(
		new TFSimpleModifier(workCopy, TFSimpleModifier::RGB, true));	//default
}

TFHolderInterface* TFCreator::createHolder_(){
	
	switch(getStructure_().holder)
	{
		case TF::Types::HolderBasic:
		{
			TFAbstractFunction<1>::Ptr function = createFunction_<1>();
			TFAbstractPainter<1>::Ptr painter = createPainter_<1>();
			TFWorkCopy<1>::Ptr workCopy(new TFWorkCopy<1>(function));
			TFAbstractModifier<1>::Ptr modifier = createModifier_<1>(workCopy);

			return new TFBasicHolder(mainWindow_, painter, modifier, getStructure_());
		}
	}

	tfAssert(!"Unknown holder");
	return NULL;
}


TFCreator::TFCreator(QMainWindow* mainWindow, const TF::Size domain):
	QDialog(mainWindow),
	ui_(new Ui::TFCreator),
	mainWindow_(mainWindow),
	domain_(domain),
	layout_(new QVBoxLayout()),
	state_(Holder),
	holderSet_(false),
	predefinedSet_(false),
	functionSet_(false),
	painterSet_(false),
	modifierSet_(false){

	ui_->setupUi(this);
	ui_->nextButton->setDefault(true);
	ui_->nextButton->setEnabled(false);

	layout_->setContentsMargins(10,10,10,10);
	layout_->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);	
	ui_->scrollArea->setLayout(layout_);

	setWindowTitle(QObject::tr("Transfer Function Creator"));
}

TFCreator::~TFCreator(){}

void TFCreator::setDomain(const TF::Size domain){

	domain_ = domain;
}

TFHolderInterface* TFCreator::createTransferFunction(){

	setStatePredefined_();

	exec();

	if(result() == QDialog::Rejected) return NULL;

	return createHolder_();	
}
	
TFHolderInterface* TFCreator::loadTransferFunction(){
/*
	QString fileName = QFileDialog::getOpenFileName(
		(QWidget*)mainWindow,
		QObject::tr("Open Transfer Function"),
		QDir::currentPath(),
		QObject::tr("TF Files (*.tf *.xml)"));

	if (fileName.isEmpty()) return NULL;

	TFBasicHolder* loaded = new TFBasicHolder(mainWindow);

	QFile qFile(fileName);

	if (!qFile.open(QFile::ReadOnly | QFile::Text)) {
		QMessageBox::warning(
			(QWidget*)mainWindow,
			QObject::tr("Transfer Functions"),
			QObject::tr("Cannot read file %1:\n%2.").arg(fileName).arg(qFile.errorString()));

		qFile.close();
		return NULL;
	}
	if(!loaded->load(qFile))
	{ 
		QMessageBox::warning(
			(QWidget*)mainWindow,
			QObject::tr("TFXmlReader"),
			QObject::tr("Parse error in file %1").arg(fileName));

		qFile.close();
		return NULL;
	}
	qFile.close();
*/
	return NULL;//loaded;
}

TF::Types::Structure& TFCreator::getStructure_(){

	if(predefinedChoice_) return predefinedStructure_;

	return customStructure_;
}

void TFCreator::on_nextButton_clicked(){

	switch(state_)
	{
		case Predefined:
		{
			if(predefinedChoice_) accept();
			else setStateHolder_();
			break;
		}
		case Holder:
		{
			setStateFunction_();
			break;
		}
		case Function:
		{
			setStatePainter_();
			break;
		}
		case Painter:
		{
			setStateModifier_();
			break;
		}
		case Modifier:
		{
			accept();
			break;
		}
	}
}

void TFCreator::on_backButton_clicked(){

	switch(state_)
	{
		case Modifier:
		{
			setStatePainter_();
			break;
		}
		case Painter:
		{
			setStateFunction_();
			break;
		}
		case Function:
		{
			setStateHolder_();
			break;
		}
		case Holder:
		{
			setStatePredefined_();
			break;
		}
		case Predefined:
		{
			reject();
			break;
		}
	}
}

void TFCreator::clearLayout_(){

	QLayoutItem* layoutIt;
	while(!layout_->isEmpty())
	{
		layoutIt = layout_->itemAt(0);
		layout_->removeItem(layoutIt);
		layoutIt->widget()->hide();
		delete layoutIt->widget();
	}
}

void TFCreator::setStatePredefined_(){

	clearLayout_();
	
	TF::Types::PredefinedTypes allPredefined = TF::Types::getPredefinedTypes();
	
	TFPredefinedDialogButton* toActivate = NULL;
	TF::Types::PredefinedTypes::iterator begin = allPredefined.begin();
	TF::Types::PredefinedTypes::iterator end = allPredefined.end();
	for(TF::Types::PredefinedTypes::iterator it = begin; it != end; ++it)
	{
		TFPredefinedDialogButton* type = new TFPredefinedDialogButton(*it);
		type->setText(QString::fromStdString(TF::convert<TF::Types::Predefined, std::string>(*it)));
		if(predefinedSet_ && getStructure_().predefined == *it) toActivate = type;

		bool typeButtonConnected = QObject::connect(type, SIGNAL(Activated(TF::Types::Predefined)), this, SLOT(predefinedButton_clicked(TF::Types::Predefined)));
		tfAssert(typeButtonConnected);

		layout_->addWidget(type);
	}
	if(toActivate) toActivate->setChecked(true);
	
	ui_->description->setText(QObject::tr("Predefined Editors"));
	ui_->backButton->setText(QObject::tr("Cancel"));
	ui_->nextButton->setEnabled(predefinedSet_);

	state_ = Predefined;
}

void TFCreator::setStateHolder_(){

	clearLayout_();
	
	TF::Types::Holders allHolders = TF::Types::getAllHolders();
	
	TFHolderDialogButton* toActivate = NULL;
	TF::Types::Holders::iterator begin = allHolders.begin();
	TF::Types::Holders::iterator end = allHolders.end();
	for(TF::Types::Holders::iterator it = begin; it != end; ++it)
	{
		TFHolderDialogButton* type = new TFHolderDialogButton(*it);
		type->setText(QString::fromStdString(TF::convert<TF::Types::Holder, std::string>(*it)));
		if(holderSet_ && customStructure_.holder == *it) toActivate = type;

		bool typeButtonConnected = QObject::connect(type, SIGNAL(Activated(TF::Types::Holder)), this, SLOT(holderButton_clicked(TF::Types::Holder)));
		tfAssert(typeButtonConnected);

		layout_->addWidget(type);
	}
	if(toActivate) toActivate->setChecked(true);
	
	ui_->description->setText(QObject::tr("Available Editors"));
	ui_->backButton->setText(QObject::tr("Back"));
	ui_->nextButton->setEnabled(holderSet_);

	state_ = Holder;
}

void TFCreator::setStateFunction_(){

	clearLayout_();

	TF::Types::Functions allowedFunctions = TF::Types::getAllowedFunctions(customStructure_.holder);
	
	TFFunctionDialogButton* toActivate = NULL;
	TF::Types::Functions::iterator begin = allowedFunctions.begin();
	TF::Types::Functions::iterator end = allowedFunctions.end();
	for(TF::Types::Functions::iterator it = begin; it != end; ++it)
	{
		TFFunctionDialogButton* type = new TFFunctionDialogButton(*it);
		type->setText(QString::fromStdString(TF::convert<TF::Types::Function, std::string>(*it)));
		if(functionSet_ && customStructure_.function == *it) toActivate = type;

		bool typeButtonConnected = QObject::connect( type, SIGNAL(Activated(TF::Types::Function)), this, SLOT(functionButton_clicked(TF::Types::Function)));
		tfAssert(typeButtonConnected);

		layout_->addWidget(type);
	}
	if(toActivate) toActivate->setChecked(true);

	ui_->description->setText(QObject::tr("Available Functions"));
	ui_->nextButton->setEnabled(functionSet_);

	state_ = Function;
}

void TFCreator::setStatePainter_(){

	clearLayout_();

	TF::Types::Painters allowedPainters = TF::Types::getAllowedPainters(customStructure_.function);

	TFPainterDialogButton* toActivate = NULL;
	TF::Types::Painters::iterator begin = allowedPainters.begin();
	TF::Types::Painters::iterator end = allowedPainters.end();
	for(TF::Types::Painters::iterator it = begin; it != end; ++it)
	{
		TFPainterDialogButton* type = new TFPainterDialogButton(*it);
		type->setText(QString::fromStdString(TF::convert<TF::Types::Painter, std::string>(*it)));
		if(painterSet_ && customStructure_.painter == *it) toActivate = type;

		bool typeButtonConnected = QObject::connect( type, SIGNAL(Activated(TF::Types::Painter)), this, SLOT(painterButton_clicked(TF::Types::Painter)));
		tfAssert(typeButtonConnected);

		layout_->addWidget(type);
	}
	if(toActivate) toActivate->setChecked(true);

	ui_->description->setText(QObject::tr("Available Painters"));
	ui_->nextButton->setText(QObject::tr("Next"));
	ui_->nextButton->setEnabled(painterSet_);

	state_ = Painter;
}

void TFCreator::setStateModifier_(){

	clearLayout_();

	TF::Types::Modifiers allowedModifiers = TF::Types::getAllowedModifiers(customStructure_.painter);

	TFModifierDialogButton* toActivate = NULL;
	TF::Types::Modifiers::iterator begin = allowedModifiers.begin();
	TF::Types::Modifiers::iterator end = allowedModifiers.end();
	for(TF::Types::Modifiers::iterator it = begin; it != end; ++it)
	{
		TFModifierDialogButton* type = new TFModifierDialogButton(*it);
		type->setText(QString::fromStdString(TF::convert<TF::Types::Modifier, std::string>(*it)));
		if(modifierSet_ && customStructure_.modifier == *it)  toActivate = type;

		bool typeButtonConnected = QObject::connect( type, SIGNAL(Activated(TF::Types::Modifier)), this, SLOT(modifierButton_clicked(TF::Types::Modifier)));
		tfAssert(typeButtonConnected);

		layout_->addWidget(type);
	}
	if(toActivate) toActivate->setChecked(true);

	ui_->description->setText(QObject::tr("Available Modifiers"));
	ui_->nextButton->setText(QObject::tr("Finish"));
	ui_->nextButton->setEnabled(modifierSet_);

	state_ = Modifier;
}

void TFCreator::predefinedButton_clicked(TF::Types::Predefined predefined){

	predefinedSet_ = true;
	predefinedChoice_ = predefined != TF::Types::PredefinedCustom;

	ui_->nextButton->setEnabled(true);
	if(predefinedChoice_) 
	{
		predefinedStructure_ = TF::Types::getPredefinedStructure(predefined);
		ui_->nextButton->setText(QObject::tr("Finish"));
	}
	else
	{
		ui_->nextButton->setText(QObject::tr("Next"));
	}
}

void TFCreator::holderButton_clicked(TF::Types::Holder holder){

	holderSet_ = true;	
	if(holder != customStructure_.holder)
	{
		functionSet_ = false;
		painterSet_ = false;
		modifierSet_ = false;
	}

	ui_->nextButton->setEnabled(true);

	customStructure_.holder = holder;
}

void TFCreator::functionButton_clicked(TF::Types::Function function){

	functionSet_ = true;	
	if(function != customStructure_.function)
	{
		painterSet_ = false;
		modifierSet_ = false;
	}

	ui_->nextButton->setEnabled(true);

	customStructure_.function = function;
}

void TFCreator::painterButton_clicked(TF::Types::Painter painter){

	painterSet_ = true;
	if(painter != customStructure_.painter)
	{
		modifierSet_ = false;
	}
	
	ui_->nextButton->setEnabled(true);
	
	customStructure_.painter = painter;
}

void TFCreator::modifierButton_clicked(TF::Types::Modifier modifier){

	modifierSet_ = true;

	ui_->nextButton->setEnabled(true);

	customStructure_.modifier = modifier;
}
//------

} // namespace GUI
} // namespace M4D