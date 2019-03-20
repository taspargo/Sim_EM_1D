//
//  EM_Sim.cpp
//  wx_EM_Sim_1D
//
//  Created by Thomas Spargo on 1/29/19.
//  Copyright © 2019 Thomas Spargo. All rights reserved.
//

#include "EM_Sim.h"

#define EM_SIZE 200

EM_Sim::EM_Sim(const wxString &title, wxSize sz)
: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, sz)
{
    wxStatusBar *sb = CreateStatusBar();
    sb->SetStatusText(wxT("0"));
    
    theEMWorld = new EM_Eqn_1D(EM_SIZE);
    thePlot = new CPlot(EM_SIZE, NULL, theEMWorld->GetEX());
    paused = 0;
}

void EM_Sim::onIdle(wxIdleEvent &evt)
{
    if (!paused)
    {
        glPane->paintNow();
        evt.RequestMore();
    }
}
