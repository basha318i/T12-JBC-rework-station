/*
 * tft_splash.c
 *
 * LVGL splash screen implementation
 * Displays a splash screen image on startup for a specified duration
 *
 * Created: May 20, 2026
 */

#include "tft_splash.h"
#include "main.h"

/* Forward declaration of the splash image from TFTSplash.c */
extern const lv_img_dsc_t TFTSplash;

/* Internal state variables */
static lv_obj_t *splash_screen = NULL;
static uint32_t splash_start_tick = 0;
static uint32_t splash_duration = 0;

/**
 * Display splash screen for specified duration (BLOCKING VERSION)
 * 
 * This version is suitable for most applications where the splash
 * is shown during boot before the main application starts.
 * 
 * The function blocks until the timeout expires, but LVGL continues
 * to handle any animations or updates during this time.
 */
void splash_screen_show(uint32_t duration_ms) {
    /* Verify LVGL display is initialized */
    if (!lv_disp_get_default()) {
        return;  /* LVGL display not ready */
    }

    /* Get the active display and its dimensions */
    lv_disp_t *disp = lv_disp_get_default();
    uint32_t scr_width = lv_disp_get_hor_res(disp);
    uint32_t scr_height = lv_disp_get_ver_res(disp);

    /* Create a container for the splash screen */
    splash_screen = lv_obj_create(NULL);
    if (!splash_screen) {
        return;  /* Failed to create object */
    }

    /* Configure the splash screen container */
    lv_obj_set_size(splash_screen, scr_width, scr_height);
    lv_obj_set_pos(splash_screen, 0, 0);
    
    /* Set black background */
    lv_obj_set_style_bg_color(splash_screen, lv_color_black(), 0);
    lv_obj_set_style_border_width(splash_screen, 0, 0);
    lv_obj_set_style_pad_all(splash_screen, 0, 0);

    /* Create image widget and set the splash image */
    lv_obj_t *img = lv_img_create(splash_screen);
    if (!img) {
        lv_obj_del(splash_screen);
        splash_screen = NULL;
        return;
    }

    lv_img_set_src(img, &TFTSplash);

    /* Center the image on the screen */
    lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);

    /* Load and display the splash screen */
    lv_scr_load(splash_screen);

    /* Display the splash screen for the specified duration */
    uint32_t start_tick = HAL_GetTick();

    while ((HAL_GetTick() - start_tick) < duration_ms) {
        /* Process LVGL tasks (animations, redraws, etc.) */
        lv_task_handler();

        /* Small delay to prevent busy-waiting and allow other tasks to run */
        HAL_Delay(10);  /* 10ms between LVGL updates */
    }

    /* Clean up after timeout expires */
    splash_screen_cleanup();
}

/**
 * Display splash screen asynchronously
 * 
 * This version is suitable for applications where you want to continue
 * initialization while the splash is displayed. Call this function once,
 * then call splash_screen_is_active() in your main loop.
 */
void splash_screen_show_async(uint32_t duration_ms) {
    /* Verify LVGL display is initialized */
    if (!lv_disp_get_default()) {
        return;
    }

    lv_disp_t *disp = lv_disp_get_default();
    uint32_t scr_width = lv_disp_get_hor_res(disp);
    uint32_t scr_height = lv_disp_get_ver_res(disp);

    /* Create splash screen container */
    splash_screen = lv_obj_create(NULL);
    if (!splash_screen) {
        return;
    }

    /* Configure container */
    lv_obj_set_size(splash_screen, scr_width, scr_height);
    lv_obj_set_pos(splash_screen, 0, 0);
    lv_obj_set_style_bg_color(splash_screen, lv_color_black(), 0);
    lv_obj_set_style_border_width(splash_screen, 0, 0);
    lv_obj_set_style_pad_all(splash_screen, 0, 0);

    /* Create and configure image */
    lv_obj_t *img = lv_img_create(splash_screen);
    if (!img) {
        lv_obj_del(splash_screen);
        splash_screen = NULL;
        return;
    }

    lv_img_set_src(img, &TFTSplash);
    lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);

    /* Load the splash screen */
    lv_scr_load(splash_screen);

    /* Store start time and duration for timeout checking */
    splash_start_tick = HAL_GetTick();
    splash_duration = duration_ms;
}

/**
 * Check if splash screen is still active (for asynchronous usage)
 * 
 * @return true if splash is still displayed, false if timeout expired
 * 
 * When this returns false, the splash screen has been automatically cleaned up
 */
bool splash_screen_is_active(void) {
    if (splash_screen == NULL) {
        return false;
    }

    /* Check if timeout has expired */
    if ((HAL_GetTick() - splash_start_tick) >= splash_duration) {
        /* Timeout expired, clean up */
        splash_screen_cleanup();
        return false;
    }

    return true;
}

/**
 * Clean up splash screen resources
 * 
 * Deletes the splash screen object and resets the pointer.
 * Called automatically by splash_screen_show() after timeout.
 * Can be called manually if needed.
 */
void splash_screen_cleanup(void) {
    if (splash_screen != NULL) {
        /* Delete the screen object and all its children */
        lv_obj_del(splash_screen);
        splash_screen = NULL;
    }

    /* Reset timer variables */
    splash_start_tick = 0;
    splash_duration = 0;
}