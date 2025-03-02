#pragma once

#include "canvas.h"

#include "bmpmini.hpp"

#include <string_view>

namespace cgfs
{
  namespace detail
  {
    std::vector<uint8_t> rgb_to_bgr(const uint8_t* data, size_t size)
    {
      std::vector<uint8_t> bgr_data(size);
      for(size_t i = 0; i < size; i += 3)
      {
        bgr_data[i] = data[i+2];
        bgr_data[i+1] = data[i+1];
        bgr_data[i+2] = data[i];
      }
      return bgr_data;
    }
  }

  void save_as_bmp(const Canvas& canvas, std::string_view filename)
  {
    image::BMPMini bmp;
    const auto num_channels = 3;
    const auto num_bytes = canvas.extent().width * canvas.extent().height * num_channels;
    auto bgr = detail::rgb_to_bgr((uint8_t*)canvas.data(), num_bytes);
    const auto img = image::ImageView{ canvas.extent().width, canvas.extent().height, num_channels, bgr.data() };
    bmp.write(img, filename.data());
  }
}