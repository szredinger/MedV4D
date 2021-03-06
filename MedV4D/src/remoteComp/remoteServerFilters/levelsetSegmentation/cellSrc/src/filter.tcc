#ifndef CELLREADYTHRESHOLDSEGMENTATIONLEVELSETIMAGEFILTER_H_
#error File filter.tcc cannot be included directly!
#else

///////////////////////////////////////////////////////////////////////////////

template<class TInputImage,class TFeatureImage, class TOutputPixelType>
MySegmtLevelSetFilter<TInputImage, TFeatureImage, TOutputPixelType>
::MySegmtLevelSetFilter()
{
}
///////////////////////////////////////////////////////////////////////////////
template<class TInputImage,class TFeatureImage, class TOutputPixelType>
MySegmtLevelSetFilter<TInputImage, TFeatureImage, TOutputPixelType>
::~MySegmtLevelSetFilter()
{}

///////////////////////////////////////////////////////////////////////////////
template<class TInputImage,class TFeatureImage, class TOutputPixelType>
void
MySegmtLevelSetFilter<TInputImage, TFeatureImage, TOutputPixelType>
::Initialize()
{
  this->InitializeInputAndConstructLayers();
	
  this->InitRunConf();
  
  this->m_SPEManager.Init();
  
  //M4D::Cell::PrintITKImageToFile<OutputImageType>(*this->GetOutput(), "beforeInitActiveVals");
  
  // Set the values in the output image for the active layer.
  this->InitializeActiveLayerValues();
  
  //M4D::Cell::PrintITKImageToFile<OutputImageType>(*this->GetOutput(), "afterInitActiveVals");
  //M4D::Cell::PrintITKImageToFile<typename Superclass::StatusImageType>(*this->m_StatusImage, "beforePropagValsStatus");
 
  // Initialize layer values using the active layer as seeds.
  this->PropagateAllLayerValues();
  
  //M4D::Cell::PrintITKImageToFile<typename Superclass::StatusImageType>(*this->m_StatusImage, "afterPropagValsStatus");
  //M4D::Cell::PrintITKImageToFile<OutputImageType>(*this->GetOutput(), "afterPropagVals");

  // Initialize pixels inside and outside the sparse field layers to positive
  // and negative values, respectively.  This is not necessary for the
  // calculations, but is useful for presenting a more intuitive output to the
  // filter.  See PostProcessOutput method for more information.
  this->InitializeBackgroundPixels();
}

///////////////////////////////////////////////////////////////////////////////

template<class TInputImage,class TFeatureImage, class TOutputPixelType>
void
MySegmtLevelSetFilter<TInputImage, TFeatureImage, TOutputPixelType>
::ApplyUpdate(TimeStepType dt)
{
	//this->_workManager.PrintLists(DOUT, true);
	//this->_workManager.m_LayerNodeStore.Print(DOUT);
	this->SetRMSChange(this->m_SPEManager.ApplyUpdate(dt));
	
	//M4D::Cell::PrintITKImageToFile<OutputImageType>(*this->GetOutput(), "afterApplyUpdate");
}

///////////////////////////////////////////////////////////////////////////////
template<class TInputImage,class TFeatureImage, class TOutputPixelType>
TimeStepType
MySegmtLevelSetFilter<TInputImage, TFeatureImage, TOutputPixelType>
::CalculateChange()
{	  
//	this->_workManager.PrintLists(DOUT, true);
	//this->_workManager.m_LayerNodeStore.Print(DOUT);
	TimeStepType dt = this->m_SPEManager.RunUpdateCalc();
	//M4D::Cell::PrintITKImageToFile<OutputImageType>(*this->GetOutput(), "afterCalcChange");
	return dt;
}
///////////////////////////////////////////////////////////////////////////////

template<class TInputImage,class TFeatureImage, class TOutputPixelType>
void
MySegmtLevelSetFilter<TInputImage, TFeatureImage, TOutputPixelType>
::PropagateAllLayerValues()
{
	//this->_workManager.PrintLists(DOUT, true);
	this->m_SPEManager.RunPropagateLayerVals();
}

///////////////////////////////////////////////////////////////////////////////

#endif
