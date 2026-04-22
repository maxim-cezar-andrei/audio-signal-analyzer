/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL_ACQ                        1       /* callback function: OnAcqPanel */
#define  PANEL_ACQ_GRAPH_RAW              2       /* control type: graph, callback function: (none) */
#define  PANEL_ACQ_GRAPH_FILTERED         3       /* control type: graph, callback function: (none) */
#define  PANEL_ACQ_BUTTON_PREV            4       /* control type: command, callback function: OnButtonPrev */
#define  PANEL_ACQ_BUTTON_NEXT            5       /* control type: command, callback function: OnButtonNext */
#define  PANEL_ACQ_NUMERIC_START          6       /* control type: numeric, callback function: (none) */
#define  PANEL_ACQ_NUMERIC_STOP           7       /* control type: numeric, callback function: (none) */
#define  PANEL_ACQ_NUMERIC_ZEROS          8       /* control type: numeric, callback function: (none) */
#define  PANEL_ACQ_NUMERIC_MEDIAN         9       /* control type: numeric, callback function: (none) */
#define  PANEL_ACQ_NUMERIC_DISP           10      /* control type: numeric, callback function: (none) */
#define  PANEL_ACQ_NUMERIC_MAX            11      /* control type: numeric, callback function: (none) */
#define  PANEL_ACQ_NUMERIC_IMAX           12      /* control type: numeric, callback function: (none) */
#define  PANEL_ACQ_NUMERIC_IMIN           13      /* control type: numeric, callback function: (none) */
#define  PANEL_ACQ_NUMERIC_MIN            14      /* control type: numeric, callback function: (none) */
#define  PANEL_ACQ_NUMERIC_MEAN           15      /* control type: numeric, callback function: (none) */
#define  PANEL_ACQ_NUMERIC_WINDOW         16      /* control type: numeric, callback function: (none) */
#define  PANEL_ACQ_RING_FILTER            17      /* control type: ring, callback function: OnFilterChange */
#define  PANEL_ACQ_BUTTON_APPLY           18      /* control type: command, callback function: OnButtonApply */
#define  PANEL_ACQ_TEXT_INTERVAL          19      /* control type: textMsg, callback function: (none) */
#define  PANEL_ACQ_BUTTON_LOAD            20      /* control type: command, callback function: OnLoadWave */
#define  PANEL_ACQ_GRAPH_HIST             21      /* control type: graph, callback function: (none) */
#define  PANEL_ACQ_NUMERIC_KURT           22      /* control type: numeric, callback function: (none) */
#define  PANEL_ACQ_NUMERIC_SKEW           23      /* control type: numeric, callback function: (none) */
#define  PANEL_ACQ_SWITCH_DERIV           24      /* control type: binary, callback function: (none) */
#define  PANEL_ACQ_SWITCH_CB              25      /* control type: binary, callback function: OnSwitchPanelCB */

#define  PANEL_FREQ                       2       /* callback function: OnFreqPanel */
#define  PANEL_FREQ_SWITCH_CB             2       /* control type: binary, callback function: OnSwitchPanelCB */
#define  PANEL_FREQ_GRAPH_SPECTRUM        3       /* control type: graph, callback function: (none) */
#define  PANEL_FREQ_NUMERIC_PUNCTE        4       /* control type: numeric, callback function: (none) */
#define  PANEL_FREQ_TIMER                 5       /* control type: timer, callback function: OnTimer */
#define  PANEL_FREQ_NUMERIC_POWER_PEAK    6       /* control type: numeric, callback function: (none) */
#define  PANEL_FREQ_NUMERIC_FREQ_PEAK     7       /* control type: numeric, callback function: (none) */
#define  PANEL_FREQ_COMMANDBUTTON         8       /* control type: command, callback function: OnButtonSave */
#define  PANEL_FREQ_RING_SPECTRU          9       /* control type: ring, callback function: (none) */
#define  PANEL_FREQ_BINARYSWITCH_SPECTRU  10      /* control type: binary, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK OnAcqPanel(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnButtonApply(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnButtonNext(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnButtonPrev(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnButtonSave(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFilterChange(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFreqPanel(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnLoadWave(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSwitchPanelCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnTimer(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif