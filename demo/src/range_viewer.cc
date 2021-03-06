//! @file       range_viewer.h
//! @brief      Defines Qt Widget that displays result of range mapping
//!
//! @author     Jeffrey Wallace
//! @copyright  MIT

// CLarity Imports
#include "camera.h"
#include "cl_range_calculator.h"
#include "device_buffer.h"
#include "terrain.h"
#include "range_viewer.h"
#include "qt_util.h"
#include "terrain_viewer.h"

// Standard Imports
#include <cmath>
#include <iostream>
#include <memory>
#include <tuple>

// Third-Party Imports
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QVBoxLayout>
#include <QWidget>
#include <QMouseEvent>

namespace clarity {
namespace demo {

static constexpr float _DEFAULT_CAM_FOV = 100.0f;
static constexpr uint16_t _DEFAULT_CAM_X_DIM = 512;
static constexpr uint16_t _DEFAULT_CAM_Y_DIM = 512;

static constexpr int16_t _MIN_YAW = -180;
static constexpr int16_t _DEFAULT_YAW = 0;
static constexpr int16_t _MAX_YAW = 180;
static const QString _YAW_TOOLTIP = "The yaw of the Camera";

static constexpr int16_t _MIN_PITCH = -180;
static constexpr int16_t _DEFAULT_PITCH = 0;
static constexpr int16_t _MAX_PITCH = 180;
static const QString _PITCH_TOOLTIP = "The pitch of the Camera";

static constexpr int16_t _MIN_ROLL = -180;
static constexpr int16_t _DEFAULT_ROLL = 0;
static constexpr int16_t _MAX_ROLL = 180;
static const QString _ROLL_TOOLTIP = "The roll of the Camera";


Range_Viewer::Range_Viewer(std::shared_ptr<cl::Context> ctx, 
                           Terrain_Viewer & terrain_viewer, 
                           QWidget * parent)
    : QWidget(parent)
    , m_ctx(ctx)
    , m_cam(_DEFAULT_CAM_FOV, _DEFAULT_CAM_X_DIM, _DEFAULT_CAM_Y_DIM)
    , m_terrain(512, 512, 25.0)
    , m_range(*m_ctx, _DEFAULT_CAM_X_DIM, _DEFAULT_CAM_Y_DIM)
    , m_calculator(new CL_Range_Calculator(m_ctx))
    , m_img_lbl(this)
    , m_rng_lbl(this)
    , m_yaw_slider(Qt::Horizontal, this)
    , m_pitch_slider(Qt::Horizontal, this)
    , m_roll_slider(Qt::Horizontal, this)
{
    // Terrain
    std::shared_ptr<Buffer> tbuffer(new Device_Buffer(*m_ctx, 512, 512));
    m_terrain = Terrain(tbuffer, 25.0);

    // Camera Position
    m_cam.set_position(std::make_tuple(256 * 25., 256 * 25., 500 * 25.));

    // Initialize Image Label
    m_img_lbl.setMinimumSize(_DEFAULT_CAM_X_DIM, _DEFAULT_CAM_Y_DIM);
    m_img_lbl.setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    m_img_lbl.setAttribute(Qt::WA_Hover, true);
    m_img_lbl.installEventFilter(this);

    // Yaw slider
    m_yaw_slider.setTickInterval(1);
    m_yaw_slider.setMinimum(_MIN_YAW);
    m_yaw_slider.setValue(_DEFAULT_YAW);
    m_yaw_slider.setMaximum(_MAX_YAW);
    
    // Pitch slider
    m_pitch_slider.setTickInterval(1);
    m_pitch_slider.setMinimum(_MIN_PITCH);
    m_pitch_slider.setValue(_DEFAULT_PITCH);
    m_pitch_slider.setMaximum(_MAX_PITCH);
    
    // Roll slider
    m_roll_slider.setTickInterval(1);
    m_roll_slider.setMinimum(_MIN_ROLL);
    m_roll_slider.setValue(_DEFAULT_ROLL);
    m_roll_slider.setMaximum(_MAX_ROLL);
    //m_roll_slider.setEnabled(false);

    // Set-up Layout
    // Image label
    QVBoxLayout * layout = new QVBoxLayout;
    layout->addWidget(&m_img_lbl);
    
    // Camera Controls
    QHBoxLayout * hlayout = new QHBoxLayout;
    QGroupBox * rlbls = new QGroupBox("Results");
    {
      QVBoxLayout * vbox = new QVBoxLayout;
      QHBoxLayout * hlbl = new QHBoxLayout;
      
      QLabel * lbl = new QLabel("Range: ");
      hlbl->addWidget(lbl);
      hlbl->addWidget(&m_rng_lbl);
      vbox->addLayout(hlbl);
      rlbls->setLayout(vbox);
    }

    QGroupBox * gbox = new QGroupBox("Camera Controls");
    QVBoxLayout * cam_layout = new QVBoxLayout;
  
    // Yaw Slider
    {
        QLabel * lbl = new QLabel("Yaw");
        lbl->setToolTip(_YAW_TOOLTIP);
        cam_layout->addWidget(lbl);
        cam_layout->addWidget(&m_yaw_slider);
    }
    
    // Pitch Slider
    {
        QLabel * lbl = new QLabel("Pitch");
        lbl->setToolTip(_PITCH_TOOLTIP);
        cam_layout->addWidget(lbl);
        cam_layout->addWidget(&m_pitch_slider);
    }
    
    // Roll Slider
    {
        QLabel * lbl = new QLabel("Roll");
        lbl->setToolTip(_ROLL_TOOLTIP);
        cam_layout->addWidget(lbl);
        cam_layout->addWidget(&m_roll_slider);
    }

    gbox->setLayout(cam_layout);
    hlayout->addWidget(rlbls, 1);
    hlayout->addWidget(gbox, 2);
    layout->addLayout(hlayout);

    // Set-up Events
    connect(&m_yaw_slider, &QSlider::valueChanged, this, &Range_Viewer::on_update_camera);
    connect(&m_pitch_slider, &QSlider::valueChanged, this, &Range_Viewer::on_update_camera);
    connect(&m_roll_slider, &QSlider::valueChanged, this, &Range_Viewer::on_update_camera);
    connect(&terrain_viewer, &Terrain_Viewer::generate, this, &Range_Viewer::on_update_terrain);

    setLayout(layout);
}


bool Range_Viewer::eventFilter(QObject * obj, QEvent * evt)
{
  if (evt->type() != QEvent::HoverMove) {
    return false;
  }

  QMouseEvent * mevt = static_cast<QMouseEvent *>(evt);
  const int x = static_cast<int>(mevt->pos().x());
  const int y = static_cast<int>(mevt->pos().y());

  // mark unused
  (void) (&obj);

  m_rng_lbl.setText(QString::number(m_range.at(y, x)));
  return true;
}


void Range_Viewer::on_display()
{
    std::cerr << "on display" << std::endl;
    const auto sz = m_cam.focal_plane_dimensions();

    try {
      m_calculator->Calculate(m_cam, m_terrain, m_range);
    } catch (const std::exception & e) {
      std::cerr << "Caught exception: " << e.what() << std::endl;
    }
    display_grayscale_buffer(m_range, m_img_lbl, std::get<0>(sz), std::get<1>(sz));
}


void Range_Viewer::on_update_terrain(Terrain & terrain)
{
    m_terrain = terrain;
    on_display();
}


void Range_Viewer::on_update_camera()
{
    const float yaw_rad = M_PI * static_cast<float>(m_yaw_slider.value()) / 180.0f;
    m_cam.set_yaw(yaw_rad);
    std::cerr << "Yaw = " << m_cam.yaw() << std::endl; 
    const float pitch_rad = M_PI * static_cast<float>(m_pitch_slider.value()) / 180.0f;
    m_cam.set_pitch(pitch_rad);
    std::cerr << "Pitch = " << m_cam.pitch() << std::endl; 

    // Roll not yet implemented
    //const float roll_rad = M_PI * static_cast<float>(m_roll_slider.value()) / 180.0f;
    //m_cam.set_roll(roll_rad);

    on_display();
}

}}
