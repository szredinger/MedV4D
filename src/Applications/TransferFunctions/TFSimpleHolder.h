#ifndef TF_SIMPLEHOLDER
#define TF_SIMPLEHOLDER

#include "common/Types.h"

#include <TFAbstractHolder.h>
#include <TFSimpleFunction.h>
#include <TFSimplePainter.h>
#include <TFXmlSimpleReader.h>
#include <TFXmlSimpleWriter.h>

#include <string>
#include <map>
#include <vector>

namespace M4D {
namespace GUI {

#define PAINTER_X 25
#define PAINTER_Y 25
#define PAINTER_MARGIN 5

class TFSimpleHolder: public TFAbstractHolder{

public:
	TFSimpleHolder(QWidget* window);
	~TFSimpleHolder();

	void setUp(QWidget *parent, const QRect rect);

protected slots:
	void size_changed(const QRect rect);

protected:
	void save_(QFile &file);
	bool load_(QFile &file);
	void updateFunction_();

	TFAbstractFunction* getFunction_();

private:
	TFSimpleFunction function_;
	TFSimplePainter painter_;

	void calculate_(const TFFunctionMapPtr input, TFFunctionMapPtr output);
};

} // namespace GUI
} // namespace M4D

#endif //TF_SIMPLEHOLDER