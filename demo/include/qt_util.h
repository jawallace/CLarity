//! @file       qt_util.h
//! @brief      Declares utilities for QT
//!
//! @author     Jeffrey Wallace
//! @copyright  MIT

#pragma once

// CLarity Imports
#include "buffer.h"

// Standard Imports
#include <cstdint>

// Third-Party Imports
#include <QLabel>

namespace clarity {
namespace demo {


void display_grayscale_buffer(const Buffer & b, 
                              QLabel & lbl, 
                              const uint32_t out_rows, 
                              const uint32_t out_cols);


}}
