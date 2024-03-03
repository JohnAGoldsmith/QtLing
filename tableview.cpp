#include "table_views.h"
#include "generaldefinitions.h"

TableView::TableView(MainWindow* main_window){
    m_main_window = main_window;
}
TableView::TableView(MainWindow* main_window, eDataType data_type){
    m_main_window = main_window;
    m_data_type = data_type;
}
