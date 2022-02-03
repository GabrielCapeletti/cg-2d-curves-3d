#include "windowGUI.h"

WindowGUI::WindowGUI(int argc, char *argv[])
{

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title(GTK_WINDOW(window), "CG");

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    movementGrid = gtk_grid_new();

}

void WindowGUI::AddMenu(Menu m)
{
    m.SetMenu(movementGrid);
}

void WindowGUI::AddCanvas(Canvas *c)
{
    c->SetCanvas(movementGrid);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
}

void WindowGUI::Show()
{
    gtk_container_add(GTK_CONTAINER(window), movementGrid);

    gtk_widget_show_all(window);

    gtk_main();
}