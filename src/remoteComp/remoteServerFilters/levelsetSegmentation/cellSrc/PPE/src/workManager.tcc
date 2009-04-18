#ifndef WORKMANAGER_H_
#error File workManager.tcc cannot be included directly!
#else

namespace M4D {
namespace Cell {

///////////////////////////////////////////////////////////////////////////////

template<typename IndexType, typename ValueType>
WorkManager<IndexType, ValueType>::WorkManager(uint32 coreCount)
	: _numOfCores(coreCount)
{
	// aloc props related to SPEs
	m_LayerSegments = new LayerListType[_numOfCores];
	m_UpdateBuffers = new UpdateBufferType[_numOfCores];
	m_configs = new TConfigStructs[_numOfCores];
	
	// layer storeage init
	m_LayerNodeStore = LayerNodeStorageType::New();
	  m_LayerNodeStore->SetGrowthStrategyToExponential();
}

///////////////////////////////////////////////////////////////////////////////

template<typename IndexType, typename ValueType>
WorkManager<IndexType, ValueType>::~WorkManager()
{
	delete [] m_configs;	
	delete [] m_UpdateBuffers;
	delete [] m_LayerSegments;	
}

///////////////////////////////////////////////////////////////////////////////

template<typename IndexType, typename ValueType>
void
WorkManager<IndexType, ValueType>
	::PUSHNode(const IndexType &index, uint32 layerNum)
{
	M4D::Multithreading::ScopedLock lock(_layerAccessMutex);
	
	// push node into segment that have the least count of nodes
	
	LayerNodeType *node = m_LayerNodeStore->Borrow();
	node->m_Value = index;
	m_LayerSegments[0].layers[layerNum]->PushFront( node );
}

///////////////////////////////////////////////////////////////////////////////

template<typename IndexType, typename ValueType>
void
WorkManager<IndexType, ValueType>::UNLINKNode(LayerNodeType *node, uint32 layerNum)
{
	M4D::Multithreading::ScopedLock lock(_layerAccessMutex);
	
	// unlink node from segment that have the biggest count of nodes
	m_LayerSegments[0].layers[layerNum]->Unlink(node);
}

///////////////////////////////////////////////////////////////////////////////
template<typename IndexType, typename ValueType>
void
WorkManager<IndexType, ValueType>::InitCalculateChangeAndUpdActiveLayerConf()
{
	for(uint32 spuIt=0; spuIt<_numOfCores; spuIt++)
		{
		m_configs[spuIt].calcChngApplyUpdateConf.layer0Begin = 
		(NodeTypeInSPU *) m_LayerSegments[spuIt].layers[0]->Begin().GetPointer();
		m_configs[spuIt].calcChngApplyUpdateConf.layer0End = 
		(NodeTypeInSPU *) m_LayerSegments[spuIt].layers[0]->End().GetPointer();
    
		m_configs[spuIt].calcChngApplyUpdateConf.updateBuffBegin = 
			&m_UpdateBuffers[spuIt][0];
		}
}

///////////////////////////////////////////////////////////////////////////////
template<typename IndexType, typename ValueType>
void
WorkManager<IndexType, ValueType>::InitPropagateValuesConf()
{
	for(uint32 spuIt=0; spuIt<_numOfCores; spuIt++)
	{
		for(uint32 i=0; i<LYERCOUNT; i++)
	    {
			m_configs[spuIt].propagateValsConf.layerBegins[i] = 
				(NodeTypeInSPU *) m_LayerSegments[spuIt].layers[i]->Begin().GetPointer();
			
			m_configs[spuIt].propagateValsConf.layerEnds[i] = 
				(NodeTypeInSPU *) m_LayerSegments[spuIt].layers[i]->End().GetPointer();
	    }
	}
}

///////////////////////////////////////////////////////////////////////////////
template<typename IndexType, typename ValueType>
void
WorkManager<IndexType, ValueType>::SetupRunConfig(RunConfiguration *conf)
{
	// copy it into configs of all SPEs 
	for(uint32 spuIt=0; spuIt<_numOfCores; spuIt++)
	{
		m_configs[spuIt].runConf = *conf;
	}
}

///////////////////////////////////////////////////////////////////////////////

template<typename IndexType, typename ValueType>
void
WorkManager<IndexType, ValueType>::AllocateUpdateBuffers()
{
  // Preallocate the update buffer.  NOTE: There is currently no way to
  // downsize a std::vector. This means that the update buffer will grow
  // dynamically but not shrink.  In newer implementations there may be a
  // squeeze method which can do this.  Alternately, we can implement our own
  // strategy for downsizing.
//	if(m_Layers[0]->Size() > 10000)
//	{
//		int i = 10;
//	}
	for(uint32 spuIt=0; spuIt<_numOfCores; spuIt++)
	{	
		m_UpdateBuffers[spuIt].clear();
		m_UpdateBuffers[spuIt].reserve(m_LayerSegments[spuIt].layers[0]->Size());
		memset(&m_UpdateBuffers[spuIt][0], 0, m_LayerSegments[spuIt].layers[0]->Size() * sizeof(ValueType));
	}
//  std::cout << "Update list, after reservation:" << std::endl;
//  PrintUpdateBuf(std::cout);
}

///////////////////////////////////////////////////////////////////////////////

//template<class TInputImage,class TFeatureImage, class TOutputPixelType>
//void
//MySegmtLevelSetFilter_InitPart<TInputImage, TFeatureImage, TOutputPixelType>
//::PrintUpdateBuf(std::ostream &s)
//{
//	s << "size=" << m_Layers[0]->Size() << std::endl;
//	ValueType *updBuffData = &m_UpdateBuffer[0];
//    for(uint32 i=0; i<m_Layers[0]->Size(); i++, updBuffData++)
//    	s << *updBuffData << ", ";
//  	  s << std::endl;
//}

///////////////////////////////////////////////////////////////////////////////

}
}
#endif
