/*
    DeaDBeeF - ultimate music player for GNU/Linux systems with X11
    Copyright (C) 2009-2010 Alexey Yakovenko <waker@users.sourceforge.net>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __DDBLISTVIEW_H
#define __DDBLISTVIEW_H

#include <gtk/gtk.h>
#include <sys/time.h>
#include <stdint.h>

// drag and drop targets
enum {
    TARGET_URILIST,
    TARGET_SAMEWIDGET,
};

G_BEGIN_DECLS

#define DDB_TYPE_LISTVIEW (ddb_listview_get_type ())
#define DDB_LISTVIEW(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), DDB_TYPE_LISTVIEW, DdbListview))
#define DDB_LISTVIEW_CLASS(obj) (G_TYPE_CHECK_CLASS_CAST((obj), DDB_TYPE_LISTVIEW, DdbListviewClass))
#define DDB_IS_LISTVIEW(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DDB_TYPE_LISTVIEW))
#define DDB_IS_LISTVIEW_CLASS(obj) (G_TYPE_CHECK_CLASS_TYPE ((obj), DDB_TYPE_LISTVIEW))
#define DDB_LISTVIEW_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), DDB_TYPE_LISTVIEW, DdbListviewClass))

typedef struct _DdbListview DdbListview;
typedef struct _DdbListviewClass DdbListviewClass;

typedef void * DdbListviewIter;
//typedef void * DdbListviewColIter;

typedef struct {
    // rows
    int (*count) (void);
    int (*sel_count) (void);

    int (*cursor) (void);
    void (*set_cursor) (int cursor);

    DdbListviewIter (*head) (void);
    DdbListviewIter (*tail) (void);
    DdbListviewIter (*next) (DdbListviewIter);
    DdbListviewIter (*prev) (DdbListviewIter);

    DdbListviewIter (*get_for_idx) (int idx);
    int (*get_idx) (DdbListviewIter);

    void (*ref) (DdbListviewIter);
    void (*unref) (DdbListviewIter);

    void (*select) (DdbListviewIter, int sel);
    int (*is_selected) (DdbListviewIter);

    // drag-n-drop
    void (*drag_n_drop) (DdbListviewIter before, uint32_t *indices, int length);
    void (*external_drag_n_drop) (DdbListviewIter before, char *mem, int length);

    // callbacks
    void (*draw_column_data) (DdbListview *listview, GdkDrawable *drawable, DdbListviewIter iter, int idx, int column, int x, int y, int width, int height);
    void (*list_context_menu) (DdbListview *listview, DdbListviewIter iter, int idx);
    void (*header_context_menu) (DdbListview *listview, int col);
    void (*handle_doubleclick) (DdbListview *listview, DdbListviewIter iter, int idx);
    void (*selection_changed) (DdbListviewIter it, int idx);
    void (*delete_selected) (void);
    void (*col_changed) (int col, void *user_data);
    void (*col_sort) (int col, int sort_order, void *user_data);
    void (*col_deleted) (int col, void *user_data);
} DdbListviewBinding;

struct _DdbListviewColumn;

struct _DdbListview {
    GtkTable parent;

    // interaction with client
    DdbListviewBinding *binding;

    // cached gtk/gdk object pointers
    GtkWidget *list;
    GtkWidget *header;
    GtkWidget *scrollbar;
    GtkWidget *hscrollbar;

    int totalwidth; // width of listview, including invisible (scrollable) part
    GdkPixmap *backbuf;
    GdkPixmap *backbuf_header;
    const char *title; // unique id, used for config writing, etc
    int lastpos[2]; // last mouse position (for list widget)
    // current state
    int scrollpos;
    int hscrollpos;
    double clicktime; // for doubleclick detection
    int nvisiblerows;
    int nvisiblefullrows;
    int rowheight;

    int col_movepos;

    int drag_motion_y;

    // scrolling
    int scroll_mode; // 0=select, 1=dragndrop
    int scroll_pointer_y;
    int scroll_direction;
    int scroll_active;
    struct timeval tm_prevscroll;
    float scroll_sleep_time;

    // selection
    int areaselect;
    int areaselect_x;
    int areaselect_y;
    int areaselect_dx;
    int areaselect_dy;
    int dragwait;
    int shift_sel_anchor;

    // header
    int header_dragging;
    int header_sizing;
    int header_dragpt[2];
    float last_header_motion_ev; //is it subject to remove?
    int prev_header_x;
    int header_prepare;

    struct _DdbListviewColumn *columns;
    struct _DdbListviewColumn *active_column;
};

struct _DdbListviewClass {
  GtkTableClass parent_class;
};

GtkType ddb_listview_get_type(void);

GtkWidget * ddb_listview_new();

void
ddb_listview_set_binding (DdbListview *listview, DdbListviewBinding *binding);
void
ddb_listview_draw_row (DdbListview *listview, int idx, DdbListviewIter iter);
int
ddb_listview_get_vscroll_pos (DdbListview *listview);
int
ddb_listview_get_hscroll_pos (DdbListview *listview);
DdbListviewIter
ddb_listview_get_iter_from_coord (DdbListview *listview, int x, int y);
int
ddb_listview_handle_keypress (DdbListview *ps, int keyval, int state);
void
ddb_listview_set_cursor (DdbListview *pl, int cursor);
void
ddb_listview_scroll_to (DdbListview *listview, int pos);
int
ddb_listview_is_scrolling (DdbListview *listview);
void
ddb_listview_column_append (DdbListview *listview, const char *title, int width, int align_right, void *user_data);
void
ddb_listview_column_insert (DdbListview *listview, int before, const char *title, int width, int align_right, void *user_data);
void
ddb_listview_column_remove (DdbListview *listview, int idx);
int
ddb_listview_column_get_info (DdbListview *listview, int col, const char **title, int *width, int *align_right, void **user_data);
int
ddb_listview_column_set_info (DdbListview *listview, int col, const char *title, int width, int align_right, void *user_data);

enum {
    DDB_REFRESH_COLUMNS = 1,
    DDB_REFRESH_HSCROLL = 2,
    DDB_REFRESH_VSCROLL = 4,
    DDB_REFRESH_LIST    = 8,
    DDB_EXPOSE_COLUMNS  = 16,
    DDB_EXPOSE_LIST     = 32,
};

void ddb_listview_refresh (DdbListview *listview, uint32_t flags);

G_END_DECLS

#endif // __DDBLISTVIEW_H
