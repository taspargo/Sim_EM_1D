/*
 *  EM equations.cpp
 *  1D EM Sim using CPlot
 *
 *  Created by Tom on 12/11/04.
 *  Copyright 2004 __MyCompanyName__. All rights reserved.
 *
 * From "EM Simulation Using The FDTD Method"
 * Program FD1D_2.2.c, p. 39
 * 1D simulation with Fourier Transform
 *
 */

#include "EM_equations.h"

// Was InitEM
// Now the ctor for EM_Eqn1D
EM_Eqn_1D::EM_Eqn_1D(int ke)
{
int k;

    this->ke = ke;
	printf("EM_Eqn1D:: Start EM_Eqn1D ctor\n");
	dx = new float[ke];
	ex = new float[ke];
	hy = new float[ke];
	ix = new float[ke];
	ga = new float[ke];
	gb = new float[ke];
    
	kc = ke/2;
	epsz = 8.854e-12;
	ddx = 0.01;
	dt = ddx / 6e8;
    printf("EM_Eqn1D:: Cell size = %6.4f  Time step = %6.4fns\n", ddx, dt*1e9);

	for (k = 0; k < GetKE(); k++)
	{
		ga[k] = 1.0;
		gb[k] = 0.0;
		dx[k] = 0.0;
		ex[k] = 0.0;
		hy[k] = 0.0;
		ix[k] = 0.0;
	}

	ex_low_m1 = 0.0;
	ex_low_m2 = 0.0;
	ex_high_m1 = 0.0;
	ex_high_m2 = 0.0;

	kstart = 100;
	epsilon = 4.0;
	sigma = 1e-7;
	
	for (k = kstart; k <= GetKE(); k++)
	{
		ga[k] = 1.0 / (epsilon + sigma*dt/epsz);
		gb[k] = sigma*dt/epsz;
	}
	
	printf("EM_Eqn1D:: ga, gb arrays\n");
	for (k = 1; k <= GetKE(); k++)
	{
		printf("     %2d  %6.2f  %6.4f\n", k, ga[k], gb[k]);
	}
	
	t0 = 50;
	spread = 10.0;
	
	T = 0;
	eTime = 0.0;
	nsteps = 10;
	
	printf("EM_Eqn1D:: End EM_Eqn1D ctor\n");
}


void EM_Eqn_1D::TimeStep(void)
{
int k;
float input;

	this->T++;
	this->eTime += this->dt;
	printf("EM_Eqn1D::TimeStep Step %5.0f, Time = %8.4fnS\n", this->T, this->eTime*1e9);
	
	// Calculate the dx field
	for (k=1; k<this->ke; k++) dx[k] += 0.5 * (hy[k-1] - hy[k]);
	
	// Soft Gaussian source
//	input = exp(-0.5* pow((t0-T)/spread, 2.0));

	// Sine wave
	input = sin(2*M_PI*1e9*eTime);
	dx[5] += input;
	
	// Calculate the Ex field
	for (k=0; k<this->ke-1; k++)
	{
		ex[k] = ga[k] * (dx[k]-ix[k]);
		ix[k] += gb[k] * ex[k];
	}

	// Boundary conditions
	ex[0] = ex_low_m2;
	ex_low_m2 = ex_low_m1;
	ex_low_m1 = ex[1];
	
	ex[this->ke-1] = ex_high_m2;
	ex_high_m2 = ex_high_m1;
	ex_high_m1 = ex[this->ke-2];
	
	// Calculate the Hy field
	for (k=0; k<this->GetKE()-1; k++) hy[k] += 0.5*(ex[k]-ex[k+1]);
}

EM_Eqn_1D::~EM_Eqn_1D(void)
{
	delete this->dx;
	delete this->ex;
	delete this->hy;
	delete this->ix;
	delete this->ga;
	delete this->gb;
}
