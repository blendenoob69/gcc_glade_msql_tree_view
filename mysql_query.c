// mysql		OPENSUSE: mariadb
// mysqlclient
// mysql-devel
// mysql-server
#include <gtk/gtk.h>
#include <mysql/mysql.h>
#include <mysql/mariadb/ma_io.h>
#include <string.h>

//#include <mysql.h>


typedef struct {
    GtkWidget *w_txt_view_main;            // Pointer to text view object
    GtkWidget *w_dlg_file_choose;       // Pointer to file chooser dialog box
    GtkTextBuffer *txt_buf_main;     // Pointer to text buffer
    GtkWidget *w_hlp_mnu_About;
    GtkWidget *w_dlg_about;
    GtkWidget *w_lbl_test;
    GtkWidget *w_lbl_time;
    GtkWidget *w_lbox_query;
    GtkWidget *w_ctn_scrollwindow;
    GtkWidget *w_btn_update_edit;		//BUTTON
    //GtkWidget *w_ntr_lastname;			//--
    GtkEntry *w_ntr_id;
    GtkEntry *w_ntr_lastname;
    //GtkEntry *w_ntr_buf_lastname;
    //GtkWidget *w_ntr_name;
    GtkEntry *w_ntr_name;
    //GtkWidget *w_ntr_weight;
    GtkEntry *w_ntr_weight;
    GtkEntry *w_ntr_country_zip;
    GtkEntry *w_ntr_street;
    GtkEntry *w_ntr_town;
    GtkEntry *w_ntr_birthday;
} app_widgets;

//--mysql
typedef struct {
	MYSQL *conn;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int  num_fields;
	char *server;				// "172.16.1.1"  "127.0.0.1"
	char *user;
	char *password;
	char *database;
	char *query;//DESC / ASC
} db_structure;
//id`, `name`, `lastname`, `weight`, `birthday`
enum
{
	TBL_ID,
	TBL_NAME,
	TBL_LASTNAME,
	TBL_WEIGHT,
	TBL_BIRTHDAY,
	TBL_LANDS,
	TBL_PLZ,
	TBL_STREET,
	TBL_TOWN
};
//--mysql

void on_xWindow_destroy(void);
gboolean timer_handler(app_widgets *widgets);
void SelectSomeThing(app_widgets *app_wdgts);
//------------TREE - START------------------
enum
{
    COLUMN_NAME_NM = 0,
    COLUMN_ID,
    N_COLUMNS
};

typedef struct
{
    GtkListStore       *articles;
    GtkTreeModelSort   *sorted;
    GtkTreeModelFilter *filtered;
    gdouble             max_price;
} Store;

//MUST be PUBLIC STRUCTUREs
db_structure *DB_Structure;//= g_slice_new(db_structure);
app_widgets     *widgets;

static gboolean row_visible (GtkTreeModel *model, GtkTreeIter *iter, Store *store)
{
    //gdouble price;

    //gtk_tree_model_get (model, iter, COLUMN_ID, &price, -1);

    //return price <= store->max_price;
    return TRUE;
}


void on_btn_update_edit_clicked(GtkButton *button/*,  app_widgets *app_wdgts*/)
{
gchar *format;
gchar **ls_plz;
//gchar out_str[100] = {0};
gchar lc_name[100], lc_lastname[100], lc_street[100], lc_town[100];

//`name`, `lastname`, `weight`, `birthday`, `LandS`, `PLZ`, `street`, `town` FROM `person` WHERE `id`=%s"
//UPDATE `person` SET `PLZ` = '35430', `street` = 'Argin SK', `town` = 'Urla' WHERE `person`.`id` = 2
format="UPDATE `person` SET  `name`='%s', `lastname`='%s', `weight`='%s', `birthday`='%s', `LandS`='%s', `PLZ`='%s', `street`='%s', `town`='%s' WHERE `person`.`id`=%s";

//sprintf(out_str, "%s",(g_convert(gtk_entry_get_text(widgets->w_ntr_town),-1,"ISO-8859-1","UTF-8",0,0,0)));
sprintf(lc_name, "%s",(g_convert(gtk_entry_get_text(widgets->w_ntr_name),-1,"ISO-8859-1","UTF-8",0,0,0)));
sprintf(lc_lastname, "%s",(g_convert(gtk_entry_get_text(widgets->w_ntr_lastname),-1,"ISO-8859-1","UTF-8",0,0,0)));
sprintf(lc_street, "%s",(/*g_convert(*/gtk_entry_get_text(widgets->w_ntr_street)/*,-1,"ISO-8859-9","UTF-8",0,0,0)*/));
sprintf(lc_town, "%s",(g_convert(gtk_entry_get_text(widgets->w_ntr_town),-1,"ISO-8859-1","UTF-8",0,0,0)));

ls_plz=g_strsplit(gtk_entry_get_text(widgets->w_ntr_country_zip),"-",-1);
DB_Structure->query = g_markup_printf_escaped (format,/*gtk_entry_get_text(widgets->w_ntr_name)*/lc_name,
/*gtk_entry_get_text(widgets->w_ntr_lastname)*/lc_lastname,gtk_entry_get_text(widgets->w_ntr_weight),
gtk_entry_get_text(widgets->w_ntr_birthday),ls_plz[0],ls_plz[1],
/*gtk_entry_get_text(widgets->w_ntr_street)*/lc_street,/*gtk_entry_get_text(widgets->w_ntr_town) -*- out_str*/lc_town,
gtk_entry_get_text(widgets->w_ntr_id));

//DB_Structure->query = g_markup_printf_escaped (format, articleID);
g_print ("SQL:%s\n",DB_Structure->query);


//---------
	DB_Structure->conn = mysql_init(NULL);
	mysql_get_client_info();
   // Connect to database 
   if (!mysql_real_connect(DB_Structure->conn, DB_Structure->server, DB_Structure->user, 
							DB_Structure->password, DB_Structure->database, 0, NULL, 0))
   {
      g_print("Error %s\n", mysql_error(DB_Structure->conn));
      exit(1);
   }

   if (mysql_query(DB_Structure->conn, DB_Structure->query)) {
      g_print("Error %s\n", mysql_error(DB_Structure->conn));
      exit(1);
   }
//   DB_Structure->result = mysql_use_result(DB_Structure->conn);
//   DB_Structure->row = mysql_fetch_row(DB_Structure->result);

	//if(sizeof(out_str)>0)g_free(out_str);
	//mysql_free_result(DB_Structure->result);
	mysql_close(DB_Structure->conn);
//---------	


























//widgets->w_ntr_country_zip
//widgets->w_ntr_name,widgets->w_ntr_lastname,
//widgets->w_ntr_weight,widgets->w_ntr_birthday,"LS","PLZ",
//widgets->w_ntr_street,widgets->w_ntr_town, widgets->w_ntr_id
/*
, out_str);

, out_str);
//TBL_STREET, TBL_TOWN
//ntr_country_zip
	sprintf(out_str, "%s-%s",(DB_Structure->row[TBL_LANDS]),(DB_Structure->row[TBL_PLZ]));
	gtk_entry_set_text(widgets->w_ntr_country_zip, out_str);

	sprintf(out_str, "%s",(DB_Structure->row[TBL_STREET]));
	gtk_entry_set_text(, out_str);
//g_locale_from_utf8 // g_locale_to_utf8
	//sprintf(out_str, "%s",(g_locale_from_utf8(DB_Structure->row[TBL_TOWN],-1,0,0,-0)));
	g_print("Bevor %s\n", DB_Structure->row[TBL_TOWN]);
	sprintf(out_str, "%s",(g_convert(DB_Structure->row[TBL_TOWN],-1,"UTF-8","ISO-8859-1",0,0,0)));
	gtk_entry_set_text(, out_str);
	g_print("After %s\n", gtk_entry_get_text(widgets->w_ntr_town));

	sprintf(out_str, "%s",(DB_Structure->row[TBL_WEIGHT]));
	gtk_entry_set_text(, out_str);
	
	sprintf(out_str, "%s",(DB_Structure->row[TBL_BIRTHDAY]));
	gtk_entry_set_text( out_str);
*/





}

static Store * create_store (db_structure *DB_Structure)
{
    Store *store;
    GtkTreeIter iter;
//--mysql
DB_Structure->query="SELECT `id`, `name`, `lastname`, `weight`, `birthday` FROM `person` WHERE 1 order by `name` DESC";//DESC / ASC
gchar out_str[100] = {0}; // buffer for string
//--mysql

g_print ("create_store\n");

    store = g_new0 (Store, 1);
    store->max_price = 12.99;
    //store->articles = gtk_list_store_new (N_COLUMNS, G_TYPE_STRING, G_TYPE_DOUBLE);
    store->articles = gtk_list_store_new (N_COLUMNS, G_TYPE_STRING, G_TYPE_STRING);
    //gtk_tree_store_new (3, G_TYPE_INT, G_TYPE_STRING, GDK_TYPE_PIXBUF); 
if(TRUE)
{
	g_print ("create_store - sql\n");
	DB_Structure->conn = mysql_init(NULL);
	mysql_get_client_info();
   // Connect to database 
   if (!mysql_real_connect(DB_Structure->conn, DB_Structure->server, DB_Structure->user, 
							DB_Structure->password, DB_Structure->database, 0, NULL, 0))
   {
							//mysql_error
      g_print("Error %s\n", mysql_error(DB_Structure->conn));
      exit(1);
   }

   // send SQL query
   if (mysql_query(DB_Structure->conn, "show tables")) {
      g_print("Error %s\n", mysql_error(DB_Structure->conn));
      exit(1);
   }

   DB_Structure->result = mysql_use_result(DB_Structure->conn);

   // output table name 
   g_print("MySQL Tables in mysql database:\n");
   while ((DB_Structure->row = mysql_fetch_row(DB_Structure->result)) != NULL) g_print("%s \n", DB_Structure->row[TBL_ID]);
//
   if (mysql_query(DB_Structure->conn, DB_Structure->query)) {
      g_print("Error %s\n", mysql_error(DB_Structure->conn));
      exit(1);
   }
   DB_Structure->result = mysql_use_result(DB_Structure->conn);
   DB_Structure->row = mysql_fetch_row(DB_Structure->result);
   //num_fields = mysql_num_fields(result);
	do
	{
		g_snprintf(out_str, sizeof(out_str), "%s %s", DB_Structure->row[TBL_LASTNAME], DB_Structure->row[TBL_NAME]);
		
		gtk_list_store_append (store->articles, &iter);
		gtk_list_store_set (store->articles, &iter, COLUMN_NAME_NM, out_str, COLUMN_ID, DB_Structure->row[TBL_ID], -1);
		
		g_print("%s %s %s \n",DB_Structure->row[TBL_ID] , DB_Structure->row[TBL_LASTNAME], DB_Structure->row[TBL_NAME]);
	}while( (DB_Structure->row = mysql_fetch_row(DB_Structure->result)) );
	//if(sizeof(out_str)>0)g_free(out_str);
	
	mysql_free_result(DB_Structure->result);
	mysql_close(DB_Structure->conn);
}
else
{
    /* Add some items */
    gtk_list_store_append (store->articles, &iter);
    gtk_list_store_set (store->articles, &iter, COLUMN_NAME_NM, "Spam", COLUMN_ID, 1.20, -1);

    gtk_list_store_append (store->articles, &iter);
    gtk_list_store_set (store->articles, &iter, COLUMN_NAME_NM, "Beer", COLUMN_ID, 5.99, -1);

    gtk_list_store_append (store->articles, &iter);
    gtk_list_store_set (store->articles, &iter, COLUMN_NAME_NM, "Chewing Gum", COLUMN_ID, 0.99, -1);
}
store->filtered = GTK_TREE_MODEL_FILTER (gtk_tree_model_filter_new (GTK_TREE_MODEL (store->articles), NULL));
store->sorted = GTK_TREE_MODEL_SORT (gtk_tree_model_sort_new_with_model (GTK_TREE_MODEL (store->filtered)));

gtk_tree_model_filter_set_visible_func (store->filtered, (GtkTreeModelFilterVisibleFunc) row_visible, store, NULL);

    return store;
}

static void on_row_activated(GtkTreeView *view, GtkTreePath *path, 
							GtkTreeViewColumn *col, Store *store/*,  app_widgets *app_wdgts*/)
{
    GtkTreeIter iter;
    GtkTreePath *filtered_path;
    GtkTreePath *child_path;
//gchar lc_name[100], lc_lastname[100], lc_street[100], lc_town[100];

gchar *article;
gchar *articleID;
gchar * format;
//gdouble price;
gchar out_str[1000]={0};
//gchar *out_str;

format="SELECT `id`, `name`, `lastname`, `weight`, `birthday`, `LandS`, `PLZ`, `street`, `town` FROM `person` WHERE `id`=%s";
//out_str = g_markup_printf_escaped (format, DB_Structure->query);

filtered_path = gtk_tree_model_sort_convert_path_to_child_path(GTK_TREE_MODEL_SORT (store->sorted), path);

child_path = gtk_tree_model_filter_convert_path_to_child_path(GTK_TREE_MODEL_FILTER (store->filtered), filtered_path);

if (gtk_tree_model_get_iter (GTK_TREE_MODEL (store->articles), &iter, child_path))
{

	//gtk_tree_model_get (GTK_TREE_MODEL (store->articles), &iter, COLUMN_NAME_NM, &article, COLUMN_ID, &price, -1);
	gtk_tree_model_get (GTK_TREE_MODEL (store->articles), &iter, COLUMN_NAME_NM, &article, COLUMN_ID, &articleID, -1);

	DB_Structure->query = g_markup_printf_escaped (format, articleID);
//---------
	DB_Structure->conn = mysql_init(NULL);
	mysql_get_client_info();
   // Connect to database 
   if (!mysql_real_connect(DB_Structure->conn, DB_Structure->server, DB_Structure->user, 
							DB_Structure->password, DB_Structure->database, 0, NULL, 0))
   {
      g_print("Error %s\n", mysql_error(DB_Structure->conn));
      exit(1);
   }

   if (mysql_query(DB_Structure->conn, DB_Structure->query)) {
      g_print("Error %s\n", mysql_error(DB_Structure->conn));
      exit(1);
   }
   DB_Structure->result = mysql_use_result(DB_Structure->conn);
   DB_Structure->row = mysql_fetch_row(DB_Structure->result);
   //num_fields = mysql_num_fields(result);

	sprintf(out_str, "%s",(DB_Structure->row[TBL_ID]));
	gtk_entry_set_text(widgets->w_ntr_id, out_str);

	//sprintf(out_str, "%s",(DB_Structure->row[TBL_LASTNAME]));
	sprintf(out_str, "%s",(g_convert(DB_Structure->row[TBL_LASTNAME],-1,"UTF-8","ISO-8859-1",0,0,0)));
	gtk_entry_set_text(widgets->w_ntr_lastname, out_str);

	//sprintf(out_str, "%s",(DB_Structure->row[TBL_NAME]));
	sprintf(out_str, "%s",(/*g_convert(*/DB_Structure->row[TBL_NAME]/*,-1,"UTF-8","ISO-8859-1",0,0,0)*/));
	gtk_entry_set_text(widgets->w_ntr_name, out_str);
//TBL_STREET, TBL_TOWN
//ntr_country_zip
	sprintf(out_str, "%s-%s",(DB_Structure->row[TBL_LANDS]),(DB_Structure->row[TBL_PLZ]));
	gtk_entry_set_text(widgets->w_ntr_country_zip, out_str);

	//sprintf(out_str, "%s",(DB_Structure->row[TBL_STREET]));
	sprintf(out_str, "%s",(g_convert(DB_Structure->row[TBL_STREET],-1,"UTF-8","ISO-8859-7",0,0,0)));//ISO-8859-9==Latin5
	gtk_entry_set_text(widgets->w_ntr_street, out_str);
//g_locale_from_utf8 // g_locale_to_utf8
	//sprintf(out_str, "%s",(g_locale_from_utf8(DB_Structure->row[TBL_TOWN],-1,0,0,-0)));
	g_print("Bevor %s\n", DB_Structure->row[TBL_TOWN]);
	sprintf(out_str, "%s",(g_convert(DB_Structure->row[TBL_TOWN],-1,"UTF-8","ISO-8859-1",0,0,0)));
	gtk_entry_set_text(widgets->w_ntr_town, out_str);
	g_print("After %s\n", gtk_entry_get_text(widgets->w_ntr_town));

	sprintf(out_str, "%s",(DB_Structure->row[TBL_WEIGHT]));
	gtk_entry_set_text(widgets->w_ntr_weight, out_str);
	
	sprintf(out_str, "%s",(DB_Structure->row[TBL_BIRTHDAY]));
	gtk_entry_set_text(widgets->w_ntr_birthday, out_str);

	g_print("%s %s %s %s\n",DB_Structure->row[TBL_ID] , DB_Structure->row[TBL_LASTNAME], DB_Structure->row[TBL_NAME], DB_Structure->row[TBL_WEIGHT]);

	//if(sizeof(out_str)>0)g_free(out_str);
	mysql_free_result(DB_Structure->result);
	mysql_close(DB_Structure->conn);
//---------	
	gtk_widget_set_sensitive(widgets->w_btn_update_edit, TRUE);
	g_print ("You selected %s with ID %s? MySQLquery:%s\n", article, articleID, DB_Structure->query);
	g_free (article);
}
}

static GtkTreeView * create_view (Store *store, db_structure *DB_Structure)
{
    GtkTreeView         *view;
    GtkCellRenderer     *renderer;
    GtkTreeViewColumn   *article_column;
    GtkTreeViewColumn   *price_column;

    view = GTK_TREE_VIEW (gtk_tree_view_new_with_model (GTK_TREE_MODEL (store->sorted)));
    gtk_widget_set_vexpand (GTK_WIDGET (view), TRUE);

    renderer = gtk_cell_renderer_text_new ();

article_column = gtk_tree_view_column_new_with_attributes ("Family Name & Name", renderer,"text", COLUMN_NAME_NM,NULL);

    gtk_tree_view_column_set_sort_column_id (article_column, COLUMN_NAME_NM);
    gtk_tree_view_append_column (view, article_column);

price_column = gtk_tree_view_column_new_with_attributes("ID", renderer,"text", COLUMN_ID,NULL);

    gtk_tree_view_column_set_sort_column_id (price_column, COLUMN_ID);
    gtk_tree_view_append_column (view, price_column);

    g_signal_connect (view, "row-activated", G_CALLBACK (on_row_activated), store);
    //g_signal_connect (view, "row-expanded", G_CALLBACK (on__row_selected), store);

    return view;
}

//------------TREE - END  ------------------
void on_lbox_query_lstitm_test( app_widgets *app_wdgts)
{
    //gtk_widget_show(app_wdgts->w_dlg_about);
	g_print ("item selected - %d\n", 0);
}

void on_lbox_query_lstitm_01_activate(GtkListBox *listitem, app_widgets *app_wdgts)
//void on_lbox_query_lstitm_01_activate(GtkMenuItem *menuitem, app_widgets *app_wdgts)
{
    //gtk_widget_show(app_wdgts->w_dlg_about);
	g_print ("item selected - %d\n", 0);
}

//db_structure *DB_Structure;//= g_slice_new(db_structure);

//
int main(int argc, char *argv[])
{
    GtkBuilder      *builder; 
    GtkWidget       *window;
    //app_widgets     *widgets = g_slice_new(app_widgets);
    widgets = g_slice_new(app_widgets);
//
    GtkWidget *view;
    GtkWidget *box;
    Store *store;
//
//--mysql
//db_structure 
DB_Structure= g_slice_new(db_structure);
//db_structure DB_Structure;
//DB_Structure->conn;
//DB_Structure->result;
//DB_Structure->row;
//DB_Structure->num_fields;
DB_Structure->server = "172.16.1.1";
DB_Structure->user   = "xxxxx";
DB_Structure->password= "xxxxx";
DB_Structure->database = "test";
DB_Structure->query="SELECT `id`, `name`, `lastname`, `weight`, `birthday` FROM `person` WHERE 1 order by `name` DESC";//DESC / ASC
//--mysql
// db_structure;

//
    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("mysql_query.glade");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "xWindow"));
    // Get pointers to widgets
    widgets->w_txt_view_main= GTK_WIDGET(gtk_builder_get_object(builder, "txt_view_main"));
    widgets->w_dlg_file_choose = GTK_WIDGET(gtk_builder_get_object(builder, "dlg_file_choose"));
    widgets->txt_buf_main 	= GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "txt_buf_main"));
    widgets->w_hlp_mnu_About= GTK_WIDGET(gtk_builder_get_object(builder, "hlp_mnu_About"));
    widgets->w_dlg_about 	= GTK_WIDGET(gtk_builder_get_object(builder, "dlg_about"));
    widgets->w_lbl_test 	= GTK_WIDGET(gtk_builder_get_object(builder, "lbl_test"));
    widgets->w_lbl_time 	= GTK_WIDGET(gtk_builder_get_object(builder, "lbl_time"));
    widgets->w_lbox_query 	= GTK_WIDGET(gtk_builder_get_object(builder, "lbox_query"));
    widgets->w_ctn_scrollwindow= GTK_WIDGET(gtk_builder_get_object(builder, "ctn_scrollwindow"));

    //widgets->w_ntr_lastname= GTK_WIDGET(gtk_builder_get_object(builder, "ntr_lastname"));
    widgets->w_ntr_id= GTK_ENTRY(gtk_builder_get_object(builder, "ntr_id"));
    widgets->w_ntr_lastname= GTK_ENTRY(gtk_builder_get_object(builder, "ntr_lastname"));
    //widgets->w_ntr_buf_lastname= GTK_ENTRY(gtk_builder_get_object(builder, "ntr_buf_lastname"));
//    widgets->w_ntr_buf_lastname= GTK_WIDGET(gtk_builder_get_object(builder, "ntr_buf_lastname"));
    widgets->w_ntr_name= GTK_ENTRY(gtk_builder_get_object(builder, "ntr_name"));
    widgets->w_ntr_weight= GTK_ENTRY(gtk_builder_get_object(builder, "ntr_weight"));
    widgets->w_ntr_country_zip= GTK_ENTRY(gtk_builder_get_object(builder, "ntr_country_zip"));
    widgets->w_ntr_street= GTK_ENTRY(gtk_builder_get_object(builder, "ntr_street"));
	widgets->w_ntr_town= GTK_ENTRY(gtk_builder_get_object(builder, "ntr_town"));
	widgets->w_ntr_birthday= GTK_ENTRY(gtk_builder_get_object(builder, "ntr_birthday"));
    widgets->w_btn_update_edit= GTK_WIDGET(gtk_builder_get_object(builder, "btn_update_edit"));

    gtk_builder_connect_signals(builder, widgets);

    g_object_unref(builder);
//
//	gtk_editable_set_editable(GTK_ENTRY(widgets->w_ntr_lastname), TRUE);
//	//gtk_entry_set_text(widgets->w_ntr_lastname, DB_Structure->row[TBL_LASTNAME]);
//	gtk_entry_set_text(GTK_ENTRY(widgets->w_ntr_lastname), "uuuutu");
//
    box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    store = create_store(DB_Structure);
    view = GTK_WIDGET (create_view (store, DB_Structure));
	gtk_container_add (GTK_CONTAINER (widgets->w_ctn_scrollwindow), box);
    gtk_container_add (GTK_CONTAINER (box), view);
//
	g_timeout_add(1, (GSourceFunc)timer_handler, widgets);
//
//    gtk_widget_show(window);
    gtk_widget_show_all (window);
    //
    SelectSomeThing(widgets);
    //--
    //
    gtk_main();
    g_slice_free(app_widgets, widgets);

    return 0;
}
//-----------------------------------------
gboolean timer_handler(app_widgets *app_wdgts)
{
    GDateTime   *time;            // for storing current time and date
    gchar       *time_str;        // current time and date as a string

char *format = "<span background=\"#AAFFAA\">%s</span>";
char *markup;

    time = g_date_time_new_now_local();
    time_str = g_date_time_format(time, "%H:%M:%S");
    //"<span foreground='red' weight='bold' font='14'>You Lose!</span>"

	markup = g_markup_printf_escaped (format, time_str);
	gtk_label_set_markup(GTK_LABEL(app_wdgts->w_lbl_time), markup);

//    gtk_label_set_markup(GTK_LABEL (app_wdgts->w_lbl_time), "<span background=\'#00FF00\'></span>");
//    gtk_label_set_markup(GTK_LABEL (widgets->w_lbl_time), "<span foreground='#00FF00'></span>");
//    gtk_label_set_text(GTK_LABEL(app_wdgts->w_lbl_time), time_str);

	free (markup);
    
    g_free (time_str);
	g_date_time_unref(time);
	
    return TRUE;
}

//-----------------------------------------
void SelectSomeThing(app_widgets *app_wdgts)
{
	gtk_label_set_text(GTK_LABEL(app_wdgts->w_lbl_test), "CLICK to QUERY");
	
}

//mysql -u root -p -e "grant all privileges on *.* to 'root'@'%' identified by 'pw.123' with grant option";
// nano /etc/my.cnf.d/50-galera.cnf
// bind-address=0.0.0.0
// nano /etc/my.cnf
// service mariadb restart
//---------------------------------------------------------------------
//CREATE TABLE skyscrapers (id INTEGER PRIMARY KEY,
//    name TEXT,
//    country TEXT,
//    height_meters INTEGER,
//    floors INTEGER
//);
//INSERT INTO skyscrapers VALUES (1, "Burj Khalifa", "UAE", 828, 163);
//INSERT INTO skyscrapers VALUES (2, "Shanghai Tower", "China", 632, 128);
//INSERT INTO skyscrapers VALUES (3, "Abraj Al-Bait Clock Tower", "Saudi Arabia", 601, 120);
//INSERT INTO skyscrapers VALUES (4, "One World Trade Center", "United States", 541, 104);
//INSERT INTO skyscrapers VALUES (5, "CTF Finance Centre",  "China", 530, 111);
// Sort by their height in meters 
//SELECT * FROM skyscrapers ORDER BY height_meters;
// Reverse that sort (highest to lowest) 
//SELECT * FROM skyscrapers ORDER BY height_meters DESC;
// Sort by their floors, lowest to highest 
//SELECT * FROM skyscrapers ORDER BY floors ASC;
//---------------------------------------------------------------------
void on_btn_conMSQL_clicked(GtkButton *button/*,  app_widgets *app_wdgts*/)
{
MYSQL *conn;
MYSQL_RES *result;
MYSQL_ROW row;
char *server = "172.16.1.1";
//char *server = "127.0.0.1";
char *user   = "xxxxx";
char *password= "xxxxx";
char *database = "test";
char *query="SELECT `id`, `name`, `lastname`, `weight`, `birthday` FROM `person` WHERE 1 order by `name` DESC";//DESC / ASC
//int  num_fields;
//int	ii;
//gchar *lboxPostxt;
//GtkWidget *rowTxt;

//for(ii=0; ii<5; ii++)
//{
	//lboxPostxt=g_strdump_print("Zeile %d",ii);
	//lboxPostxt=g_markup_printf_escaped ("Zeile %d", ii);
	//rowTxt=create_row(lboxPostxt);
	//gtk_list_box_insert(GTK_LIST_BOX(app_wdgts->w_lbox_query),rowTxt,-1);
//}



   conn = mysql_init(NULL);
   mysql_get_client_info();
   


   // Connect to database 
   if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
   {
							//mysql_error
      g_print("Error %s\n", mysql_error(conn));
      exit(1);
   }

   // send SQL query
   if (mysql_query(conn, "show tables")) {
      g_print("Error %s\n", mysql_error(conn));
      exit(1);
   }

   result = mysql_use_result(conn);

   // output table name 
   g_print("MySQL Tables in mysql database:\n");
   while ((row = mysql_fetch_row(result)) != NULL)
      g_print("%s \n", row[0]);
//
   if (mysql_query(conn, query)) {
      g_print("Error %s\n", mysql_error(conn));
      exit(1);
   }
   result = mysql_use_result(conn);
   row = mysql_fetch_row(result);
   //num_fields = mysql_num_fields(result);
   do
	{
		g_print("%s %s %s \n",row[0] , row[1], row[2]);
	}	while((row = mysql_fetch_row(result)));

//
mysql_free_result(result);
mysql_close(conn);
}


// File --> Open
void on_file_mnu_open_activate(GtkMenuItem *menuitem, app_widgets *app_wdgts)
{
    gchar *file_name = NULL;        // Name of file to open from dialog box
    gchar *file_contents = NULL;    // For reading contents of file
    gboolean file_success = FALSE;  // File read status
    
    // Show the "Open Text File" dialog box
    gtk_widget_show(app_wdgts->w_dlg_file_choose);
    
    // Check return value from Open Text File dialog box to see if user clicked the Open button
    if (gtk_dialog_run(GTK_DIALOG (app_wdgts->w_dlg_file_choose)) == GTK_RESPONSE_OK) {
        // Get the file name from the dialog box
        file_name = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(app_wdgts->w_dlg_file_choose));
        if (file_name != NULL) {
            // Copy the contents of the file to dynamically allocated memory
            file_success = g_file_get_contents(file_name, &file_contents, NULL, NULL);
            if (file_success) {
                // Put the contents of the file into the GtkTextBuffer
                gtk_text_buffer_set_text(app_wdgts->txt_buf_main, file_contents, -1);
            }
            g_free(file_contents);
        }
        g_free(file_name);
    }

    // Finished with the "Open Text File" dialog box, so hide it
    gtk_widget_hide(app_wdgts->w_dlg_file_choose);
}


void on_hlp_mnu_About_activate(GtkMenuItem *menuitem, app_widgets *app_wdgts)
{
    gtk_widget_show(app_wdgts->w_dlg_about);
}

// About dialog box Close button
//   on_dlg_about_response
void on_dlg_about_response(GtkDialog *dialog, gint response_id, app_widgets *app_wdgts)
{
    gtk_widget_hide(app_wdgts->w_dlg_about);
}

// File --> Close
void on_file_mnu_close_activate(GtkMenuItem *menuitem, app_widgets *app_wdgts)
{
    // Clear the text from window "Close the file"
    gtk_text_buffer_set_text(app_wdgts->txt_buf_main, "", -1);
}

// File --> Quit
void on_file_mnu_quit_activate(GtkMenuItem *menuitem, app_widgets *app_wdgts)
{
    //gtk_main_quit();
    on_xWindow_destroy();
}

// called when window is closed
void on_xWindow_destroy()
{
    gtk_main_quit();
}
