/*
*    PseuArknights
*
*    Copyright (C) 2023  Tyler Parret True
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU Affero General Public License as published
*    by the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU Affero General Public License for more details.
*
*    You should have received a copy of the GNU Affero General Public License
*    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*
* @Authors
*    Tyler Parret True <mysteryworldgod@outlook.com><https://github.com/OwlHowlinMornSky>
*/
#include "btncore.h"

OHMS::Button::Button() {
	this->button_box[0] = { 0.0f, 0.0f };
	this->button_box[1] = { 0.0f, 0.0f };
	this->bStatus = 0;
	this->text_enable = false;
	this->ahoverRGB = 220;
	this->ahoverAlpha = 255;
	this->pressRGB = 140;
	this->pressAlpha = 255;
	return;
}

OHMS::Button::~Button() {
	return;
}

void OHMS::Button::initialize(sf::Texture &texture) {
	this->setSpriteFromTexture(texture);
	this->reloadButtonBox();
	return;
}

void OHMS::Button::reloadButtonBox() {
	sf::FloatRect box = this->button_sprite.getGlobalBounds();
	this->button_box[0] = { box.left, box.top };
	this->button_box[1] = { box.left + box.width, box.top + box.height };
	return;
}

void OHMS::Button::setSpriteFromTexture(sf::Texture &texture) {
	this->button_sprite.setTexture(texture);
	this->reloadButtonBox();
	return;
}

void OHMS::Button::setPosition(float x, float y) {
	this->button_sprite.setPosition(x, y);
	this->button_text.setPosition(x, y);
	this->reloadButtonBox();
	return;
}

void OHMS::Button::setScale(float x, float y) {
	this->button_sprite.setScale(x, y);
	this->button_text.setScale(x, y);
	this->reloadButtonBox();
	return;
}

void OHMS::Button::setOrigin(float x, float y) {
	this->button_sprite.setOrigin(x, y);
	return;
}

bool OHMS::Button::checkOn(sf::Vector2i mousepos) {
	return ((this->button_box[0].x <= mousepos.x) && (mousepos.x <= this->button_box[1].x) && (this->button_box[0].y <= mousepos.y) && (mousepos.y <= this->button_box[1].y));
}

void OHMS::Button::setStyle(unsigned char nAhoverRGB, unsigned char nPressRGB, unsigned char nAhoverAlpha, unsigned char nPressAlpha) {
	this->ahoverRGB = nAhoverRGB;
	this->pressRGB = nPressRGB;
	this->ahoverAlpha = nAhoverAlpha;
	this->pressAlpha = nPressAlpha;
	return;
}

void OHMS::Button::setText(sf::Font &font, sf::String str, unsigned int csize, bool enable_text) {
	this->button_text.setFont(font);
	this->button_text.setString(str);
	this->button_text.setCharacterSize(csize);
	this->button_text.setOutlineColor(sf::Color::Black);
	this->button_text.setOutlineThickness(3);
	this->text_enable = enable_text;
	return;
}

void OHMS::Button::draw(sf::RenderWindow *window) {
	switch (this->bStatus) {
	case 0: {
		this->button_sprite.setColor(sf::Color(255, 255, 255, 255));
		if (this->text_enable)
			this->button_text.setFillColor(sf::Color(255, 255, 255, 255));
		break;
	}
	case 1: {
		this->button_sprite.setColor(sf::Color(this->ahoverRGB, this->ahoverRGB, this->ahoverRGB, this->ahoverAlpha));
		if (this->text_enable)
			this->button_text.setFillColor(sf::Color(this->ahoverRGB, this->ahoverRGB, this->ahoverRGB, this->ahoverAlpha));
		break;
	}
	case 2: {
		this->button_sprite.setColor(sf::Color(this->pressRGB, this->pressRGB, this->pressRGB, this->pressAlpha));
		if (this->text_enable)
			this->button_text.setFillColor(sf::Color(this->pressRGB, this->pressRGB, this->pressRGB, this->pressAlpha));
		break;
	}
	default: {
		this->button_sprite.setColor(sf::Color(255, 255, 255, 255));
		if (this->text_enable)
			this->button_text.setFillColor(sf::Color(255, 255, 255, 255));
		break;
	}
	}
	(*window).draw(this->button_sprite);
	if (this->text_enable)
		(*window).draw(this->button_text);
	return;
}

OHMS::Button OHMS::Button::make_button(sf::Texture &texture) {
	OHMS::Button new_button;
	new_button.initialize(texture);
	return new_button;
}
