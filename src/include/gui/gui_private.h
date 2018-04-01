/**	
 * \file            gui_private.h
 * \brief           Private functions and enumeration
 */
 
/*
 * Copyright (c) 2017 Tilen Majerle
 *  
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software, 
 * and to permit persons to whom the Software is furnished to do so, 
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Author:          Tilen Majerle <tilen@majerle.eu>
 */
#ifndef __GUI_PRIVATE_H
#define __GUI_PRIVATE_H

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup        GUI_PRIVATE Private functions
 * \brief           Private functions and enumerations
 * \{
 */

#include "gui/gui.h"

#if GUI_CFG_OS
#include "system/gui_sys.h"

#define GUI_SYS_MBOX_TYPE_TOUCH             0x01
#define GUI_SYS_MBOX_TYPE_KEYBOARD          0x02
#define GUI_SYS_MBOX_TYPE_REMOVE            0x03
#define GUI_SYS_MBOX_TYPE_TIMER             0x04
#define GUI_SYS_MBOX_TYPE_WIDGET_CREATED    0x05
#define GUI_SYS_MBOX_TYPE_INVALIDATE        0x06

/**
 * \brief           Message box data type
 */
typedef struct {
    uint32_t type;                          /*!< Message type */
} gui_mbox_msg_t;

/**
 * \brief           OS dependant variables
 */
typedef struct {
    gui_sys_thread_t thread_id;             /*!< GUI thread ID */
    gui_sys_mbox_t mbox;                    /*!< Operating system message box */
} GUI_CFG_OS_t;
#endif /* GUI_CFG_OS */

/**
 * \brief           GUI main object structure
 */
typedef struct {
    GUI_LCD_t LCD;                          /*!< LCD low-level settings */
    GUI_LL_t LL;                            /*!< Low-level drawing routines for LCD */
    
    uint32_t Flags;                         /*!< Core GUI flags management */
    
    gui_display_t Display;                  /*!< Clipping management */
    gui_display_t DisplayTemp;              /*!< Clipping for widgets for drawing and touch */
    
    gui_handle_p WindowActive;              /*!< Pointer to currently active window when creating new widgets */
    gui_handle_p FocusedWidget;             /*!< Pointer to focused widget for keyboard events if any */
    gui_handle_p FocusedWidgetPrev;         /*!< Pointer to previously focused widget */
    
    gui_linkedlistroot_t root;              /*!< Root linked list of widgets */
    gui_timer_core_t timers;                /*!< Software structure management */
    
    gui_linkedlistroot_t RootFonts;         /*!< Root linked list of font widgets */
    
    gui_widget_param_t WidgetParam;
    gui_widget_result_t WidgetResult;
    
#if GUI_CFG_USE_TOUCH || __DOXYGEN__
    guii_touch_data_t TouchOld;              /*!< Old touch data, used for event management */
    guii_touch_data_t Touch;                 /*!< Current touch data and processing tool */
    gui_handle_p ActiveWidget;              /*!< Pointer to widget currently active by touch */
    gui_handle_p ActiveWidgetPrev;          /*!< Previously active widget */
#endif /* GUI_CFG_USE_TOUCH */

#if GUI_CFG_USE_TRANSLATE
    gui_translate_t translate;              /*!< Translation management structure */
#endif /* GUI_CFG_USE_TRANSLATE */
    
#if GUI_CFG_OS
    GUI_CFG_OS_t OS;                        /*!< Operating system dependant structure */
#endif /* GUI_CFG_OS */

    gui_eventcallback_t evt_cb;            /*!< Pointer to global GUI event callback function */
    
    uint8_t Initialized;                    /*!< Status indicating GUI is initialized */
} gui_t;

extern gui_t GUI;

/**
 * \brief           Check if 2 rectangle objects covers each other in any way
 * \hideinitializer
 */
#define __GUI_RECT_MATCH(h1x1, h1y1, h1x2, h1y2, h2x1, h2y1, h2x2, h2y2)    \
    !(                                                      \
        (h1x1) > (h2x2) ||                                  \
        (h1y1) > (h2y2) ||                                  \
        (h2x1) > (h1x2) ||                                  \
        (h2y1) > (h1y2)                                  \
    )

/**
 * \brief           GUI Handle object from main object
 * \retval          Pointer to GUI handle
 * \hideinitializer
 */ 
#define __GH(x)                     ((struct gui_handle *)(x))

/**
 * \brief           GUI Handle root object from main object with children widgets
 * \retval          Pointer to GUI root handle
 * \hideinitializer
 */ 
#define __GHR(x)                    ((struct gui_handle_root *)(x))

/**
 * \brief           Check input parameters and return value on failure
 * \hideinitializer
 */
#define __GUI_ASSERTPARAMS(c)       do {            \
    if (!(c) || !(GUI.Initialized)) {                                     \
        GUI_DEBUG("Assert param failed in file %s and line %d\r\n", (const char *)__FILE__, (unsigned)__LINE__);  \
        return 0;                                   \
    }                                               \
} while (0)

/**
 * \brief           Check if window is active for widget create
 * \hideinitializer
 */
#define __GUI_ASSERTACTIVEWIN()     do {            \
    if (GUI.WindowActive == NULL) {                        \
        __GUI_DEBUG("There is no active window for widget in file %s on line %d\r\n", __FILE__, __LINE__);  \
        return NULL;                                \
    }                                               \
} while (0)

/**
 * \}
 */

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif /* __GUI_PRIVATE_H */