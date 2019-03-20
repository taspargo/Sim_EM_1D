//
//  EM_Sim.hpp
//  wx_EM_Sim_1D
//
//  Created by Thomas Spargo on 1/29/19.
//  Copyright © 2019 Thomas Spargo. All rights reserved.
//

#ifndef EM_Sim_h
#define EM_Sim_h

#include <stdio.h>
#include "CPlot.h"
#include "EM_equations.h"
//#include "BasicGLPane.h"

class EM_Sim: public wxFrame
{
public:
    EM_Sim(const wxString &title, wxSize sz);
    void onIdle(wxIdleEvent &evt);

    EM_Eqn_1D   *theEMWorld;
    CPlot   *thePlot;

    int paused;
    void activateRenderLoop(bool on);
};
#endif /* EM_Sim_h */
