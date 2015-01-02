/*
 Copyright (C) 2010-2014 Kristian Duske
 
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
 along with TrenchBroom. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ToolChain.h"

#include "View/ToolAdapter.h"

#include <cassert>

namespace TrenchBroom {
    namespace View {
        ToolChain::ToolChain() :
        m_tool(NULL),
        m_suffix(NULL) {}
        
        ToolChain::~ToolChain() {
            delete m_suffix;
        }

        void ToolChain::append(ToolAdapter* tool) {
            assert(checkInvariant());
            if (chainEndsHere()) {
                assert(m_suffix == NULL);
                m_tool = tool;
                m_suffix = new ToolChain();
            } else {
                assert(m_suffix != NULL);
                m_suffix->append(tool);
            }
            assert(checkInvariant());
        }
        
        void ToolChain::pick(const InputState& inputState, Model::PickResult& pickResult) {
            assert(checkInvariant());
            if (!chainEndsHere()) {
                m_tool->pick(inputState, pickResult);
                m_suffix->pick(inputState, pickResult);
            }
        }
        
        void ToolChain::modifierKeyChange(const InputState& inputState) {
            assert(checkInvariant());
            if (!chainEndsHere()) {
                m_tool->modifierKeyChange(inputState);
                m_suffix->modifierKeyChange(inputState);
            }
        }
        
        void ToolChain::mouseDown(const InputState& inputState) {
            assert(checkInvariant());
            if (!chainEndsHere()) {
                m_tool->mouseDown(inputState);
                m_suffix->mouseDown(inputState);
            }
        }
        
        void ToolChain::mouseUp(const InputState& inputState) {
            assert(checkInvariant());
            if (!chainEndsHere()) {
                m_tool->mouseUp(inputState);
                m_suffix->mouseUp(inputState);
            }
        }
        
        bool ToolChain::mouseClick(const InputState& inputState) {
            assert(checkInvariant());
            if (chainEndsHere())
                return false;
            if (m_tool->mouseClick(inputState))
                return true;
            return m_suffix->mouseClick(inputState);
        }

        bool ToolChain::mouseDoubleClick(const InputState& inputState) {
            assert(checkInvariant());
            if (chainEndsHere())
                return false;
            if (m_tool->mouseDoubleClick(inputState))
                return true;
            return m_suffix->mouseDoubleClick(inputState);
        }
        
        void ToolChain::mouseScroll(const InputState& inputState) {
            assert(checkInvariant());
            if (!chainEndsHere()) {
                m_tool->mouseScroll(inputState);
                m_suffix->mouseScroll(inputState);
            }
        }
        
        void ToolChain::mouseMove(const InputState& inputState) {
            assert(checkInvariant());
            if (!chainEndsHere()) {
                m_tool->mouseMove(inputState);
                m_suffix->mouseMove(inputState);
            }
        }
        
        ToolAdapter* ToolChain::startMouseDrag(const InputState& inputState) {
            assert(checkInvariant());
            if (chainEndsHere())
                return NULL;
            if (m_tool->startMouseDrag(inputState))
                return m_tool;
            return m_suffix->startMouseDrag(inputState);
        }
        
        ToolAdapter* ToolChain::dragEnter(const InputState& inputState, const String& payload) {
            assert(checkInvariant());
            if (chainEndsHere())
                return NULL;
            if (m_tool->dragEnter(inputState, payload))
                return m_tool;
            return m_suffix->dragEnter(inputState, payload);
        }
        
        void ToolChain::setRenderOptions(const InputState& inputState, Renderer::RenderContext& renderContext) const {
            assert(checkInvariant());
            if (!chainEndsHere()) {
                m_tool->setRenderOptions(inputState, renderContext);
                m_suffix->setRenderOptions(inputState, renderContext);
            }
        }
        
        void ToolChain::render(const InputState& inputState, Renderer::RenderContext& renderContext, Renderer::RenderBatch& renderBatch) {
            assert(checkInvariant());
            if (!chainEndsHere()) {
                m_tool->render(inputState, renderContext, renderBatch);
                m_suffix->render(inputState, renderContext, renderBatch);
            }
        }
        
        bool ToolChain::cancel() {
            assert(checkInvariant());
            if (chainEndsHere())
                return false;
            if (m_tool->cancel())
                return true;
            return m_suffix->cancel();
        }

        bool ToolChain::checkInvariant() const {
            return (m_tool == NULL) == (m_suffix == NULL);
        }

        bool ToolChain::chainEndsHere() const {
            return m_tool == NULL;
        }
    }
}
