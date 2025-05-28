#include <gtk/gtk.h> // Bao gồm thư viện GTK+
#include <stdio.h>   // Để sử dụng sprintf
#include <stdlib.h>  // Để sử dụng strtod (chuyển đổi chuỗi sang double)
#include <string.h>  // Để sử dụng strcpy
#include <math.h>    // Để sử dụng round và fmod

// Các biến toàn cục để truy cập các widget từ các hàm khác nhau
GtkWidget *entry_hours;
GtkWidget *entry_minutes;
GtkWidget *entry_seconds;
GtkNotebook *notebook;

// Các nhãn để hiển thị kết quả trong từng tab
GtkWidget *label_original_duration;
GtkWidget *label_x1_2_duration;
GtkWidget *label_x1_5_duration;
GtkWidget *label_x1_8_duration;
GtkWidget *label_x2_0_duration;

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
    char buffer[100];
    sprintf(buffer, "%s %02d:%02d:%02d", prefix, h, m, s);
    gtk_label_set_text(GTK_LABEL(label), buffer);
}

// Callback function khi nút "Tính toán" được nhấn
static void calculate_button_clicked(GtkWidget *widget, gpointer data) {
    int hours, minutes, seconds;
    long long original_total_seconds;
    char *text_hours, *text_minutes, *text_seconds;

    // Lấy giá trị từ các ô nhập liệu
    text_hours = (char *)gtk_entry_get_text(GTK_ENTRY(entry_hours));
    text_minutes = (char *)gtk_entry_get_text(GTK_ENTRY(entry_minutes));
    text_seconds = (char *)gtk_entry_get_text(GTK_ENTRY(entry_seconds));

    // Chuyển đổi chuỗi sang số nguyên
    // Sử dụng sscanf để an toàn hơn với đầu vào không phải số
    if (sscanf(text_hours, "%d", &hours) != 1 ||
        sscanf(text_minutes, "%d", &minutes) != 1 ||
        sscanf(text_seconds, "%d", &seconds) != 1) {
        GtkWidget *dialog = gtk_message_dialog_new(
            GTK_WINDOW(gtk_widget_get_toplevel(widget)),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_CLOSE,
            "Lỗi đầu vào: Vui lòng nhập số nguyên cho giờ, phút và giây.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    // Kiểm tra giá trị hợp lệ
    if (minutes < 0 || minutes >= 60 ||
        seconds < 0 || seconds >= 60 ||
        hours < 0) {
        GtkWidget *dialog = gtk_message_dialog_new(
            GTK_WINDOW(gtk_widget_get_toplevel(widget)),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_CLOSE,
            "Lỗi đầu vào: Phút và giây phải từ 0-59. Giờ phải là số không âm.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    original_total_seconds = convert_to_seconds(hours, minutes, seconds);

    // Cập nhật tab "Thời lượng Gốc"
    update_tab_content(label_original_duration, "Thời lượng gốc:", hours, minutes, seconds);
    gtk_notebook_set_current_page(notebook, 0); // Chuyển sang tab gốc

    // Các hệ số tua nhanh
    double speed_factors[] = {1.2, 1.5, 1.8, 2.0};
    GtkWidget *labels[] = {label_x1_2_duration, label_x1_5_duration, label_x1_8_duration, label_x2_0_duration};
    char *prefixes[] = {"Tốc độ x1.2:", "Tốc độ x1.5:", "Tốc độ x1.8:", "Tốc độ x2.0:"};
    int num_factors = sizeof(speed_factors) / sizeof(speed_factors[0]);

    for (int i = 0; i < num_factors; i++) {
        double sped_up_total_seconds = original_total_seconds / speed_factors[i];
        int new_h, new_m, new_s;
        convert_to_hms(sped_up_total_seconds, &new_h, &new_m, &new_s);
        update_tab_content(labels[i], prefixes[i], new_h, new_m, new_s);
    }
}

// Hàm khởi tạo giao diện người dùng
static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *vbox_main; // Hộp chính chứa tất cả
    GtkWidget *input_grid; // Lưới cho phần nhập liệu
    GtkWidget *button_calculate;
    GtkWidget *frame_notebook; // Khung cho Notebook

    // 1. Tạo cửa sổ chính
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Ứng dụng Chuyển đổi Thời lượng Video (C + GTK)");
    gtk_window_set_default_size(GTK_WINDOW(window), 450, 450);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    // 2. Tạo hộp đứng chính để sắp xếp các phần tử
    vbox_main = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), vbox_main);

    // 3. Phần nhập liệu (sử dụng GtkGrid để căn chỉnh tốt hơn)
    input_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(input_grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(input_grid), 5);
    gtk_box_pack_start(GTK_BOX(vbox_main), input_grid, FALSE, FALSE, 0); // FALSE, FALSE để không giãn nở

    // Nhãn "Nhập thời lượng video gốc"
    GtkWidget *label_input_title = gtk_label_new("<b>Nhập thời lượng video gốc:</b>");
    gtk_label_set_use_markup(GTK_LABEL(label_input_title), TRUE); // Cho phép dùng markup (bold)
    gtk_grid_attach(GTK_GRID(input_grid), label_input_title, 0, 0, 2, 1); // Cột 0, Hàng 0, chiếm 2 cột, 1 hàng

    // Hàng giờ
    gtk_grid_attach(GTK_GRID(input_grid), gtk_label_new("Giờ:"), 0, 1, 1, 1);
    entry_hours = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(entry_hours), "0"); // Giá trị mặc định
    gtk_grid_attach(GTK_GRID(input_grid), entry_hours, 1, 1, 1, 1);

    // Hàng phút
    gtk_grid_attach(GTK_GRID(input_grid), gtk_label_new("Phút:"), 0, 2, 1, 1);
    entry_minutes = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(entry_minutes), "0"); // Giá trị mặc định
    gtk_grid_attach(GTK_GRID(input_grid), entry_minutes, 1, 2, 1, 1);

    // Hàng giây
    gtk_grid_attach(GTK_GRID(input_grid), gtk_label_new("Giây:"), 0, 3, 1, 1);
    entry_seconds = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(entry_seconds), "0"); // Giá trị mặc định
    gtk_grid_attach(GTK_GRID(input_grid), entry_seconds, 1, 3, 1, 1);

    // Nút "Tính toán"
    button_calculate = gtk_button_new_with_label("Tính toán Thời lượng Tua nhanh");
    // Kết nối tín hiệu "clicked" của nút với hàm callback
    g_signal_connect(button_calculate, "clicked", G_CALLBACK(calculate_button_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(vbox_main), button_calculate, FALSE, FALSE, 0);

    // 4. Khung cho Notebook (để nó có viền và khoảng cách)
    frame_notebook = gtk_frame_new(NULL);
    gtk_frame_set_shadow_type(GTK_FRAME(frame_notebook), GTK_SHADOW_ETCHED_IN);
    gtk_box_pack_start(GTK_BOX(vbox_main), frame_notebook, TRUE, TRUE, 0); // TRUE, TRUE để giãn nở

    // 5. Tạo Notebook (các tab)
    notebook = GTK_NOTEBOOK(gtk_notebook_new());
    gtk_container_add(GTK_CONTAINER(frame_notebook), GTK_WIDGET(notebook)); // Thêm notebook vào frame

    // Hàm trợ giúp để tạo một trang tab mới với label
    GtkWidget* create_tab_page(const char* tab_label_text, GtkWidget** content_label_ptr) {
        GtkWidget *page_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
        gtk_container_set_border_width(GTK_CONTAINER(page_box), 15); // Padding cho nội dung tab
        *content_label_ptr = gtk_label_new("Chưa tính toán"); // Nhãn nội dung của tab
        gtk_widget_set_halign(*content_label_ptr, GTK_ALIGN_CENTER); // Căn giữa nội dung
        gtk_widget_set_valign(*content_label_ptr, GTK_ALIGN_CENTER);
        gtk_box_pack_start(GTK_BOX(page_box), *content_label_ptr, TRUE, TRUE, 0);

        GtkWidget *tab_label = gtk_label_new(tab_label_text); // Nhãn cho tiêu đề tab
        gtk_notebook_append_page(notebook, page_box, tab_label);
        return page_box;
    }

    // Tạo các trang tab
    create_tab_page("Thời lượng Gốc", &label_original_duration);
    create_tab_page("Tốc độ x1.2", &label_x1_2_duration);
    create_tab_page("Tốc độ x1.5", &label_x1_5_duration);
    create_tab_page("Tốc độ x1.8", &label_x1_8_duration);
    create_tab_page("Tốc độ x2.0", &label_x2_0_duration);


    // 6. Hiển thị tất cả các widget
    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    // Khởi tạo ứng dụng GTK+
    app = gtk_application_new("com.yourcompany.videoconverter", G_APPLICATION_DEFAULT_FLAGS);
    // Kết nối tín hiệu "activate" với hàm activate để tạo GUI
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    // Chạy ứng dụng GTK+
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app); // Giải phóng đối tượng ứng dụng

    return status;
}