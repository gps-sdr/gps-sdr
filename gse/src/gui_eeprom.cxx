/*
 * gui_toplevel.cpp
 *
 *  Created on: Nov 4, 2008
 *      Author: gheckler
 */

#include "gui.h"

void endian_swap(void *_b, int32 _bytes, int32 _flag);

DECLARE_APP(GUI_App)

/*----------------------------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(GUI_EEPROM, wxFrame)
    EVT_CLOSE(GUI_EEPROM::onClose)
    EVT_BUTTON(ID_EEPROM_START, GUI_EEPROM::onStart)
    EVT_BUTTON(ID_EEPROM_STOP, GUI_EEPROM::onStop)
END_EVENT_TABLE()
/*----------------------------------------------------------------------------------------------*/

GUI_EEPROM::GUI_EEPROM():iGUI_EEPROM(NULL, wxID_ANY, wxT("EEPROM"), wxDefaultPosition, wxSize(800,600), wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL)
{

	execution_tic = 0;
	active 	= false;
	bank 	= 0;
	type 	= 0;
	bwrote 	= 0;
	b2write = 0;
	fsize 	= 0;
	offset 	= 0;
	mode	= 0;
	fp		= NULL;

}

GUI_EEPROM::~GUI_EEPROM()
{


}

void GUI_EEPROM::onClose(wxCloseEvent& evt)
{
	/* Prevent window from closing if active */

	wxMutexLocker lock(mutex);
	if(active == false)
	{
		wxCommandEvent cevt;
		evt.Veto();
		pToplevel->onEEPROM(cevt);
	}

}

void GUI_EEPROM::paintNow()
{
    wxClientDC dc(this);
    render(dc);
}

void GUI_EEPROM::render(wxDC& dc)
{
	execution_tic++;
	writeEEPROM();
}

void GUI_EEPROM::onStop(wxCommandEvent& event)
{
	wxString str;

	wxMutexLocker lock(mutex);
	active = false;
	resetState();

	str = wxT("Stopped EEPROM load\n");
	tFeedback->AppendText(str);
}

void GUI_EEPROM::onStart(wxCommandEvent& event)
{
	wxString str, fname;
	wxString bankstr[2];
	wxString typestr[3];
	uint32 mlen[3];

	wxMutexLocker lock(mutex);

	bankstr[0] = wxT("Bank A");
	bankstr[1] = wxT("Bank B");

	typestr[0] = wxT("Application Code");
	typestr[1] = wxT("Checksum Block");
	typestr[2] = wxT("Configuration Table");

	mlen[0] = EEPROM_APPLICATION_CODE_LENGTH;
	mlen[1] = CHECKSUM_LENGTH;
	mlen[2] = CONFIG_TABLE_LENGTH;

	tFeedback->Clear();

	/* Burn/dump/verify */
	mode = mBurn->GetCurrentSelection();

	/* First check to see if file exists */
	bank = mBank->GetCurrentSelection();

	/* Get type (app code, checksum, config table */
	type = mType->GetCurrentSelection();

	fname = mEEPROMFile->GetPath();

	str = bankstr[bank];
	str += wxT(", ");
	str += typestr[type];
	str += wxT("\n");

	str += wxT("File: ");
	str += fname;
	str += wxT("\n");

	tFeedback->AppendText(str);

	/* Offset */
	switch(type)
	{
		case 0: offset = 0; break;
		case 1: offset = EEPROM_APPLICATION_CODE_LENGTH >> 2; break;
		case 2: offset = (EEPROM_APPLICATION_CODE_LENGTH + CHECKSUM_LENGTH) >> 2; break;
		default: offset = 0; break;
	}

	/* Open the file */
	fp = fopen(fname.mb_str(),"rb");
	if(fp == NULL)
	{
		str = wxT("Could not open file!\n");
		tFeedback->AppendText(str);
		resetState();
		return;
	}

	/* File length */
    fseek(fp, 0, SEEK_END);
    fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    str.Printf(wxT("Filesize is %d bytes\n"),fsize); tFeedback->AppendText(str);

    if(fsize > mlen[type])
    {
		str.Printf(wxT("File is too big for region size of %d bytes\n"),mlen[type]); tFeedback->AppendText(str);
		resetState();
		return;
    }

    if(fsize < 0)
    {
		str = wxT("File is too small\n"); tFeedback->AppendText(str);
		resetState();
		return;
    }

	pSerial->Lock();
	count = pSerial->GetCommandTic()+666;
	pSerial->Unlock();

    //b2write = fsize;
    b2write = EEPROM_LENGTH/16;
    bwrote  = 0;
    active = true;


}

void GUI_EEPROM::writeEEPROM()
{

	wxString str;
	uint32 bread = 0;
	uint32 tcount;
	int32 lcv;

	wxMutexLocker lock(mutex);

	pSerial->Lock();
	tcount = pSerial->GetCommandTic();
	pSerial->Unlock();

	/* Setup the command, poll for ack of previous command */
	if(active == true && (tcount != count))
	{

		memset(&setcmd.payload[0], 0x0, 64 << 2);
		setcmd.command_id = SET_EEPROM_C_ID;
		setcmd.bank = bank;
		setcmd.offset = offset + (bwrote >> 2);
		setcmd.dwords = 64;
		setcmd.checksum = 0;

		count = tcount;

		/* Clear EEPROM */
		memset(&setcmd.payload[0], 0x55, 256);
		pSerial->formCommand(SET_EEPROM_C_ID, &setcmd);

		/* Get the ack command */

		/* Read 256 bytes at a time */
//		if(fp != NULL)
//		{
//			if(b2write > 256)
//				bread = fread(&setcmd.payload[0], 1, 256, fp);
//			else
//				bread = fread(&setcmd.payload[0], 1, b2write, fp);
//		}
//
//		b2write -= bread;
//		bwrote += bread;
//
		b2write -= 256;
		bwrote += 256;

//		tFeedback->AppendText(str);
//		endian_swap(&setcmd.payload[0], 256, 0);

		if(ackmsg.command_status == SUCCESS_A_ID)
		{
			str.Printf(wxT("Wrote %8d of %8d bytes at 0x%08X, %7.2f %% complete\n"),bwrote,fsize,offset+bwrote,100.0*(float)bwrote/(float)fsize); tFeedback->AppendText(str);
		}
		else
		{
			str.Printf(wxT("Wrote %8d of %8d bytes at 0x%08X, %7.2f %% failed\n"),bwrote,fsize,offset+bwrote,100.0*(float)bwrote/(float)fsize); tFeedback->AppendText(str);
		}

		//pSerial->formCommand(SET_EEPROM_C_ID, &setcmd);

		if(b2write == 0)
		{
			active = false;
			resetState();
		}

	}

}


void GUI_EEPROM::readEEPROM()
{

	wxString str;
	uint32 bread = 0;
	int32 lcv;

	wxMutexLocker lock(mutex);

	/* Setup the command */
	if(active == true)
	{

		/* Send command to get the data */
		//pSerial->formCommand(SET_EEPROM_C_ID, &cmd);


		/* Copy resulting packet to local space */



		/* Write payload to the file */


		//str.Printf(wxT("Wrote %8d of %8d bytes at 0x%08X, %7.2f %% complete\n"),bwrote,fsize,offset+bwrote,100.0*(float)bwrote/(float)fsize); tFeedback->AppendText(str);
		//pSerial->formCommand(SET_EEPROM_C_ID, &cmd);
	}

}


void GUI_EEPROM::resetState()
{

	active 	= false;
	bank 	= 0;
	type 	= 0;
	bwrote 	= 0;
	b2write = 0;
	fsize 	= 0;
	offset 	= 0;
	mode	= 0;
	count	= 0;

	if(fp != NULL)
		fclose(fp);

	fp = NULL;

}

