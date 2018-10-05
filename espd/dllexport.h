/****************************************************************************
 *
 * eSpeed, Inc.
 * 
 *
 * This system and/or its use is protected under one or more of the following 
 * United States patents:  5,905,974; 6,560,580.  All rights reserved.  Other 
 * patents pending 
 * 
 ***************************************************************************/
/*
 * Win32 DLL specifics
 */

#if !defined _espdexport_h_
#define _espdexport_h_

#if defined(_USRDLL)
/* The build type is dynamic */
#if defined(BUILDING_ESPD_DLL)
#define ESPD_SHARE            __declspec( dllexport )
#else
#define ESPD_SHARE            __declspec( dllimport )
#endif 

#else
/* The build type is static */
#define ESPD_SHARE
#endif


#endif /* _espdexport_h_ */
