#include "TFSimpleHolder.h"

namespace M4D {
namespace GUI {

TFSimpleHolder::TFSimpleHolder(QWidget* window){

	setParent(window);
	type_ = TFTYPE_SIMPLE;
}

TFSimpleHolder::~TFSimpleHolder(){}

void TFSimpleHolder::setUp(QWidget *parent, const QRect rect){

	painter_.setUp(this, PAINTER_MARGIN);
	size_changed(rect);
	setParent(parent);
	show();
}

void TFSimpleHolder::save_(QFile &file){

	updateFunction_();

	 TFXmlSimpleWriter writer;
     writer.write(&file, function_);
	 //writer.writeTestData(&file);	//testing
}

bool TFSimpleHolder::load_(QFile &file){

	TFXmlSimpleReader reader;

	bool error = false;

	reader.readTestData(&function_);	//testing
	//reader.read(&file, &function_, error);

	if (error || reader.error())
	{
		return false;
	}

	calculate_(function_.getFunction(), painter_.getView());

	return true;
}

void TFSimpleHolder::updateFunction_(){

	if(!painter_.changed()) return;

	calculate_(painter_.getView(), function_.getFunction());
}

void TFSimpleHolder::size_changed(const QRect rect){
	
	setGeometry(rect);

	int newWidth = rect.width() - 2*PAINTER_X;
	int newHeight = rect.height() - 2*PAINTER_Y;

	updateFunction_();

	painter_.resize(QRect(PAINTER_X, PAINTER_Y, newWidth, newHeight));
	
	calculate_(function_.getFunction(), painter_.getView());
}

TFAbstractFunction* TFSimpleHolder::getFunction_(){

	return &function_;
}

void TFSimpleHolder::calculate_(const TFFunctionMapPtr input, TFFunctionMapPtr output){

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

		TFFunctionMapIt outIt = output->begin();

		TFFunctionMap::const_iterator inBegin = input->begin();
		TFFunctionMap::const_iterator inEnd = input->end();

		for(TFFunctionMap::const_iterator it = inBegin; it != inEnd; ++it)
		{
			float computedValue = 0;
			TFSize valueCount = inOutRatio + (int)inOutCorrection;
			for(TFSize i = 0; i < valueCount; ++i)
			{
				if(it == inEnd) return;		//TODO fail
				computedValue += *it;
				if(i < (valueCount-1)) ++it;
			}
			inOutCorrection -= (int)inOutCorrection;
			inOutCorrection += corrStep;

			tfAssert(outIt != output->end());
			float avarage = computedValue/valueCount;
			*outIt = avarage;
			++outIt;
		}
	}
	else
	{
		float outInCorrection = outputSize/(float)inputSize;
		int outInRatio = (int)(outInCorrection);	//how many input values are used for computing 1 output values
		outInCorrection -= outInRatio;
		float corrStep = outInCorrection;

		TFFunctionMapIt outIt = output->begin();

		TFFunctionMap::const_iterator inBegin = input->begin();
		TFFunctionMap::const_iterator inEnd = input->end();

		for(TFFunctionMap::const_iterator it = inBegin; it != inEnd; ++it)
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
/*
void TFSimpleHolder::receiveHistogram(const TFHistogram& histogram){
	
	painter_.setHistogram(histogram);
	painter_.paintHistogram(true);
}
*/

} // namespace GUI
} // namespace M4D
