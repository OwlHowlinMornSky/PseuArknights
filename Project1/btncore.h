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
#pragma once

#include <SFML/Graphics.hpp>

namespace OHMS {

	class Button {
		friend class GSbuttonArray;
	private:
		sf::Vector2f button_box[2]; // 碰撞箱 
		sf::Sprite button_sprite, back_sprite; // 前景图； 背景图
		void reloadButtonBox(); // 重载碰撞箱
		unsigned char bStatus; // 状态
		sf::Text button_text; // 前景文字
		bool text_enable; // 前景文字是否可用
	public:
		unsigned char ahoverRGB, ahoverAlpha; // 格式 （悬停） 
		unsigned char pressRGB, pressAlpha; // 格式 （按押） 

		Button();
		~Button();

		void initialize(sf::Texture &texture); // 以材质初始化

		void setStyle(unsigned char nAhoverRGB, unsigned char nPressRGB, unsigned char nAhoverAlpha = 255, unsigned char nPressAlpha = 255); // 设置格式 
		void setText(sf::Font &font, sf::String str, unsigned int csize, bool enable_text = true); // 设置前景文字

		void setSpriteFromTexture(sf::Texture &texture); //效果等同 initialize
		void setPosition(float x, float y); // 设置位置（绝对）
		void setScale(float x, float y); // 设置缩放（绝对）
		void setOrigin(float x, float y); // 设置原点（相对于sprite）（只改变sprite）
		bool checkOn(sf::Vector2i mousepos); // 检查鼠标坐标

		void draw(sf::RenderWindow *window); // 在window中绘制

		static OHMS::Button make_button(sf::Texture &texture); // 静态构造函数
	};

}
