#include <gtk/gtk.h>
#include <stdlib.h>

// 버튼 클릭 시 호출되는 함수
void on_button_clicked(GtkWidget *button, gpointer data) {
    const char *input1 = gtk_entry_get_text(GTK_ENTRY(((GtkWidget **)data)[0]));
    const char *input2 = gtk_entry_get_text(GTK_ENTRY(((GtkWidget **)data)[1]));
    GtkWidget *result_label = ((GtkWidget **)data)[2];

    // 두 입력값을 숫자로 변환
    double num1 = atof(input1);
    double num2 = atof(input2);
    const char *operation = gtk_button_get_label(GTK_BUTTON(button));
    char result_text[100];

    // 연산 수행
    if (strcmp(operation, "+") == 0) {
        sprintf(result_text, "Result: %.2f", num1 + num2);
    } else if (strcmp(operation, "-") == 0) {
        sprintf(result_text, "Result: %.2f", num1 - num2);
    } else if (strcmp(operation, "x") == 0) {
        sprintf(result_text, "Result: %.2f", num1 * num2);
    } else if (strcmp(operation, "/") == 0) {
        if (num2 != 0)
            sprintf(result_text, "Result: %.2f", num1 / num2);
        else
            sprintf(result_text, "Error: Division by zero!");
    }

    // 결과 표시
    gtk_label_set_text(GTK_LABEL(result_label), result_text);
}

int main(int argc, char *argv[]) {
    GtkWidget *window, *grid;
    GtkWidget *entry1, *entry2, *result_label;
    GtkWidget *add_button, *sub_button, *mul_button, *div_button;

    gtk_init(&argc, &argv);

    // 윈도우 생성 및 설정
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "계산기");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // 그리드 레이아웃 생성
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // 입력 필드 생성
    entry1 = gtk_entry_new();
    entry2 = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Input 1:"), 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry1, 1, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Input 2:"), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry2, 1, 1, 2, 1);

    // 버튼 생성
    add_button = gtk_button_new_with_label("+");
    sub_button = gtk_button_new_with_label("-");
    mul_button = gtk_button_new_with_label("x");
    div_button = gtk_button_new_with_label("/");
    gtk_grid_attach(GTK_GRID(grid), add_button, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), sub_button, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), mul_button, 2, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), div_button, 3, 2, 1, 1);

    // 결과 레이블
    result_label = gtk_label_new("Result:");
    gtk_grid_attach(GTK_GRID(grid), result_label, 0, 3, 4, 1);

    // 버튼 클릭 이벤트 연결
    GtkWidget *data[3] = {entry1, entry2, result_label};
    g_signal_connect(add_button, "clicked", G_CALLBACK(on_button_clicked), data);
    g_signal_connect(sub_button, "clicked", G_CALLBACK(on_button_clicked), data);
    g_signal_connect(mul_button, "clicked", G_CALLBACK(on_button_clicked), data);
    g_signal_connect(div_button, "clicked", G_CALLBACK(on_button_clicked), data);

    // 모든 위젯 표시
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
