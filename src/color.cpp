/*
 * Created by Rebekah Rowe on 04.07.22. GPL3
 */

#include <cmath>
#include <chrono>
#include <iostream>
#include <glm/gtx/color_space.hpp>

#include <glez/color.hpp>

namespace glez::color {

static auto start_time = std::chrono::steady_clock::now();
rgba RainbowCurrent() {
    std::chrono::duration<float, std::deca> ctime = std::chrono::steady_clock::now() - start_time;
    auto ret_raw = glm::rgbColor(glm::vec3(fabs(sin(ctime.count())) * 360.0f, 0.85f, 0.9f));
    rgba ret;
    ret.r = ret_raw.r;
    ret.g = ret_raw.g;
    ret.b = ret_raw.b;
    return ret;
}

} // namespace glez::color
