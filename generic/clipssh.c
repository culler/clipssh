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

extern void addTransientClip(const char *clip);
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
    Tcl_Size length;

    if (objc != 2) {
	Tcl_WrongNumArgs(interp, 1, objv, "clipstring");
	return TCL_ERROR;
    }
    clip = Tcl_GetStringFromObj(objv[1], &length);
    addTransientClip(clip);
    ckfree((void *) clip);
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
