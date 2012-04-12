/*
 Copyright (C) 2010-2012 Kristian Duske
 
 This file is part of TrenchBroom.
 
 TrenchBroom is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 TrenchBroom is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with TrenchBroom.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TrenchBroom_SelectionTool_h
#define TrenchBroom_SelectionTool_h

#include "Tool.h"

namespace TrenchBroom {
    namespace Model {
        class Selection;
    }
    
    namespace Controller {
        class SelectionTool : public Tool {
        public:
            SelectionTool(Editor& editor) : Tool(editor) {}
            
            bool leftMouseUp(ToolEvent& event);
            bool beginLeftDrag(ToolEvent& event);
            void leftDrag(ToolEvent& event);

            static bool multiSelectionModiferPressed(ToolEvent& event);
            static bool gridSizeModifierPressed(ToolEvent& event);
        };
    }
}

#endif