//
// Created by Andy on 2018-02-21.
//

#pragma once


#include "../actionNode.hpp"

class MoveIntoRange : public ActionNode {
public:
    STATUS tick(Enemy *enemy, float ms) override;

};



