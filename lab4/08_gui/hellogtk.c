#include <gtk/gtk.h>
/* 콜백함수*/
static void destroy(GtkWidget *widget, gpointer data)
{
    gtk_main_quit();
}
int main(int argc, char *argv[])
{
    /* GtkWidget은위젯들에대한저장타입임. */
    GtkWidget *window, *button;
    gtk_init(&argc, &argv);
    /* 새로운윈도우를생성하고테두리폭설정*/
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    /* “Hello World” 라는라벨을가진버튼생성*/
    button = gtk_button_new_with_label("Hello World");
    /* 버튼의clicked 시그널과destroy 콜백함수를연결*/
    g_signal_connect(button, "clicked", G_CALLBACK(destroy), NULL);
    /* 버튼을gtk컨테이너위젯인윈도우에패킹함*/
    gtk_container_add(GTK_CONTAINER(window), button);
    /* 생성한모든위젯들을화면에표시*/
    gtk_widget_show(button);
    gtk_widget_show(window);
    /* 무한루프를돌며시그널과이벤트를처리하는메인함수실행*/
    gtk_main();
    return 0;
}
