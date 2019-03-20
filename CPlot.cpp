/*
 *  Cplot.cpp
 *  Basic 2D EM
 *
 *  Created by Tom on Sat Dec 11 2004.
 *  Copyright (c) 2004 __MyCompanyName__. All rights reserved.
 *
 *		An attempt to emulate the easy to use Matlab 'plot' command.
 *		Should support both static and animated plots.
 */

//#include <Carbon/Carbon.h>
#include <OpenGL/glu.h>
#include <wx/time.h>
//#include <GLUT/glut.h>
#include <string.h>
#include <stdio.h>
#include "CPlot.h"

extern bool InitGlut(CPlot *plot);

GLfloat mat_white[4] = {1.0, 1.0, 1.0, 1.0};
GLfloat mat_black[4] = {0.0, 0.0, 0.0, 1.0};
GLfloat mat_yellow[] = {0.8, 0.8, 0.0, 1.0};
GLfloat mat_red[] = {1.0, 0.0, 0.0, 1.0};
GLfloat mat_blue[] = {0.2, 0.5, 1.0, 0.2};
GLfloat mat_green[] = {0.3, 1.0, 0.1, 1.0};
GLfloat mat_brown[] = {0.5, 0.3, 0.3, 1.0};
GLfloat mat_grey[] = {0.3, 0.5, 0.4, 1.0};

extern GLvoid RenderGLScene(GLvoid);
extern GLvoid Mouse(int button, int state, int x, int y);
extern GLvoid MouseMoved(int x, int y);
extern GLvoid Keyboard(unsigned char key, int x, int y);
extern GLvoid ReSizeGLScene(int Width, int Height);
extern void Idle(void);


CPlot::CPlot(const int length, const float *xArray, const float *yArray,
					const int width, const int height)
{
int i;

	if (yArray==NULL)
	{
		printf("CPlot ctor called with NULL yArray\n");
		exit(1);
	}
	GridOn();
	FPSOn();
    prev_time = 0;
	autoScaleOn();
	dataLength=length;
	windowHeight = height;
	windowWidth = width;
	SetTitle("X-Y Plot");
	SetXLabel("X");
	SetYLabel("Y");
	SetColor(1.0, 1.0, 1.0);
	
	printf("CPlot: creating value arrays\n");
	xValues = new float[dataLength];
	yValues = new float[dataLength];
	
	printf("CPlot: copying arrays; finding min's and max's\n");
	for (i=0; i<dataLength; i++)
	{
		if (xArray==NULL) xValues[i]=i;
		else xValues[i]=xArray[i];
		yValues[i]=yArray[i];
	}

	FindMinMax();
	
    /*  No init seems to be needed.
	if (!InitGlut(this))
	{
		printf("InitGlut failed!\n");
		exit(1);
	}
     */

	CalculateSpaces();
	xDivisions = yDivisions = 4;
}


CPlot::CPlot(const int length, const float *xValues, const float *yValues)
{
	CPlot(length, xValues, yValues, 600, 400);
}


CPlot::CPlot(const int length, const float *yValues, const int width, const int height)
{
	CPlot(length, NULL, yValues, width, height);
}


CPlot::CPlot(const int length, const float *yValues)
{
	CPlot(length, NULL, yValues, 600, 400);
}


CPlot::~CPlot()
{
	delete xValues;
	delete yValues;
}


void CPlot::SetColor(const float rc, const float gc, const float bc)
{
	plotColor[0]=rc;
	plotColor[1]=gc;
	plotColor[2]=bc;
	plotColor[3]=1.0;
}
	

void CPlot::SetColor(const float rc, const float gc, const float bc, const float ac)
{
	plotColor[0]=rc;
	plotColor[1]=gc;
	plotColor[2]=bc;
	plotColor[3]=ac;
}


void CPlot::SetColor(const float c[])
{
	plotColor[0]=c[0];
	plotColor[1]=c[1];
	plotColor[2]=c[2];
	plotColor[3]=c[3];
}


void CPlot::SetAxis(const float xmin, const float xmax, const float ymin, const float ymax)
{
	plotXMin=xmin;
	plotXMax=xmax;
	plotYMin=ymin;
	plotYMax=ymax;
	autoScaleOff();
}


void CPlot::SetTitle(const char *t)
{
	strcpy(title, t);
}


void CPlot::SetXLabel(const char *x)
{
	strcpy(xLabel, x);
}


void CPlot::SetYLabel(const char *y)
{
	strcpy(yLabel, y);
}

// Change data in plot
void CPlot::UpdateYValues(const float *yArray)
{
int i;

	for (i=0; i<dataLength; i++) yValues[i]=yArray[i];
	if (autoScale) FindMinMax();
}


void CPlot::Render(void)
{
int i;
float f, xDelta, yDelta;
GLint vp[4];


	glClear(GL_COLOR_BUFFER_BIT);
	if (fpsOn)
	{
		glLoadIdentity();
		GetFPS();
		drawGLInfo();
	}

	glGetIntegerv(GL_VIEWPORT, vp);
	
	glViewport(YAXISMARGIN+YTITLEMARGIN, XAXISMARGIN+XTITLEMARGIN + fpsOn*FPSMARGIN,
						windowWidth - xSpace, windowHeight - ySpace);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(plotXMin, plotXMax, plotYMin, plotYMax, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	// Draw frame
	glLoadIdentity();
	glColor3fv(mat_white);
	glBegin(GL_LINE_LOOP);
		glVertex2f(plotXMin, plotYMin);
		glVertex2f(plotXMin, plotYMax);
		glVertex2f(plotXMax, plotYMax);
		glVertex2f(plotXMax, plotYMin);
	glEnd();

	// Draw gridlines
	if (gridOn)
	{
		glColor3fv(mat_brown);
		xDelta = (plotXMax - plotXMin) / (float) xDivisions;
		yDelta = (plotYMax - plotYMin) / (float) yDivisions;
		glBegin(GL_LINES);
			for (f=1; f < xDivisions; f++)
			{
				glVertex2f(plotXMin+f*xDelta, plotYMin);
				glVertex2f(plotXMin+f*xDelta, plotYMax);
			}
			
			for (f=1; f < yDivisions; f++)
			{
				glVertex2f(plotXMin, plotYMin + f*yDelta);
				glVertex2f(plotXMax, plotYMin + f*yDelta);
			}
		glEnd();
	}
	
	// Plot x=0 and y=0 axis if they are on screen
	glColor3fv(mat_blue);
	if (plotXMin < 0 && plotXMax > 0)
	{
		glBegin(GL_LINES);
		glVertex2f(0.0, plotYMin);
		glVertex2f(0.0, plotYMax);
		glEnd();
	}
	
	if (plotYMin < 0 && plotYMax > 0)
	{
		glBegin(GL_LINES);
		glVertex2f(plotXMin, 0.0);
		glVertex2f(plotXMax, 0.0);
		glEnd();
	}

	// Plot the actual data
	glColor3fv(plotColor);
	glBegin(GL_LINE_STRIP);
		for (i=0; i<dataLength; i++)
		{
			glVertex2f(xValues[i], yValues[i]);
		}
	glEnd();

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glViewport(vp[0], vp[1], vp[2], vp[3]);
	
}
	

// Total margins along x and y axis for labels, titles, etc.
void CPlot::CalculateSpaces()
{
	xSpace = YAXISMARGIN + YTITLEMARGIN + RIGHTMARGIN;
	ySpace = XAXISMARGIN + XTITLEMARGIN + TITLEMARGIN;
	if (fpsOn) ySpace += FPSMARGIN;
}


void CPlot::FindMinMax(void)
{
int i;
float temp;

	plotXMin = plotYMin = 1e38;
	plotXMax = plotYMax = -1e38;
	for (i=0; i<dataLength; i++)
	{
		temp = xValues[i];
		if (temp > plotXMax) plotXMax=temp;
		if (temp < plotXMin) plotXMin=temp;
		temp = yValues[i];
		if (temp > plotYMax) plotYMax=temp;
		if (temp < plotYMin) plotYMin=temp;
	}
	
	if (plotYMin == 0.0 && plotYMax == 0.0) plotYMax = 1.0;
	printf("plotXMin = %f  plotXMax = %f\n", plotXMin, plotXMax);
	printf("plotYMin = %f  plotYMax = %f\n", plotYMin, plotYMax);
}

#define TS_MAX_LENGTH 100

//GLvoid CPlot::drawGLString(GLfloat x, GLfloat y, , GLfloat rot, char *string)
GLvoid CPlot::drawGLString(GLfloat x, GLfloat y, size_t ystep, GLfloat rot, char charArray[])
{
    size_t inx=0, len=0, offset=0, count=0;
    char aStr[TS_MAX_LENGTH];
    int z;
    float ycoor=y;
  //int len, i
/*
	glTranslatef(x, y, 0);
	glRotatef(rot, 0.0, 0.0, 1.0);
	glScalef(0.1, 0.1, 1.0);
	len = (int) strlen(string);
	for (i = 0; i < len; i++) {
		glutStrokeCharacter(GLUT_STROKE_ROMAN, string[i]);
	}
*/
/*    wxGLStringArray my_messages();
    my_messages.addString("wxMac");
    my_messages.addString("wxGTK");
    my_messages.addString("wxMSW");
    ...
    
    glColor3f(0,0,0); // black text
    my_messages.bind();
    my_messages.get(0).render( x, y      );
    my_messages.get(1).render( x, y + 25 );
    my_messages.get(2).render( x, y + 50 );
    */
    wxGLStringArray mssgs;
    len = strlen(charArray);
    while (inx<len) {
        offset=0;
        while ( (offset < TS_MAX_LENGTH) && (charArray[inx] != '\n') && (inx < len) ) {
            aStr[offset] = charArray[inx];
            offset++;
            inx++;
        }
        if (offset >= TS_MAX_LENGTH) {
            throw "offset >= TS_MAX_LENGTH!";
        }
        else if (inx >= len) {
            throw "inx >= len!!";
        }
        else {
            aStr[offset]=0x0;
            mssgs.addString(aStr);
            inx++;
            count++;
        }
    }
    glColor3f(255, 255, 255);
    mssgs.bind();
    for (z=1; z <=count; z++) {
        mssgs.get(z).render(x, ycoor );
        ycoor += ystep;
    }
}

// Draw optional info at bottom left corner of plot
GLvoid CPlot::drawGLInfo(void)
{
	char outString [256] = "";
	GLint matrixMode;
	GLint vp[4];
	GLint lineSpacing = FPSMARGIN;
	GLint line = 0;
	GLint startOffset = 7;
	
	glGetIntegerv(GL_VIEWPORT, vp);
	glViewport(0, 0, windowWidth, windowHeight);
	
	glGetIntegerv(GL_MATRIX_MODE, &matrixMode);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, (GLfloat) windowWidth, 0.0, (GLfloat) windowHeight, -1.0, 1.0);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	glColor3f(0.0, 1.0, 1.0);
//	glScalef(0.1, 0.1, 1.0);

	sprintf (outString, "FPS: %d\n", fps);
	drawGLString (10, (lineSpacing * line++) + startOffset,0, 0.0, outString);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(matrixMode);
	
	glViewport(vp[0], vp[1], vp[2], vp[3]);
}

void CPlot::GetFPS(void)
{
    static int count=0;
    static long prev_sec;
    long curr_sec;
    
    count++;
    curr_sec = wxGetUTCTime();
    if (curr_sec!=prev_sec) {
        fps=count;
        count=0;
        prev_sec=curr_sec;
        printf("GetFPS: prev=%ld  curr=%ld  fps=%d\n", prev_sec, curr_sec, fps);
    }
}

// delta_t is the time between frame updates. 
float GetDeltaT(wxLongLong &prev_time)
{
    wxLongLong now;
    double temp;
    float dT;

    now = wxGetUTCTimeMillis();
    temp = ( ((now-prev_time).ToDouble()) / 1e3);
    dT = (float) temp;
    printf("delta-T = %8.3f %8.3f %8.3f\n", now.ToDouble(), prev_time.ToDouble(), dT);
    prev_time = now;
    return dT;
}



