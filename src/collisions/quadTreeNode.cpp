//
// Created by Andy on 2018-03-24.
//

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

}

std::vector<Entity> QuadTreeNode::getNearbyEntities(const Entity &entity) const {

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


    // insert entities into correct child nodes

}