/*
 *  Cplot.h
 *  1D EM Sim using CPlot
 *
 *  Created by Tom on Sat Dec 11 2004.
 *  Copyright (c) 2004 __MyCompanyName__. All rights reserved.
 *
 *		An attempt to emulate the easy to use Matlab 'plot' command.
 *		Should support both static and animated plots.
 */

#ifndef _CPLOT_H_
#define _CPLOT_H_

#include "wx/wx.h"
#include "wx/glcanvas.h"
#include "wxGLString.h"


// Function prototypes
float GetDeltaT(wxLongLong now);

// Space for labels, etc., in pixels not screen coor.
#define XTITLEMARGIN 14			// space for axis titles
#define YTITLEMARGIN 14
#define TITLEMARGIN 20				// space for plot title
#define XAXISMARGIN 14				// space for x axis numbering
#define YAXISMARGIN 14
#define FPSMARGIN 14				// room on the bottom to put fps info
#define RIGHTMARGIN 5				// space between graph and right edge of window

class CPlot
{
public:
    // length = length of data to plot
    // xArray = optional xArray, independent data (integers 1...length if not supplied)
    // yArray = the data
    // width = window width
    // height = window height
	CPlot(const int length, const float *yArray, const float *xArray, const int width, const int height);
	CPlot(const int length, const float *yArray, const int width, const int height);
	CPlot(const int length, const float *yArray, const float *xArray);
	CPlot(const int length, const float *yArray);
	~CPlot();
	
	void SetColor(const float rc, const float gc, const float bc);
	void SetColor(const float rc, const float gc, const float bc, const float ac);
	void SetColor(const GLfloat cV[]);
	void SetAxis(const float xmin, const float xmax, const float ymin, const float ymax); 
	int GetWindowWidth(void) const {return windowWidth;}
	int GetWindowHeight(void) const {return windowHeight;}
	void SetWindowWidth(const int x) {windowWidth=x;}
	void SetWindowHeight(const int y) {windowHeight=y;}
	void GridOn() {gridOn=TRUE;}
	void GridOff() {gridOn=FALSE;}
    void FPSOn() {fpsOn=TRUE; fps=0;}
	void FPSOff() {fpsOn=FALSE;}
    void GetFPS(void);
	void autoScaleOn() {autoScale=TRUE;}
	void autoScaleOff() {autoScale=FALSE;}
	void SetTitle(const char *title);
	void SetXLabel(const char *label);
	void SetYLabel(const char *label);
	
	void UpdateYValues(const float *yArray);
	void Render(void);
	void FindMinMax(void);
//    GLvoid drawGLString(GLfloat x, GLfloat y, GLfloat rot, char *string);
    GLvoid drawGLString(GLfloat x, GLfloat y, size_t ystep, GLfloat rot, char strArray[]);
	GLvoid drawGLInfo(void);
    
private:
	void CalculateSpaces();
	bool gridOn;
	bool fpsOn;
    int fps;
    float delta_t;
    wxLongLong prev_time;
	bool autoScale;
	char title[100];
	char xLabel[100];
	char yLabel[100];
	GLfloat plotColor[4];
	float plotXMin, plotXMax, plotYMin, plotYMax;
	int xSpace, ySpace;					// non-plotting space- room for labels, etc.
	int xDivisions, yDivisions;			// number of divisions in grid
	int windowHeight, windowWidth;		// total window size in pixels
	int dataLength;
	
	float *yValues, *xValues;
};

#endif
	
