#include <gtk/gtk.h>
#include "windowGUI.h"
#include <locale.h>

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "usa");

    WindowGUI w(argc, argv);
    Menu m;
    Canvas *c = new Canvas();

    m.SetCanvas(c);

    w.AddMenu(m);
    w.AddCanvas(c);
    w.Show();

    return 0;
}
