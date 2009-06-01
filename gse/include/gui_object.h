#ifndef GUI_OBJECT_H_
#define GUI_OBJECT_H_

#include "gui.h"

/*----------------------------------------------------------------------------------------------*/
class GUI_Object
{

	protected:

		Message_Struct *p;
		class GUI_Serial *pSerial;
		class GUI_Toplevel *pToplevel;

	public:

		GUI_Object(){};
		~GUI_Object(){};

		void setSerial(GUI_Serial *_p){pSerial = _p;};
		void setToplevel(GUI_Toplevel *_p){pToplevel = _p;};
		void setPointer(Message_Struct *_p){p = _p;};
};
/*----------------------------------------------------------------------------------------------*/

#endif
