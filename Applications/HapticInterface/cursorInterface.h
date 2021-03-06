#ifndef M4D_GUI_OPENGLHAPTICVIEWERWIDGET_H_HAPTIC_VIEWER_CURSOR_INTERFACE
#define M4D_GUI_OPENGLHAPTICVIEWERWIDGET_H_HAPTIC_VIEWER_CURSOR_INTERFACE
#define _MSVC

#include "Imaging/Imaging.h"
#include "chai3d.h"
#include "vtkImageData.h"

namespace M4D
{
	namespace Viewer
	{
		class cursorInterface
		{
		public:
			virtual double GetX(); // returns X part of coordinates of cursor
			virtual double GetY(); // returns Y part of coordinates of cursor
			virtual double GetZ(); // returns Z part of coordinates of cursor
			virtual void GetCursorCenter(double center[3]); // returns cursor position as vector
			virtual void GetCursorCenterAsIndexes(int center[3]); // returns cursor position as vector of indexes in data file
			virtual void GetRadiusCubeCenter(double center[3]); // returns cube center position as vector
			virtual void GetRadiusCubeCenterAsIndexes(int center[3]); // returns cube center position as vector of indexes in data file
			virtual double GetScale(); // returns size of cube where is action radius of cursor
			virtual int GetScaleAsIndexDifference(); // returns size of cube where is action radius of cursor as difference of indexes in data file
			virtual void reloadParameters(); // reload image parameters from inPort
			virtual void SetScale(double scale); // Sets scale
			virtual int GetZSlice();
			virtual int GetDataMinValue();
			virtual int GetDataMaxValue();
			virtual void SetToOriginal();
			virtual void SetData( vtkImageData* a_input);
			cursorInterface(vtkImageData* input);
			cursorInterface();
		protected: 
			boost::mutex cursorMutex;
			virtual void SetCursorPosition(const cVector3d& position);
			vtkImageData* input; // link to dataset
			double cursorCenter[3];
			double cursorRadiusCubeCenter[3];
			double cursorRadiusCubeCenterOriginal[3];
			unsigned short minVolumeValue, maxVolumeValue;
			double scale, originalScale; // size of cube where is action radius of cursor
			double imageRealHeight, imageRealWidth, imageRealDepth; // parameters of volume dataset - size in mm
			double imageRealOffsetHeight, imageRealOffsetWidth, imageRealOffsetDepth; // offset which indicates how far VTK starts drawing of object from 0,0,0
			double imageSpacingWidth, imageSpacingHeight, imageSpacingDepth; // spacing of eachdimension
			int imageOffsetHeight, imageOffsetWidth, imageOffsetDepth;
			int imageDataHeight, imageDataWidth, imageDataDepth; // parameters of volume dataset - size in voxels
		};
	}
}

#endif