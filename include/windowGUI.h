#ifndef __WINDOWGUI_H_INCLUDED__
#define __WINDOWGUI_H_INCLUDED__

#include <gtk/gtk.h>
#include "menu.h"
#include "canvas.h"

class WindowGUI
{
public:
    GtkWidget *window;
    GtkWidget *movementGrid;

    WindowGUI(int argc, char *argv[]);

    void AddMenu(Menu m);

    void AddCanvas(Canvas *c);

    void Show();

private:

};

#endif