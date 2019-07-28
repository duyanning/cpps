
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Text_Editor.H>
#include "ui.h"  // using nocheck ui.cxx

// cpps-make ui.cxx ui.h : ui.fl // fluid -c ui.fl

// linklib fltk
// vc-extra-compile-flags: -DWIN32
// mingw-extra-compile-flags: -DWIN32
// mingw-extra-link-flags: -mwindows -lole32 -luuid -lcomctl32

int main() {
  Fl_Double_Window* win = make_window();
  win->show();
	return(Fl::run());
}

