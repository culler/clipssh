/*
 * tkgl.c --
 *
 * Copyright (C) 2024, Marc Culler, Nathan Dunfield, Matthias Goerner
 *
 * This file is part of the Clipssh project.  Clipssh is distributed under the
 * Tcl license.  The terms of the license are described in the file
 * "license.terms" which should be included with this distribution.
 */

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include "tcl.h"
#include "tk.h"
#include <string.h>

extern void addTransientClip(const char *clip, double delay);
extern void initPasteboard();

/*
 *--------------------------------------------------------------
 *
 * ClipsshObjCmd --
 *
 *	This procedure is invoked to process the "clipssh" Tcl command.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	A transient clip is quietly added to the system clipboard.
 *
 *--------------------------------------------------------------
 */

int
ClipsshObjCmd(
    void *clientData,
    Tcl_Interp *interp,		/* Current interpreter. */
    int objc,			/* Number of arguments. */
    Tcl_Obj *const objv[])	/* Argument objects. */
{
    const char *clip;
    int millis = 500;
    Tcl_Size length;
    static const char *const optionStrings[] = {"-delay", NULL};
    if (objc != 2 && objc != 4) {
	Tcl_WrongNumArgs(interp, 1, objv, "?-delay millis? string");
	return TCL_ERROR;
    }
    clip = Tcl_GetStringFromObj(objv[objc -1], &length);
    if (objc == 4) {
	if (Tcl_GetIndexFromObj(NULL, objv[1], optionStrings,
				"option", 0, NULL) != TCL_OK) {
	    return TCL_ERROR;
	}
	if (Tcl_GetIntFromObj(interp, objv[2], &millis) != TCL_OK) {
	    return TCL_ERROR;
	}
    }
    addTransientClip(clip, millis / 1000.0);
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Clipssh_Init --
 *
 *	Initialize the Clipssh package.
 *
 * Results:
 *	A standard Tcl result
 *
 * Side effects:
 *	The Clipssh package is created.
 *
 *----------------------------------------------------------------------
 */

DLLEXPORT int
Clipssh_Init(
    Tcl_Interp* interp)		/* Tcl interpreter */
{
    if (Tcl_InitStubs(interp, TCL_VERSION, 0) == NULL) {
	return TCL_ERROR;
    }

    if (Tk_InitStubs(interp, TK_VERSION, 0) == NULL) {
        return TCL_ERROR;
    }
    if (Tcl_PkgProvideEx(interp, PACKAGE_NAME, PACKAGE_VERSION, NULL) != TCL_OK) {
	return TCL_ERROR;
    }
    if (!Tcl_CreateObjCommand(interp, "clipssh", (Tcl_ObjCmdProc *)ClipsshObjCmd,
			      NULL, NULL)) {
	return TCL_ERROR;
    }
    initPasteboard();
    return TCL_OK;
}

#ifdef __cplusplus
}
#endif  /* __cplusplus */

/*
 * Local Variables:
 * mode: c
 * c-basic-offset: 4
 * fill-column: 78
 * End:
 */
