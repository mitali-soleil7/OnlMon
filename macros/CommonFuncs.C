#ifndef MACROS_COMMONFUNCS_C
#define MACROS_COMMONFUNCS_C

#include <onlmon/OnlMonClient.h>

// cppcheck-suppress unknownMacro
R__LOAD_LIBRARY(libonlmonclient.so)

void CreateHostList(const int online = 0)
{
  OnlMonClient *cl = OnlMonClient::instance();
  if (!online)
  {
    cl->AddServerHost("localhost");  // check local host first
    for (int i = 2061; i <= 2068; i++)
    {
      string node = "rcas" + to_string(i);
      cl->AddServerHost(node);  // put all va machines in search list
    }
    for (int i = 1; i <= 8; i++)
    {
      string node = "sphnx0" + to_string(i);
      cl->AddServerHost(node);  // put all va machines in search list
    }
    cl->AddServerHost("sphnxdev01");
  }
  else if (online == 1)
    {
      const char *hostlist = gSystem->Getenv("ONLMON_RUNDIR");
      char hostlistname[200];
      if (hostlist)
	{
          char node[20];
	  sprintf(hostlistname, "%s/monitoring_hosts.list", hostlist);
	  FILE *f = fopen(hostlistname, "r");
	  while (fscanf(f, "%19s", &node[0]) != EOF)
	    {
	      cout << "adding " << node << endl;
	      cl->AddServerHost(node);      // put monitoring machines in search list
	    }
	}
    }
  else
  {
    cl->AddServerHost("localhost");  // check only local host
  }
}

void CleanUpClient()
{
  OnlMonClient *cl = OnlMonClient::instance();
  delete cl;
  gSystem->Exit(0);
  return;
}

void ClearCanvases()
{
  TSeqCollection* allCanvases = gROOT->GetListOfCanvases();
  TCanvas* canvas = nullptr;
  while ((canvas = (TCanvas*) allCanvases->First()))
  {
    std::cout << "Deleting Canvas " << canvas->GetName() << std::endl;
    delete canvas;
  }
}

void CreateSubsysHostlist(const std::string &list, const int online)
{
  if (online != 1)
  {
    CreateHostList(online);
    return;
  }
  OnlMonClient *cl = OnlMonClient::instance();  // get pointer to framewrk
  const char *hostlistdir = gSystem->Getenv("ONLMON_RUNDIR");
  char hostlistname[200];
  if (hostlistdir)
  {
    char node[20];
    sprintf(hostlistname, "%s/%s", hostlistdir,list.c_str());
    cout << "trying to open " << hostlistname << endl;
    FILE *f = fopen(hostlistname, "r");
    if (! f)
    {
      CreateHostList(online);
      return;
    }
    while (fscanf(f, "%19s", &node[0]) != EOF)
    {
      cout << "adding " << node << endl;
      cl->AddServerHost(node);      // put monitoring machines in search list
    }
    fclose(f);
  }
}

#endif
