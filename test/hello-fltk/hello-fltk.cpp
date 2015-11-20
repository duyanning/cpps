#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>

// linklib fltk

int main(int argc, char **argv)
{
    Fl_Window* window = new Fl_Window(340,180);
    //Fl_Box* box = new Fl_Box(20,40,300,100,"Hello, World!");
    Fl_Button *button = new Fl_Button(20, 40, 300, 100, "hello");
    //button->type(FL_TOGGLE_BUTTON);
    // box->box(FL_UP_BOX);
    // box->labelfont(FL_BOLD+FL_ITALIC);
    // box->labelsize(36);
    // box->labeltype(FL_SHADOW_LABEL);
    window->end();
    window->show(argc, argv);
    return Fl::run();
}
