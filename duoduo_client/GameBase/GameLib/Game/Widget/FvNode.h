#pragma once

#include "cocos2d.h"
#include "Game/FV/FvReferencePtr.h"

class NodeEx
	: public cocos2d::Node
{
public:
	static NodeEx* create();
public:
	FV_REFERENCE_HEAD;
};

typedef FvReferencePtr<NodeEx> FvNode;