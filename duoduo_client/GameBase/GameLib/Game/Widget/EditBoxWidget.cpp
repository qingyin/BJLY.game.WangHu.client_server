#include "EditBoxWidget.h"
#include "WidgetFun.h"
#include "Game/Script/utility.h"
#include "Game/Script/WindowInfo.h"
#include "ui/UIEditBox/UIEditBoxImpl.h"


NS_CC_BEGIN;

EditBoxWidget::EditBoxWidget()
{

}
EditBoxWidget::~EditBoxWidget()
{

}
EditBoxWidget* EditBoxWidget::create(const Size& size, ui::Scale9Sprite* pNormal9SpriteBg, ui::Scale9Sprite* pPressed9SpriteBg, ui::Scale9Sprite* pDisabled9SpriteBg)
{
	EditBoxWidget* pRet = new  EditBoxWidget();

	if (pRet != nullptr && pRet->initWithSizeAndBackgroundSprite(size, pNormal9SpriteBg))
	{
		pRet->autorelease();
		pRet->setDelegate(pRet);
		pRet->initState();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}
void EditBoxWidget::initState()
{
	this->setFontColor(cocos2d::Color3B::RED);
	this->setPlaceholderFontColor(cocos2d::Color3B::WHITE);
	this->setMaxLength(20);
	this->setFontSize(30);
}
void EditBoxWidget::SetReturnAction(std::function<void()> pFunReturn )
{
	m_pReturnSelector = pFunReturn;
}

void EditBoxWidget::editBoxEditingDidBegin(EditBox* editBox) 
{
}
void EditBoxWidget::editBoxEditingDidEnd(EditBox* editBox)
{
}
void EditBoxWidget::editBoxTextChanged(EditBox* editBox, const std::string& text)
{
}
void EditBoxWidget::editBoxReturn(EditBox* editBox)
{
	if (m_pReturnSelector)
	{
		(m_pReturnSelector)();
	}
}
NS_CC_END