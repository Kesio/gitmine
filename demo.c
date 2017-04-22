#include <gtk/gtk.h>

int main(int argc, char * argv[]){
	gtk_init(&argc, &argv);//初始化，设置缺省值，检查及完成工作

	//创建一个窗口，返回指针
	GtkWidget * window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	//创建按钮
	GtkWidget * button = gtk_button_new_with_label("Click");

	//添加到窗口
	//gtk_container_add((GtkContainer *)window, button);
	gtk_container_add(GTK_CONTAINER(window), button);

	//显示控件
	gtk_widget_show(button);
	//显示控件
	gtk_widget_show(window);


	gtk_main();//主事件循环，监听鼠标键盘

	return 0;
}
