//! @file       qt_util.cc
//! @brief      Defines utilities for QT
//!
//! @author     Jeffrey Wallace
//! @copyright  MIT

// CLarity Imports
#include "buffer.h"

// Standard Imports
#include <limits>

// Third-Party Imports
#include <QByteArray>
#include <QImage>
#include <QLabel>
#include <QPixmap>


namespace clarity {
namespace demo {


void display_grayscale_buffer(const Buffer & b, 
                              QLabel & lbl, 
                              const uint32_t out_rows, 
                              const uint32_t out_cols)
{
    const auto size = b.size();
    const uint32_t rows = std::get<0>(size);
    const uint32_t cols = std::get<1>(size);

    float max = std::numeric_limits<float>::min();
    float min = std::numeric_limits<float>::max();
    // Find max and min values in the terrain map
    for (uint32_t r = 0; r < rows; r++) {
        for (uint32_t c = 0; c < cols; c++) {
            const float val = b.at(r, c);
            
            if (val > max) {
                max = val;
            }

            if (val < min) {
                min = val;
            }
        }
    }

    QByteArray grayscale(rows * cols * 4, 255);
    for (uint32_t r = 0; r < rows; r++) {
        for (uint32_t c = 0; c < cols; c++) {
            const uint32_t offset = (r * cols * 4) + (c * 4);

            const float val = b.at(r, c);
            const uint8_t gray = static_cast<uint8_t>(255.f * (val - min) / (max - min));

            grayscale[offset + 0] = gray; // R
            grayscale[offset + 1] = gray; // G
            grayscale[offset + 2] = gray; // B
            // Alpha set by byte array constructor
        }
    }
    
    ////////////////////////////////////////////////////////////////////////////////// 
    //// Display the grayscale image
    ////////////////////////////////////////////////////////////////////////////////// 
    QImage img(reinterpret_cast<const unsigned char *>(grayscale.constData()), 
               rows, 
               cols, 
               QImage::Format_RGBA8888);
    lbl.setPixmap(QPixmap::fromImage(img).scaled(out_rows, out_cols));
}


}}
