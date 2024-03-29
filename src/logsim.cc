#include "logsim.h"
#include "userint.h"
#include "gui.h"
#include <GL/glut.h>

#define USE_GUI

IMPLEMENT_APP(MyApp)
  
bool MyApp::OnInit()
  // This function is automatically called when the application starts
{
  if (argc > 2) { // check we have one command line argument
    wcout << "Usage:      " << argv[0] << " [filename]" << endl;
    exit(1);
  }
 
  // Construct the six classes required by the innards of the logic simulator
  nmz = new names();
  netz = new network(nmz);
  dmz = new devices(nmz, netz);
  mmz = new monitor(nmz, netz);
  if(argc==2){
    smz = new scanner(nmz, wxString(argv[1]).mb_str());
    pmz = new parser(netz, dmz, mmz, smz);
    
    if (pmz->readin ()) { // check the logic file parsed correctly
#ifdef USE_GUI
      // glutInit cannot cope with Unicode command line arguments, so we pass
      // it some fake ASCII ones instead
      char **tmp1; int tmp2 = 0; glutInit(&tmp2, tmp1);
      //char *tmp1[]={"My app", NULL }; int tmp2 = 1; glutInit(&tmp2, tmp1);//needed to compile on windows
      // Construct the GUI
      wxTextCtrl *console = new wxTextCtrl();
      cout.rdbuf(console);
      MyFrame *frame = new MyFrame(NULL, wxT("Logic simulator"), wxDefaultPosition,  wxSize(800, 600), nmz, dmz, mmz, netz, console);
      frame->Show(true);
      return(true); // enter the GUI event loop
#else
      // Construct the text-based interface
      userint umz(nmz, dmz, mmz);
      umz.userinterface();
#endif /* USE_GUI */
    }
  }else{
    bool ok;
    name id = nmz -> newname("No Devices");
    dmz->makedevice(aswitch,id,low,ok);
    char **tmp1; int tmp2 = 0; glutInit(&tmp2, tmp1);
    //char *tmp1[]={"My app", NULL }; int tmp2 = 1; glutInit(&tmp2, tmp1);//needed to compile on windows
	// Construct the GUI
    wxTextCtrl *console = new wxTextCtrl();
    cout.rdbuf(console);
    MyFrame *frame = new MyFrame(NULL, wxT("Logic simulator"), wxDefaultPosition,  wxSize(800, 600), nmz, dmz, mmz, netz, console);
    frame->Show(true);
    return(true); // enter the GUI event loop
  }
  return(false); // exit the application
}
