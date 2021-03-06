#include <gtk/gtk.h>
#include <string.h>

char buf[50] = " ";

void deal_num(GtkButton * button, gpointer data){
	const char * text = gtk_button_get_label(button);
	if (strcmp(text, "c") == 0){
		buf[strlen(buf) - 1] = 0;		
	}
	else{
		int a,b;
		char c;
		if(!strcmp(text, "=") == 0){
			strcat(buf, text);
		}
		else{
			sscanf(buf, "%d%c%d", &a, &c, &b);//&
			if(c == '+'){
				sprintf(buf, "%d", a+b);
			}
			if(c == '-'){
				sprintf(buf, "%d", a-b);
			}
			if(c == '*'){
				sprintf(buf, "%d", a*b);
			}
			if(c == '/'){
				sprintf(buf, "%d", a/b);
			}
		}
	}
	gtk_entry_set_text(GTK_ENTRY(data), buf);
}

void closeapp(GtkWidget * window, gpointer data){
	gtk_main_quit();
}

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
	GtkWidget * button7 = gtk_button_new_with_label("7");
	gtk_table_attach_defaults(GTK_TABLE(table), button7, 0, 1, 1, 2);
	GtkWidget * button8 = gtk_button_new_with_label("8");
	gtk_table_attach_defaults(GTK_TABLE(table), button8, 1, 2, 1, 2);
	GtkWidget * button9 = gtk_button_new_with_label("9");
	gtk_table_attach_defaults(GTK_TABLE(table), button9, 2, 3, 1, 2);
	GtkWidget * buttonc = gtk_button_new_with_label("c");
	gtk_table_attach_defaults(GTK_TABLE(table), buttonc, 3, 4, 1, 2);
	GtkWidget * button4 = gtk_button_new_with_label("4");
	gtk_table_attach_defaults(GTK_TABLE(table), button4, 0, 1, 2, 3);
	GtkWidget * button5 = gtk_button_new_with_label("5");
	gtk_table_attach_defaults(GTK_TABLE(table), button5, 1, 2, 2, 3);
	GtkWidget * button6 = gtk_button_new_with_label("6");
	gtk_table_attach_defaults(GTK_TABLE(table), button6, 2, 3, 2, 3);
	GtkWidget * button_mul = gtk_button_new_with_label("*");
	gtk_table_attach_defaults(GTK_TABLE(table), button_mul, 3, 4, 2, 3);
	GtkWidget * button1 = gtk_button_new_with_label("1");
	gtk_table_attach_defaults(GTK_TABLE(table), button1, 0, 1, 3, 4);
	GtkWidget * button2 = gtk_button_new_with_label("2");
	gtk_table_attach_defaults(GTK_TABLE(table), button2, 1, 2, 3, 4);
	GtkWidget * button3 = gtk_button_new_with_label("3");
	gtk_table_attach_defaults(GTK_TABLE(table), button3, 2, 3, 3, 4);
	GtkWidget * button_div = gtk_button_new_with_label("/");
	gtk_table_attach_defaults(GTK_TABLE(table), button_div, 3, 4, 3, 4);
	GtkWidget * button0 = gtk_button_new_with_label("0");
	gtk_table_attach_defaults(GTK_TABLE(table), button0, 0, 1, 4, 5);
	GtkWidget * button_add = gtk_button_new_with_label("+");
	gtk_table_attach_defaults(GTK_TABLE(table), button_add, 1, 2, 4, 5);
	GtkWidget * button_dec = gtk_button_new_with_label("-");
	gtk_table_attach_defaults(GTK_TABLE(table), button_dec, 2, 3, 4, 5);
	GtkWidget * button_equ = gtk_button_new_with_label("=");
	gtk_table_attach_defaults(GTK_TABLE(table), button_equ, 3, 4, 4, 5);
	
	g_signal_connect(button0, "pressed", G_CALLBACK(deal_num), entry);
	g_signal_connect(button1, "pressed", G_CALLBACK(deal_num), entry);
	g_signal_connect(button2, "pressed", G_CALLBACK(deal_num), entry);
	g_signal_connect(button3, "pressed", G_CALLBACK(deal_num), entry);
	g_signal_connect(button4, "pressed", G_CALLBACK(deal_num), entry);
	g_signal_connect(button5, "pressed", G_CALLBACK(deal_num), entry);
	g_signal_connect(button6, "pressed", G_CALLBACK(deal_num), entry);
	g_signal_connect(button7, "pressed", G_CALLBACK(deal_num), entry);
	g_signal_connect(button8, "pressed", G_CALLBACK(deal_num), entry);
	g_signal_connect(button9, "pressed", G_CALLBACK(deal_num), entry);
	g_signal_connect(button_add, "pressed", G_CALLBACK(deal_num), entry);
	g_signal_connect(button_dec, "pressed", G_CALLBACK(deal_num), entry);
	g_signal_connect(button_mul, "pressed", G_CALLBACK(deal_num), entry);
	g_signal_connect(button_div, "pressed", G_CALLBACK(deal_num), entry);
	g_signal_connect(button_equ, "pressed", G_CALLBACK(deal_num), entry);
	g_signal_connect(buttonc, "pressed", G_CALLBACK(deal_num), entry);
	g_signal_connect(window, "destroy", G_CALLBACK(closeapp), entry);

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
