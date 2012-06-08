#include "gui.h"
#include <GL/glut.h>
#include "wx_icon.xpm"
#include <iostream>
#include <fstream>
#include "translate.h"

using namespace std;

// MyGLCanvas ////////////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(MyGLCanvas, wxGLCanvas)
  EVT_SIZE(MyGLCanvas::OnSize)
  EVT_PAINT(MyGLCanvas::OnPaint)
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

void MyGLCanvas::setPointers(monitor *mons,names *names)
	//Sets monitor and names class when they change
{
  mmz = mons;
  nmz = names;
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
  if(cyclesdisplayed>0){
	//Draw the traces
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
      xdiff=(float)(width-60)/cyclesdisplayed;
      if(xdiff>20) xdiff=20;
      for (int i=0; i<cyclesdisplayed; i++) {
	if (mmz->getsignaltrace(j, i, s)) {
	  if (s==low) ydiff = -10.0;
	  if (s==high) ydiff = 10.0;
	  glVertex2f(xdiff*i+20.0, y+ydiff); 
	  glVertex2f(xdiff*(i+1)+20.0, y+ydiff);
	}
      }
      glEnd();
      glEnable(GL_LINE_STIPPLE);
      glLineStipple(1,0xBBBB);
      glColor3f(0.0, 0.0, 0.0);
      glBegin(GL_LINE_STRIP);
      glVertex2f(20.0, y); 
      glVertex2f(xdiff*cyclesdisplayed+20.0, y);
      glEnd();
      glDisable(GL_LINE_STIPPLE);
      y-=50;
    }
  }
  // We've been drawing to the back buffer, flush the graphics pipeline and swap the back buffer to the front
  glFlush();
  SwapBuffers();
}

void MyGLCanvas::WriteText(wxString message, float x, float y)
//Write text to the canvas
{
  glRasterPos2f(x, y);
  for (unsigned int i = 0; i < message.Len(); i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, message[i]);
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

// MyFrame ///////////////////////////////////////////////////////////////////////////////////////


BEGIN_EVENT_TABLE(MyFrame, wxFrame)
  EVT_MENU(wxID_EXIT, MyFrame::OnExit)
  EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
  EVT_MENU(LOAD_ID, MyFrame::OnLoad)
  EVT_MENU(CONSOLESELECT_ID, MyFrame::OnSelect)
  EVT_MENU(CONSOLECOPY_ID, MyFrame::OnCopy)
  EVT_MENU(CONSOLESAVE_ID, MyFrame::OnSave)
  EVT_MENU(CONSOLECLEAR_ID, MyFrame::OnClear)
  EVT_MENU(CONSOLEOPTIONS_ID, MyFrame::OnConsole)
  EVT_BUTTON(RUN_BUTTON_ID, MyFrame::OnRun)
  EVT_BUTTON(CONTINUE_BUTTON_ID, MyFrame::OnContinue)
  EVT_BUTTON(SWITCH_BUTTON_ID, MyFrame::OnSwitches)
  EVT_BUTTON(MONITOR_BUTTON_ID, MyFrame::OnMonitors)
END_EVENT_TABLE()
  
MyFrame::MyFrame(wxWindow *parent, const wxString& title, const wxPoint& pos, const wxSize& size,
		 names *names_mod, devices *devices_mod, monitor *monitor_mod, network *net_mod, wxTextCtrl *console, long style):
  wxFrame(parent, wxID_ANY, title, pos, size, style), console(console)
  // Constructor - initialises pointers to names, devices and monitor classes, lays out widgets
  // using sizers
{
  SetIcon(wxIcon(wx_icon));
  cyclescompleted = -1;

  nmz = names_mod;
  dmz = devices_mod;
  mmz = monitor_mod;
  netz = net_mod;

  //set up locale stuff
  if(wxLocale::IsAvailable(wxLocale::GetSystemLanguage())){
      if(locale.Init(wxLocale::GetSystemLanguage(),wxLOCALE_CONV_ENCODING)){
	locale.AddCatalogLookupPathPrefix(wxT("."));
	locale.AddCatalog(wxT("logsim"));
      }else{
	wxMessageDialog warning(this, wxT("Selected language is wrong"), wxT("Warning"), wxICON_ERROR | wxOK);
	warning.ShowModal();
	locale.Init( wxLANGUAGE_ENGLISH );
      }
    }else{
      wxMessageDialog warning(this, wxT("Launguage not available"), wxT("Warning"), wxICON_ERROR | wxOK);
		warning.ShowModal();
		locale.Init( wxLANGUAGE_ENGLISH );
    }
  

  if (nmz == NULL || dmz == NULL || mmz == NULL) {
    cout << s_("Cannot operate GUI without names, devices and monitor classes") << endl;
    exit(1);
  }
  //instantiate objects and set up console
  switches = new SwitchPanel(this, _("Switches"), wxDefaultPosition, nmz, dmz, netz);
  monitors = new MonitorPanel(this, _("Monitors"), wxDefaultPosition, nmz, dmz, mmz, this);
  consolePanel = new ConsolePanel(this, _("Console Options"), wxDefaultPosition, this);
  console->Create(this,wxID_ANY,wxT(""),wxDefaultPosition,wxDefaultSize, wxTE_READONLY|wxTE_DONTWRAP|wxTE_MULTILINE);
  console ->SetMinSize(wxSize(0,120));
  console ->SetFont(wxFont(8, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL));

  //Create Menus
  wxMenu *fileMenu = new wxMenu;
  fileMenu->Append(LOAD_ID, _("&Parse File") );
  fileMenu->AppendSeparator();
  fileMenu->Append(wxID_ABOUT, _("&About"));
  fileMenu->Append(wxID_EXIT, _("&Quit"));
  wxMenu *consoleMenu = new wxMenu;
  consoleMenu->Append(CONSOLESELECT_ID,_("&Select All"));
  consoleMenu->Append(CONSOLECOPY_ID,_("&Copy"));
  consoleMenu->Append(CONSOLESAVE_ID,_("Save to &File"));
  consoleMenu->Append(CONSOLECLEAR_ID,_("Clea&r"));
  consoleMenu->AppendSeparator();
  consoleMenu->Append(CONSOLEOPTIONS_ID, _("&Options"));
  wxMenuBar *menuBar = new wxMenuBar;
  menuBar->Append(fileMenu, _("&File"));
  menuBar->Append(consoleMenu, _("&Console"));
  SetMenuBar(menuBar);

  //Sizers and canvas
  wxBoxSizer *topsizer = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer *canvassizer = new wxBoxSizer(wxVERTICAL);
  canvas = new MyGLCanvas(this, wxID_ANY, monitor_mod, names_mod);
  topsizer->Add(canvassizer, 1, wxEXPAND | wxALL, 10);
  canvassizer->Add(canvas, 1, wxEXPAND | wxALL, 10);
  canvassizer->Add(console, 0, wxEXPAND | wxRIGHT | wxLEFT, 10);

  wxBoxSizer *button_sizer = new wxBoxSizer(wxVERTICAL);
  //Run Button
  button_sizer->Add(new wxButton(this, RUN_BUTTON_ID, _("Run")), 0, wxALL, 10);
  //Continue Button
  button_sizer->Add(new wxButton(this, CONTINUE_BUTTON_ID, _("Continue")), 0, wxALL, 10);
  //Cycle Selector
  button_sizer->Add(new wxStaticText(this, wxID_ANY, _("Cycles")), 0, wxTOP|wxLEFT|wxRIGHT, 10);
  spin = new wxSpinCtrl(this, wxID_ANY, wxString(wxT("10")));
  button_sizer->Add(spin, 0 , wxALL, 10);
  //Switch Button
  button_sizer->Add(new wxButton(this, SWITCH_BUTTON_ID, _("Switches")), 0, wxALL, 10);
  //Monitor Button
  button_sizer->Add(new wxButton(this, MONITOR_BUTTON_ID, _("Monitors")), 0, wxALL, 10);

  topsizer->Add(button_sizer, 0, wxALIGN_CENTER);

  SetSizeHints(400, 400);
  SetSizer(topsizer);

}

void MyFrame::reset()
  // Callback for the exit menu item
{
  cyclescompleted=-1;
  canvas->Render(0);
  cout << s_("Logic Circuit Reset.") << endl;
}

void MyFrame::consoleSettings(int height, int fontsize)
//Allows console options to be set
{
	console->SetMinSize(wxSize(0,height));
	console->SetFont(wxFont(fontsize, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL));
	GetSizer()->Layout();
	Refresh();
	Update();
}

void MyFrame::OnExit(wxCommandEvent &event)
  // Callback for the exit menu item
{
  Close(true);
}

void MyFrame::OnAbout(wxCommandEvent &event)
  // Callback for the about menu item
{
  wxMessageDialog about(this, _("Logic Simulator GUI\nTom Mottram\nMay 2012"), _("About Logsim"), wxICON_INFORMATION | wxOK);
  about.ShowModal();
}

void MyFrame::OnLoad(wxCommandEvent &event)
	//Callback for file parser
{
  wxString filename = wxFileSelector(_("Choose file to parse"));
  if(!filename.IsEmpty()){
	  //create six new instances of the scanner/parser/network class set
	names *namz = new names();
	network *netz = new network(namz);
	devices *demz = new devices(namz, netz);
	monitor *momz = new monitor(namz, netz);
	scanner *smz = new scanner(namz, filename.mb_str());
	parser *pmz = new parser(netz, demz, momz, smz);
	//try parsing the input file
	if (pmz->readin ()){
	  cout << filename.mb_str() << s_(" parsed correctly.") << endl;
		//if successful set local pointers and subclass pointers
		delete nmz;
		delete dmz;
		delete mmz;
		nmz = namz;
		dmz = demz;
		mmz = momz;
		canvas->setPointers(mmz,nmz);
		monitors->refresh(nmz,dmz,mmz);
		switches->refresh(nmz,dmz,netz);
		reset();
	}else{
		//Error popup
		wxString message = _("Could not parse ");
		message.Append(filename);
		message.Append(_(" correctly!"));
		wxMessageDialog warning(this, message, _("Warning"), wxICON_ERROR | wxOK);
		warning.ShowModal();
	}
  }
}

void MyFrame::OnConsole(wxCommandEvent &event)
	//Callback for console options
{
	consolePanel->show(this);
}

void MyFrame::OnSelect(wxCommandEvent &event)
{
  console->SetSelection(0,console->GetLastPosition());
}

void MyFrame::OnCopy(wxCommandEvent &event)
{
  console->Copy();	
}

void MyFrame::OnClear(wxCommandEvent &event)
{
  wxMessageDialog check(this, _("Do you really want to clear the console?"), _("Are You Sure?"), wxICON_QUESTION | wxYES_NO);
  if(check.ShowModal()==wxID_YES){
    console->SetValue(wxT(""));
  }
}

void MyFrame::OnSave(wxCommandEvent &event)
{
  wxString filename = wxFileSelector(_("Save"), wxT(""),_("Console.log") , _("log"),_("*.log"), wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
  if(!filename.IsEmpty()){
    ofstream fout;
    try{
      fout.open(filename.mb_str(), ofstream::out|ofstream::trunc);
      fout << console->GetValue().mb_str();
      fout.close();
    }catch(ofstream::failure e){
      cout << s_("Fail") << endl;
    }
  }
}

void MyFrame::OnRun(wxCommandEvent &event)
  // Callback for the run button
{
  cyclescompleted = 0;
  mmz->resetmonitor ();
  runnetwork(spin->GetValue());
  canvas->Render(cyclescompleted);
}

void MyFrame::OnSwitches(wxCommandEvent &event)
	//Callback for switch button
{
  switches->show(this);
}

void MyFrame::OnMonitors(wxCommandEvent &event)
	//Callback for monitor button
{  
  monitors->show(this);
}

void MyFrame::OnContinue(wxCommandEvent &event)
	//callback for continue button
{
	if(cyclescompleted>0){
		runnetwork(spin->GetValue());
		canvas->Render(cyclescompleted);
	}else{
		wxMessageDialog warning(this, _("Must run the network before continuing."), _("Warning"), wxICON_ERROR | wxOK);
		warning.ShowModal();
	}
}

void MyFrame::runnetwork(int ncycles)
  // Function to run the network, derived from corresponding function in userint.cc
{
  bool ok = true; 
  if(cyclescompleted == maxcycles){
	  wxMessageDialog warning(this, _("Maximum number of cycles reached, please run again."), _("Warning"), wxICON_ERROR | wxOK);
		warning.ShowModal();
	  return;
  }
  if(cyclescompleted+ncycles>maxcycles){
	  ncycles=maxcycles-cyclescompleted;
	  cout << s_("Warning max cycles reached, only ") << ncycles << s_(" were completed.") << endl;
  }
  int n = ncycles;
  while ((n > 0) && ok) {
    dmz->executedevices (ok);
    if (ok) {
      n--;
      mmz->recordsignals ();
    } else
      cout << s_("Error: network is oscillating") << endl;
  }
  if (ok){
	  cyclescompleted = cyclescompleted + ncycles;
	  cout << ncycles << s_(" cycles run. ") << cyclescompleted << s_(" cycles total.") << endl;
  }
  else cyclescompleted = 0;
}

///////////// MY PANEL /////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(MyPanel, wxFrame)
  EVT_CLOSE(MyPanel::OnExit)
END_EVENT_TABLE()
void MyPanel::OnExit(wxCloseEvent &event)
//Callback on close event
{
  Show(false);
}

void MyPanel::show(MyFrame *frame)
	//Used to position and show the pop up when it is opened
{
	wxPoint position = GetScreenPosition();
  if(position.x==wxDefaultPosition.x && position.y==wxDefaultPosition.y){
     wxSize frameSize = frame->GetSize();
     position = frame->GetScreenPosition();
     position.x+=(frameSize.GetWidth()-GetSize().GetWidth())/2;
     position.y+=(frameSize.GetHeight()-GetSize().GetHeight())/2;
  }
  SetPosition(position);

  Iconize(false);
  SetFocus();
  Raise();  
  Show(true);
}

////////// SWITCH PANEL //////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(SwitchPanel, MyPanel)
  EVT_BUTTON(SWITCHON_BUTTON_ID,SwitchPanel::OnOn)
  EVT_BUTTON(SWITCHOFF_BUTTON_ID, SwitchPanel::OnOff)
  EVT_CHOICE(SWITCH_CHOICE_ID, SwitchPanel::OnSelect)
END_EVENT_TABLE()
SwitchPanel::SwitchPanel(wxWindow *parent, const wxString& title, const wxPoint& pos, names *names_mod, devices *devices_mod, network *net_mod, long style):MyPanel(parent, wxID_ANY, title, pos, wxSize(300,100), style) 
//Panel to allow you to set the switches
{
  //instantiate objects
  wxBoxSizer *mainsizer = new wxBoxSizer(wxVERTICAL);
  switchChoice = new wxChoice(this,SWITCH_CHOICE_ID);
  switcharray=new vector<name>;
  state = new wxStaticText(this,wxID_ANY,wxT(""));

  //Add switches to choice box
  refresh(names_mod, devices_mod, net_mod);

  //Add components
  wxBoxSizer *top = new wxBoxSizer(wxHORIZONTAL);
  top->Add(switchChoice, 0, wxALL, 10);
  top->Add(state, 0, wxTOP|wxLEFT|wxRIGHT, 15);
  mainsizer->Add(top, 0, wxALIGN_CENTRE);
  wxBoxSizer *buttons = new wxBoxSizer(wxHORIZONTAL);
  buttons->Add(new wxButton(this, SWITCHON_BUTTON_ID, _("Set On")), 0, wxALL, 10);
  buttons->Add(new wxButton(this, SWITCHOFF_BUTTON_ID, _("Set Off")), 0, wxALL, 10);
  mainsizer->Add(buttons, 0, wxALIGN_CENTRE);
  SetSizer(mainsizer);
}

void SwitchPanel::refresh(names *names_mod, devices *devices_mod,network *net_mod)
	//Used to reset the choice boxes
{
	devs=devices_mod;
	nets=net_mod;
	devlink switches = devices_mod -> getSwitches();
	switchChoice->Clear();
	if(switches==NULL){
		switchChoice->Append(_("No Switches"));
		return;
	}
  namestring switchname;
  delete switcharray;
  switcharray=new vector<name>;
  for (devlink s = switches; s != NULL; s = s->next){ 
    switchname = names_mod->getname(s->id);
    switchChoice->Append(wxString::FromUTF8(switchname.c_str()));
    switcharray->push_back(s->id);
  }
  switchChoice->SetSelection(0);
  wxCommandEvent event;
  OnSelect(event);
}

void SwitchPanel::OnOn(wxCommandEvent &event)
	//Callback for set on button
{
  int index = switchChoice->GetSelection();
  bool ok;
  devs->setswitch(switcharray->at(index),high,ok);
  if(!ok){
    wxMessageDialog warning(this, _("No Switches"), _("Warning"), wxICON_ERROR | wxOK);
    warning.ShowModal();
  }else{
	  state->SetLabel(_("On"));
  }
}

void SwitchPanel::OnOff(wxCommandEvent &event)
	//callback for set off button
{
  int index = switchChoice->GetSelection();
  bool ok;
  devs->setswitch(switcharray->at(index),low,ok);
  if(!ok){
    wxMessageDialog warning(this, _("No Switches"), _("Warning"), wxICON_ERROR | wxOK);
    warning.ShowModal();
  }else{
	  state->SetLabel(_("Off"));
  }
}

void SwitchPanel::OnSelect(wxCommandEvent &event)
	//Callback for switch choice box to set state
{
	int index = switchChoice->GetSelection();
	devlink dev=nets->finddevice(switcharray->at(index));
	if(dev->swstate==high){
		state->SetLabel(_("On"));
	}else{
		state->SetLabel(_("Off"));
	}
}

////////// MONITOR PANEL ////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(MonitorPanel, MyPanel)
  EVT_BUTTON(MONITORADD_BUTTON_ID,MonitorPanel::OnAdd)
  EVT_BUTTON(MONITORREMOVE_BUTTON_ID, MonitorPanel::OnRemove)
  EVT_CHOICE(MONITORADDDEV_CHOICE_ID, MonitorPanel::OnDeviceSelect)
END_EVENT_TABLE()

MonitorPanel::MonitorPanel(wxWindow *parent, const wxString& title, const wxPoint& pos, names *names_mod, devices *devices_mod, monitor *monitor_mod, MyFrame *frame, long style):
	MyPanel(parent, wxID_ANY, title, pos,wxSize(300,150), style),frame(frame) {

  //instantiate all objects
  sigs = new vector<name>;
  monnames = new vector<name*>;
  devios = new vector<devio*>;
  removeChoice = new wxChoice(this,wxID_ANY);
  addDevice = new wxChoice(this,MONITORADDDEV_CHOICE_ID);
  addSignal = new wxChoice(this,wxID_ANY);

  //Load up all strings into choices
  refresh(names_mod, devices_mod,monitor_mod); 
  
  //place components
  wxBoxSizer *mainsizer = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer *add = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer *remove = new wxBoxSizer(wxHORIZONTAL);
  mainsizer->Add(add, 0, wxALIGN_CENTRE);
  mainsizer->Add(remove, 0, wxALIGN_CENTRE);
  add->Add(addDevice,0,wxLEFT|wxTOP,20);
  add->Add(addSignal,0,wxALL,20);
  add->Add(new wxButton(this, MONITORADD_BUTTON_ID, _("Add Monitor")), 0, wxRIGHT|wxTOP, 20);
  remove->Add(removeChoice,0,wxALL,20);
  remove->Add(new wxButton(this, MONITORREMOVE_BUTTON_ID, _("Remove Monitor")), 0, wxRIGHT|wxTOP|wxBOTTOM , 20);
  SetSizer(mainsizer);
}

void MonitorPanel::refresh(names *names_mod, devices *devices_mod, monitor *monitor_mod)
	//Resets the strings in the choice boxes 
{
	mons = monitor_mod;
	nmz = names_mod;	
	removeChoice->Clear();
	addDevice->Clear();
	//load monitors
  int monitors = mons->moncount();
  name dev, outp, *monname;
  namestring displayname;
  deletevectorarrays(monnames);
  monnames = new vector<name*>;
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
  deletevector(devios);
  devios = new vector<devio*>;
  for (devlink d = devs; d != NULL; d = d->next){ 
    devios->push_back(new devio(d->id,d->ilist,d->olist));
    addDevice->Append(wxString::FromUTF8(nmz->getname(d->id).c_str()));
  }
  addDevice->SetSelection(0);
  wxCommandEvent event;
  OnDeviceSelect(event);
}

void MonitorPanel::OnAdd(wxCommandEvent &event)
	//Callback for Add monitor button
{
  int devindex = addDevice->GetSelection();
  int sigindex = addSignal->GetSelection();
  name dev = devios->at(devindex)->id;
  name sig = sigs->at(sigindex);
  bool ok = true;
  for(size_t i=0; i<monnames->size();i++){
	  if(monnames->at(i)[0]==dev && monnames->at(i)[1]==sig) ok = false;
  }
  if(!ok){
	  wxMessageDialog warning(this, _("Monitor already exists"), _("Warning"), wxICON_ERROR | wxOK);
    warning.ShowModal();
	return;
  }
  mons->makemonitor(dev,sig,ok);
  if(ok){
    cout << s_("Added Monitor: ") << nmz->getname(dev)<< " . " << nmz->getname(sig) << endl;
    frame->reset();
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
  }else{
    wxMessageDialog warning(this, _("Max number of monitors reached: 10"), _("Warning"), wxICON_ERROR | wxOK);
    warning.ShowModal();
  }
}

void MonitorPanel::OnRemove(wxCommandEvent &event)
	//Callback for remove monitor button
{
  frame->reset();
  int index = removeChoice->GetSelection();
  if(index!=-1){
    removeChoice->Delete(index);
    removeChoice->SetSelection(0);
    name* devsig = monnames->at(index);
    monnames->erase(monnames->begin()+index);
    bool ok;
    mons->remmonitor(devsig[0],devsig[1],ok);
  }else{
    wxMessageDialog warning(this, _("No monitors to remove"), _("Warning"), wxICON_ERROR | wxOK);
    warning.ShowModal();
  }
}

void MonitorPanel::OnDeviceSelect(wxCommandEvent &event)
	//Callback for device select to set up output choice box
{
  int index = addDevice->GetSelection();
  devio *dev = devios->at(index);
  addSignal->Clear();
  sigs->clear();
  for (outplink o = dev->olist; o != NULL; o = o->next){ 
    addSignal->Append(wxString::FromUTF8(nmz->getname(o->id).c_str()));
    sigs->push_back(o->id);
  }
  addSignal->SetSelection(0);
}

////////// CONSOLE PANEL ////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(ConsolePanel, MyPanel)
	EVT_BUTTON(CONSOLESAVE_BUTTON_ID, ConsolePanel::OnSave)
	EVT_BUTTON(CONSOLECANCEL_BUTTON_ID, ConsolePanel::OnCancel)
END_EVENT_TABLE()

ConsolePanel::ConsolePanel(wxWindow *parent, const wxString& title, const wxPoint& pos, MyFrame *frame,
	  long style):MyPanel(parent, wxID_ANY, title, pos,wxSize(300,150), style),frame(frame)
{
  //instantiate all objects
  consoleSize = new wxChoice(this,wxID_ANY);
  textSize = new wxChoice(this,wxID_ANY);
  csize=1;
  tsize=1; 
  //Load up all strings into choices
  consoleSize ->Append(_("Small"));
  consoleSize ->Append(_("Medium"));
  consoleSize ->Append(_("Large"));
  textSize->Append(wxT("6"));
  textSize->Append(wxT("8"));
  textSize->Append(wxT("10"));
  textSize->Append(wxT("12"));
  consoleSize->SetSelection(csize);
  textSize->SetSelection(tsize);
  //store values in arrays for quick access
  csizes=new int[3];
  tsizes=new int[4];
  csizes[0]=80;
  csizes[1]=120;
  csizes[2]=180;
  tsizes[0]=6;
  tsizes[1]=8;
  tsizes[2]=10;
  tsizes[3]=12;
  //place components
  wxBoxSizer *mainsizer = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer *top = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer *middle = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer *buttons = new wxBoxSizer(wxHORIZONTAL);
  mainsizer->Add(top, 0, wxALIGN_CENTRE|wxEXPAND);
  mainsizer->Add(middle, 0, wxALIGN_CENTRE|wxEXPAND);
  mainsizer->Add(buttons, 0, wxALIGN_BOTTOM|wxALIGN_RIGHT);
  top->Add(new wxStaticText(this, wxID_ANY, _("Console Height:")), 0, wxTOP|wxLEFT|wxRIGHT, 20);
  top->Add(consoleSize,0,wxTOP,20);
  middle->Add(new wxStaticText(this, wxID_ANY, _("Text Size:")), 0, wxTOP|wxLEFT|wxRIGHT, 20);
  middle->Add(0,0,0,wxLEFT,28);
  middle->Add(textSize,0,wxTOP,20);
  buttons->Add(new wxButton(this, CONSOLESAVE_BUTTON_ID, _("Save")), 0, wxRIGHT|wxTOP, 20);
  buttons->Add(new wxButton(this, CONSOLECANCEL_BUTTON_ID, _("Cancel")), 0, wxRIGHT|wxTOP, 20);
  SetSizer(mainsizer);
}

void ConsolePanel::OnSave(wxCommandEvent &event)
	//Callback for save button
{
	csize = consoleSize->GetSelection();
	tsize = textSize->GetSelection();
	frame->consoleSettings(csizes[csize], tsizes[tsize]);
	Show(false);
}

void ConsolePanel::OnCancel(wxCommandEvent &event)
// Callback for cancel button
{
	consoleSize->SetSelection(csize);
	textSize->SetSelection(tsize);
	Show(false);
}
