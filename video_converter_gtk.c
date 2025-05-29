#include <gtk/gtk.h> // Bao gồm thư viện GTK+
#include <stdio.h>   // Để sử dụng sprintf, sscanf
#include <stdlib.h>  // Để sử dụng strtod (chuyển đổi chuỗi sang double)
#include <string.h>  // Để sử dụng strcpy, strcat
#include <math.h>    // Để sử dụng round, fmod

// Biến toàn cục cho các widget và dữ liệu
GtkWidget *entry_hours;
GtkWidget *entry_minutes;
GtkWidget *entry_seconds;
GtkWidget *entry_custom_speed_factor; // Ô nhập liệu cho hệ số tùy chỉnh
GtkNotebook *notebook;

// Nhãn để hiển thị kết quả trong từng tab
GtkWidget *label_original_duration;
GtkWidget *label_x1_2_duration;
GtkWidget *label_x1_5_duration;
GtkWidget *label_x1_8_duration;
GtkWidget *label_x2_0_duration;
GtkWidget *label_custom_speed_duration; // Nhãn cho tốc độ tùy chỉnh

// Hàm chuyển đổi thời lượng từ giờ, phút, giây sang tổng số giây
long long convert_to_seconds(int hours, int minutes, int seconds) {
    return (long long)hours * 3600 + (long long)minutes * 60 + seconds;
}

// Hàm chuyển đổi tổng số giây thành định dạng giờ:phút:giây
void convert_to_hms(double total_seconds, int *hours, int *minutes, int *seconds) {
    *hours = (int)(total_seconds / 3600);
    *minutes = (int)((total_seconds - (*hours * 3600)) / 60);
    *seconds = (int)round(fmod(total_seconds, 60));
}

// Hàm cập nhật nội dung của một tab cụ thể
void update_tab_content(GtkWidget *label, const char *prefix, int h, int m, int s) {
    char buffer[256]; // Tăng kích thước buffer
    sprintf(buffer, "%s %02d:%02d:%02d", prefix, h, m, s);
    gtk_label_set_text(GTK_LABEL(label), buffer);
}

// Hàm trợ giúp để tạo một trang tab mới với label nội dung
GtkWidget* create_tab_page(const char* tab_label_text, GtkWidget** content_label_ptr) {
    GtkWidget *page_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(page_box), 15);
    
    *content_label_ptr = gtk_label_new("Chưa tính toán");
    gtk_label_set_justify(GTK_LABEL(*content_label_ptr), GTK_JUSTIFY_CENTER); // Căn giữa văn bản
    gtk_widget_set_halign(*content_label_ptr, GTK_ALIGN_CENTER); // Căn giữa theo chiều ngang
    gtk_widget_set_valign(*content_label_ptr, GTK_ALIGN_CENTER); // Căn giữa theo chiều dọc
    gtk_widget_set_hexpand(*content_label_ptr, TRUE); // Cho phép giãn nở theo chiều ngang
    gtk_widget_set_vexpand(*content_label_ptr, TRUE); // Cho phép giãn nở theo chiều dọc

    gtk_box_pack_start(GTK_BOX(page_box), *content_label_ptr, TRUE, TRUE, 0);
    
    GtkWidget *tab_label_widget = gtk_label_new(tab_label_text); // Nhãn cho tiêu đề tab
    gtk_notebook_append_page(notebook, page_box, tab_label_widget);
    return page_box;
}

// Callback function khi nút "Tính toán" được nhấn
static void calculate_button_clicked(GtkWidget *widget, gpointer data) {
    int hours, minutes, seconds;
    double custom_factor;
    long long original_total_seconds;
    char *text_hours, *text_minutes, *text_seconds, *text_custom_factor;

    // Lấy giá trị từ các ô nhập liệu
    text_hours = (char *)gtk_entry_get_text(GTK_ENTRY(entry_hours));
    text_minutes = (char *)gtk_entry_get_text(GTK_ENTRY(entry_minutes));
    text_seconds = (char *)gtk_entry_get_text(GTK_ENTRY(entry_seconds));
    text_custom_factor = (char *)gtk_entry_get_text(GTK_ENTRY(entry_custom_speed_factor));

    // Chuyển đổi chuỗi sang số nguyên và số thực
    if (sscanf(text_hours, "%d", &hours) != 1 ||
        sscanf(text_minutes, "%d", &minutes) != 1 ||
        sscanf(text_seconds, "%d", &seconds) != 1 ||
        sscanf(text_custom_factor, "%lf", &custom_factor) != 1) { // %lf cho double
        GtkWidget *dialog = gtk_message_dialog_new(
            GTK_WINDOW(gtk_widget_get_toplevel(widget)),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_CLOSE,
            "Lỗi đầu vào: Vui lòng nhập số nguyên cho giờ, phút, giây và số (thập phân) cho hệ số tua nhanh.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    // Kiểm tra giá trị hợp lệ
    if (minutes < 0 || minutes >= 60 ||
        seconds < 0 || seconds >= 60 ||
        hours < 0 ||
        custom_factor <= 0) { // Hệ số tua nhanh phải dương
        GtkWidget *dialog = gtk_message_dialog_new(
            GTK_WINDOW(gtk_widget_get_toplevel(widget)),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_CLOSE,
            "Lỗi đầu vào: Phút và giây phải từ 0-59. Giờ phải là số không âm. Hệ số tua nhanh phải là số dương.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    original_total_seconds = convert_to_seconds(hours, minutes, seconds);

    // Cập nhật tab "Thời lượng Gốc"
    update_tab_content(label_original_duration, "Thời lượng gốc:", hours, minutes, seconds);
    gtk_notebook_set_current_page(notebook, 0); // Chuyển sang tab gốc

    // Các hệ số tua nhanh cố định
    double speed_factors[] = {1.2, 1.5, 1.8, 2.0};
    GtkWidget *labels[] = {label_x1_2_duration, label_x1_5_duration, label_x1_8_duration, label_x2_0_duration};
    char *prefixes[] = {"Tốc độ x1.2:", "Tốc độ x1.5:", "Tốc độ x1.8:", "Tốc độ x2.0:"};
    int num_fixed_factors = sizeof(speed_factors) / sizeof(speed_factors[0]);

    for (int i = 0; i < num_fixed_factors; i++) {
        double sped_up_total_seconds = original_total_seconds / speed_factors[i];
        int new_h, new_m, new_s;
        convert_to_hms(sped_up_total_seconds, &new_h, &new_m, &new_s);
        update_tab_content(labels[i], prefixes[i], new_h, new_m, new_s);
    }

    // Cập nhật tab cho tốc độ tùy chỉnh
    double custom_sped_up_total_seconds = original_total_seconds / custom_factor;
    int custom_h, custom_m, custom_s;
    convert_to_hms(custom_sped_up_total_seconds, &custom_h, &custom_m, &custom_s);
    char custom_prefix[50];
    sprintf(custom_prefix, "Tốc độ x%.2f:", custom_factor); // Định dạng tùy chỉnh với 2 chữ số thập phân
    update_tab_content(label_custom_speed_duration, custom_prefix, custom_h, custom_m, custom_s);
}

// Hàm khởi tạo giao diện người dùng
static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *vbox_main;
    GtkWidget *input_grid;
    GtkWidget *button_calculate;
    GtkWidget *frame_notebook;

    // 1. Tạo cửa sổ chính
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Ứng dụng Chuyển đổi Thời lượng Video");
    gtk_window_set_default_size(GTK_WINDOW(window), 480, 550); // Tăng kích thước cửa sổ để chứa thêm ô nhập liệu và tab
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    // 2. Tạo hộp đứng chính
    vbox_main = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), vbox_main);

    // 3. Phần nhập liệu (GtkGrid)
    input_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(input_grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(input_grid), 5);
    gtk_box_pack_start(GTK_BOX(vbox_main), input_grid, FALSE, FALSE, 0);

    GtkWidget *label_input_title = gtk_label_new("<b>Nhập thời lượng video gốc:</b>");
    gtk_label_set_use_markup(GTK_LABEL(label_input_title), TRUE);
    gtk_grid_attach(GTK_GRID(input_grid), label_input_title, 0, 0, 2, 1);

    gtk_grid_attach(GTK_GRID(input_grid), gtk_label_new("Giờ:"), 0, 1, 1, 1);
    entry_hours = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(entry_hours), "0");
    gtk_grid_attach(GTK_GRID(input_grid), entry_hours, 1, 1, 1, 1);

    gtk_grid_attach(GTK_GRID(input_grid), gtk_label_new("Phút:"), 0, 2, 1, 1);
    entry_minutes = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(entry_minutes), "0");
    gtk_grid_attach(GTK_GRID(input_grid), entry_minutes, 1, 2, 1, 1);

    gtk_grid_attach(GTK_GRID(input_grid), gtk_label_new("Giây:"), 0, 3, 1, 1);
    entry_seconds = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(entry_seconds), "0");
    gtk_grid_attach(GTK_GRID(input_grid), entry_seconds, 1, 3, 1, 1);

    // Ô nhập liệu cho hệ số tua nhanh tùy chỉnh
    GtkWidget *label_custom_speed = gtk_label_new("<b>Hệ số tua nhanh tùy chỉnh (ví dụ: 1.75):</b>");
    gtk_label_set_use_markup(GTK_LABEL(label_custom_speed), TRUE);
    gtk_grid_attach(GTK_GRID(input_grid), label_custom_speed, 0, 4, 2, 1);

    entry_custom_speed_factor = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(entry_custom_speed_factor), "1.0"); // Mặc định là 1.0 (không tua)
    gtk_grid_attach(GTK_GRID(input_grid), entry_custom_speed_factor, 0, 5, 2, 1);

    // Nút "Tính toán"
    button_calculate = gtk_button_new_with_label("Tính toán Thời lượng Tua nhanh");
    g_signal_connect(button_calculate, "clicked", G_CALLBACK(calculate_button_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(vbox_main), button_calculate, FALSE, FALSE, 0);

    // 4. Khung chứa Notebook
    frame_notebook = gtk_frame_new(NULL);
    gtk_frame_set_shadow_type(GTK_FRAME(frame_notebook), GTK_SHADOW_ETCHED_IN);
    gtk_box_pack_start(GTK_BOX(vbox_main), frame_notebook, TRUE, TRUE, 0);

    // 5. Tạo Notebook (các tab)
    notebook = GTK_NOTEBOOK(gtk_notebook_new());
    gtk_container_add(GTK_CONTAINER(frame_notebook), GTK_WIDGET(notebook));

    // Tạo các trang tab cố định
    create_tab_page("Thời lượng Gốc", &label_original_duration);
    create_tab_page("Tốc độ x1.2", &label_x1_2_duration);
    create_tab_page("Tốc độ x1.5", &label_x1_5_duration);
    create_tab_page("Tốc độ x1.8", &label_x1_8_duration);
    create_tab_page("Tốc độ x2.0", &label_x2_0_duration);
    
    // Tạo trang tab cho tốc độ tùy chỉnh
    create_tab_page("Tốc độ Tùy chỉnh", &label_custom_speed_duration); // Tab mới cho tốc độ tùy chỉnh

    // 6. Hiển thị tất cả các widget
    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("com.yourcompany.videoconverterv2", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}