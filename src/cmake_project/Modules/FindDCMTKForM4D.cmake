IF( ${WIN32} )
	SET( WIN32_USE_PREPARED_PACKAGES_DCMTK TRUE )
ENDIF( ${WIN32} )


	#SET(DCMTK_Components dcmnet dcmdata ofstd )
	SET(DCMTK_Components charls dcmdata dcmimgle dcmpstat dcmtls dcmdsig dcmjpeg dcmqrdb dcmwlm dcmimage dcmnet dcmsr ijg16 ijg12 ijg8 ofstd oflog )
IF( WIN32_USE_PREPARED_PACKAGES_DCMTK )

	SET(DCMTK_LIBRARY_DIR "${MEDV4D_CMAKE_SOURCE_DIR}/../lib/dcmtkLibs")
	
	PREPEND_STRING_TO_LIST_MEMBERS( "${DCMTK_LIBRARY_DIR}/Release/"  DCMTK_Components DCMTK_LIBS_OPTIMIZED )
	APPEND_STRING_TO_LIST_MEMBERS( ".lib"  DCMTK_LIBS_OPTIMIZED DCMTK_LIBS_OPTIMIZED )
	INSERT_KEYWORD_BEFORE_EACH_MEMBER( "optimized" DCMTK_LIBS_OPTIMIZED DCMTK_LIBS_OPTIMIZED )
	
	PREPEND_STRING_TO_LIST_MEMBERS( "${DCMTK_LIBRARY_DIR}/Debug/"  DCMTK_Components DCMTK_LIBS_DEBUG )
	APPEND_STRING_TO_LIST_MEMBERS( ".lib"  DCMTK_LIBS_DEBUG DCMTK_LIBS_DEBUG )
	INSERT_KEYWORD_BEFORE_EACH_MEMBER( "debug" DCMTK_LIBS_DEBUG DCMTK_LIBS_DEBUG )
		
	SET(DCMTK_LIBRARIES ${DCMTK_LIBS_DEBUG} ${DCMTK_LIBS_OPTIMIZED} netapi32 )
	SET(DCMTK_INCLUDE_DIR "${MEDV4D_CMAKE_SOURCE_DIR}/../include/dcmtk")

	SET( DCMTK_OPTIONS "" )
ELSE( WIN32_USE_PREPARED_PACKAGES_DCMTK )
	FIND_PACKAGE(DCMTK REQUIRED)
	IF( NOT DCMTK_LIBRARIES )
		SET(DCMTK_LIBRARIES ${DCMTK_Components} )
	ENDIF( NOT DCMTK_LIBRARIES )

	IF( NOT DCMTK_INCLUDE_DIR )
		SET(DCMTK_INCLUDE_DIR "/usr/include/dcmtk" )
	ENDIF( NOT DCMTK_INCLUDE_DIR )

	FIND_LIBRARY( TIFF_LIBRARY tiff )


	#MESSAGE( "${DCMTK_LIBRARIES}" )
	#SET( DCMTK_LIBRARIES dcmdata dcmimgle dcmpstat dcmdsig dcmjpeg dcmjpls dcmqrdb dcmwlm  dcmnet dcmsr dcmtls dcmimage ijg16 ijg12 ijg8 ${TIFF_LIBRARY} ofstd oflog crypto ssl xml2 )
	SET( DCMTK_LIBRARIES ${DCMTK_LIBRARIES} ${DCMTK_LIBRARIES} ofstd oflog )
	SET( DCMTK_OPTIONS HAVE_CONFIG_H )
ENDIF( WIN32_USE_PREPARED_PACKAGES_DCMTK )

