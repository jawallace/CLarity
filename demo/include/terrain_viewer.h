//! @file       terrain_viewer.h
//! @brief      Declaration for Qt Widget that generates terrain
//!
//! @author     Jeffrey Wallace
//! @copyright  MIT

#pragma once

// CLarity Imports
#include "terrain_generator.h"

// Standard Imports
#include <memory>

// Third-Party Imports
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSlider>
#include <QWidget>


namespace clarity {
namespace demo {


class Terrain_Viewer : public QWidget
{
private:
    Q_OBJECT

public:
    Terrain_Viewer(QWidget * parent = nullptr);

private slots:
    void on_generate();

private:
    Terrain m_terrain;
    std::unique_ptr<Terrain_Generator> m_generator;

    QLabel m_img_lbl;
    QLineEdit m_scale_box;
    QSlider m_detail_slider;
    QSlider m_roughness_slider;
    QPushButton m_generate_btn;
};


}}
