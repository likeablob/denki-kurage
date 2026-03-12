#include "jellyfish.h"

#include <algorithm>
#include <vector>

static void drawLineSafe(TFT_eSprite &canvas, Point2D p1, Point2D p2,
                         uint16_t color) {
    if(p1.valid && p2.valid) {
        canvas.drawLine(p1.x, p1.y, p2.x, p2.y, color);
    }
}

// Pre-calculate indices for bell triangles (84 triangles)
static Triangle bell_triangles[84];
static bool triangles_initialized = false;

void initTriangles() {
    if(triangles_initialized)
        return;
    int tri_idx = 0;

    // Cap: Peak (0) to Ring 0 (1-12)
    for(int i = 0; i < BELL_POINTS_PER_RING; i++) {
        bell_triangles[tri_idx].v[0] = 0;
        bell_triangles[tri_idx].v[1] = 1 + i;
        bell_triangles[tri_idx].v[2] = 1 + (i + 1) % BELL_POINTS_PER_RING;
        tri_idx++;
    }

    // Rings: Ring r to Ring r+1
    for(int r = 0; r < BELL_RINGS - 1; r++) {
        for(int i = 0; i < BELL_POINTS_PER_RING; i++) {
            int current_ring_base = 1 + r * BELL_POINTS_PER_RING;
            int next_ring_base = 1 + (r + 1) * BELL_POINTS_PER_RING;

            int i1 = i;
            int i2 = (i + 1) % BELL_POINTS_PER_RING;

            // Triangle 1
            bell_triangles[tri_idx].v[0] = current_ring_base + i1;
            bell_triangles[tri_idx].v[1] = next_ring_base + i1;
            bell_triangles[tri_idx].v[2] = current_ring_base + i2;
            tri_idx++;

            // Triangle 2
            bell_triangles[tri_idx].v[0] = current_ring_base + i2;
            bell_triangles[tri_idx].v[1] = next_ring_base + i1;
            bell_triangles[tri_idx].v[2] = next_ring_base + i2;
            tri_idx++;
        }
    }
    triangles_initialized = true;
}

void drawJellyfish(TFT_eSprite &canvas, Point2D bell[NUM_BELL_VERTICES],
                   Point2D tentacles[NUM_TENTACLES][TENTACLE_SEGMENTS],
                   ColorMode mode, bool wireframe) {
    uint16_t base_color = getJellyfishColor(mode);
    if(!wireframe) {
        initTriangles();

        // Backface Culling & Avg Z Calculation
        int visible_count = 0;
        static int visible_indices[84];

        for(int i = 0; i < 84; i++) {
            Point2D &v0 = bell[bell_triangles[i].v[0]];
            Point2D &v1 = bell[bell_triangles[i].v[1]];
            Point2D &v2 = bell[bell_triangles[i].v[2]];

            if(!v0.valid || !v1.valid || !v2.valid) {
                bell_triangles[i].visible = false;
                continue;
            }

            // 2D Cross Product for Backface Culling
            int cross =
                (v1.x - v0.x) * (v2.y - v0.y) - (v1.y - v0.y) * (v2.x - v0.x);
            if(cross < 0) { // Clockwise winding order for front faces
                bell_triangles[i].visible = true;
                bell_triangles[i].avgZ = (v0.z + v1.z + v2.z) / 3.0f;
                visible_indices[visible_count++] = i;
            } else {
                bell_triangles[i].visible = false;
            }
        }

        // Z-Sort visible triangles
        std::sort(visible_indices, visible_indices + visible_count,
                  [](int a, int b) {
                      return bell_triangles[a].avgZ > bell_triangles[b].avgZ;
                  });

        // Render Solid Triangles with depth-based shading
        for(int i = 0; i < visible_count; i++) {
            int idx = visible_indices[i];
            Triangle &tri = bell_triangles[idx];

            // Shading: map Z to brightness (closer = brighter)
            float depth_factor = 1.0f - ((tri.avgZ - 350.0f) / 300.0f);
            if(depth_factor < 0.2f)
                depth_factor = 0.2f;
            if(depth_factor > 1.0f)
                depth_factor = 1.0f;

            uint16_t shaded_col = getJellyfishColor(mode, depth_factor);

            canvas.fillTriangle(bell[tri.v[0]].x, bell[tri.v[0]].y,
                                bell[tri.v[1]].x, bell[tri.v[1]].y,
                                bell[tri.v[2]].x, bell[tri.v[2]].y, shaded_col);
        }
    } else {
        // Wireframe (Old logic)
        for(int i = 0; i < BELL_POINTS_PER_RING; i++) {
            drawLineSafe(canvas, bell[0], bell[1 + i], base_color);
            for(int r = 0; r < BELL_RINGS; r++) {
                int idx = 1 + r * BELL_POINTS_PER_RING + i;
                int n_idx = 1 + r * BELL_POINTS_PER_RING +
                            (i + 1) % BELL_POINTS_PER_RING;
                drawLineSafe(canvas, bell[idx], bell[n_idx], base_color);
                if(r < BELL_RINGS - 1) {
                    int nr_idx = 1 + (r + 1) * BELL_POINTS_PER_RING + i;
                    drawLineSafe(canvas, bell[idx], bell[nr_idx], base_color);
                }
            }
        }
    }

    // Tentacles (Always lines but could use base_color)
    for(int j = 0; j < NUM_TENTACLES; j++) {
        int bi = 1 + (BELL_RINGS - 1) * BELL_POINTS_PER_RING +
                 (j * (BELL_POINTS_PER_RING / NUM_TENTACLES));
        Point2D p_prev = bell[bi];
        for(int s = 0; s < TENTACLE_SEGMENTS; s++) {
            drawLineSafe(canvas, p_prev, tentacles[j][s], base_color);
            p_prev = tentacles[j][s];
        }
    }
}

uint16_t getJellyfishColor(ColorMode mode, float brightness) {
    uint8_t r = 0, g = 0, b = 0;
    switch(mode) {
    case PURPLE:
        r = 144;
        g = 28;
        b = 230;
        break;
    case GOLD:
        r = 230;
        g = 194;
        b = 0;
        break;
    case WHITE:
        r = 230;
        g = 230;
        b = 230;
        break;
    case CYAN:
    default:
        r = 0;
        g = 230;
        b = 230;
        break;
    }

    r = (uint8_t)(r * brightness);
    g = (uint8_t)(g * brightness);
    b = (uint8_t)(b * brightness);

    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}
