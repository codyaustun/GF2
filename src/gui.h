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
  MY_SPINCNTRL_ID = wxID_HIGHEST + 1,
  MY_TEXTCTRL_ID,
  RUN_BUTTON_ID,
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
  void reset();
  void consoleSettings(int height, int fontsize);
 private:
  MyGLCanvas *canvas;                     // GL drawing area widget to draw traces
  wxSpinCtrl *spin;                       // control widget to select the number of cycles
  wxTextCtrl *console;
  names *nmz;                             // pointer to names class
  devices *dmz;                           // pointer to devices class
  monitor *mmz;							  // pointer to monitor class
  network *netz;
  SwitchPanel *switches;
  MonitorPanel *monitors;
  ConsolePanel *consolePanel;
  int cyclescompleted;                    // how many simulation cycles have been completed
  void runnetwork(int ncycles);           // function to run the logic network
  void OnExit(wxCommandEvent& event);     // callback for exit menu item
  void OnAbout(wxCommandEvent& event);    // callback for about menu item
  void OnLoad(wxCommandEvent& event); 
  void OnConsole(wxCommandEvent& event); 
  void OnSelect(wxCommandEvent& event); 
  void OnCopy(wxCommandEvent& event); 
  void OnSave(wxCommandEvent& event); 
  void OnClear(wxCommandEvent& event); 
  void OnRun(wxCommandEvent& event);   // callback for run button
  void OnContinue(wxCommandEvent& event); 
  void OnSwitches(wxCommandEvent& event); 
  void OnMonitors(wxCommandEvent& event); 
  void OnSpin(wxSpinEvent& event);        // callback for spin control
  DECLARE_EVENT_TABLE()
};
    
class MyGLCanvas: public wxGLCanvas
{
 public:
  MyGLCanvas(wxWindow *parent, wxWindowID id = wxID_ANY, monitor* monitor_mod = NULL, names* names_mod = NULL,
	     const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0,
	     const wxString& name = wxT("MyGLCanvas")); // constructor
  void Render(int cycles = -1); // function to draw canvas contents
  void setPointers(monitor *mons,names *names);
 private:
  bool init;                         // has the GL context been initialised?
  int cyclesdisplayed;               // how many simulation cycles have been displayed
  monitor *mmz;                      // pointer to monitor class, used to extract signal traces
  names *nmz;                        // pointer to names class, used to extract signal names
  void InitGL();                     // function to initialise GL context
  void OnSize(wxSizeEvent& event);   // callback for when canvas is resized
  void OnPaint(wxPaintEvent& event); // callback for when canvas is exposed
  void OnMouse(wxMouseEvent& event); // callback for mouse events inside canvas
  void WriteText(wxString message, float x, float y);
  DECLARE_EVENT_TABLE()
};

class MyPanel : public wxFrame{
public:
	MyPanel(wxWindow *parent, wxWindowID id, const wxString& title,const wxPoint& pos, wxSize size,long style):wxFrame(parent, id, title, pos, size, style){}
	void show(MyFrame *frame);
private:
	void OnExit(wxCloseEvent& event);
	DECLARE_EVENT_TABLE()
};

class SwitchPanel : public MyPanel {
 public:
  SwitchPanel(wxWindow *parent, const wxString& title, const wxPoint& pos,
	  names *names_mod, devices *devices_mod, network *net_mod,
	  long style = wxDEFAULT_FRAME_STYLE);
    void refresh(names *names_mod, devices *devices_mod, network *net_mod);
 private:
  void OnOn(wxCommandEvent &event);
  void OnOff(wxCommandEvent &event);
  void OnSelect(wxCommandEvent &event);
  wxStaticText *state;
  devices *devs;
  network *nets;
  wxChoice *switchChoice;
  vector<name> *switcharray;
  DECLARE_EVENT_TABLE()
};

class MonitorPanel: public MyPanel {

 public:
  MonitorPanel(wxWindow *parent, const wxString& title, const wxPoint& pos,
	  names *names_mod, devices *devices_mod, monitor *monitor_mod, MyFrame *frame,
	  long style = wxDEFAULT_FRAME_STYLE);
  void refresh(names *names_mod, devices *devices_mod, monitor *monitor_mod);
 private:
  struct devio{
  devio(name id, inplink ilist, outplink olist):id(id),ilist(ilist),olist(olist){}
    name id;
    inplink ilist;
    outplink olist;
  };
  void OnAdd(wxCommandEvent &event);
  void OnRemove(wxCommandEvent &event);
  void OnDeviceSelect(wxCommandEvent &event);
  wxChoice *removeChoice;
  wxChoice *addDevice;
  wxChoice *addSignal;
  monitor *mons;
  names *nmz;
  MyFrame *frame;
  vector<name*> *monnames;
  vector<devio*> *devios;
  vector<name> *sigs;
  DECLARE_EVENT_TABLE()
};

class ConsolePanel : public MyPanel {
public:
	ConsolePanel(wxWindow *parent, const wxString& title, const wxPoint& pos, MyFrame *frame,
	  long style = wxDEFAULT_FRAME_STYLE);
private:
	MyFrame *frame;
	wxChoice *consoleSize;
	wxChoice *textSize;
	int csize,tsize;
	int *csizes,*tsizes;
	void OnSave(wxCommandEvent &event);
	void OnCancel(wxCommandEvent &event);
	DECLARE_EVENT_TABLE()
};

template<class T>
void deletevector(vector<T> *vec){
	for(size_t size=0; size < vec->size(); size++){
	  delete[] vec->at(size);
  }
	delete vec;
}
    
#endif /* gui_h */
