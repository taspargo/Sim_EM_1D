/*
 *  EM equations.h
 *  1D EM Sim using CPlot
 *
 *  Created by Tom on 12/11/04.
 *  Copyright 2004 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _EM_EQUATIONS_H
#define _EM_EQUATIONS_H

#include "CPlot.h"

//#include <Carbon/Carbon.h>

class EM_Eqn_1D
{
public:
    // Was Init_EM
    EM_Eqn_1D(int);
    ~EM_Eqn_1D();
    
    void TimeStep(void);
    void CleaupEM(void);
    int GetKE() {return this->ke;}
    float *GetDX() {return this->dx;}
    float *GetEX() {return this->ex;}
    float *GetHY() {return this->hy;}
    
    //CPlot *exPlot(int);
    
private:
    int     ke;
    float   *dx, *ex, *hy, *ix;
    float   *ga, *gb;
    int     kc, kstart, nsteps;
    float   ddx, dt, T, eTime, epsz, epsilon, sigma;
    float   t0, spread, pulse;
    
    float   ex_low_m1, ex_low_m2, ex_high_m1, ex_high_m2;
};

#endif
