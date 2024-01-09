// credit to kui: https://gist.github.com/kui/2622504

#include <stdio.h>
#include <stdlib.h>


#include <X11/Xlib.h>           // `apt-get install libx11-dev`
#include <X11/Xmu/WinUtil.h>    // `apt-get install libxmu-dev`
#include "window-stuff.h"

Bool xerror = False;

// get the top window.
// a top window have the following specifications.
//  * the start window is contained the descendent windows.
//  * the parent window is the root window.
Window get_top_window(Display* d, Window start){
  Window w = start;
  Window parent = start;
  Window root = None;
  Window *children;
  unsigned int nchildren;
  Status s;

  //printf("getting top window ... \n");
  while (parent != root) {
    w = parent;
    s = XQueryTree(d, w, &root, &parent, &children, &nchildren); // see man

    if (s)
      XFree(children);

    if(xerror){
      //printf("fail\n");
      exit(1);
    }

    //printf("  get parent (window: %d)\n", (int)w);
  }

  //printf("success (window: %d)\n", (int)w);

  return w;
}

// search a named window (that has a WM_STATE prop)
// on the descendent windows of the argment Window.
Window get_named_window(Display* d, Window start){
  Window w;
  //printf("getting named window ... ");
  w = XmuClientWindow(d, start); // see man
  if(w == start)
    //printf("fail\n");
  //printf("success (window: %d)\n", (int) w);
  return w;
}

char* get_window_name(Display* d, Window w){
  XTextProperty prop;
  Status s;

  //printf("window name:\n");

  s = XGetWMName(d, w, &prop); // see man
  if(!xerror && s){
    int count = 0, result;
    char **list = NULL;
    result = XmbTextPropertyToTextList(d, &prop, &list, &count); // see man
    if(result == Success){
      return prop.value;
    }else{
      //printf("ERROR: XmbTextPropertyToTextList\n");
    }
  }else{
    //printf("ERROR: XGetWMName\n");
  }

  return "";
}

char* get_window_res_name(Display* d, Window w){
  Status s;
  XClassHint* class;

  //printf("application: \n");

  class = XAllocClassHint(); // see man
  if(xerror){
    //printf("ERROR: XAllocClassHint\n");
  }

  s = XGetClassHint(d, w, class); // see man
  if(xerror || s){
    return class->res_name;
    //printf("\tname: %s\n\tclass: %s\n", class->res_name, class->res_class);
  }else{
    //printf("ERROR: XGetClassHint\n");
  }
}

