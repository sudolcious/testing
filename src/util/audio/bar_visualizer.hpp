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

#pragma once
#include "spectrum_visualizer.hpp"

namespace audio {
class bar_visualizer : public spectrum_visualizer {
    void draw_rectangle_bars();
    void draw_stereo_rectangle_bars();
    void draw_rounded_bars();
    void draw_stereo_rounded_bars();

public:
    explicit bar_visualizer(source::config *cfg);
    void render(gs_effect_t *effect) override;
};
}
