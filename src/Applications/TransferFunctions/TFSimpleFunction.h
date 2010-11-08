#ifndef TF_SIMPLEFUNCTION
#define TF_SIMPLEFUNCTION

#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <cassert>

#include <TFAbstractFunction.h>
#include <GUI/utils/TransferFunctionBuffer.h>

namespace M4D {
namespace GUI {

class TFSimpleFunction: public TFAbstractFunction{

public:

	TFSimpleFunction(TFSize domain = 4096);
	TFSimpleFunction(TFSimpleFunction &function);

	~TFSimpleFunction();

	void operator=(TFSimpleFunction &function);
	TFAbstractFunction* clone();

	void setPoint(TFSize point, float value);
	void setFunction(TFFunctionMapPtr function);

	TFFunctionMapPtr getFunction();
	TFSize getDomain();

	void clear();

	template<typename ElementIterator>
	bool apply(
		ElementIterator begin,
		ElementIterator end){

		tfAssert((end-begin)==domain_);
			
		tfAbort("unsupported buffer type");

		return false;
	}

	template<>
	bool apply<TransferFunctionBuffer1D::Iterator>(
		TransferFunctionBuffer1D::Iterator begin,
		TransferFunctionBuffer1D::Iterator end){

		tfAssert((end-begin)==points_->size());
			
		typedef TransferFunctionBuffer1D::ValueType ValueType;

		TFFunctionMapIt currentPoint = points_->begin();
		for(TransferFunctionBuffer1D::Iterator it = begin; it!=end; ++it)
		{
			float value = *currentPoint;
			*it = ValueType(value, value, value, 1);
			++currentPoint;
		}

		return true;
	}

private:	
	TFFunctionMapPtr points_;
};

} // namespace GUI
} // namespace M4D

#endif //TF_SIMPLEFUNCTION