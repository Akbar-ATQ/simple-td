#ifndef UI_HPP
#define UI_HPP

#include "grid_helper.hpp"
#include "global_data.hpp"

#include "raylib.h"
#include "raygui.h"

namespace UI
{
    const float sidePanelWidth {3.0f};
    const Vec2f sidePanelPos = {(MAP_SIZE.x - sidePanelWidth), 0};
    const Rectangle sidePanel {sidePanelPos.x * GRID_SIZE, sidePanelPos.y, (GRID_SIZE * sidePanelWidth), SCREEN_HEIGHT};
    const Rectangle closeButton {(sidePanelPos.x + 1) * GRID_SIZE, sidePanelPos.y, (2 * GRID_SIZE), GRID_SIZE};

    inline void SidePanel()
    {
        GH::DrawRec(sidePanel, BROWN);
    };

    inline void SidePanelCloseButton(bool &toggle)
    {
        if (GuiButton(closeButton, "Close"))
            toggle = false;
    };
};

#endif