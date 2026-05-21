/*
 * tft_splash.h
 *
 * LVGL splash screen display module
 * Displays a splash screen image for a specified duration on startup
 *
 * Usage:
 *   splash_screen_show(3000);  // Display splash for 3 seconds (blocking)
 *
 * Created: May 20, 2026
 */

#ifndef TFT_SPLASH_H_
#define TFT_SPLASH_H_

#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Display splash screen for specified duration
 * 
 * This function is BLOCKING - it will not return until the splash timeout expires.
 * During the splash display, LVGL task handler continues to process events.
 * 
 * @param duration_ms Duration to display splash in milliseconds
 *                     Examples: 1000 (1 sec), 3000 (3 sec), 5000 (5 sec)
 * 
 * @return void
 * 
 * @note The splash screen image (TFTSplash) must be defined in TFTSplash.c
 * @note LVGL display driver must be initialized before calling this function
 * @note Backlight should be turned on before calling this function
 */
void splash_screen_show(uint32_t duration_ms);

/**
 * Asynchronous version - call periodically from main loop
 * 
 * Usage:
 *   splash_screen_show_async(3000);  // Start splash
 *   while (splash_screen_is_active()) {
 *       lv_task_handler();
 *       HAL_Delay(10);
 *   }
 * 
 * @param duration_ms Duration to display splash in milliseconds
 * @return void
 */
void splash_screen_show_async(uint32_t duration_ms);

/**
 * Check if splash screen is still active
 * 
 * @return true if splash is active, false if timeout expired or not started
 */
bool splash_screen_is_active(void);

/**
 * Clean up splash screen resources
 * Called automatically by splash_screen_show(), but can be called manually
 * if using the asynchronous version.
 * 
 * @return void
 */
void splash_screen_cleanup(void);

#ifdef __cplusplus
}
#endif

#endif /* TFT_SPLASH_H_ */