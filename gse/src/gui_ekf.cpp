/*
 * gui_Channel.cpp
 *
 *  Created on: Nov 4, 2008
 *      Author: gheckler
 */

#include "gui.h"

DECLARE_APP(GUI_App)

/*----------------------------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(GUI_EKF, wxFrame)
    EVT_CLOSE(GUI_EKF::onClose)
END_EVENT_TABLE()
/*----------------------------------------------------------------------------------------------*/

GUI_EKF::GUI_EKF():iGUI_EKF(NULL, wxID_ANY, wxT("EKF"), wxDefaultPosition, wxSize(1000,600), wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL)
{


}

GUI_EKF::~GUI_EKF()
{


}

void GUI_EKF::onClose(wxCloseEvent& evt)
{
	wxCommandEvent cevt;
	evt.Veto();
	pToplevel->onEKF(cevt);
}

void GUI_EKF::paintNow()
{

    wxClientDC dc(this);
    render(dc);

}

void GUI_EKF::render(wxDC& dc)
{
	renderEKF();
	renderResidual();
}

enum EKF_STATE_CODES
{
	EKF_STATE_INITIALIZED,
	EKF_STATE_CONVERGED,
	EKF_STATE_UPDATE_OCCURED,
	EKF_STATE_POSITION_SIGMA_ERR,
	EKF_STATE_VELOCITY_SIGMA_ERR,
	EKF_STATE_COVARIANCE_OVERFLOW_ERR
};

enum EKF_MEASUREMENT_CODES
{
	EKF_MEAS_HORP_ERR,
	EKF_MEAS_PSEUDO_ERR,
	EKF_MEAS_DOPPLER_ERR,
	EKF_MEAS_MANEUVER_ERR
};


void GUI_EKF::renderEKF()
{

	int32 lcv;
	int32 nsvs;
	wxDateTime theTime;
	wxString str, str2;
	time_t utcsec;

	EKF_State_M *s =  &p->ekf_state;
	EKF_Residual_M *r = &p->ekf_residual;
	EKF_Covariance_M *c = &p->ekf_covariance;

	double vel;
	vel = sqrt(s->vz*s->vz + s->vy*s->vy + s->vx*s->vx);

	nsvs = 0;
	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		if((s->nsvs >> lcv) & 0x1)
			nsvs++;
	}

	/* The main filter states */
//	double x;			//!< ECEF x coordinate (meters)
//	double y;			//!< ECEF y coordinate (meters)
//	double z;			//!< ECEF z coordinate (meters)
//	double vx;			//!< ECEF x velocity (meters/sec)
//	double vy;			//!< ECEF x velocity (meters/sec)
//	double vz;			//!< ECEF x velocity (meters/sec)
//	double solar;		//!< Solar radiation pressure
//	double drag;		//!< Atmospheric drag
//	double clock_bias;	//!< clock bias in seconds
//	double clock_rate;  //!< clock rate in meters/second
//
//	/* These are tags, not covariances */
//	double time;		//!< Time in seconds (GPS)
//	uint32 week;		//!< Week (GPS)
//	uint32 status;		//!< Has this state failed any of the GEONS error checking, convergence flag, etc
//	uint32 tic;			//!< This information is associated with the given RECEIVER tic

	str.Printf(wxT("%15d"),s->tic);						rticks->SetLabel(str);
	str.Printf(wxT("%15d"),s->geons_ticks);				sticks->SetLabel(str);
	str.Printf(wxT("%15d"),s->status >> 16);			nsv->SetLabel(str);
	str.Printf(wxT("%15.2f m"),s->x);					px->SetLabel(str);
	str.Printf(wxT("%15.2f m"),s->y);					py->SetLabel(str);
	str.Printf(wxT("%15.2f m"),s->z);					pz->SetLabel(str);
	str.Printf(wxT("%.2f cm/s"),100.0*s->vx);			vx->SetLabel(str);
	str.Printf(wxT("%.2f cm/s"),100.0*s->vy);			vy->SetLabel(str);
	str.Printf(wxT("%.2f cm/s"),100.0*s->vz);			vz->SetLabel(str);
	str.Printf(wxT("%.2f m/s"),vel);					speed->SetLabel(str);
	str.Printf(wxT("%.2f"),s->solar);					srp->SetLabel(str);
	str.Printf(wxT("%.2f"),s->drag);					drag->SetLabel(str);
	str.Printf(wxT("%.9f"),s->clock_bias/SPEED_OF_LIGHT);	cb->SetLabel(str);
	str.Printf(wxT("%.2f"),s->clock_rate);					cr->SetLabel(str);
	str.Printf(wxT("%.6f"),s->time);						gpss->SetLabel(str);
	str.Printf(wxT("%8u"),s->week);							gpsw->SetLabel(str);
	str.Printf(wxT("%.2f"),(float)s->period * 500e-6);		period->SetLabel(str);

	str.Printf(wxT("%15.2f m"),c->x);					cpx->SetLabel(str);
	str.Printf(wxT("%15.2f m"),c->y);					cpy->SetLabel(str);
	str.Printf(wxT("%15.2f m"),c->z);					cpz->SetLabel(str);
	str.Printf(wxT("%.2f cm/s"),100.0*c->vx);			cvx->SetLabel(str);
	str.Printf(wxT("%.2f cm/s"),100.0*c->vy);			cvy->SetLabel(str);
	str.Printf(wxT("%.2f cm/s"),100.0*c->vz);			cvz->SetLabel(str);
	str.Printf(wxT("%.2f"),c->solar);					csrp->SetLabel(str);
	str.Printf(wxT("%.2f"),c->drag);					cdrag->SetLabel(str);
	str.Printf(wxT("%.2f m"),c->clock_bias);				ccb->SetLabel(str);
	str.Printf(wxT("%.2f cm/s"),100.0*c->clock_rate);		ccr->SetLabel(str);

	str.Clear();
	if(s->status >> EKF_STATE_INITIALIZED & 0x1)
		str += wxT("Initialized");
	if(s->status >> EKF_STATE_CONVERGED & 0x1)
		str += wxT(", Converged");

	status->SetLabel(str);

	str.Clear();
	if(s->status >> EKF_STATE_UPDATE_OCCURED & 0x1)
		str += wxT("Nominal");
	if(s->status >> EKF_STATE_POSITION_SIGMA_ERR & 0x1)
		str += wxT(" Pos Sig");
	if(s->status >> EKF_STATE_VELOCITY_SIGMA_ERR & 0x1)
		str += wxT(" Vel Sig");
	if(s->status >> EKF_STATE_COVARIANCE_OVERFLOW_ERR & 0x1)
		str += wxT(" Cov Ovrflw");

	err->SetLabel(str);

	/* Get into unix time */
	utcsec = floor(s->time);
	utcsec += (s->week + 1024*CURRENT_GPS_WEEK_ROLLOVER)*SECONDS_IN_WEEK;
	//utcsec += (s->day)*SECONDS_IN_DAY;
	utcsec -= 10; //Difference of TAI and UTC
	utcsec += 315964819; //!< 0 Unix time --> GPS time offset

	theTime.Set(utcsec);
	str = theTime.FormatISODate();
	str += wxT('\t');
	str += theTime.FormatISOTime();
	str2.Printf(wxT("%15.6f"),fmod(s->time,1.0));
	str += str2;
	utct->SetLabel(str);

}

void GUI_EKF::renderResidual()
{

	wxTextAttr text;
    wxString str, str2;
	int32 lcv, start, stop, lines;
	int32 x, y;

	EKF_State_M *s =  &p->ekf_state;
	EKF_Residual_M *r = &p->ekf_residual;
	EKF_Covariance_M *c = &p->ekf_covariance;

	tResidual->Clear();

	str =      wxT("Ch#  SV          Residual                           Errors\n\n");
	tResidual->AppendText(str);

	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		if((s->nsvs >> lcv) & 0x1)
		{
			str.Printf(wxT("%02d   %2d  %16e"),lcv,r->sv[lcv],r->pseudorange_residuals[lcv]);

			if(r->status[lcv] >> EKF_MEAS_HORP_ERR & 0x1)
				str += wxT(" HORP");
			else
				str += wxT("     ");

			if(r->status[lcv] >> EKF_MEAS_PSEUDO_ERR & 0x1)
				str += wxT(" Pseudo");
			else
				str += wxT("       ");

			if(r->status[lcv] >> EKF_MEAS_MANEUVER_ERR & 0x1)
				str += wxT(" Maneuver   \n");
			else
				str += wxT("            \n");
		}
		else
		{
			str =      wxT("                                                          \n");
		}

		tResidual->AppendText(str);
	}

	text = tResidual->GetDefaultStyle();
	lines = tResidual->GetNumberOfLines();
	start = 0; stop = 0;
	for(lcv = 0; lcv < lines; lcv++)
	{
		stop += tResidual->GetLineLength(lcv)+1;

		if((lcv+1) & 0x1)
		{
			text.SetBackgroundColour(wxColor(248,248,255));
			tResidual->SetStyle(start, stop, text);
		}
		else
		{
			text.SetBackgroundColour(wxColor(255,255,255));
			tResidual->SetStyle(start, stop, text);
		}

		start = stop+1;
	}


}

