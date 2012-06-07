#ifndef gui_h
#define gui_h

#include <wx/wx.h>
#include <wx/glcanvas.h>
#include <wx/spinctrl.h>
#include <wx/textctrl.h>
#include "names.h"
#include "devices.h"
#include "monitor.h"
#include "parser.h"
#include "scanner.h"
#include "network.h"
#include <vector>

enum { 
  RUN_BUTTON_ID = wxID_HIGHEST + 1,
  CONTINUE_BUTTON_ID,
  SWITCH_BUTTON_ID,
  MONITOR_BUTTON_ID,
  MONITORADDDEV_CHOICE_ID,
  MONITORADD_BUTTON_ID,
  MONITORREMOVE_BUTTON_ID,
  SWITCH_CHOICE_ID,
  SWITCHON_BUTTON_ID,
  SWITCHOFF_BUTTON_ID,
  CONSOLESAVE_BUTTON_ID,
  CONSOLECANCEL_BUTTON_ID,
  LOAD_ID,
  CONSOLESELECT_ID,
  CONSOLECOPY_ID,
  CONSOLESAVE_ID,
  CONSOLECLEAR_ID,
  CONSOLEOPTIONS_ID,
}; // widget identifiers

class MyGLCanvas;
class SwitchPanel;
class MonitorPanel;
class ConsolePanel;

class MyFrame: public wxFrame
{
 public:
  MyFrame(wxWindow *parent, const wxString& title, const wxPoint& pos, const wxSize& size, 
	  names *names_mod = NULL, devices *devices_mod = NULL, monitor *monitor_mod = NULL, network *net_mod=NULL ,wxTextCtrl *console = NULL,
	  long style = wxDEFAULT_FRAME_STYLE); // constructor
  void reset(); //Resets the graphic area
  void consoleSettings(int height, int fontsize); //Sets the console settings
 private:
  MyGLCanvas *canvas;                     // GL drawing area widget to draw traces
  wxSpinCtrl *spin;                       // control widget to select the number of cycles
  wxTextCtrl *console;					  // The gui console
  names *nmz;                             // pointer to names class
  devices *dmz;                           // pointer to devices class
  monitor *mmz;							  // pointer to monitor class
  network *netz;						  // pointer to network class
  SwitchPanel *switches;				  // Switch Panel
  MonitorPanel *monitors;				  // Monitor Panel
  ConsolePanel *consolePanel;             // Console Option Panel
  wxLocale locale;
  int cyclescompleted;                    // how many simulation cycles have been completed
  void runnetwork(int ncycles);           // function to run the logic network
  void OnExit(wxCommandEvent& event);     // callback for exit menu item
  void OnAbout(wxCommandEvent& event);    // callback for about menu item
  void OnLoad(wxCommandEvent& event);	  // callback for file parser
  void OnConsole(wxCommandEvent& event);  // callback for console options
  void OnSelect(wxCommandEvent& event); 
  void OnCopy(wxCommandEvent& event); 
  void OnSave(wxCommandEvent& event); 
  void OnClear(wxCommandEvent& event); 
  void OnRun(wxCommandEvent& event);   // callback for run button
  void OnContinue(wxCommandEvent& event); //calback for continue button
  void OnSwitches(wxCommandEvent& event); //callback for switch button
  void OnMonitors(wxCommandEvent& event); //callback for monitor button
  DECLARE_EVENT_TABLE()
};
    
class MyGLCanvas: public wxGLCanvas
{
 public:
  MyGLCanvas(wxWindow *parent, wxWindowID id = wxID_ANY, monitor* monitor_mod = NULL, names* names_mod = NULL,
	     const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0,
	     const wxString& name = wxT("MyGLCanvas")); // constructor
  void Render(int cycles = -1); // function to draw canvas contents
  void setPointers(monitor *mons,names *names); //Sets new monitor and names class for in gui parsing
 private:
  bool init;                         // has the GL context been initialised?
  int cyclesdisplayed;               // how many simulation cycles have been displayed
  monitor *mmz;                      // pointer to monitor class, used to extract signal traces
  names *nmz;                        // pointer to names class, used to extract signal names
  void InitGL();                     // function to initialise GL context
  void OnSize(wxSizeEvent& event);   // callback for when canvas is resized
  void OnPaint(wxPaintEvent& event); // callback for when canvas is exposed
  void WriteText(wxString message, float x, float y); //writes text to the console
  DECLARE_EVENT_TABLE()
};

//Base class for the panels
class MyPanel : public wxFrame{
public:
	MyPanel(wxWindow *parent, wxWindowID id, const wxString& title,const wxPoint& pos, wxSize size,long style)
		:wxFrame(parent, id, title, pos, size, style){} //use parent constructor
	void show(MyFrame *frame);//Method for showing the panel 
private:
	void OnExit(wxCloseEvent& event);//Change close operation to hide window
	DECLARE_EVENT_TABLE()
};

class SwitchPanel : public MyPanel {
 public:
  SwitchPanel(wxWindow *parent, const wxString& title, const wxPoint& pos,
	  names *names_mod, devices *devices_mod, network *net_mod,
	  long style = wxDEFAULT_FRAME_STYLE); // constructor
    void refresh(names *names_mod, devices *devices_mod, network *net_mod); //Resets it when in gui parsing
 private:
  void OnOn(wxCommandEvent &event); //callback for on button 
  void OnOff(wxCommandEvent &event); // callback for off button
  void OnSelect(wxCommandEvent &event);	//callback for choice box
  wxStaticText *state; //Shows state of switches
  devices *devs; // device pointer
  network *nets; // network pointer
  wxChoice *switchChoice; //switch pointer
  vector<name> *switcharray; //Holds switches names
  DECLARE_EVENT_TABLE()
};

class MonitorPanel: public MyPanel {

 public:
  MonitorPanel(wxWindow *parent, const wxString& title, const wxPoint& pos,
	  names *names_mod, devices *devices_mod, monitor *monitor_mod, MyFrame *frame,
	  long style = wxDEFAULT_FRAME_STYLE);
  void refresh(names *names_mod, devices *devices_mod, monitor *monitor_mod); //resets when in gui parsing
 private:
  struct devio //structure for storing device info I need
  {
  devio(name id, inplink ilist, outplink olist):id(id),ilist(ilist),olist(olist){}
    name id; // the device id
    inplink ilist; // list of inputs
    outplink olist; // list of outputs
  };
  void OnAdd(wxCommandEvent &event); // callback for add button
  void OnRemove(wxCommandEvent &event);// callback for remove button
  void OnDeviceSelect(wxCommandEvent &event);//  callback for device choice box
  wxChoice *removeChoice; //Shows monitors to remove
  wxChoice *addDevice; // Shows devices to add
  wxChoice *addSignal; // Shows signals to add (only really needed for D-types)
  monitor *mons; //Monitor pointer
  names *nmz;//names pointer
  MyFrame *frame;//Master Frame pointer
  vector<name*> *monnames;// Stores device and signal names for monitors
  vector<devio*> *devios;// stores all devices and all signals
  vector<name> *sigs; //Stores the names of signals for the signal choice box
  DECLARE_EVENT_TABLE()
};

class ConsolePanel : public MyPanel {
public:
	ConsolePanel(wxWindow *parent, const wxString& title, const wxPoint& pos, MyFrame *frame,
	  long style = wxDEFAULT_FRAME_STYLE);//constructor
private:
	MyFrame *frame;//Master Frame pointer
	wxChoice *consoleSize;//Console size choice box
	wxChoice *textSize;//Text size choice box
	int csize,tsize; //Current sizes
	int *csizes,*tsizes;//Options for sizes
	void OnSave(wxCommandEvent &event); // callback for save button 
	void OnCancel(wxCommandEvent &event);// callbakc for cancle button
	DECLARE_EVENT_TABLE()
};

template<class T> // template to make cleaning vectors of pointers easier 
void deletevector(vector<T> *vec){
	for(size_t size=0; size < vec->size(); size++){
	  delete vec->at(size);
  }
	delete vec;
}

template<class T> // template to make cleaning vectors of arrays easier 
void deletevectorarrays(vector<T> *vec){
	for(size_t size=0; size < vec->size(); size++){
	  delete[] vec->at(size);
  }
	delete vec;
}
    
#endif /* gui_h */
