//
// Created by Andy on 2018-03-24.
//

#pragma once


#include <vector>
#include <memory>
#include "../entities/entity.hpp"
#include "../region.hpp"

class QuadTreeNode {
public:

    QuadTreeNode(const Region &region);

    /**
     * Clear the tree
     */
    void clear();

    /**
     * Adds the given entity to this node. If MAX_ENTITIES is exceeded, then the node will gain four child nodes and the node's entities
     * will be distributed amongst its children
     */
    void insert(const Entity &entity);

    /**
     * Retrieves all entities that the given entity could potentially collide with.
     */
    std::vector<Entity> getNearbyEntities(const Entity &entity) const;

private:
    /**
     * The maximum number of entities per leaf node - exceeding this max results in four subnodes being added
     * as children to the leaf node
     */
    static const int MAX_ENTITIES;

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
     * The area of the world that this node corresponds to
     */
    Region m_region;

private:

    /**
     * Divide this node's entities amongst four new nodes that are created as children to this node
     */
    void split();

    /**
     *
     */
    int getIndex(const Entity &entity);


};


