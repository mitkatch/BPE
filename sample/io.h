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

void Screen(const char *banner, int iUseCurses);
void SetLog(char *file);
void CloseScreen();
void Output(const char *fmt, ...);
void Input(char *string);
void Refresh();
