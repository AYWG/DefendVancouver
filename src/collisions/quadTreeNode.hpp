//
// Created by Andy on 2018-03-24.
//

#pragma once


#include <vector>
#include <array>
#include <memory>
#include "../entities/entity.hpp"
#include "../region.hpp"

class QuadTreeNode {
public:
    QuadTreeNode(const int &level, const Region &region);

    /**
     * Clear the tree
     */
    void clear();

    /**
     * Adds the given entity to this node. If MAX_ENTITIES is exceeded, then the node will gain four child nodes and the node's entities
     * will be distributed amongst its children
     */
    void insert(const std::shared_ptr<Entity> &entity);

    /**
     * Retrieves all entities that the given entity could potentially collide with. To be called after the tree is formed.
     */
    std::vector<std::shared_ptr<Entity>> getNearbyEntities(const std::shared_ptr<Entity> &entity) const;

private:
    /**
     * The maximum number of entities per leaf node - exceeding this max results in four subnodes being added
     * as children to the leaf node
     */
    static const int MAX_ENTITIES;

    /**
     * The deepest level leaf node (root is 0)
     */
    static const int MAX_LEVELS;

    /**
     * A node can either be childless or have four children.
     * Indices map to quadrants as follows:
     * 0 - Top left
     * 1 - Top right
     * 2 - Bottom left
     * 3 - Bottom right
     */
    std::array<std::unique_ptr<QuadTreeNode>, 4> m_children;

    /**
     * The entities contained by this node
     */
    std::vector<std::shared_ptr<Entity>> m_entities;

    /**
     * The node's level
     */
    int m_level;

    /**
     * The area of the world that this node corresponds to
     */
    Region m_region;

private:

    /**
     * Divide this node's entities amongst four new nodes that are created as children to this node
     */
    void split();

    void getNearbyEntitiesHelper(std::vector<std::shared_ptr<Entity>> &nearbyEntities,
                                 const std::shared_ptr<Entity> &entity) const;

    /**
     * Checks if the given entity is in this node's region (partially or fully)
     * @param entity
     * @return
     */
    bool isEntityInNode(const std::shared_ptr<Entity> &entity) const;

};


