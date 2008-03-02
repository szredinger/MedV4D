#ifndef M4DDICOM_MOVE_SERVICE
#define M4DDICOM_MOVE_SERVICE

/**
 *  Implements C-MOVE service to DICOM server
 */

using namespace std;

class M4DMoveService : M4DAbstractService
{
private :

	enum eCallType {
		SINGLE_IMAGE,		// only single image -> retype to DicomObj
		IMAGE_SET			// retype to vector<DicomObj>
	};

	void GetQuery( 
		DcmDataset **query,
		const string *patientID,
		const string *studyID,
		const string *setID,
		const string *imageID);

	void MoveSupport( DcmDataset *query, void *data,
		enum eCallType type) throw (...);

	static void
	MoveCallback(void *callbackData, T_DIMSE_C_MoveRQ *request,
		int responseCount, T_DIMSE_C_MoveRSP *response);

	static void
	SubAssocCallback(void *subOpCallbackData,
        T_ASC_Network *aNet, T_ASC_Association **subAssoc);

	inline static void
	SubAssocCallbackSupp(void *subOpCallbackData,
        T_ASC_Network *aNet, T_ASC_Association **subAssoc, eCallType type);

	static void
	SubAssocCallbackWholeSet(void *subOpCallbackData,
        T_ASC_Network *aNet, T_ASC_Association **subAssoc);

	static void AcceptSubAssoc(
		T_ASC_Network * aNet, T_ASC_Association ** assoc) throw (...);

	static void	SubTransferOperationSCP(
		T_ASC_Association **subAssoc, 
		void *dicomOBJRef, eCallType type) throw (...);

	static void StoreSCPCallback(    
		void *callbackData,					/* in */
		T_DIMSE_StoreProgress *progress,    /* progress state */
		T_DIMSE_C_StoreRQ *req,             /* original store request */
		char *imageFileName, 
		DcmDataset **imageDataSet, /* being received into */
		/* out */
		T_DIMSE_C_StoreRSP *rsp,            /* final store response */
		DcmDataset **statusDetail);

public:
	M4DMoveService();
	~M4DMoveService();

	void MoveImage( 
		const string &patientID,
		const string &studyID,
		const string &setID,
		const string &imageID,
		M4DDcmProvider::DicomObj &rs) throw (...);

	void MoveImageSet(
		const string &patientID,
		const string &studyID,
		const string &serieID,
		M4DDcmProvider::M4DDicomObjSet &result) throw (...);
	
};

#endif