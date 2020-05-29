#include "Widget.hpp"

#include "../../ui/UiManager.hpp"

Widget::Widget(
		SDL_Rect rect,
		HorizontalAlignment horzAlign,
		VerticalAlignment vertAlign)

		: GuiRegion(rect, horzAlign, vertAlign) {

    this->active = true;

    this->clickable = false;
    this->funcOnClick = [](){};
    this->funcOnRelease = [](){};
    this->selected = false;

    this->visible = false;
    this->drawFunc = [](const Widget&, UiManager&){};
}
Widget::~Widget() {
    for (Widget* child : children) {
		delete child;
	}
	children.clear();
}

bool Widget::getActive() const {
	return active;
}
bool Widget::getClickable() const {
    return clickable;
}
bool Widget::getVisible() const {
    return visible;
}
bool Widget::getSelected() const {
    return selected;
}

void Widget::setClickFunction(std::function<void()> funcOnClick) {
    setClickFunction(funcOnClick, [](){});
}
void Widget::setClickFunction(std::function<void()> funcOnClick, std::function<void()> funcOnRelease) {
    this->funcOnClick = funcOnClick;
    this->funcOnRelease = funcOnRelease;
    enableClick();
}
void Widget::setDrawFunction(std::function<void(const Widget&, UiManager&)> drawFunc) {
    this->drawFunc = drawFunc;
    enableDraw();
}
void Widget::addChild(Widget* widget) {
    children.push_front(widget);
}

void Widget::activate() {
	active = true;
}
void Widget::deactivate() {
	active = false;
}
void Widget::enableClick() {
    clickable = true;
}
void Widget::disableClick() {
    clickable = false;
}
void Widget::enableDraw() {
    visible = true;
}
void Widget::disableDraw() {
    visible = false;
}
void Widget::select() {
    selected = true;
}
void Widget::deselect() {
    selected = false;
}

Widget* Widget::checkOn(float x, float y) {
	if (!withinScreenRect(x, y)) return nullptr;
	// (Future: do further checks in the case where this widget is not rectangular)

	// Check children
	for (Widget* child : children) {
		Widget* childCheck = child->checkOn(x, y);
		if (childCheck != nullptr) return childCheck;
	}

	return clickable ? this : nullptr; // ignore non-clickable widgets
}

void Widget::click() {
	funcOnClick();
}
void Widget::releaseMouse() {
	funcOnRelease();
}

void Widget::update(const SDL_Rect &psRect) {
    if (!active) return;

    calcScreenRect(psRect);

	// Update all the children
	for (Widget* child : children) {
		child->update(screenRect);
	}
}

void Widget::drawBgSprite(UiManager &uiManager, SpriteId spriteId) const {
    uiManager.drawSpriteStretched(screenRect.x, screenRect.y, screenRect.w, screenRect.h, spriteId);
}
void Widget::renderText(UiManager &uiManager, const char *text) const {
    // Implement text alignment and wrap later

    uiManager.drawText(screenRect.x, screenRect.y, text);
}

void Widget::draw(UiManager &uiManager) const {
    if (!active) return;

    // Debug display
	if (clickable) {
        if (selected) uiManager.setColorMask({.4f, .4f, .0f});
        else          uiManager.setColorMask({.0f, .3f, .4f});
        drawBgSprite(uiManager, SPRITE_ID_WIDGET_BG_DEBUG);
    }

	drawFunc(*this, uiManager);

	// Call draw for all the children
	for (Widget* child : children) {
		child->draw(uiManager);
	}
}
