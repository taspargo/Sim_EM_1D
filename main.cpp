// NOTE: To run, it is recommended not to be in Compiz or Beryl, they have shown some instability.

#include <wx/wx.h>
#include <wx/glcanvas.h>
#include "main.h"

// include OpenGL
#ifdef __WXMAC__
#include "OpenGL/glu.h"
#include "OpenGL/gl.h"
#else
#include <GL/glu.h>
#include <GL/gl.h>
#endif

#define EM_SIZE 200

MyApp::MyApp()
{}

bool MyApp::OnInit()
{
    render_loop_on = false;
    
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    frame = new MyFrame();
    
    drawPane = new DrawPane( frame );
    sizer->Add(drawPane, 1, wxEXPAND);
    
    frame->SetSizer(sizer);
    frame->Show();
    
    theEMWorld = new EM_Eqn_1D(EM_SIZE);
    
    xArray = new float(EM_SIZE);
    yArray = new float(EM_SIZE);
    theGraph = new CPlot(EM_SIZE, xArray, yArray);

    activateRenderLoop(true);
    return true;
}

void MyApp::activateRenderLoop(bool on)
{
    if(on && !render_loop_on)
    {
        Connect( wxID_ANY, wxEVT_IDLE, wxIdleEventHandler(MyApp::onIdle) );
        render_loop_on = true;
    }
    else if(!on && render_loop_on)
    {
        Disconnect( wxEVT_IDLE, wxIdleEventHandler(MyApp::onIdle) );
        render_loop_on = false;
    }
}
void MyApp::onIdle(wxIdleEvent& evt)
{
    if(render_loop_on)
    {
        drawPane->paintNow();
        evt.RequestMore(); // render continuously, not only once on idle
        theEMWorld->TimeStep();
    }
}

/*
BEGIN_EVENT_TABLE(BasicGLPane, wxGLCanvas)
EVT_MOTION(BasicGLPane::mouseMoved)
EVT_LEFT_DOWN(BasicGLPane::mouseDown)
EVT_LEFT_UP(BasicGLPane::mouseReleased)
EVT_RIGHT_DOWN(BasicGLPane::rightClick)
EVT_LEAVE_WINDOW(BasicGLPane::mouseLeftWindow)
EVT_SIZE(BasicGLPane::resized)
EVT_KEY_DOWN(BasicGLPane::keyPressed)
EVT_KEY_UP(BasicGLPane::keyReleased)
EVT_MOUSEWHEEL(BasicGLPane::mouseWheelMoved)
EVT_PAINT(BasicGLPane::render)
END_EVENT_TABLE()
 
// some useful events to use
void BasicGLPane::mouseMoved(wxMouseEvent& event) {}
void BasicGLPane::mouseDown(wxMouseEvent& event) {}
void BasicGLPane::mouseWheelMoved(wxMouseEvent& event) {}
void BasicGLPane::mouseReleased(wxMouseEvent& event) {}
void BasicGLPane::rightClick(wxMouseEvent& event) {}
void BasicGLPane::mouseLeftWindow(wxMouseEvent& event) {}
void BasicGLPane::keyPressed(wxKeyEvent& event) {}
void BasicGLPane::keyReleased(wxKeyEvent& event) {}
*/

BEGIN_EVENT_TABLE(DrawPane, wxPanel)
EVT_PAINT(DrawPane::paintEvent)
END_EVENT_TABLE()

DrawPane::DrawPane(wxFrame* parent) :
wxPanel(parent)
{
}

void DrawPane::paintEvent(wxPaintEvent& evt)
{
    wxPaintDC dc(this);
    render(dc);
    SwapBuffers();
}

void DrawPane::paintNow()
{
    wxClientDC dc(this);
    render(dc);
}

void DrawPane::render( wxDC& dc )
{
    
    dc.SetBackground( *wxBLACK_BRUSH );
    dc.SetPen(*wxCYAN_PEN);
    dc.Clear();
    dc.DrawCircle(wxPoint(40, 20), wxCoord(10));
}

MyFrame::MyFrame() :
wxFrame((wxFrame *)NULL, -1,  wxT("Hello wxDC"), wxPoint(50,50), wxSize(400,200))
{
    width = 400;
    height = 200;
}

void MyFrame::resized(wxSizeEvent &event) {
    GetClientSize(width, height);
    glViewport(0, 0, *width, *height);
    glWindow->SetSize(*width, *height);
    glWindow->aspect= *width / *height;
    Refresh();
}

void MyFrame::onClose(wxCloseEvent& evt)
{
    wxGetApp().activateRenderLoop(false);
    evt.Skip(); // don't stop event, we still want window to close
}
