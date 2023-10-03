/**
* Tyler Parret True (OwlHowlinMornSky) <mysteryworldgod@outlook.com>
*
*                   GNU AFFERO GENERAL PUBLIC LICENSE
*                      Version 3, 19 November 2007
*
*    Copyright (c) 2023  Tyler Parret True
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
*/
#pragma once

#include <SFML/Audio.hpp>
#include <fstream>
#include "OHMS.h"

namespace OHMS {

	class AudioCtrl {
	private:
		sf::Music *BGM;
		sf::SoundBuffer *sb;
		sf::Sound *sound;
		unsigned int BGMNum, BGMFlag;
		unsigned int soundNum, soundFlag;
	public:
		AudioCtrl(const unsigned int BGMNum, const unsigned int soundNum); // ��ָ��������ʼ�� BGM, sb �� sound 
		~AudioCtrl();

		int setBGM(unsigned int num, sf::String path, sf::String name); // �� path �� name (�ļ�·��) ���õ� num �� BGM 
		int setSound(unsigned int num, sf::String path, sf::String name); // �� path �� name (�ļ�·��) ���õ� num �� sound (���ȼ���sb) 

		void playBGM(unsigned int num = 0); // ��û�� BGM �ڲ���ʱ����һ�� BGM (�Ѿ��� BGM ��������Ч��) 
		void playSound(unsigned int num = 0); // ��û�� sound �ڲ���ʱ����һ�� sound (�Ѿ��� sound ��������Ч��) 

		void setBGMVolumn(float vl); // ���� BGM ���� (0 ~ 100) 
		void setSoundVolumn(float vl); // ���� sound ���� (0 ~ 100) 
		void setVolumn(float vl); // �������� (ʹ�� setBGMVolumn �� setSoundVolumn) (0 ~ 100) 

		void changeBGM(unsigned int num = 0); // ���� BGM (�Ѿ��� BGM ������ֹ֮ͣǰ���ŵ�) 
		void changeSound(unsigned int num = 0); // ���� sound (�Ѿ��� sound ������ֹ֮ͣǰ���ŵ�) 
		void addSound(unsigned int num = 0); // ���� sound (���ı�֮ǰ���ŵ�) 

		sf::Music *getBGM(); // ȡ�� BGM ��ַ 
		sf::Sound *getSound(); // ȡ�� sound ��ַ 

		void reset(); // ��ֹ���� (sf::Music::stop, sf::Sound::stop) 

		unsigned int getBGMFlag(); // ȡ�õ�ǰ BGM ��� 
		unsigned int getSoundFlag(); // ȡ�õ�ǰ sound ��� 
	};

}
