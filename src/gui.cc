#include "gui.h"
#include <GL/glut.h>
#include "wx_icon.xpm"
#include <iostream>

using namespace std;

// MyGLCanvas ////////////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(MyGLCanvas, wxGLCanvas)
  EVT_SIZE(MyGLCanvas::OnSize)
  EVT_PAINT(MyGLCanvas::OnPaint)
  EVT_MOUSE_EVENTS(MyGLCanvas::OnMouse)
END_EVENT_TABLE()
  
MyGLCanvas::MyGLCanvas(wxWindow *parent, wxWindowID id, monitor* monitor_mod, names* names_mod,
		       const wxPoint& pos, const wxSize& size, long style, const wxString& name):
    wxGLCanvas(parent, id, pos, size, style, name)
  // Constructor - initialises private variables
{
  mmz = monitor_mod;
  nmz = names_mod;
  init = false;
  cyclesdisplayed = -1;
}

void MyGLCanvas::Render(int cycles)
  // Draws canvas contents - the following example writes the string "example text" onto the canvas
  // and draws a signal trace. The trace is artificial if the simulator has not yet been run.
  // When the simulator is run, the number of cycles is passed as a parameter and the first monitor
  // trace is displayed.
{
  float y = GetSize().GetHeight()-50,ydiff,xdiff;
  int width = GetSize().GetWidth();
  asignal s;
  name dev,sig;
  namestring monname;
  
  if (cycles >= 0) cyclesdisplayed = cycles;

  SetCurrent();
  if (!init) {
    InitGL();
    init = true;
  }
  glClear(GL_COLOR_BUFFER_BIT);
  if(cyclesdisplayed>=0){
    for(int j =0; j<mmz->moncount(); j++){
      mmz->getmonname(j,dev,sig);
      monname = nmz->getname(dev);
      if(sig!=blankname){
	monname+=".";
	monname+=nmz->getname(sig);
      }
      glColor3f(0.0, 0.0, 0.0);
      WriteText(wxString::FromUTF8(monname.c_str()),20,y+17);
      switch(j%6){
      case 0:
	glColor3f(1.0, 0.0, 0.0);
	break;
      case 1:
	glColor3f(0.0, 1.0, 0.0);
	break;
      case 2:
	glColor3f(0.0, 0.0, 1.0);
	break;
      case 3:
	glColor3f(1.0, 1.0, 0.0);
	break;
      case 4:
	glColor3f(1.0, 0.0, 1.0);
	break;
      case 5:
	glColor3f(0.0, 1.0, 1.0);
	break;
      }
      glBegin(GL_LINE_STRIP);
      for (int i=0; i<cyclesdisplayed; i++) {
	if (mmz->getsignaltrace(j, i, s)) {
	  if (s==low) ydiff = -10.0;
	  if (s==high) ydiff = 10.0;
	  xdiff=(width-60)/cyclesdisplayed;
	  if(xdiff>20) xdiff=20;
	  glVertex2f(xdiff*i+20.0, y+ydiff); 
	  glVertex2f(xdiff*(i+1)+20.0, y+ydiff);
	}
      }
      y-=50;
      glEnd();
    }
  }
  // We've been drawing to the back buffer, flush the graphics pipeline and swap the back buffer to the front
  glFlush();
  SwapBuffers();
}

void MyGLCanvas::WriteText(wxString message, float x, float y){
  glRasterPos2f(x, y);
  for (int i = 0; i < message.Len(); i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, message[i]);
}

void MyGLCanvas::InitGL()
  // Function to initialise the GL context
{
  int w, h;

  GetClientSize(&w, &h);
  SetCurrent();
  glDrawBuffer(GL_BACK);
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glViewport(0, 0, (GLint) w, (GLint) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, w, 0, h, -1, 1); 
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void MyGLCanvas::OnPaint(wxPaintEvent& event)
  // Callback function for when the canvas is exposed
{
  wxPaintDC dc(this); // required for correct refreshing under MS windows
  Render();
}

void MyGLCanvas::OnSize(wxSizeEvent& event)
  // Callback function for when the canvas is resized
{
  wxGLCanvas::OnSize(event); // required on some platforms
  init = false;
  Refresh(); // required by some buggy nvidia graphics drivers,
  Update();  // harmless on other platforms!
}

void MyGLCanvas::OnMouse(wxMouseEvent& event)
  // Callback function for mouse events inside the GL canvas
{
 
}

// MyFrame ///////////////////////////////////////////////////////////////////////////////////////


BEGIN_EVENT_TABLE(MyFrame, wxFrame)
  EVT_MENU(wxID_EXIT, MyFrame::OnExit)
  EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
  EVT_BUTTON(RUN_BUTTON_ID, MyFrame::OnRun)
  EVT_BUTTON(CONTINUE_BUTTON_ID, MyFrame::OnContinue)
  EVT_BUTTON(SWITCH_BUTTON_ID, MyFrame::OnSwitches)
  EVT_BUTTON(MONITOR_BUTTON_ID, MyFrame::OnMonitors)
  EVT_SPINCTRL(MY_SPINCNTRL_ID, MyFrame::OnSpin)
  EVT_TEXT_ENTER(MY_TEXTCTRL_ID, MyFrame::OnText)
END_EVENT_TABLE()
  
MyFrame::MyFrame(wxWindow *parent, const wxString& title, const wxPoint& pos, const wxSize& size,
		 names *names_mod, devices *devices_mod, monitor *monitor_mod, long style):
  wxFrame(parent, wxID_ANY, title, pos, size, style)
  // Constructor - initialises pointers to names, devices and monitor classes, lays out widgets
  // using sizers
{
  SetIcon(wxIcon(wx_icon));

  nmz = names_mod;
  dmz = devices_mod;
  mmz = monitor_mod;
  
  if (nmz == NULL || dmz == NULL || mmz == NULL) {
    cout << "Cannot operate GUI without names, devices and monitor classes" << endl;
    //exit(1);
  }

  switches = new SwitchPanel(this, wxT("Switches"), wxDefaultPosition, nmz, dmz, mmz);
  monitors = new MonitorPanel(this, wxT("Monitors"), wxDefaultPosition, nmz, dmz, mmz);

  wxMenu *fileMenu = new wxMenu;
  fileMenu->Append(wxID_ABOUT, wxT("&About"));
  fileMenu->Append(wxID_EXIT, wxT("&Quit"));
  wxMenuBar *menuBar = new wxMenuBar;
  menuBar->Append(fileMenu, wxT("&File"));
  SetMenuBar(menuBar);

  wxBoxSizer *topsizer = new wxBoxSizer(wxHORIZONTAL);
  canvas = new MyGLCanvas(this, wxID_ANY, monitor_mod, names_mod);
  topsizer->Add(canvas, 1, wxEXPAND | wxALL, 10);

  wxBoxSizer *button_sizer = new wxBoxSizer(wxVERTICAL);
  //Run Button
  button_sizer->Add(new wxButton(this, RUN_BUTTON_ID, wxT("Run")), 0, wxALL, 10);
  //Continue Button
  button_sizer->Add(new wxButton(this, CONTINUE_BUTTON_ID, wxT("Continue")), 0, wxALL, 10);
  //Cycle Selector
  button_sizer->Add(new wxStaticText(this, wxID_ANY, wxT("Cycles")), 0, wxTOP|wxLEFT|wxRIGHT, 10);
  spin = new wxSpinCtrl(this, MY_SPINCNTRL_ID, wxString(wxT("10")));
  button_sizer->Add(spin, 0 , wxALL, 10);
  //Switch Button
  button_sizer->Add(new wxButton(this, SWITCH_BUTTON_ID, wxT("Switches")), 0, wxALL, 10);
  //Monitor Button
  button_sizer->Add(new wxButton(this, MONITOR_BUTTON_ID, wxT("Monitors")), 0, wxALL, 10);

  topsizer->Add(button_sizer, 0, wxALIGN_CENTER);

  SetSizeHints(400, 400);
  SetSizer(topsizer);
}

void MyFrame::OnExit(wxCommandEvent &event)
  // Callback for the exit menu item
{
  Close(true);
}

void MyFrame::OnAbout(wxCommandEvent &event)
  // Callback for the about menu item
{
  wxMessageDialog about(this, wxT("Example wxWidgets GUI\nAndrew Gee\nFebruary 2011"), wxT("About Logsim"), wxICON_INFORMATION | wxOK);
  about.ShowModal();
}

void MyFrame::OnRun(wxCommandEvent &event)
  // Callback for the push button
{
  cyclescompleted = 0;
  mmz->resetmonitor ();
  runnetwork(spin->GetValue());
  canvas->Render(cyclescompleted);
}

void MyFrame::OnSpin(wxSpinEvent &event)
  // Callback for the spin control
{

}

void MyFrame::OnText(wxCommandEvent &event)
  // Callback for the text entry field
{

}

void MyFrame::OnSwitches(wxCommandEvent &event){
  wxPoint position = switches->GetScreenPosition();
  if(position.x==wxDefaultPosition.x && position.y==wxDefaultPosition.y){
     wxSize currentSize = GetSize();
     position = GetScreenPosition();
     position.x+=(currentSize.GetWidth()-300)/2;
     position.y+=(currentSize.GetHeight()-100)/2;
  }
  switches->SetPosition(position);
  switches->Iconize(false);
  switches->SetFocus();
  switches->Raise();  
  switches->Show(true);
}

void MyFrame::OnMonitors(wxCommandEvent &event){
  wxPoint position = monitors->GetScreenPosition();
  if(position.x==wxDefaultPosition.x && position.y==wxDefaultPosition.y){
     wxSize currentSize = GetSize();
     position = GetScreenPosition();
     position.x+=(currentSize.GetWidth()-300)/2;
     position.y+=(currentSize.GetHeight()-150)/2;
  }
  monitors->SetPosition(position);
  monitors->Iconize(false);
  monitors->SetFocus();
  monitors->Raise();  
  monitors->Show(true);
}

void MyFrame::OnContinue(wxCommandEvent &event){
  runnetwork(spin->GetValue());
  canvas->Render(cyclescompleted);
}

void MyFrame::runnetwork(int ncycles)
  // Function to run the network, derived from corresponding function in userint.cc
{
  bool ok = true;
  int n = ncycles;

  while ((n > 0) && ok) {
    dmz->executedevices (ok);
    if (ok) {
      n--;
      mmz->recordsignals ();
    } else
      cout << "Error: network is oscillating" << endl;
  }
  if (ok) cyclescompleted = cyclescompleted + ncycles;
  else cyclescompleted = 0;
}

BEGIN_EVENT_TABLE(SwitchPanel, wxFrame)
  EVT_CLOSE(SwitchPanel::OnExit)
  EVT_BUTTON(SWITCHON_BUTTON_ID,SwitchPanel::OnOn)
  EVT_BUTTON(SWITCHOFF_BUTTON_ID, SwitchPanel::OnOff)
END_EVENT_TABLE()

SwitchPanel::SwitchPanel(wxWindow *parent, const wxString& title, const wxPoint& pos, names *names_mod, devices *devices_mod, monitor *monitor_mod, long style):wxFrame(parent, wxID_ANY, title, pos, wxSize(300,100), style) {
  devs=devices_mod;
  devlink switches = devices_mod -> getSwitches();
  wxBoxSizer *mainsizer = new wxBoxSizer(wxVERTICAL);
  switchChoice = new wxChoice(this,SWITCH_CHOICE_ID);
  if(switches==NULL) switchChoice->Append(wxT("No Switches"));
  namestring switchname;
  switcharray=new vector<name>;
  for (devlink s = switches; s != NULL; s = s->next){ 
    switchname = names_mod->getname(s->id);
    switchChoice->Append(wxString::FromUTF8(switchname.c_str()));
    switcharray->push_back(s->id);
  }
  switchChoice->SetSelection(0);
  //state = new wxStaticText(this,wxID_ANY,getState(0));
  wxBoxSizer *top = new wxBoxSizer(wxHORIZONTAL);
  top->Add(switchChoice, 0, wxALL, 10);
  //top->Add(state, 0, wxTOP|wxLEFT|wxRIGHT, 15);
  mainsizer->Add(top, 0, wxALIGN_CENTRE);
  wxBoxSizer *buttons = new wxBoxSizer(wxHORIZONTAL);
  buttons->Add(new wxButton(this, SWITCHON_BUTTON_ID, wxT("Set On")), 0, wxALL, 10);
  buttons->Add(new wxButton(this, SWITCHOFF_BUTTON_ID, wxT("Set Off")), 0, wxALL, 10);
  mainsizer->Add(buttons, 0, wxALIGN_CENTRE);
  SetSizer(mainsizer);
}
/*
wxString SwitchPanel::getState(int selection){
  switchid sid = switcharray[selection];
  
} 
*/

void SwitchPanel::OnExit(wxCloseEvent &event){
  Show(false);
}

void SwitchPanel::OnOn(wxCommandEvent &event){
  int index = switchChoice->GetSelection();
  bool ok;
  devs->setswitch(switcharray->at(index),high,ok);
  if(!ok){
    wxMessageDialog warning(this, wxT("No Switches"), wxT("Warning"), wxICON_ERROR | wxOK);
    warning.ShowModal();
  }
}

void SwitchPanel::OnOff(wxCommandEvent &event){
  int index = switchChoice->GetSelection();
  bool ok;
  devs->setswitch(switcharray->at(index),low,ok);
  if(!ok){
    wxMessageDialog warning(this, wxT("No Switches"), wxT("Warning"), wxICON_ERROR | wxOK);
    warning.ShowModal();
  }
}

BEGIN_EVENT_TABLE(MonitorPanel, wxFrame)
  EVT_CLOSE(MonitorPanel::OnExit)
  EVT_BUTTON(MONITORADD_BUTTON_ID,MonitorPanel::OnAdd)
  EVT_BUTTON(MONITORREMOVE_BUTTON_ID, MonitorPanel::OnRemove)
  EVT_CHOICE(MONITORADDDEV_CHOICE_ID, MonitorPanel::OnDeviceSelect)
END_EVENT_TABLE()

MonitorPanel::MonitorPanel(wxWindow *parent, const wxString& title, const wxPoint& pos, names *names_mod, devices *devices_mod, monitor *monitor_mod, long style):
  wxFrame(parent, wxID_ANY, title, pos,wxSize(300,150), style) {
  monnames = new vector<name*>;
  devios = new vector<devio*>;
  sigs = new vector<name>;
  removeChoice = new wxChoice(this,MONITORREMOVE_CHOICE_ID);
  addDevice = new wxChoice(this,MONITORADDDEV_CHOICE_ID);
  addSignal = new wxChoice(this,MONITORADDSIG_CHOICE_ID);
  mons=monitor_mod;
  nmz=names_mod;

  //load monitors
  int monitors = mons->moncount();
  name dev, outp, *monname;
  namestring displayname;
  for(int i = 0; i < monitors; i++){
    mons->getmonname(i,dev,outp);
    monname = new name[2];
    monname[0] = dev;
    monname[1] = outp;
    monnames->push_back(monname);
    displayname=nmz->getname(dev);
    if(outp>blankname){
      displayname+=".";
      displayname+=nmz->getname(outp);
    }
    removeChoice->Append(wxString::FromUTF8(displayname.c_str()));
  }
  if(monitors>0) removeChoice->SetSelection(0);

  //load devices
  devlink devs = devices_mod->getDevices();
  namestring devname;
  for (devlink d = devs; d != NULL; d = d->next){ 
    devios->push_back(new devio(d->id,d->ilist,d->olist));
    addDevice->Append(wxString::FromUTF8(nmz->getname(d->id).c_str()));
  }
  addDevice->SetSelection(0);
  wxCommandEvent event;
  OnDeviceSelect(event);
  
  //place components
  wxBoxSizer *mainsizer = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer *add = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer *remove = new wxBoxSizer(wxHORIZONTAL);
  mainsizer->Add(add, 0, wxALIGN_CENTRE);
  mainsizer->Add(remove, 0, wxALIGN_CENTRE);
  add->Add(addDevice,0,wxLEFT|wxTOP,20);
  add->Add(addSignal,0,wxALL,20);
  add->Add(new wxButton(this, MONITORADD_BUTTON_ID, wxT("Add Monitor")), 0, wxRIGHT|wxTOP, 20);
  remove->Add(removeChoice,0,wxALL,20);
  remove->Add(new wxButton(this, MONITORREMOVE_BUTTON_ID, wxT("Remove Monitor")), 0, wxRIGHT|wxTOP|wxBOTTOM , 20);
  SetSizer(mainsizer);
}

void MonitorPanel::OnExit(wxCloseEvent &event){
  Show(false);
}

void MonitorPanel::OnAdd(wxCommandEvent &event){
  int devindex = addDevice->GetSelection();
  int sigindex = addSignal->GetSelection();
  name dev = devios->at(devindex)->id;
  name sig = sigs->at(sigindex);
  bool ok;
  mons->makemonitor(dev,sig,ok);
  name* mon = new name[2];
  mon[0]=dev;
  mon[1]=sig;
  monnames->push_back(mon);
  namestring monname = nmz->getname(dev);
  if(sig!=blankname){
    monname+=".";
    monname+=nmz->getname(sig);
  }
  removeChoice->Append(wxString::FromUTF8(monname.c_str()));
  removeChoice->SetSelection(0);
}

void MonitorPanel::OnRemove(wxCommandEvent &event){
  int index = removeChoice->GetSelection();
  if(index!=-1){
    removeChoice->Delete(index);
    removeChoice->SetSelection(0);
    name* devsig = monnames->at(index);
    monnames->erase(monnames->begin()+index);
    bool ok;
    mons->remmonitor(devsig[0],devsig[1],ok);
  }else{
    wxMessageDialog warning(this, wxT("No monitors to remove"), wxT("Warning"), wxICON_ERROR | wxOK);
    warning.ShowModal();
  }
}

void MonitorPanel::OnDeviceSelect(wxCommandEvent &event){
  int index = addDevice->GetSelection();
  devio *dev = devios->at(index);
  addSignal->Clear();
  sigs->clear();
  for (inplink i = dev->ilist; i != NULL; i = i->next){ 
    addSignal->Append(wxString::FromUTF8(nmz->getname(i->id).c_str()));
    sigs->push_back(i->id);
  }
  for (outplink o = dev->olist; o != NULL; o = o->next){ 
    addSignal->Append(wxString::FromUTF8(nmz->getname(o->id).c_str()));
    sigs->push_back(o->id);
  }
  addSignal->SetSelection(0);
}
