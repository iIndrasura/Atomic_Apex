#include "screen.h"

/*
ScreenSize getScreenSize() {
    xcb_connection_t *connection = xcb_connect(NULL, NULL);
    ScreenSize screenSize = {0, 0};

    if (!xcb_connection_has_error(connection)) {
        const xcb_setup_t *setup = xcb_get_setup(connection);
        xcb_screen_iterator_t screen_iterator = xcb_setup_roots_iterator(setup);
        xcb_screen_t *screen = screen_iterator.data;

        screenSize.width = screen->width_in_pixels;
        screenSize.height = screen->height_in_pixels;

        xcb_disconnect(connection);
    }

    return screenSize;
}
*/

ScreenSize getScreenSize() {
    ScreenSize screenSize = {0, 0};
    xcb_connection_t *connection = xcb_connect(NULL, NULL);

    if (xcb_connection_has_error(connection)) {
        fprintf(stderr, "Error connecting to X server\n");
        return screenSize;
    }

    xcb_screen_t *screen = xcb_setup_roots_iterator(xcb_get_setup(connection)).data;

    if (!screen) {
        fprintf(stderr, "Error getting screen information\n");
        xcb_disconnect(connection);
        return screenSize;
    }

    screenSize.width = screen->width_in_pixels;
    screenSize.height = screen->height_in_pixels;

    xcb_disconnect(connection);

    return screenSize;
}

ScreenMidpoint getScreenMidpoint() {
    ScreenSize screenSize = getScreenSize();
    ScreenMidpoint midpoint = {screenSize.width / 2, screenSize.height / 2};
    return midpoint;
}

vec3 worldToScreen(const vec3 pos, Matrix matrix) {
    vec3 out;
    float _x = matrix.matrix[0] * pos.x + matrix.matrix[1] * pos.y + matrix.matrix[2] * pos.z + matrix.matrix[3];
    float _y = matrix.matrix[4] * pos.x + matrix.matrix[5] * pos.y + matrix.matrix[6] * pos.z + matrix.matrix[7];
    out.z = matrix.matrix[12] * pos.x + matrix.matrix[13] * pos.y + matrix.matrix[14] * pos.z + matrix.matrix[15];

    _x *= 1.f / out.z;
    _y *= 1.f / out.z;

    /*
    int width = 1920; // Change this to your resolution.
    int height = 1080;

    out.x = width * .5f;
    out.y = height * .5f;

    out.x += 0.5f * _x * width + 0.5f;
    out.y -= 0.5f * _y * height + 0.5f;
    */

    ScreenSize screenSize = getScreenSize();
    int screenWidth = screenSize.width;
    int screenHeight = screenSize.height;

    out.x = screenWidth * .5f;
    out.y = screenHeight * .5f;

    out.x += 0.5f * _x * screenWidth + 0.5f;
    out.y -= 0.5f * _y * screenHeight + 0.5f;



    return out;
}

PlayerData calculatePlayerData(vec3 w2sEntHead, vec3 w2sEntFeet) {
    PlayerData data;
    data.height = w2sEntFeet.y - w2sEntHead.y;
    data.width = data.height / 1.2f;
    data.middleX = (w2sEntHead.x + w2sEntHead.x) / 2;
    data.middleY = (w2sEntHead.y + w2sEntFeet.y) / 2;
    return data;
}

float vec_WTSdistance(vec3 v1, vec3 v2) {
    float dx = v1.x - v2.x;
    float dy = v1.y - v2.y;
    float dz = v1.z - v2.z;
    return sqrt(dx * dx + dy * dy + dz * dz);
}
