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
		AudioCtrl(const unsigned int BGMNum, const unsigned int soundNum); // 以指定数量初始化 BGM, sb 和 sound 
		~AudioCtrl();

		int setBGM(unsigned int num, sf::String path, sf::String name); // 用 path 和 name (文件路径) 设置第 num 个 BGM 
		int setSound(unsigned int num, sf::String path, sf::String name); // 用 path 和 name (文件路径) 设置第 num 个 sound (首先加载sb) 

		void playBGM(unsigned int num = 0); // 在没有 BGM 在播放时播放一个 BGM (已经有 BGM 播放则无效果) 
		void playSound(unsigned int num = 0); // 在没有 sound 在播放时播放一个 sound (已经有 sound 播放则无效果) 

		void setBGMVolumn(float vl); // 设置 BGM 音量 (0 ~ 100) 
		void setSoundVolumn(float vl); // 设置 sound 音量 (0 ~ 100) 
		void setVolumn(float vl); // 设置音量 (使用 setBGMVolumn 和 setSoundVolumn) (0 ~ 100) 

		void changeBGM(unsigned int num = 0); // 播放 BGM (已经有 BGM 播放则停止之前播放的) 
		void changeSound(unsigned int num = 0); // 播放 sound (已经有 sound 播放则停止之前播放的) 
		void addSound(unsigned int num = 0); // 播放 sound (不改变之前播放的) 

		sf::Music *getBGM(); // 取得 BGM 地址 
		sf::Sound *getSound(); // 取得 sound 地址 

		void reset(); // 终止播放 (sf::Music::stop, sf::Sound::stop) 

		unsigned int getBGMFlag(); // 取得当前 BGM 编号 
		unsigned int getSoundFlag(); // 取得当前 sound 编号 
	};

}
