// EmbeddingTcl.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int square (int i) {
	return i*i;
}

static int _wrap_square(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {
	int  _result;
	int  _arg0;
	Tcl_Obj * tcl_result;
	int tempint;

	clientData = clientData; objv = objv;
	tcl_result = Tcl_GetObjResult(interp);
	if ((objc < 2) || (objc > 2)) {
		Tcl_SetStringObj(tcl_result,"Wrong # args. square i ",-1);
		return TCL_ERROR;
	}
	if (Tcl_GetIntFromObj(interp,objv[1],&tempint) == TCL_ERROR) return TCL_ERROR;
	_arg0 = (int ) tempint;
	_result = (int )square(_arg0);
	tcl_result = Tcl_GetObjResult(interp);
	Tcl_SetIntObj(tcl_result,(long) _result);
	return TCL_OK;
}

int Tcl_AppInit(Tcl_Interp *interp){
	if (Tcl_Init(interp) == TCL_ERROR)
		return TCL_ERROR;
	/* Now initialize our functions */
	Tcl_CreateObjCommand(interp, "square", _wrap_square, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
	return TCL_OK;
}

int _tmain(int argc, _TCHAR* argv[])
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	Tcl_Interp *interp;
	interp = Tcl_CreateInterp();
	Tcl_AppInit(interp);

	
	FILE       * infile   ;
	size_t       filesize ;
	char       * pstr   ;
	int          rc       =0;


	/* Read the configuration file with specific script code
	(in reality, I use a function that searches for the file and opens it!
	Hence I need to do more work ... If you have the file name, then use
	Tcl_EvalFile()!)
	*/
	infile = fopen( "script.tcl", "r" ) ;

	if ( infile == NULL )
	{
		return 1 ;
	}

	/* Now, read the whole file ... */
	fseek( infile, 0L, SEEK_END ) ;
	filesize = ftell( infile ) ;
	pstr     = (char *) malloc( (filesize+1) * sizeof(char) ) ;
	fseek( infile, 0L, SEEK_SET ) ;

	fread( pstr, filesize, 1, infile ) ;
	pstr[filesize] = '\0' ;

	rc = Tcl_EvalFile( interp, "script.tcl" ) ;
	if ( rc != TCL_OK )
	{
		fprintf( stderr, "Error loading script library\n" ) ;
		return 1 ;
	}

	rc = Tcl_Eval(interp, "printSquare 10");
	if ( rc != TCL_OK )
	{
		fprintf( stderr, "Error loading script library\n" ) ;
		return 1 ;
	}
	rc = Tcl_Eval(interp, "puts \"g_x value is $g_x \"");
	if ( rc != TCL_OK )
	{
		fprintf( stderr, "Error loading script library\n" ) ;
		return 1 ;
	}

	

	free( pstr ) ;

	std::cout << "Hello" << std::endl;
	Tcl_Finalize();


	return 0;
}

