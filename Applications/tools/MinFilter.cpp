#include "MedV4D/Common/Common.h"
#include "Filtering.h"
#include "Imaging/filters/MinFilter.h"
#undef min
#undef max
#include <tclap/CmdLine.h>


using namespace M4D;
using namespace M4D::Imaging;

typedef Image< uint8, 2 > ImageType;

int
main( int argc, char **argv )
{
	std::ofstream logFile( "Log.txt" );
        SET_LOUT( logFile );

        D_COMMAND( std::ofstream debugFile( "Debug.txt" ); );
        SET_DOUT( debugFile );
	
	TCLAP::CmdLine cmd( "Min filter.", ' ', "");
	/*---------------------------------------------------------------------*/

	TCLAP::ValueArg<unsigned> radiusArg( "r", "radius", "Filter mask radius.", false, 1, "Unsigned integer" );
	cmd.add( radiusArg );

	/*---------------------------------------------------------------------*/
	TCLAP::UnlabeledValueArg<std::string> inFilenameArg( "input", "Input image filename", true, "", "filename1" );
	cmd.add( inFilenameArg );

	TCLAP::UnlabeledValueArg<std::string> outFilenameArg( "output", "Output image filename", true, "", "filename2" );
	cmd.add( outFilenameArg );

	cmd.parse( argc, argv );

	/***************************************************/

	std::string inFilename = inFilenameArg.getValue();
	std::string outFilename = outFilenameArg.getValue();

	std::cout << "Loading file '" << inFilename << "' ..."; std::cout.flush();
	M4D::Imaging::AImage::Ptr image = 
			M4D::Imaging::ImageFactory::LoadDumpedImage( inFilename );
	std::cout << "Done\n";


	std::cout << "Initializing..."; std::cout.flush();
	M4D::Imaging::PipelineContainer *container = NULL;
	FinishHook  *hook = new FinishHook;
	M4D::Imaging::ConnectionInterfaceTyped< M4D::Imaging::AImage > *inConnection = NULL;
	M4D::Imaging::ConnectionInterfaceTyped< M4D::Imaging::AImage > *outConnection = NULL;
	M4D::Imaging::APipeFilter *filter = NULL;
	/*---------------------------------------------------------------------*/
	unsigned radius = radiusArg.getValue();

	IMAGE_NUMERIC_TYPE_PTR_SWITCH_MACRO( image, 
		M4D::Imaging::MinFilter2D< IMAGE_TYPE > *minFilter = new M4D::Imaging::MinFilter2D< IMAGE_TYPE >();
		minFilter->SetRadius( radius );
		filter = minFilter;
	);

	/*---------------------------------------------------------------------*/
	container = PrepareSimplePipeline( *filter, M4D::Imaging::MessageReceiverInterface::Ptr( hook ), inConnection, outConnection );
	//container = PreparePipeline<ImageType>( *filter, M4D::Imaging::MessageReceiverInterface::Ptr( hook ), inConnection, outConnection );
	//
	inConnection->PutDataset( image );

	std::cout << "Done\n";

	std::cout << "Computing..."; std::cout.flush();
	container->ExecuteFirstFilter();

	while( !(hook->Finished()) ){ /*empty*/ }
	if( hook->OK() ) {
		std::cout << "Done in "<< filter->GetLastComputationTime() << " seconds.\n";

		std::cout << "Saving file '" << outFilename << "' ..."; std::cout.flush();
		M4D::Imaging::ImageFactory::DumpImage( outFilename, outConnection->GetDatasetReadOnlyTyped() );
		std::cout << "Done\n";
	} else {
		std::cout << "FAILED\n";
	}
	delete container;

	return 0;
}
