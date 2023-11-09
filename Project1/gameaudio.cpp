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
#include "gameaudio.h"

OHMS::AudioCtrl::AudioCtrl(const unsigned int BGMNum, const unsigned int soundNum) {
	this->BGM = new sf::Music[BGMNum];
	this->sb = new sf::SoundBuffer[soundNum];
	this->sound = new sf::Sound[soundNum];
	this->BGMNum = BGMNum;
	this->BGMFlag = 0;
	this->soundNum = soundNum;
	this->soundFlag = 0;
	return;
}

OHMS::AudioCtrl::~AudioCtrl() {
	delete[] this->BGM;
	delete[] this->sb;
	delete[] this->sound;
	return;
}

int OHMS::AudioCtrl::setBGM(unsigned int num, sf::String path, sf::String name) {
	if (num < this->BGMNum) {
		this->BGM[num].openFromFile(path + name + ".ogg");
		unsigned long long t1, t2;
		OHMS::filescanf(path + name + ".txt", "%llu %llu", &t1, &t2);
		this->BGM[num].setLoop(true);
		this->BGM[num].setLoopPoints(sf::Music::TimeSpan(sf::microseconds(t1), sf::microseconds(t2)));
		return num;
	}
	return -1;
}

int OHMS::AudioCtrl::setSound(unsigned int num, sf::String path, sf::String name) {
	if (num < this->soundNum) {
		this->sb[num].loadFromFile(path + name + ".wav");
		this->sound[num].setBuffer(this->sb[num]);
		return num;
	}
	return -1;
}

void OHMS::AudioCtrl::playBGM(unsigned int num) {
	if (num < this->BGMNum) {
		if (this->BGM[this->BGMFlag].getStatus() != sf::Music::Status::Playing) {
			this->BGMFlag = num;
			this->BGM[num].play();
		}
	}
	return;
}

void OHMS::AudioCtrl::playSound(unsigned int num) {
	if (num < this->BGMNum) {
		if (this->sound[this->soundFlag].getStatus() != sf::Sound::Status::Playing) {
			this->soundFlag = num;
			this->sound[num].play();
		}
	}
	return;
}

void OHMS::AudioCtrl::setBGMVolumn(float vl) {
	if (vl > 100.0f) vl = 100.0f;
	else if (vl < 0.0f) vl = 0.0f;
	for (unsigned int i = 0; i < this->BGMNum; ++i) {
		this->BGM[i].setVolume(vl);
	}
	return;
}

void OHMS::AudioCtrl::setSoundVolumn(float vl) {
	if (vl > 100.0f) vl = 100.0f;
	else if (vl < 0.0f) vl = 0.0f;
	for (unsigned int i = 0; i < this->soundNum; i++) {
		this->sound[i].setVolume(vl);
	}
	return;
}

void OHMS::AudioCtrl::setVolumn(float vl) {
	this->setBGMVolumn(vl);
	return this->setSoundVolumn(vl);
}

void OHMS::AudioCtrl::changeBGM(unsigned int num) {
	if (num < this->BGMNum) {
		if (this->BGM[this->BGMFlag].getStatus() != sf::Music::Status::Playing) {
			this->BGMFlag = num;
			this->BGM[num].play();
		}
		else {
			this->BGM[this->BGMFlag].stop();
			this->BGMFlag = num;
			this->BGM[num].play();
		}
	}
	return;
}

void OHMS::AudioCtrl::changeSound(unsigned int num) {
	if (num < this->soundNum) {
		if (this->sound[this->soundFlag].getStatus() != sf::Sound::Status::Playing) {
			this->soundFlag = num;
			this->sound[num].play();
		}
	}
	return;
}

void OHMS::AudioCtrl::addSound(unsigned int num) {
	this->soundFlag = num;
	return this->sound[num].play();
}

sf::Music * OHMS::AudioCtrl::getBGM() {
	return this->BGM + this->BGMFlag;
}

sf::Sound * OHMS::AudioCtrl::getSound() {
	return this->sound + this->soundFlag;
}

void OHMS::AudioCtrl::reset() {
	this->BGM[this->BGMFlag].stop();
	this->sound[this->soundFlag].stop();
	this->BGMFlag = 0;
	this->soundFlag = 0;
	return;
}

unsigned int OHMS::AudioCtrl::getBGMFlag() {
	return this->BGMFlag;
}

unsigned int OHMS::AudioCtrl::getSoundFlag() {
	return this->soundFlag;
}
