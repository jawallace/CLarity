//! @file       terrain_viewer.h
//! @brief      Declaration for Qt Widget that generates terrain
//!
//! @author     Jeffrey Wallace
//! @copyright  MIT

#pragma once

// CLarity Imports
#include "buffer.h"
#include "camera.h"
#include "range_calculator.h"
#include "terrain.h"

// Standard Imports
#include <memory>

// Third-Party Imports
#include <QLabel>
#include <QSlider>
#include <QWidget>

namespace clarity {
namespace demo {

class Range_Viewer : public QWidget
{
private:
    Q_OBJECT

public:
    Range_Viewer(QWidget * parent = nullptr);

private slots:
    void on_display();
    void on_update_terrain(Terrain & terrain);
    void on_update_camera();

private:
    Camera m_cam;
    Terrain m_terrain;
    Buffer m_range;
    std::unique_ptr<Range_Calculator> m_calculator;

    QLabel m_img_lbl;
    QSlider m_yaw_slider;
    QSlider m_pitch_slider;
    QSlider m_roll_slider;
};

}}
