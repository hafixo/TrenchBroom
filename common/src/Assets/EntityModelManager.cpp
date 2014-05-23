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

#include "EntityModelManager.h"

#include "CollectionUtils.h"
#include "Exceptions.h"
#include "Logger.h"
#include "Assets/EntityModel.h"
#include "Model/Game.h"
#include "Renderer/MeshRenderer.h"

namespace TrenchBroom {
    namespace Assets {
        EntityModelManager::EntityModelManager(Logger* logger) :
        m_logger(logger),
        m_vbo(0xFFFFF),
        m_prepared(true) {}
        
        EntityModelManager::~EntityModelManager() {
            clear();
        }
        
        void EntityModelManager::clear() {
            MapUtils::clearAndDelete(m_renderers);
            MapUtils::clearAndDelete(m_models);
            m_rendererMismatches.clear();
            m_modelMismatches.clear();
            m_prepared = true;
            
            if (m_logger != NULL)
                m_logger->debug("Cleared entity models");
        }
        
        void EntityModelManager::reset(Model::GamePtr game) {
            if (m_game == game)
                return;
            clear();
            m_game = game;
        }
        
        EntityModel* EntityModelManager::model(const IO::Path& path) const {
            if (path.isEmpty())
                return NULL;
            
            ModelCache::const_iterator it = m_models.find(path);
            if (it != m_models.end())
                return it->second;
            
            if (m_modelMismatches.count(path) > 0)
                return NULL;
            
            try {
                EntityModel* model = m_game->loadModel(path);
                assert(model != NULL);
                m_models[path] = model;
                
                if (m_logger != NULL)
                    m_logger->debug("Loaded entity model %s", path.asString().c_str());
                return model;
            } catch (const GameException& e) {
                m_modelMismatches.insert(path);
                if (m_logger != NULL)
                    m_logger->debug("Failed to load entity model %s: %s", path.asString().c_str(), e.what());
                throw;
            }
        }
        
        Renderer::MeshRenderer* EntityModelManager::renderer(const Assets::ModelSpecification& spec) const {
            EntityModel* entityModel = model(spec.path);
            if (entityModel == NULL)
                return NULL;
            
            RendererCache::const_iterator it = m_renderers.find(spec);
            if (it != m_renderers.end())
                return it->second;
            
            if (m_rendererMismatches.count(spec) > 0)
                return NULL;
            
            Renderer::MeshRenderer* renderer = entityModel->buildRenderer(spec.skinIndex, spec.frameIndex);
            if (renderer == NULL) {
                m_rendererMismatches.insert(spec);
                
                if (m_logger != NULL)
                    m_logger->debug("Failed to construct entity model renderer for %s", spec.asString().c_str());
            } else {
                m_renderers[spec] = renderer;
                m_prepared = false;
                
                if (m_logger != NULL)
                    m_logger->debug("Constructed entity model renderer for %s", spec.asString().c_str());
            }
            return renderer;
        }
        
        void EntityModelManager::activateVbo() {
            m_vbo.activate();
            prepareRenderers();
        }
        
        void EntityModelManager::deactivateVbo() {
            m_vbo.deactivate();
        }
        
        void EntityModelManager::prepareRenderers() {
            if (m_prepared)
                return;
            
            Renderer::SetVboState setVboState(m_vbo);
            setVboState.mapped();
            
            RendererCache::const_iterator it, end;
            for (it = m_renderers.begin(), end = m_renderers.end(); it != end; ++it) {
                Renderer::MeshRenderer* renderer = it->second;
                renderer->prepare(m_vbo);
            }
            m_prepared = true;
        }
    }
}