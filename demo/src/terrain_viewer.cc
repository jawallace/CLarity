//! @file       terrain_viewer.h
//! @brief      Declaration for Qt Widget that generates terrain
//!
//! @author     Jeffrey Wallace
//! @copyright  MIT

// CLarity Imports
#include "diamond_square_terrain_generator.h"
#include "terrain.h"
#include "terrain_generator.h"
#include "terrain_viewer.h"

// Standard Imports
#include <cmath>
#include <iostream>
#include <limits>

// Third-Party Imports
#include <QByteArray>
#include <QHBoxLayout>
#include <QImage>
#include <QLabel>
#include <QLineEdit>
#include <QPixmap>
#include <QPushButton>
#include <QSizePolicy>
#include <QSlider>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>


static constexpr int _VIEW_SIZE = 256;

static const QString _DEFAULT_SCALE = "25";
static const QString _SCALE_TOOLTIP = "Sets the scale of the terrain, in meters per pixel.";

static constexpr uint8_t _MIN_DETAIL = 8;
static constexpr uint8_t _DEFAULT_DETAIL = 10;
static constexpr uint8_t _MAX_DETAIL = 12;
static const QString _DETAIL_TOOLTIP = 
    "Sets the level of detail of the generated terrain. More detail means more pixels generated";

static constexpr uint8_t _MIN_ROUGHNESS = 1;
static constexpr uint8_t _DEFAULT_ROUGHNESS = 50;
static constexpr uint8_t _MAX_ROUGHNESS = 100;
static const QString _ROUGHNESS_TOOLTIP = 
    "Sets the roughness of the generated terrain. A higher roughness value increases the variance"
    " between neighboring pixels";


namespace clarity {
namespace demo {

Terrain_Viewer::Terrain_Viewer(QWidget * parent)
    : QWidget(parent)
    , m_terrain(512, 512, 25.)
    , m_generator(new Diamond_Square_Generator())
    , m_img_lbl(this)
    , m_scale_box(this)
    , m_detail_slider(Qt::Horizontal, this)
    , m_roughness_slider(Qt::Horizontal, this)
    , m_generate_btn("Generate Terrain", this)
{
    // Initialize the image label
    m_img_lbl.setMinimumSize(_VIEW_SIZE, _VIEW_SIZE);
    m_img_lbl.setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    // Initialize the scale box
    m_scale_box.setText(_DEFAULT_SCALE);
    m_scale_box.setAlignment(Qt::AlignRight);

    // Initialize the detail slider
    m_detail_slider.setMinimum(_MIN_DETAIL);
    m_detail_slider.setValue(_DEFAULT_DETAIL);
    m_detail_slider.setMaximum(_MAX_DETAIL);
    
    // Initialize the roughness slider
    m_roughness_slider.setMinimum(_MIN_ROUGHNESS);
    m_roughness_slider.setValue(_DEFAULT_ROUGHNESS);
    m_roughness_slider.setMaximum(_MAX_ROUGHNESS);

    // Initialize layout
    QVBoxLayout * layout = new QVBoxLayout;
    
    layout->addWidget(&m_img_lbl);
    
    {
        QLabel * lbl = new QLabel("Scale", this);
        lbl->setToolTip(_SCALE_TOOLTIP);
        QHBoxLayout * hbox = new QHBoxLayout;
        hbox->addWidget(lbl);
        hbox->addStretch();
        hbox->addWidget(&m_scale_box);
        layout->addLayout(hbox);
    }

    // Add Detail slider
    {
        QLabel * lbl = new QLabel("Detail", this);
        lbl->setToolTip(_DETAIL_TOOLTIP);
        layout->addWidget(lbl);
        layout->addWidget(&m_detail_slider);
    }
    
    // Add Roughness slider
    {
        QLabel * lbl = new QLabel("Roughness", this);
        lbl->setToolTip(_ROUGHNESS_TOOLTIP);
        layout->addWidget(lbl);
        layout->addWidget(&m_roughness_slider);
    }

    // Add Button
    layout->addWidget(&m_generate_btn);
    layout->addStretch();

    setLayout(layout);

    // Setup events
    connect(&m_generate_btn, &QPushButton::clicked, this, &Terrain_Viewer::on_generate);

    // Generate an initial terrain
    on_generate();
}


void Terrain_Viewer::on_generate()
{
    std::cerr << "Generate!" << std::endl;
    
    ////////////////////////////////////////////////////////////////////////////////// 
    //// Generate the terrain
    ////////////////////////////////////////////////////////////////////////////////// 
    const uint32_t size = std::pow(2, m_detail_slider.value()) + 1;
    const float roughness = m_roughness_slider.value() / static_cast<float>(_MAX_ROUGHNESS);
    const float scale = m_scale_box.text().toFloat();
    m_terrain = m_generator->generate_terrain(size, size, scale, roughness);

    ////////////////////////////////////////////////////////////////////////////////// 
    //// Convert to a grayscale image
    ////////////////////////////////////////////////////////////////////////////////// 
    float max = std::numeric_limits<float>::min();
    float min = std::numeric_limits<float>::max();
    // Find max and min values in the terrain map
    for (uint32_t r = 0; r < size; r++) {
        for (uint32_t c = 0; c < size; c++) {
            const float val = m_terrain.data().at(r, c);
            
            if (val > max) {
                max = val;
            }

            if (val < min) {
                min = val;
            }
        }
    }

    QByteArray grayscale(size * size * 4, 255);
    for (uint32_t r = 0; r < size; r++) {
        for (uint32_t c = 0; c < size; c++) {
            const uint32_t offset = (r * size * 4) + (c * 4);

            const float val = m_terrain.data().at(r, c);
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
               size, 
               size, 
               QImage::Format_RGBA8888);
    m_img_lbl.setPixmap(QPixmap::fromImage(img).scaled(_VIEW_SIZE, _VIEW_SIZE));
}

}}
