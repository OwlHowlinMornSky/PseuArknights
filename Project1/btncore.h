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
		sf::Vector2f button_box[2]; // ��ײ�� 
		sf::Sprite button_sprite, back_sprite; // ǰ��ͼ�� ����ͼ
		void reloadButtonBox(); // ������ײ��
		unsigned char bStatus; // ״̬
		sf::Text button_text; // ǰ������
		bool text_enable; // ǰ�������Ƿ����
	public:
		unsigned char ahoverRGB, ahoverAlpha; // ��ʽ ����ͣ�� 
		unsigned char pressRGB, pressAlpha; // ��ʽ ����Ѻ�� 

		Button();
		~Button();

		void initialize(sf::Texture &texture); // �Բ��ʳ�ʼ��

		void setStyle(unsigned char nAhoverRGB, unsigned char nPressRGB, unsigned char nAhoverAlpha = 255, unsigned char nPressAlpha = 255); // ���ø�ʽ 
		void setText(sf::Font &font, sf::String str, unsigned int csize, bool enable_text = true); // ����ǰ������

		void setSpriteFromTexture(sf::Texture &texture); //Ч����ͬ initialize
		void setPosition(float x, float y); // ����λ�ã����ԣ�
		void setScale(float x, float y); // �������ţ����ԣ�
		void setOrigin(float x, float y); // ����ԭ�㣨�����sprite����ֻ�ı�sprite��
		bool checkOn(sf::Vector2i mousepos); // ����������

		void draw(sf::RenderWindow *window); // ��window�л���

		static OHMS::Button make_button(sf::Texture &texture); // ��̬���캯��
	};

}
