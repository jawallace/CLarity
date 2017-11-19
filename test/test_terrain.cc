//! @file       test_terrain.cc
//! @brief      Unit tests for the terrain type
//!
//! @author     Jeffrey Wallace
//! @copyright  MIT


// CLarity Imports
#include "terrain.h"

// Standard Imports
#include <cstdint>
#include <memory>
#include <utility>

// Third-Party Importss
#include "gtest/gtest.h"

namespace {

using namespace clarity;

TEST(terrain, construction)
{
    Terrain t(512, 512, 30.0);

    const uint32_t sz = 512;
    ASSERT_EQ(sz, t.data().size().first) << "Terrain reported incorrect size";
    ASSERT_EQ(sz, t.data().size().second) << "Terrain reported incorrect size";
    ASSERT_FLOAT_EQ(30.0, t.scale()) << "Terrain reported incorrect scale";

    for (auto r = 0; r < 512; r++) {
        for (auto c = 0; c < 512; c++) {
            ASSERT_FLOAT_EQ(0.0, t.data().at(r, c)) << "Terrain was not zeroed on construction";
        }
    }
}


TEST(terrain, at)
{
    Terrain t(512, 512, 30.0);
    
    t.data().at(100, 100) = 12.5;
    ASSERT_FLOAT_EQ(12.5, t.data().at(100, 100)) << "Could not set/get value with Terrain.at";
}


TEST(terrain, assignment_and_copy)
{
    Terrain * t1 = new Terrain(512, 512, 30.0);
    t1->data().at(100, 100) = 12.5;

    Terrain t2(*t1);

    ASSERT_FLOAT_EQ(12.5, t2.data().at(100, 100)) 
        << "Copied Terrain does not point to the same buffer";

    delete t1;

    ASSERT_FLOAT_EQ(12.5, t2.data().at(100, 100)) 
        << "Copied Terrain did not persist after original was deleted";

    Terrain t3(256, 256, 10.0);
    const uint32_t sz = 256;
    ASSERT_EQ(sz, t3.data().size().first);
    ASSERT_EQ(sz, t3.data().size().second);
    ASSERT_FLOAT_EQ(10.0, t3.scale());

    t3 = t2;
    const uint32_t new_sz = 512;
    ASSERT_EQ(new_sz, t3.data().size().first)
        << "Assignment operator did not set size correctly";
    ASSERT_EQ(new_sz, t3.data().size().second)
        << "Assignment operator did not set size correctly";
    ASSERT_FLOAT_EQ(30.0, t3.scale()) << "Assignment operator did not set scale correclty";
    ASSERT_FLOAT_EQ(12.5, t3.data().at(100, 100)) 
        << "Assignment operator did not copy data pointer correctly";
}


}
