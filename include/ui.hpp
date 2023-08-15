#ifndef UI_HPP
#define UI_HPP

#include "tile.hpp"
#include "global_data.hpp"

#include "raylib.h"
#include "raygui.h"

namespace UI
{
    const float sidePanelWidth {3.0f};
    const Rectangle sidePanel {(MAP_SIZE.x - sidePanelWidth), 0, (TILE_SIZE * sidePanelWidth), SCREEN_HEIGHT};
    const Rectangle closeButton {(sidePanel.x + 2) * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE};

    inline void SidePanel()
    {
        Tile::DrawRec(sidePanel, BROWN);
    };

    inline void SidePanelCloseButton(bool& toggle)
    {
        if (GuiButton(closeButton, "Close")) toggle = false;
    };
};

#endif