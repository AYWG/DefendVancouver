//
// Created by Andy on 2018-03-24.
//

#include <algorithm>
#include "quadTreeNode.hpp"

const int QuadTreeNode::MAX_ENTITIES = 5;

QuadTreeNode::QuadTreeNode(const Region &region) : m_region(region) {}

void QuadTreeNode::clear() {
    m_entities.clear();

    for (auto &childNode: m_children) {
        childNode->clear();
        childNode.reset();
    }
}

void QuadTreeNode::insert(const Entity &entity) {
    if (isEntityInNode(entity)) {
        // Check if there are any children
        if (m_children[0]) {
            for (auto &childNode: m_children) {
                childNode->insert(entity);
            }
        } else {
            // reached leaf node, so add it to this node
            m_entities.emplace_back(std::make_shared(&entity));

            // check if we need to split
            if (m_entities.size() > MAX_ENTITIES) {
                split();
            }
        }
    }
}

std::vector<std::shared_ptr<Entity>> QuadTreeNode::getNearbyEntities(const Entity &entity) const {
    std::vector<std::shared_ptr<Entity>> nearbyEntities;
    getNearbyEntitiesHelper(nearbyEntities, entity);

    // remove duplicates
    std::sort(nearbyEntities.begin(), nearbyEntities.end());
    nearbyEntities.erase(std::unique(nearbyEntities.begin(), nearbyEntities.end()), nearbyEntities.end());

    return nearbyEntities;
}

void QuadTreeNode::split() {
    vec2 quadrantSize = {m_region.size.x / 2, m_region.size.y / 2};

    // create 4 new nodes
    vec2 topLeftOrigin = {m_region.origin.x, m_region.origin.y};
    m_children[0].reset(new QuadTreeNode({topLeftOrigin, quadrantSize}));
    vec2 topRightOrigin = {m_region.origin.x + quadrantSize.x, m_region.origin.y};
    m_children[1].reset(new QuadTreeNode({topRightOrigin, quadrantSize}));
    vec2 bottomLeftOrigin = {m_region.origin.x, m_region.origin.y + quadrantSize.y};
    m_children[2].reset(new QuadTreeNode({bottomLeftOrigin, quadrantSize}));
    vec2 bottomRightOrigin = {m_region.origin.x + quadrantSize.x, m_region.origin.y + quadrantSize.y};
    m_children[3].reset(new QuadTreeNode({bottomRightOrigin, quadrantSize}));

    for (auto &entity: m_entities) {
        for (auto &childNode: m_children) {
            childNode->insert(*entity);
        }
        entity.reset();
    }
}

void QuadTreeNode::getNearbyEntitiesHelper(std::vector<std::shared_ptr<Entity>> &nearbyEntities,
                                           const Entity &entity) const {
    if (isEntityInNode(entity)) {
        if (m_children[0]) {
            for (auto &childNode: m_children) {
                childNode->getNearbyEntitiesHelper(nearbyEntities, entity);
            }
        } else {
            for (auto &nearbyEntity: m_entities) {
                nearbyEntities.emplace_back(nearbyEntity);
            }
        }
    }
}

bool QuadTreeNode::isEntityInNode(const Entity &entity) const {
    // Following cases where it is "in":
    // 1. Completely inside
    // 2. Within left and right bounds, but only partially in top/bottom bounds
    // 3. Within top and bottom bounds, but only partially in left/right bounds
    Region entityBoundingBox = entity.getBoundingBox();
    bool isWithinLeftBounds = entityBoundingBox.origin.x >= m_region.origin.x;
    bool isWithinRightBounds =
            entityBoundingBox.origin.x + entityBoundingBox.size.x <= m_region.origin.x + m_region.size.x;
    bool isWithinTopBounds = entityBoundingBox.origin.y >= m_region.origin.y;
    bool isWithinBottomBounds =
            entityBoundingBox.origin.y + entityBoundingBox.size.y <= m_region.origin.y + m_region.size.y;

    if (isWithinLeftBounds && isWithinRightBounds) {
        if (isWithinTopBounds || isWithinBottomBounds) {
            return true;
        }
    } else if (isWithinTopBounds && isWithinBottomBounds) {
        if (isWithinLeftBounds || isWithinRightBounds) {
            return true;
        }
    }
    return false;
}