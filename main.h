//
//  wcGLCanvas.h
//  wxOpenGL
//
//  Created by Thomas Spargo on 1/26/19.
//  Copyright © 2019 Thomas Spargo. All rights reserved.
//

#ifndef wcGLCanvas_h
#define wcGLCanvas_h

#include "wx/wx.h"
#include "wx/glcanvas.h"
#include "EM_equations.h"
#include "CPlot.h"

class DrawPane : public wxPanel
{
public:
    DrawPane(wxFrame* parent);
    
    void paintEvent(wxPaintEvent& evt);
    void paintNow();
    void render( wxDC& dc );
    
    wxDECLARE_EVENT_TABLE()
};

class MyFrame;

class MyApp: public wxApp
{
public:
    MyApp();
    bool OnInit();
    void onIdle(wxIdleEvent& evt);
    void activateRenderLoop(bool on);

    MyFrame     *frame;
    DrawPane    *drawPane;
    EM_Eqn_1D   *theEMWorld;
    CPlot       *theGraph;

    float   *xArray;
    float   *yArray;
    bool render_loop_on;
};

wxIMPLEMENT_APP(MyApp)
class MyFrame : public wxFrame
{
public:
    MyFrame();
    void onClose(wxCloseEvent& evt);
    void resized(wxSizeEvent &event);
    wxDECLARE_EVENT_TABLE()
    
    int width;
    int height;
};

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_SIZE(MyFrame::resized)
    EVT_CLOSE(MyFrame::onClose)
wxEND_EVENT_TABLE()

    // events
    /*
    void mouseMoved(wxMouseEvent& event);
    void mouseDown(wxMouseEvent& event);
    void mouseWheelMoved(wxMouseEvent& event);
    void mouseReleased(wxMouseEvent& event);
    void rightClick(wxMouseEvent& event);
    void mouseLeftWindow(wxMouseEvent& event);
    void keyPressed(wxKeyEvent& event);
    void keyReleased(wxKeyEvent& event);
    */

#endif /* wcGLCanvas_h */

