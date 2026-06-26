#ifndef POST_PROCESSING_H
#define POST_PROCESSING_H

#include "../core/window.h"
#include "../scene/scene.h"

/**
 *  @brief Appply post processing effects
 *
 *  @param scene The scene
 *  @param window The window
 */
void render_scene (const Scene *scene, const Window *window);

#endif // POST_PROCESSING_H
