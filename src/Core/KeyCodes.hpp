/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** KeyCodes
*/

#ifndef KEYCODES_H_
#define KEYCODES_H_

// Basic ASCII values
#define K_BACKSPACE   8   /* Backspace key */
#define K_TAB        9   /* Tab key */
#define K_ENTER      10  /* Enter key */
#define K_ESC        27  /* Escape key */
#define K_SPACE      32  /* Space key */

// Arrow keys
#define K_DOWN       258 /* Down arrow key */
#define K_UP         259 /* Up arrow key */
#define K_LEFT       260 /* Left arrow key */
#define K_RIGHT      261 /* Right arrow key */

// Navigation keys
#define K_HOME       262 /* Home K */
#define K_END        263 /* End K */
#define K_PPAGE      264 /* Page Up K */
#define K_NPAGE      265 /* Page Down K */
#define K_IC         266 /* Insert K */
#define K_DC         267 /* Delete K */

// Function Ks
#define K_F0         300 /* Function K F0 */
#define K_F1         301 /* Function K F1 */
#define K_F2         302 /* Function K F2 */
#define K_F3         303 /* Function K F3 */
#define K_F4         304 /* Function K F4 */
#define K_F5         305 /* Function K F5 */
#define K_F6         306 /* Function K F6 */
#define K_F7         307 /* Function K F7 */
#define K_F8         308 /* Function K F8 */
#define K_F9         309 /* Function K F9 */
#define K_F10        310 /* Function K F10 */
#define K_F11        311 /* Function K F11 */
#define K_F12        312 /* Function K F12 */

// Shift + Function Ks
#define K_SHF1       313 /* Shift + F1 */
#define K_SHF2       314 /* Shift + F2 */
#define K_SHF3       315 /* Shift + F3 */
#define K_SHF4       316 /* Shift + F4 */
#define K_SHF5       317 /* Shift + F5 */
#define K_SHF6       318 /* Shift + F6 */
#define K_SHF7       319 /* Shift + F7 */
#define K_SHF8       320 /* Shift + F8 */
#define K_SHF9       321 /* Shift + F9 */
#define K_SHF10      322 /* Shift + F10 */
#define K_SHF11      323 /* Shift + F11 */
#define K_SHF12      324 /* Shift + F12 */

// Ctrl + Function Ks
#define K_CTRL1      325 /* Ctrl + F1 */
#define K_CTRL2      326 /* Ctrl + F2 */
#define K_CTRL3      327 /* Ctrl + F3 */
#define K_CTRL4      328 /* Ctrl + F4 */
#define K_CTRL5      329 /* Ctrl + F5 */
#define K_CTRL6      330 /* Ctrl + F6 */
#define K_CTRL7      331 /* Ctrl + F7 */
#define K_CTRL8      332 /* Ctrl + F8 */
#define K_CTRL9      333 /* Ctrl + F9 */
#define K_CTRL10     334 /* Ctrl + F10 */
#define K_CTRL11     335 /* Ctrl + F11 */
#define K_CTRL12     336 /* Ctrl + F12 */

// Special Ks
#define K_PADENTER   340 /* Numeric Kpad Enter K */
#define K_NL         341 /* Newline K */

// Mouse events
#define K_RCLICK     342 /* Right-click mouse button */
#define K_LCLICK     344 /* Left-click mouse button */
#define K_MCLICK     339 /* Middle-click mouse button */
#define K_MOTION     345 /* Mouse motion */
#define K_MOUSE      346 /* Mouse event */

// Modifier Ks
#define K_SHIFT      347 /* Shift K */
#define K_CTRL       348 /* Control K */
#define K_ALT        349 /* Alt K */

// Helper macros
#define CTRL(key)     ((key) - 64) /* CTRL + key */
#define ALT(key)      ((key) | 0x80) /* ALT + key */

#endif /* KEYCODES_H_ */
