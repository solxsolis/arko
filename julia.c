#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

const double DEFAULT_CONST[2] = {0.248, 0.1};
const double DEFAULT_RANGES[4] = {-3.0, 6.0, -2.0, 4.0};
const uint32_t DEFAULT_SIZE[2] = {300, 200};

const double RED = 0xFF;
const double GREEN = 0xBE;
const double BLUE = 0x0B;

void drawJulia(uint8_t * buffer, uint32_t lineSize, uint32_t lines, double reConst, double imConst, double minRe, double sizeRe, double minIm, double sizeIm);

void initDefaultValues(double * reConst, double * imConst, double * minRe, double * sizeRe, double * minIm, double * sizeIm, uint32_t * lineSize, uint32_t * lines, uint8_t ** pixelBuffer) {
    *reConst = DEFAULT_CONST[0];
    *imConst = DEFAULT_CONST[1];
    *minRe = DEFAULT_RANGES[0];
    *sizeRe = DEFAULT_RANGES[1];
    *minIm = DEFAULT_RANGES[2];
    *sizeIm = DEFAULT_RANGES[3];
    *lineSize = DEFAULT_SIZE[0];
    *lines = DEFAULT_SIZE[1];
    *pixelBuffer = (uint8_t *)malloc((*lineSize) * (*lines));
}

bool initAllegro(ALLEGRO_EVENT_QUEUE **eventQueue, ALLEGRO_DISPLAY **display, int lineSize, int lines) {
    if(!al_init()) return false;
    if (!al_install_keyboard()) return false;
    *eventQueue = al_create_event_queue();
    *display = al_create_display(lineSize, lines);
    al_register_event_source(*eventQueue, al_get_keyboard_event_source());
    al_register_event_source(*eventQueue, al_get_display_event_source(*display));
    return true;
}

void handleRedraw(ALLEGRO_DISPLAY * display, double  reConst, double  imConst, double  minRe, double  sizeRe, double  minIm, double  sizeIm, uint32_t  lineSize, uint32_t  lines, char * pixelBuffer) {
    int y = 0;
    drawJulia(pixelBuffer, lineSize, lines, reConst, imConst, minRe, sizeRe, minIm, sizeIm);
    al_lock_bitmap(al_get_backbuffer(display), ALLEGRO_PIXEL_FORMAT_ANY, 0);
    for (; y < lines; ++y) {
        for (int x = 0; x < lineSize; ++x) {
            uint8_t iterations = pixelBuffer[y * lineSize + x];
            ALLEGRO_COLOR color = al_map_rgb(iterations * RED / 255, iterations * GREEN / 255, iterations * BLUE / 255);
            al_put_pixel(x, y, color);
        }
    }
    al_unlock_bitmap(al_get_backbuffer(display));
    al_flip_display();
}

void zoomIn(double *minRe, double *sizeRe, double * minIm, double *sizeIm) {
    *sizeRe /= 2;
    *minRe += *sizeRe / 2;
    *sizeIm /= 2;
    *minIm += *sizeIm / 2;
}

void zoomOut(double *minRe, double *sizeRe, double * minIm, double *sizeIm) {
    *minRe -= *sizeRe / 2;
    *sizeRe *= 2;
    *minIm -= *sizeIm / 2;
    *sizeIm *= 2;
}

void handleEvents(ALLEGRO_EVENT event, bool * redraw, double * reConst, double * imConst, double * minRe, double * sizeRe, double * minIm, double * sizeIm) {
    if (event.type == ALLEGRO_EVENT_KEY_CHAR) {
        *redraw = true;
        switch (event.keyboard.keycode) {
            case ALLEGRO_KEY_LEFT: *minRe -= *sizeRe / 50; break;
            case ALLEGRO_KEY_RIGHT: *minRe += *sizeRe / 50; break;
            case ALLEGRO_KEY_DOWN: *minIm += *sizeIm / 50; break;
            case ALLEGRO_KEY_UP: *minIm -= *sizeIm / 50; break;
            case ALLEGRO_KEY_SPACE: zoomIn(minRe, sizeRe, minIm, sizeIm); break;
            case ALLEGRO_KEY_ENTER: zoomOut(minRe, sizeRe, minIm, sizeIm); break;
        }
        switch (event.keyboard.unichar) {
            case 'w': *imConst += (double)*sizeIm / 500; break;
            case 's': *imConst -= (double)*sizeIm / 500; break;
            case 'a': *reConst -= (double)*sizeRe / 500; break;
            case 'd': *reConst += (double)*sizeRe / 500; break;
        }
    }
}

int main() {
    // Init default values, and allocate a pixel buffer
    double reConst, imConst, minRe, sizeRe, minIm, sizeIm;
    uint32_t lineSize, lines;
    uint8_t *pixelBuffer;
    initDefaultValues(&reConst, &imConst, &minRe, &sizeRe, &minIm, &sizeIm, &lineSize, &lines, &pixelBuffer);
    // Init allegro's display and event queue
    ALLEGRO_EVENT_QUEUE *eventQueue;
    ALLEGRO_DISPLAY *display;
    if (!initAllegro(&eventQueue, &display, lineSize, lines)) return 1;
    // Redraw is to be set when an event changes any of the drawing values
    bool redraw = true;
    while (true) {
        // Redraw (if any) is postponed until there are no events
        if (al_is_event_queue_empty(eventQueue) && redraw)
        {
            handleRedraw(display, reConst, imConst, minRe, sizeRe, minIm, sizeIm, lineSize, lines, pixelBuffer);
            redraw = false;
        }
        ALLEGRO_EVENT event;
        // Get new event
        al_wait_for_event(eventQueue, &event);
        // Exit loop on window close
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) break;
        handleEvents(event, &redraw, &reConst, &imConst, &minRe, &sizeRe, &minIm, &sizeIm);
    }
    // Clean the memory
    al_destroy_display(display);
    al_destroy_event_queue(eventQueue);
    free(pixelBuffer);
    return 0;
}