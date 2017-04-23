#include <gtk/gtk.h>

int main(int argc, char * argv[]){
	gtk_init(&argc, &argv);//初始化，设置缺省值，检查及完成工作

	//创建一个窗口，返回指针
	GtkWidget * window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	//title
	gtk_window_set_title(GTK_WINDOW(window), "demo");
	
	//行编辑
	GtkWidget * entry = gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(entry), "2+2=4");
	gtk_editable_set_editable(GTK_EDITABLE(entry), FALSE);

	//table
	GtkWidget * table = gtk_table_new(5, 4, TRUE);
//	GtkWidget * fix = gtk_fixed_new();

	//add in window
	gtk_container_add(GTK_CONTAINER(window), table);

	//创建按钮并添加到容器
//	GtkWidget * button2 = gtk_button_new_with_label("Click2");
//	gtk_fixed_put(GTK_FIXED(fix), button2, 0, 0);//放置起点
	gtk_table_attach_defaults(GTK_TABLE(table), entry, 0, 4, 0, 1);
	GtkWidget * button = gtk_button_new_with_label("7");
	gtk_table_attach_defaults(GTK_TABLE(table), button, 0, 1, 1, 2);
	button = gtk_button_new_with_label("8");
	gtk_table_attach_defaults(GTK_TABLE(table), button, 1, 2, 1, 2);
	button = gtk_button_new_with_label("9");
	gtk_table_attach_defaults(GTK_TABLE(table), button, 2, 3, 1, 2);
	button = gtk_button_new_with_label("c");
	gtk_table_attach_defaults(GTK_TABLE(table), button, 3, 4, 1, 2);
	button = gtk_button_new_with_label("4");
	gtk_table_attach_defaults(GTK_TABLE(table), button, 0, 1, 2, 3);
	button = gtk_button_new_with_label("5");
	gtk_table_attach_defaults(GTK_TABLE(table), button, 1, 2, 2, 3);
	button = gtk_button_new_with_label("6");
	gtk_table_attach_defaults(GTK_TABLE(table), button, 2, 3, 2, 3);
	button = gtk_button_new_with_label("*");
	gtk_table_attach_defaults(GTK_TABLE(table), button, 3, 4, 2, 3);
	button = gtk_button_new_with_label("1");
	gtk_table_attach_defaults(GTK_TABLE(table), button, 0, 1, 3, 4);
	button = gtk_button_new_with_label("2");
	gtk_table_attach_defaults(GTK_TABLE(table), button, 1, 2, 3, 4);
	button = gtk_button_new_with_label("3");
	gtk_table_attach_defaults(GTK_TABLE(table), button, 2, 3, 3, 4);
	button = gtk_button_new_with_label("/");
	gtk_table_attach_defaults(GTK_TABLE(table), button, 3, 4, 3, 4);
	button = gtk_button_new_with_label("0");
	gtk_table_attach_defaults(GTK_TABLE(table), button, 0, 1, 4, 5);
	button = gtk_button_new_with_label("+");
	gtk_table_attach_defaults(GTK_TABLE(table), button, 1, 2, 4, 5);
	button = gtk_button_new_with_label("-");
	gtk_table_attach_defaults(GTK_TABLE(table), button, 2, 3, 4, 5);
	button = gtk_button_new_with_label("=");
	gtk_table_attach_defaults(GTK_TABLE(table), button, 3, 4, 4, 5);
	
	//移动click2
//	gtk_fixed_move(GTK_FIXED(fix), button2, 150, 150);
	//设置大小
//	gtk_widget_set_size_request(button2, 100, 50);

	//显示控件
	//gtk_widget_show(window);
	gtk_widget_show_all(window);

	gtk_main();//主事件循环，监听鼠标键盘

	return 0;
}
