//
// Created by Andy on 2018-02-20.
//

#pragma once


#include "compositeNode.hpp"

/**
 * A SequenceNode returns FAILURE upon the first child failing; RUNNING if any child is being processed, and
 * SUCCESS if all children return SUCCESS.
 */

class SequenceNode : public CompositeNode {
    STATUS tick() override;
};


