/*************************************************************************
 * This file is part of spectralizer
 * github.con/univrsal/spectralizer
 * Copyright 2020 univrsal <universailp@web.de>.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *************************************************************************/

#include "bar_visualizer.hpp"
#include "../../source/visualizer_source.hpp"
#include "../../util/util.hpp"

namespace audio {

void bar_visualizer::draw_rectangle_bars()
{
    size_t i = 0, pos_x = 0;
    uint32_t height;
    for (; i < m_bars_left.size() - DEAD_BAR_OFFSET; i++) { /* Leave the four dead bars the end */
        auto val = m_bars_left[i] > 1.0 ? m_bars_left[i] : 1.0;
        height = UTIL_MAX(static_cast<uint32_t>(round(val)), 1);
        height = UTIL_MIN(height, m_cfg->bar_height);

        pos_x = i * (m_cfg->bar_width + m_cfg->bar_space);
        gs_matrix_push();
        gs_matrix_translate3f(pos_x, (m_cfg->bar_height - height), 0);
        gs_draw_sprite(nullptr, 0, m_cfg->bar_width, height);
        gs_matrix_pop();
    }
}

void bar_visualizer::draw_stereo_rectangle_bars()
{
    size_t i = 0, pos_x = 0;
    uint32_t height_l, height_r;
    uint32_t offset = m_cfg->stereo_space / 2;
    uint32_t center = m_cfg->bar_height / 2 + offset;

    for (; i < m_bars_left.size() - DEAD_BAR_OFFSET; i++) { /* Leave the four dead bars the end */
        double bar_left = (m_bars_left[i] > 1.0 ? m_bars_left[i] : 1.0);
        double bar_right = (m_bars_right[i] > 1.0 ? m_bars_right[i] : 1.0);

        height_l = UTIL_MAX(static_cast<uint32_t>(round(bar_left)), 1);
        height_l = UTIL_MIN(height_l, (m_cfg->bar_height / 2));
        height_r = UTIL_MAX(static_cast<uint32_t>(round(bar_right)), 1);
        height_r = UTIL_MIN(height_r, (m_cfg->bar_height / 2));

        pos_x = i * (m_cfg->bar_width + m_cfg->bar_space);

        /* Top */
        gs_matrix_push();
        gs_matrix_translate3f(pos_x, (center - height_l) - offset, 0);
        gs_draw_sprite(nullptr, 0, m_cfg->bar_width, height_l);
        gs_matrix_pop();

        /* Bottom */
        gs_matrix_push();
        gs_matrix_translate3f(pos_x, center + offset, 0);
        gs_draw_sprite(nullptr, 0, m_cfg->bar_width, height_r);
        gs_matrix_pop();
    }
}

void bar_visualizer::draw_rounded_bars()
{
    size_t i = 0, pos_x = 0;
    uint32_t height;
    uint32_t vert_count = m_cfg->corner_points * 4;
    for (; i < m_bars_left.size() - DEAD_BAR_OFFSET; i++) { /* Leave the four dead bars the end */
        vert_count = 0;
        auto val = m_bars_left[i] > 1.0 ? m_bars_left[i] : 1.0;

        // The bar needs to be at least a square so the circle fits
        height = UTIL_MAX(static_cast<uint32_t>(round(val)), m_cfg->bar_width);
        height = UTIL_MIN(height, m_cfg->bar_height);

        pos_x = i * (m_cfg->bar_width + m_cfg->bar_space);
        auto verts = make_rounded_rectangle(height);
        gs_matrix_push();
        gs_load_vertexbuffer(verts);
        gs_matrix_translate3f(pos_x, (m_cfg->bar_height - height), 0);
        gs_draw(GS_TRISTRIP, 0, (m_cfg->corner_points + 1) * 8 + 20);
        gs_matrix_pop();
        gs_vertexbuffer_destroy(verts);
    }
}

void bar_visualizer::draw_stereo_rounded_bars()
{

    size_t i = 0, pos_x = 0;
    uint32_t height_l, height_r;
    int32_t offset = m_cfg->stereo_space / 2;
    uint32_t center = m_cfg->bar_height / 2 + offset;

    for (; i < m_bars_left.size() - DEAD_BAR_OFFSET; i++) { /* Leave the four dead bars the end */
        double bar_left = (m_bars_left[i] > 1.0 ? m_bars_left[i] : 1.0);
        double bar_right = (m_bars_right[i] > 1.0 ? m_bars_right[i] : 1.0);

        // The bar needs to be at least a square so the circle fits
        height_l = UTIL_MAX(static_cast<uint32_t>(round(bar_left)), m_cfg->bar_width);
        height_l = UTIL_MIN(height_l, (m_cfg->bar_height / 2));
        height_r = UTIL_MAX(static_cast<uint32_t>(round(bar_right)), m_cfg->bar_width);
        height_r = UTIL_MIN(height_r, (m_cfg->bar_height / 2));

        pos_x = i * (m_cfg->bar_width + m_cfg->bar_space);
        auto verts_left = make_rounded_rectangle(height_l);
        auto verts_right = make_rounded_rectangle(height_r);

        /* Top */
        gs_matrix_push();
        gs_load_vertexbuffer(verts_left);
        gs_matrix_translate3f(pos_x, (center - height_l) - offset, 0);
        gs_draw(GS_TRISTRIP, 0, (m_cfg->corner_points + 1) * 8 + 20);
        gs_matrix_pop();

        /* Bottom */
        gs_matrix_push();
        gs_load_vertexbuffer(verts_right);
        gs_matrix_translate3f(pos_x, center + offset, 0);
        gs_draw(GS_TRISTRIP, 0, (m_cfg->corner_points + 1) * 8 + 20);
        gs_matrix_pop();

        gs_vertexbuffer_destroy(verts_left);
        gs_vertexbuffer_destroy(verts_right);
    }
}

bar_visualizer::bar_visualizer(source::config *cfg) : spectrum_visualizer(cfg) {}

void bar_visualizer::render(gs_effect_t *effect)
{
    /* Just in case */
    if (m_bars_left.size() != m_cfg->detail + DEAD_BAR_OFFSET)
        m_bars_left.resize(m_cfg->detail + DEAD_BAR_OFFSET, 0.0);
    if (m_bars_right.size() != m_cfg->detail + DEAD_BAR_OFFSET)
        m_bars_right.resize(m_cfg->detail + DEAD_BAR_OFFSET, 0.0);

    if (m_cfg->stereo) {
        if (m_cfg->rounded_corners) {
            draw_stereo_rounded_bars();
        } else {
            draw_stereo_rectangle_bars();
        }
    } else {
        if (m_cfg->rounded_corners) {
            draw_rounded_bars();
        } else {
            draw_rectangle_bars();
        }
    }
    UNUSED_PARAMETER(effect);
}

}
