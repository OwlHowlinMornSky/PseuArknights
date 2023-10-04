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
#include "gamecore.h"

void OHMS::BattleGroundMain::setEntityPos(sf::Vector2f pos2d, OHMS::EntityMain * entity) {
	if ( pos2d.x < 0.0f || pos2d.y < 0.0f )
		return this->myGraphic.setAnimaPosition({ pos2d.x, pos2d.y, 0.0f }, entity->myAnime);
	return this->myGraphic.setAnimaPosition({ pos2d.x, pos2d.y, (this->pile.isHighLand((int)pos2d.x, (int)pos2d.y)) ? 0.2f : 0.0f }, entity->myAnime);
}

void OHMS::BattleGroundMain::readFromFile(const std::string path) {
	char str[64], musicName[64];
	char levelIDtmp[64]{ 0 };
	char levelNametmp[64]{ 0 };
	int levelLife = 15, maxOpe = 10, eneNum = 100;
	sf::Vector3f cameraPos;
	float downAngle;
	OHMS::filescanf(path + "info.txt", "%s%s%s %s%u%u %s%f%f %f%s%f %s%s%s %d%s%d %s%d",
		levelIDtmp, 64, str, 64, levelNametmp, 64,
		str, 64, &(this->battleGroundSize.x), &(this->battleGroundSize.y),
		str, 64, &(cameraPos.x), &(cameraPos.y),
		&(cameraPos.z), str, 64, &downAngle,
		str, 64, musicName, 64, str, 64,
		&levelLife, str, 64, &maxOpe,
		str, 64, &eneNum);
	this->levelID.assign(levelIDtmp);
	this->levelName.assign(levelNametmp);

	if (this->levelName == "NULL") {
		this->levelName = " ";
	}
	this->myGraphic.setCamera(cameraPos, downAngle);
	this->myGraphic.setBackground(path + "ground.png");
	this->myAudio->setBGM(0, "assets/battle/bgm/", musicName);
	this->pile.readFromFile(path, this);
	return;
}

sf::Vector2u OHMS::BattleGroundMain::getBattleGroundSize() {
	return this->battleGroundSize;
}

void OHMS::BattleGroundMain::initEntityOrgOpe(const unsigned short num) {
	this->entityOrgOpe = new OHMS::EntityMain[num];
	this->entityOrgOpeCnt = num;
	this->operatorReset.veryFirstSet(num, 120, 0, this->font);
	return;
}

void OHMS::BattleGroundMain::initEntityOrgEne(const unsigned short num) {
	this->entityOrgEne = new OHMS::EntityMain[num];
	this->entityOrgEneCnt = num;
	return;
}

void OHMS::BattleGroundMain::initEntityOrg(const unsigned short OpeNum, const unsigned short EneNum) {
	this->myGraphic.initAllAnime();
	this->myGraphic.initAnimeORG(OpeNum + EneNum);
	this->initEntityOrgOpe(OpeNum);
	this->initEntityOrgEne(EneNum);
	this->myGraphic.lihui.veryFirstSet(OpeNum);
	return;
}

void OHMS::BattleGroundMain::setEntityOrg(const bool isEnemy, const unsigned short entityID, std::string path) {
	if (isEnemy) {
		if (entityID < this->entityOrgEneCnt) {
			(this->entityOrgEne + entityID)->loadFromFile(path + "info.txt");
			(this->entityOrgEne + entityID)->id = entityID;
			this->myGraphic.setAnimeOrg(this->entityOrgOpeCnt + entityID, path + "animation/");
		}
	}
	else {
		if (entityID < this->entityOrgOpeCnt) {
			(this->entityOrgOpe + entityID)->loadFromFile(path + "info.txt");
			(this->entityOrgOpe + entityID)->id = entityID;
			this->myGraphic.setAnimeOrg(entityID, path + "animation/");
			this->myGraphic.lihui.setLihui(entityID, path);
			this->myGraphic.lihui.setAsBattle(entityID);
		}
	}
	return;
}

OHMS::OptionClass::OptionClass(OHMS::BattleGroundMain * myBG, OHMS::GSbattleGround * myGraphic) {
	this->CanPlayHeadPosReal = nullptr;
	this->CanPlayHeadPos = nullptr;
	this->theBG = myBG;
	this->theGraphic = myGraphic;
	this->CanPlayList = nullptr;
	this->CanPlayFocus = 0;
	this->CanPlayNum = 0;
	this->CanPlayShowInfo = false;
	this->CanPlayAble = nullptr;
	this->CanPlayInfoPos = 0.0f;
	this->CanPlayInfoPosReal = 0.0f;
	this->CanPlayFocus = -1;
	this->opc = new OHMS::OptionCore(this);
	this->CanPlayOnBG = new int[this->theBG->battleGroundSize.x * this->theBG->battleGroundSize.y];
	this->CanPlayHeadBK.setSize({ (float)canPlayHeadSize, (float)canPlayHeadSize });
	this->CanPlayHeadBK.setFillColor(sf::Color(255, 255, 255, 60));
	this->CanPlayHeadBK.setOrigin(0.0f, (float)canPlayHeadSize);
	this->HeadFTX.loadFromFile("assets/battle/texture/head.png");
	this->HeadFTX.setSmooth(true);
	this->HeadFFF.setTexture(this->HeadFTX);
	this->HeadFFF.setOrigin(0.0f, 256.0f);
	this->HeadFFF.setScale(canPlayHeadSize / 256.0f, canPlayHeadSize / 256.0f);
	this->jishi.setFont(this->theBG->font);

	this->pauseBtn.setTexture(this->theGraphic->uiTX);
	this->pauseBtn.setTextureRect(sf::IntRect(1346, 1275, 188, 163));
	this->pauseBtn.setOrigin(188.0f, 0.0f);
	this->pauseBtn.setScale(0.5f, 0.5f);
	this->pauseBtn.setPosition(winWidth, 0.0f);

	this->speedBtn.setTexture(this->theGraphic->uiTX);
	this->speedBtn.setTextureRect(sf::IntRect(1617, 1615, 180, 145));
	this->speedBtn.setOrigin(188.0f, 0.0f);
	this->speedBtn.setScale(0.5f, 0.5f);
	this->speedBtn.setPosition(winWidth - 75, 0.0f);
	return;
}

OHMS::OptionClass::~OptionClass() {
	delete this->opc;
	delete[] this->CanPlayList;
	delete[] this->CanPlayAble;
	delete[] this->CanPlayOnBG;
	return;
}

void OHMS::OptionClass::loadCanPlayList() {
	this->CanPlayNum = this->theBG->entityOrgOpeCnt;
	this->CanPlayList = new int[this->CanPlayNum];
	this->CanPlayAble = new bool[this->CanPlayNum];
	this->CanPlayHeadPos = new float[this->CanPlayNum];
	this->CanPlayHeadPosReal = new float[this->CanPlayNum];
	this->theGraphic->loadCanPlayHead(this->CanPlayNum);
	for (unsigned short i = 0; i < this->theBG->entityOrgOpeCnt; ++i) {
		*(this->CanPlayList + i) = i;
		*(this->CanPlayAble + i) = true;
		*(this->CanPlayHeadPos + i) = 0.0f;
		*(this->CanPlayHeadPosReal + i) = 0.0f;
	}
	for (unsigned short i = 0; i < this->theBG->battleGroundSize.x; ++i) {
		for (unsigned short j = 0; j < this->theBG->battleGroundSize.y; ++j) {
			*(this->CanPlayOnBG + this->theBG->battleGroundSize.x * j + i) = -1;
		}
	}
	return;
}

bool OHMS::OptionClass::setFocus(unsigned int n) {
	++n;
	for (unsigned int i = 0, j = 0; i < this->CanPlayNum; ++i) {
		if (*(this->CanPlayAble + i)) {
			++j;
		}
		if (j == n) {
			if (this->CanPlayFocus > -1) {
				*(this->CanPlayHeadPos + this->CanPlayFocus) = 0.0f;
			}
			this->CanPlayFocus = i;
			*(this->CanPlayHeadPos + this->CanPlayFocus) = 1.0f;
			this->theBG->setSpeed(0);
			return true;
		}
	}
	return false;
}

void OHMS::OptionClass::setCanPlayInfoPos(float pos) {
	this->CanPlayInfoPos = pos;
	return;
}

void OHMS::OptionClass::setCanPlayInfoPosOrg() {
	this->CanPlayInfoPos = 0.0f;
	this->CanPlayInfoPosReal = 2.0f;
	this->CanPlayShowInfo = true;
	return;
}

short OHMS::OptionClass::isFocus() {
	return this->CanPlayFocus;
}

void OHMS::OptionClass::cclFocus() {
	if (this->CanPlayFocus > -1) {
		this->CanPlayInfoPos = 2.0f;
		*(this->CanPlayHeadPos + this->CanPlayFocus) = 0.0f;
		this->theBG->setSpeed(1);
	}
	return;
}

void OHMS::OptionClass::setCanPlayInfo() {
	this->CanPlayShowInfo = true;
	return;
}

sf::Texture * OHMS::OptionClass::getFocusTexture() {
	return this->theGraphic->getAnimeOrg(this->CanPlayFocus)->getFrameAnima(0)->getTexture();
}

bool OHMS::OptionClass::setIt(sf::Vector2u pos, unsigned char dist) {
	if (this->CanPlayFocus > -1) {
		if (*(this->CanPlayAble + this->CanPlayFocus)) {
			if (this->theBG->setOperatorIn(*(this->CanPlayList + this->CanPlayFocus), pos, dist)) {
				*(this->CanPlayAble + this->CanPlayFocus) = false;
				*(this->CanPlayOnBG + this->theBG->battleGroundSize.x * pos.y + pos.x) = this->CanPlayFocus;
				this->theBG->myAudio->addSound(4);
				return true;
			}
		}
	}
	return false;
}

bool OHMS::OptionClass::canSetIt(sf::Vector2u pos) {
	if (this->CanPlayFocus > -1) {
		if (*(this->CanPlayAble + this->CanPlayFocus)) {
			if (this->theBG->canSetOperatorIn(*(this->CanPlayList + this->CanPlayFocus), pos)) {
				return true;
			}
		}
	}
	return false;
}

void OHMS::OptionClass::comeBack(unsigned short n) {
	*(this->CanPlayAble + n) = true;
	this->theBG->operatorReset.getBack(n);
	return;
}

void OHMS::OptionClass::update() {
	float delta;
	for (unsigned short i = 0; i < this->CanPlayNum; ++i) {
		if (*(this->CanPlayAble + i)) {
			delta = (*(this->CanPlayHeadPos + i) + *(this->CanPlayHeadPosReal + i)) / 2.0f;
			if (fabs(delta - *(this->CanPlayHeadPosReal + i)) > 0.005f) {
				*(this->CanPlayHeadPosReal + i) = delta;
			}
			else {
				*(this->CanPlayHeadPosReal + i) = *(this->CanPlayHeadPos + i);
			}
		}
	}
	delta = this->CanPlayInfoPosReal + (this->CanPlayInfoPos - this->CanPlayInfoPosReal) / 3.0f;
	if (fabs(delta - this->CanPlayInfoPosReal) > 0.005f) {
		this->CanPlayInfoPosReal = delta;
	}
	else {
		this->CanPlayInfoPosReal = this->CanPlayInfoPos;
		if (this->CanPlayInfoPos > 1.8f) {
			this->CanPlayShowInfo = false;
			this->justShowInfo = false;
			this->CanPlayFocus = -1;
			return;
		}
	}
	return;
}

void OHMS::OptionClass::check(sf::Event event, sf::Vector2i moupos) {
	int left, right, top, bottom;
	switch (event.type) {
	case sf::Event::MouseButtonReleased:
		left = winWidth - 75, right = winWidth, top = 0, bottom = 75;
		if (moupos.x > left && moupos.x < right && moupos.y > top && moupos.y < bottom) {
			this->theBG->changePause();
		}
		left = winWidth - 150, right = winWidth - 75, top = 0, bottom = 75;
		if (moupos.x > left && moupos.x < right && moupos.y > top && moupos.y < bottom) {
			this->theBG->setSpeed(2);
		}
		break;
	default:;
	}
	return;
}

void OHMS::OptionClass::diplayIn(sf::RenderWindow & window) {
	this->update();
	window.draw(this->pauseBtn);
	window.draw(this->speedBtn);
	if (this->justShowInfo) {
		this->displayInfoOnBG(window);
	}
	else if (this->CanPlayFocus > -1) {
		this->displayInfo(window);
	}
	this->displayCanP(window);
	this->opc->displayIn(window);
	return;
}

OHMS::OptionCore::OptionCore(OHMS::OptionClass * thePt) {
	this->myPt = thePt;
	this->optingOpe.setOrigin(128.0f, 230.0f);
	this->optingOpe.setScale(1.5f, 1.5f);
	this->optingOpeMove = 0.0f;
	this->optingOpeMoveReal = 0.0f;
	this->optingOpe.setPosition((float)winWidth + 500.0f, (float)winHight + 500.0f);
	this->center = { winWidth / 2.0f, winHight / 2.0f };
	this->settingOpe = false;
	this->mouseButtonPressed = false;
	this->finalChoose = false;
	this->chooseBox.setTexture(this->myPt->theGraphic->uiTX);
	this->chooseBox.setTextureRect(sf::IntRect(0, 1106, 943, 943));
	this->chooseBox.setOrigin(471.5f, 471.5f);
	this->chooseBox.setScale(0.56f, 0.56f);
	this->choosePoint.setTexture(this->myPt->theGraphic->uiTX);
	this->choosePoint.setTextureRect(sf::IntRect(1347, 1118, 152, 152));
	this->choosePoint.setOrigin(76.0f, 76.0f);
	this->choosePoint.setScale(0.8f, 0.8f);
	this->chooseDist.setTexture(this->myPt->theGraphic->uiTX);
	this->chooseDist.setTextureRect(sf::IntRect(750, 879, 173, 229));
	this->chooseDist.setOrigin(360.0f, 124.5f);
	this->chooseDist.setScale(0.56f, 0.56f);
	return;
}

void OHMS::OptionCore::check(sf::Event event, sf::Vector2i mouPos) {
	this->mousePos = mouPos;
	short chooseOpe = this->myPt->isFocus();
	float distantToLast;
	int n;
	sf::Vector2f mouposOpe;
	if (this->finalChoose) {
		mouposOpe = { (float)this->mousePos.x, (float)this->mousePos.y };
		mouposOpe = this->myPt->theGraphic->posMap2dTo3d(mouposOpe - this->center, 0.0f);
		if (mouposOpe.x < 0.0f) mouposOpe.x -= 1.2f;
		if (mouposOpe.y < 0.0f) mouposOpe.y -= 1.2f;
		unsigned char dist;
		bool rt;
		bool rd;
		switch (event.type) {
		case sf::Event::MouseButtonPressed:
			mouposOpe.x -= this->finalPos.x + 0.5f;
			mouposOpe.y -= this->finalPos.y + 0.5f;
			if (mouposOpe.x * mouposOpe.x + mouposOpe.y * mouposOpe.y > 4.0f) {
				this->optingOpeMove = 0.0f;
				this->myPt->theBG->pile.clearShow();
				this->finalChoose = false;
				this->settingOpe = false;
			}
			else {
				this->mouseButtonPressed = true;
				this->choosePoint.setPosition((float)this->mousePos.x, (float)this->mousePos.y);
			}
			break;
		case sf::Event::MouseMoved:
			if (this->finalChoose && this->mouseButtonPressed) {
				mouposOpe.x -= this->finalPos.x + 0.5f;
				mouposOpe.y -= this->finalPos.y + 0.5f;
				this->choosePoint.setPosition((float)this->mousePos.x, (float)this->mousePos.y);
				if (mouposOpe.x* mouposOpe.x + mouposOpe.y * mouposOpe.y < 2.0f) break;
				rt = mouposOpe.x + mouposOpe.y > 0;
				rd = mouposOpe.x - mouposOpe.y > 0;
				if (rt) {
					if (rd) {
						dist = 0;
						this->chooseDist.setRotation(180.0f);
					}
					else {
						dist = 1;
						this->chooseDist.setRotation(90.0f);
					}
				}
				else {
					if (rd) {
						dist = 3;
						this->chooseDist.setRotation(-90.0f);
					}
					else {
						dist = 2;
						this->chooseDist.setRotation(0.0f);
					}
				}
				this->optingOpeTurn = (dist > 1);
				this->tmpRange.setDist(dist);
				this->myPt->theBG->pile.addShow(OHMS::AllPile::SHOWTYPE::CAN_ATTACK, &(this->tmpRange));
			}
			break;
		case sf::Event::MouseButtonReleased:
			if (!this->mouseButtonPressed) {
				this->optingOpeMove = 0.0f;
				this->myPt->cclFocus();
				this->finalChoose = false;
				this->settingOpe = false;
				break;
			}
			mouposOpe.x -= this->finalPos.x + 0.5f;
			mouposOpe.y -= this->finalPos.y + 0.5f;
			if (mouposOpe.x * mouposOpe.x + mouposOpe.y * mouposOpe.y < 3.6f) {
				sf::Vector2f tmp_t = this->center + this->myPt->theGraphic->posMap3dTo2d({ this->finalPos.x + 0.5f, this->finalPos.y + 0.5f, 0.0f });
				this->choosePoint.setPosition(tmp_t);
				this->mouseButtonPressed = false;
				break;
			}
			if (this->myPt->setIt(this->finalPos, this->tmpRange.dist)) {
				this->optingOpeMoveReal = 0.0f;
			}
			this->optingOpeMove = 0.0f;
			this->myPt->cclFocus();
			this->finalChoose = false;
			this->settingOpe = false;
			this->mouseButtonPressed = false;
			this->myPt->theBG->pile.clearShow();
			break;
		default:;
		}
	}
	else if (this->showOnBG) {
		mouposOpe = { (float)this->mousePos.x, (float)this->mousePos.y };
		mouposOpe = this->myPt->theGraphic->posMap2dTo3d(mouposOpe - this->center, 0.0f);
		if (mouposOpe.x < 0.0f) mouposOpe.x -= 1.2f;
		if (mouposOpe.y < 0.0f) mouposOpe.y -= 1.2f;
		OHMS::EntityMain* ptr;
		unsigned short kx, ky;
		switch (event.type) {
		case sf::Event::MouseButtonPressed:
			this->mouseButtonPressed = true;
			break;
		case sf::Event::MouseButtonReleased:
			if (!this->mouseButtonPressed) {
				this->optingOpeMove = 0.0f;
				this->myPt->cclFocus();
				this->finalChoose = false;
				this->settingOpe = false;
				break;
			}
			kx = (unsigned short)mouposOpe.x;
			ky = (unsigned short)mouposOpe.y;
			ptr = this->myPt->theBG->pile.getOpeOnPtr(kx, ky);
			mouposOpe.x -= this->finalPos.x + 0.5f;
			mouposOpe.y -= this->finalPos.y + 0.5f;
			if (mouposOpe.x * mouposOpe.x + mouposOpe.y * mouposOpe.y > 2.0f) {
				this->showOnBG = false;
				this->mouseButtonPressed = false;
				this->myPt->cclFocus();
				this->myPt->theBG->pile.clearShow();
				break;
			}
			if (ptr != nullptr) {
				ptr->LeaveNow();
			}
			this->showOnBG = false;
			this->settingOpe = false;
			this->mouseButtonPressed = false;
			this->myPt->cclFocus();
			this->myPt->theBG->pile.clearShow();
			break;
		default:;
		}
	}
	else {
		mouposOpe = { (float)this->mousePos.x, (float)this->mousePos.y };
		mouposOpe = this->myPt->theGraphic->posMap2dTo3d(mouposOpe - this->center, 0.0f);
		if (mouposOpe.x < 0.0f) mouposOpe.x -= 1.2f;
		if (mouposOpe.y < 0.0f) mouposOpe.y -= 1.2f;
		switch (event.type) {
		case sf::Event::MouseButtonPressed:
			if (this->mousePos.y > winHight - canPlayHeadSize) {
				n = (winWidth - this->mousePos.x) / canPlayHeadSize;
				if (this->myPt->isFocus() == -1) {
					if (this->myPt->setFocus(n)) {
						this->optingOpe.setTexture(*(this->myPt->getFocusTexture()), true);
						this->myPt->setCanPlayInfoPosOrg();
						this->lastPos = this->mousePos;
						this->comeFrom = { winWidth - (n + 0.5f) * canPlayHeadSize, winHight - canPlayHeadSize / 2.0f };
						this->mouseButtonPressed = true;
						int opeid = this->myPt->isFocus();
						if (opeid != -1) {
							this->myPt->theBG->pile.addShow(OHMS::AllPile::SHOWTYPE::CAN_PLACE, this->myPt->theBG->getEntityOrgOpe(opeid)->getPileFliter());
						}
					}
				}
				else {
					if (this->myPt->setFocus(n)) {
						this->optingOpe.setTexture(*(this->myPt->getFocusTexture()), true);
						this->myPt->setCanPlayInfoPos(0.0f);
						this->myPt->setCanPlayInfo();
						this->lastPos = this->mousePos;
						this->comeFrom = { winWidth - (n + 0.5f) * canPlayHeadSize, winHight - canPlayHeadSize / 2.0f };
						this->mouseButtonPressed = true;
						int opeid = this->myPt->isFocus();
						if (opeid != -1) {
							this->myPt->theBG->pile.addShow(OHMS::AllPile::SHOWTYPE::CAN_PLACE, this->myPt->theBG->getEntityOrgOpe(opeid)->getPileFliter());
						}
					}
				}
			}
			this->showOnBG = false;
			break;
		case sf::Event::MouseMoved:
			distantToLast = (float)(this->mousePos.x - lastPos.x) * (this->mousePos.x - lastPos.x) + (this->mousePos.y - lastPos.y) * (this->mousePos.y - lastPos.y);
			if (chooseOpe > -1 && !this->settingOpe && this->mouseButtonPressed && distantToLast > optionDist) {
				if (this->myPt->theBG->operatorReset.canGo(chooseOpe)) {
					if (this->myPt->theBG->getEntityOrgOpe(chooseOpe)->getCost() <= this->myPt->theBG->operatorReset.hasCost()) {
						this->settingOpe = true;
						this->optingOpeMove = 0.0f;
						this->optingOpeMoveReal = 0.0f;
						this->optingOpeTurn = false;
					}
				}
			}
			if (this->settingOpe) {
				this->finalPos = { (unsigned int)mouposOpe.x, (unsigned int)mouposOpe.y };
				this->optingOpeMove = std::min(std::max(distantToLast - optionDist, 0.0f) * 0.6f / optionDist, 1.0f);
				this->myPt->setCanPlayInfoPos(this->optingOpeMove);
			}
			break;
		case sf::Event::MouseButtonReleased:
			if (this->settingOpe) {
				if (mouposOpe.x > -0.0001f && mouposOpe.y > -0.0001f) {
					this->finalPos = { (unsigned int)mouposOpe.x, (unsigned int)mouposOpe.y };
					if (this->myPt->canSetIt(this->finalPos)) {
						this->finalChoose = true;
						this->optingOpe.setPosition(this->center + this->myPt->theGraphic->posMap3dTo2d({ this->finalPos.x + 0.5f, this->finalPos.y + 0.5f, this->myPt->theBG->pile.isHighLand(this->finalPos.x, this->finalPos.y) ? 0.2f : 0.0f }));
						sf::Vector2f tmp = this->center + this->myPt->theGraphic->posMap3dTo2d({ this->finalPos.x + 0.5f, this->finalPos.y + 0.5f, 0.0f });
						tmp.y -= 12.0f;
						this->chooseBox.setPosition(tmp);
						this->chooseDist.setPosition(tmp);
						this->chooseDist.setRotation(180.0f);
						this->choosePoint.setPosition(tmp);
						int opeid = this->myPt->isFocus();
						if (opeid != -1) {
							this->tmpRange = *((this->myPt->theBG->entityOrgOpe + opeid)->getRange());
							this->tmpRange.setDist(0);
							this->tmpRange.setOrigin({ this->finalPos.x + 0.5f, this->finalPos.y + 0.5f });
							this->myPt->theBG->pile.addShow(OHMS::AllPile::SHOWTYPE::CAN_ATTACK, &(this->tmpRange));
						}
						this->mouseButtonPressed = false;
						break;
					}
				}
				this->optingOpeMove = 0.0f;
				this->myPt->theBG->pile.clearShow();
				this->myPt->cclFocus();
			}
			else {
				this->finalPos = { (unsigned int)mouposOpe.x, (unsigned int)mouposOpe.y };
				if (this->mousePos.y < winHight - canPlayHeadSize) {
					int getOpe = this->myPt->theBG->pile.getOpeOn((unsigned short)mouposOpe.x, (unsigned short)mouposOpe.y);
					if (getOpe != -1) {
						this->myPt->cclFocus();
						if (this->myPt->CanPlayFocus > -1) {
							this->myPt->CanPlayInfoPosReal = 0.0f;
						}
						this->showOnBG = true;
						sf::Vector2f tmp = this->center + this->myPt->theGraphic->posMap3dTo2d({ this->finalPos.x + 0.5f, this->finalPos.y + 0.5f, 0.0f });
						this->chooseBox.setPosition(tmp);
						this->myPt->theBG->setSpeed(0);
						this->myPt->justShowInfo = true;
						this->myPt->CanPlayFocus = getOpe;
						this->myPt->CanPlayInfoPos = 0.0f;
						OHMS::EntityMain* eptr = this->myPt->theBG->pile.getOpeOnPtr((unsigned short)mouposOpe.x, (unsigned short)mouposOpe.y);
						this->myPt->showIt = eptr;
						this->myPt->theBG->pile.addShow(OHMS::AllPile::SHOWTYPE::CAN_ATTACK, eptr->getRange());
					}
					else {
						this->myPt->cclFocus();
						this->myPt->theBG->pile.clearShow();
					}
				}
			}
			this->mouseButtonPressed = false;
			this->settingOpe = false;
			break;
		}
	}
	return;
}

void OHMS::OptionCore::update()
{
	float delta = this->optingOpeMoveReal + (this->optingOpeMove - this->optingOpeMoveReal) / 4.0f;
	bool yiju = fabs(delta - this->optingOpeMoveReal) > 0.005f;
	if (yiju) {
		this->optingOpeMoveReal = delta;
	}
	else {
		this->optingOpeMoveReal = this->optingOpeMove;
	}
	if (this->settingOpe) {
		if (!this->finalChoose) {
			if (this->myPt->canSetIt(this->finalPos)) {
				this->optingOpe.setPosition(this->center + this->myPt->theGraphic->posMap3dTo2d({ this->finalPos.x + 0.5f, this->finalPos.y + 0.5f, this->myPt->theBG->pile.isHighLand(this->finalPos.x, this->finalPos.y) ? 0.2f : 0.0f }));
			}
			else {
				this->optingOpe.setPosition((float)this->mousePos.x, (float)this->mousePos.y);
			}
		}
	}
	else {
		sf::Vector2f de = this->optingOpe.getPosition();
		de.x = de.x + (this->comeFrom.x - de.x) / 3.0f;
		de.y = de.y + (this->comeFrom.y - de.y) / 6.0f;
		if (yiju) {
			this->optingOpe.setPosition(de);
		}
		else {
			this->optingOpe.setPosition((float)winWidth + 500.0f, (float)winHight + 500.0f);

		}
	}
	return;
}

void OHMS::OptionCore::displayIn(sf::RenderWindow & window) {
	this->update();
	this->optingOpe.setScale((this->optingOpeMoveReal * 0.6f + 0.8f) * (this->optingOpeTurn ? -1.0f : 1.0f), this->optingOpeMoveReal * 0.6f + 0.8f);
	this->optingOpe.setColor(sf::Color(255, 255, 255, (int)(255.0f * this->optingOpeMoveReal)));
	if (this->finalChoose) {
		window.draw(this->chooseBox);
		window.draw(this->choosePoint);
		window.draw(this->chooseDist);
	}
	if (this->showOnBG) {
		window.draw(this->chooseBox);

	}
	return window.draw(this->optingOpe);
}

OHMS::EntityMain * OHMS::BattleGroundMain::getEntityOrgOpe(const unsigned short num) {
		return (this->entityOrgOpe + num);
	return nullptr;
}

OHMS::EntityMain * OHMS::BattleGroundMain::getEntityOrgEne(const unsigned short num) {
		return (this->entityOrgEne + num);
	return nullptr;
}

OHMS::EntityMain* OHMS::BattleGroundMain::entityJoin(const bool isEnemy, const unsigned short entityID, sf::Vector2f pos) {
	OHMS::EntityMain *flag = this->entity.addEntity();
	if (isEnemy) {
		if (entityID < this->entityOrgEneCnt) {
			(*flag) = *(this->entityOrgEne + entityID);
		}
		else {
			this->entity.delEntity(flag);
			return nullptr;
		}
	}
	else {
		if (entityID < this->entityOrgOpeCnt) {
			(*flag) = *(this->entityOrgOpe + entityID);
		}
		else {
			this->entity.delEntity(flag);
			return nullptr;
		}
		this->operatorReset.giveoutCost(flag->getCost());
		this->pile.setOpeOn(entityID, flag, (unsigned short)pos.x, (unsigned short)pos.y);
	}
	flag->initPtr(&(this->entity), this->myGraphic.animeJoin((isEnemy ? this->entityOrgOpeCnt : 0) + entityID));
	flag->veryFirstSetSTM(isEnemy);
	flag->setPosition({ pos.x, pos.y });
	flag->getReady();
	return flag;
}

bool OHMS::BattleGroundMain::setOperatorIn(unsigned short opeID, sf::Vector2u pos, unsigned char dist) {
	if (pos.x < this->battleGroundSize.x && pos.y < this->battleGroundSize.y && this->pile.canPlace(pos.x, pos.y)) {
		if ((this->entityOrgOpe + opeID)->pileFliter.checkPile(this->pile.getPile(pos))) {
			(this->entityJoin(0, opeID, { pos.x + 0.5f, pos.y + 0.5f }))->changeDist(dist);
			return true;
		}
	}
	return false;
}

bool OHMS::BattleGroundMain::canSetOperatorIn(unsigned short opeID, sf::Vector2u pos) {
	if (pos.x < this->battleGroundSize.x && pos.y < this->battleGroundSize.y && this->pile.canPlace(pos.x, pos.y)) {
		if ((this->entityOrgOpe + opeID)->pileFliter.checkPile(this->pile.getPile(pos))) {
			return true;
		}
	}
	return false;
}

void OHMS::OptionClass::displayCanP(sf::RenderWindow & window) {
	sf::Vector2f pos;
	for (unsigned short i = 0, j = 1; i < this->CanPlayNum; ++i) {
		if (*(this->CanPlayAble + i)) {
			pos.x = (float)winWidth - canPlayHeadSize * j;
			pos.y = winHight - *(this->CanPlayHeadPosReal + i) * 25.0f;
			(this->theGraphic->CanPlayHead + i)->setPosition(pos);
			if (this->theBG->operatorReset.canGo(i)) {
				if (this->theBG->getEntityOrgOpe(i)->getCost() <= this->theBG->operatorReset.hasCost()) {
					(this->theGraphic->CanPlayHead + i)->setColor(sf::Color(255, 255, 255));
					this->CanPlayHeadBK.setPosition(pos);
					this->HeadFFF.setPosition(pos);
					window.draw(this->CanPlayHeadBK);
					window.draw(*(this->theGraphic->CanPlayHead + i));
					window.draw(this->HeadFFF);
				}
				else {
					(this->theGraphic->CanPlayHead + i)->setColor(sf::Color(160, 160, 160));
					this->CanPlayHeadBK.setPosition(pos);
					this->HeadFFF.setPosition(pos);
					window.draw(this->CanPlayHeadBK);
					window.draw(*(this->theGraphic->CanPlayHead + i));
					window.draw(this->HeadFFF);
				}
			}
			else {
				(this->theGraphic->CanPlayHead + i)->setColor(sf::Color(255, 100, 100));
				this->jishi.setSecond(this->theBG->operatorReset.leftTime(i));
				this->CanPlayHeadBK.setPosition(pos);
				this->HeadFFF.setPosition(pos);
				window.draw(this->CanPlayHeadBK);
				window.draw(*(this->theGraphic->CanPlayHead + i));
				window.draw(this->HeadFFF);
				window.draw(this->jishi.getText(pos.x + canPlayHeadSize * 0.5f, pos.y - canPlayHeadSize * 0.5f));
			}
			++j;
		}
	}
	return;
}

void OHMS::OptionClass::displayInfo(sf::RenderWindow & window) {
	if (this->CanPlayShowInfo) {
		this->theGraphic->lihui.setPos(this->CanPlayFocus, { 250.0f - 180.0f * this->CanPlayInfoPosReal, winHight / 2.0f - 40.0f });
		this->theGraphic->lihui.setAlpha(this->CanPlayFocus, (int)(255 - 127 * this->CanPlayInfoPosReal));
		this->theGraphic->lihui.displayIn(this->CanPlayFocus, window);
	}
	return;
}

void OHMS::OptionClass::displayInfoOnBG(sf::RenderWindow& window) {
	if (this->showIt != nullptr) {
		this->theGraphic->lihui.setPos(this->CanPlayFocus, { 250.0f - 180.0f * this->CanPlayInfoPosReal, winHight / 2.0f - 40.0f });
		this->theGraphic->lihui.setAlpha(this->CanPlayFocus, (int)(255 - 127 * this->CanPlayInfoPosReal));
		this->theGraphic->lihui.displayIn(this->CanPlayFocus, window);
	}
	return;
}

void OHMS::BattleGroundMain::loadCanPlayList() {
	this->op = new OHMS::OptionClass(this, &(this->myGraphic));
	this->op->loadCanPlayList();
	return;
}

OHMS::AllEntity* OHMS::BattleGroundMain::test___getEntity()
{
	return &this->entity;
}

OHMS::BattleGroundMain::BattleGroundMain() {
	this->frameBufferMax = 2;
	this->speed_shift = false;
	this->rank = 0;
	this->enemykilled = 0;
	this->entityOrgEne = nullptr;
	this->entityOrgEneCnt = 0;
	this->entityOrgOpe = nullptr;
	this->entityOrgOpeCnt = 0;
	this->op = nullptr;
	this->nowPause = false;
	this->myAudio = new OHMS::AudioCtrl(4, 12);
	this->myAudio->setBGM(1, "assets/battle/music/", "victory");
	this->myAudio->setBGM(2, "assets/battle/music/", "failed_voice");
	this->myAudio->setBGM(3, "assets/battle/music/", "failed");
	this->myAudio->setSound(0, "assets/audio/sounds/", "ui_btn");
	this->myAudio->setSound(1, "assets/battle/sounds/", "ui_win");
	this->myAudio->setSound(2, "assets/battle/sounds/", "ui_lose");
	this->myAudio->setSound(3, "assets/battle/sounds/", "ui_alarmenter");
	this->myAudio->setSound(4, "assets/battle/sounds/", "char_set");
	this->myAudio->setSoundVolumn(100.0f);
	this->entity.theBattleGround = this;
	this->summon = new OHMS::SummonCtrl(this);
	this->pauseText.setFont(*this->font);
	this->pauseText.setFillColor(sf::Color::White);
	this->pauseText.setCharacterSize(100);
	this->pauseText.setString("---- PAUSE ----");
	sf::FloatRect tmpRect = this->pauseText.getGlobalBounds();
	this->pauseText.setOrigin(tmpRect.width / 2.0f, tmpRect.height / 2.0f);
	this->pauseText.setPosition(winWidth / 2.0f, winHight / 2.0f);
	return;
}

OHMS::BattleGroundMain::~BattleGroundMain() {
	delete this->myAudio;
	delete this->op;
	delete[] this->entityOrgOpe;
	delete[] this->entityOrgEne;
	return;
}

void OHMS::BattleGroundMain::update() {
	if (this->nowPause) return;
	this->entity.update();
	this->myGraphic.update();
	this->operatorReset.update();
	this->summon->update();
	if (this->life < 1) {
		this->over = true;
		this->rank = 0;
		this->myGraphic.update();
		this->operatorReset.update();
		this->entity.update();
		this->summon->update();
	}
	else if (this->enemykilled == this->summon->allcnt) {
		this->over = true;
		this->rank = this->life < 3 ? 2 : 3;
		this->myGraphic.update();
		this->operatorReset.update();
		this->entity.update();
		this->summon->update();
	}
	return;
}

void OHMS::BattleGroundMain::displayIn(sf::RenderWindow & window) {
	this->myGraphic.displayGroundIn(window);
	this->pile.showArrange(window);
	this->myGraphic.displayAnimeIn(window);
	this->op->diplayIn(window);
	this->operatorReset.displayIn(window);
	char tmp[50];
	sprintf_s(tmp, 50, "Killed:%d/%d Life:%d", this->enemykilled, this->summon->allcnt, this->life);
	this->headline.setString(tmp);
	sf::FloatRect tmpr = this->headline.getGlobalBounds();
	this->headline.setOrigin(tmpr.width / 2.0f, tmpr.height / 2.0f);
	window.draw(this->headline);
	if (this->nowPause) {
		window.draw(this->pauseText);
	}
	return;
}

void OHMS::BattleGroundMain::changePause() {
	this->nowPause = !this->nowPause;
	if (this->nowPause) {
		this->op->pauseBtn.setTextureRect(sf::IntRect(1429, 1603, 188, 163));
	}
	else {
		this->op->pauseBtn.setTextureRect(sf::IntRect(1346, 1275, 188, 163));
	}
	return;
}

void OHMS::BattleGroundMain::setFont(sf::Font* f) {
	this->font = f;
	this->pauseText.setFont(*f);
	this->headline.setFont(*f);
	sf::FloatRect tmpRect = this->pauseText.getGlobalBounds();
	this->pauseText.setOrigin(tmpRect.width / 2.0f, tmpRect.height / 2.0f);
	this->pauseText.setPosition(winWidth / 2.0f, winHight / 2.0f);
	this->headline.setPosition(winWidth / 2.0f, 40.0f);
	this->headline.setCharacterSize(20);
	this->headline.setFillColor(sf::Color::White);
	return;
}

void OHMS::BattleGroundMain::setSpeed(unsigned char status) {
	switch (status) {
	case 0:
		this->frameBufferMax = 10u;
		break;
	case 1:
		if (this->frameBufferMax != 10u) {
			this->frameBufferMax = 2u;
			this->speed_shift = false;
		}
		else {
			if (this->speed_shift) {
				this->frameBufferMax = 1u;
			}
			else {
				this->frameBufferMax = 2u;
			}
		}
		break;
	case 2:
		if (this->frameBufferMax != 10u) {
			if (this->frameBufferMax == 1u) {
				this->frameBufferMax = 2u;
				this->speed_shift = false;
				this->op->speedBtn.setTextureRect(sf::IntRect(1617, 1615, 180, 145));
			}
			else {
				this->frameBufferMax = 1u;
				this->speed_shift = true;
				this->op->speedBtn.setTextureRect(sf::IntRect(820, 420, 180, 145));
			}
		}
		else {
			if (this->speed_shift) {
				this->frameBufferMax = 1u;
			}
			else {
				this->frameBufferMax = 2u;
			}
		}
		break;
	default:;
	}
	return;
}

OHMS::EntityMain::EntityMain() {
	this->myTeam = nullptr;
	this->myAnime = nullptr;
	this->STM = nullptr;
	this->arm = nullptr;
	this->atkRange.clear();
	this->dist = 0;
	this->isEnemy = 0;
	this->pileFliter.wantStyle = OHMS::PileFilter::style::PLAIN;
	return;
}

OHMS::EntityMain::~EntityMain() {
	delete this->STM;
	delete this->arm;
	return;
}

void OHMS::EntityMain::loadFromFile(const std::string & path) {
	std::ifstream fin;
	fin.open(path.c_str(), std::ios::in);
	std::string str;
	int type, n, m;
	char tmp[50];
	fin >> str >> str >> type;
	this->atkRange.clear();
	this->atkRange.type = (type == 4) ? 1 : 0;
	fin >> tmp >> n >> m;
	this->atkRange.setDist(n);
	this->atkRange.setBackPile(m);
	for (int i = 0; i < n; ++i) {
		fin >> m;
		this->atkRange.setRangePushBack(m);
	}
	fin >> tmp >> m;
	this->pileFliter.wantStyle = m;
	float ftmp;
	fin >> tmp >> ftmp;
	this->attribute.healthMax = ftmp;
	this->attribute.health = ftmp;
	fin >> tmp >> ftmp;
	this->attribute.attack = ftmp;
	fin >> tmp >> ftmp;
	fin >> tmp >> ftmp;
	this->attribute.defende = ftmp;
	fin >> tmp >> ftmp;
	this->attribute.magicalDef = ftmp / 100.0f;
	fin >> tmp >> ftmp;
	this->speed = ftmp;
	fin.close();
	this->entityFilter.wantStyle = OHMS::EntityFilter::Style::isEnemy;
	return;
}

void OHMS::EntityMain::initPtr(OHMS::AllEntity * theTeam, OHMS::GSframeAnimaCtrl * theAnime) {
	this->myTeam = theTeam;
	this->myAnime = theAnime;
	return;
}

void OHMS::EntityMain::setPosition(sf::Vector2f pos) {
	this->position = pos;
	this->atkRange.setOrigin(pos);
	return this->myTeam->theBattleGround->setEntityPos(pos, this);
}

void OHMS::EntityMain::setPosition(float x, float y) {
	return this->setPosition(sf::Vector2f(x, y));
}

void OHMS::EntityMain::getPosition(float* x, float* y) {
	*x = this->position.x;
	*y = this->position.y;
	return;
}

sf::Vector2f OHMS::EntityMain::getPosition() {
	return this->position;
}

OHMS::GSframeAnimaCtrl* OHMS::EntityMain::getAnime() {
	return this->myAnime;
}

OHMS::Range* OHMS::EntityMain::getRange() {
	return &(this->atkRange);
}

OHMS::PileFilter* OHMS::EntityMain::getPileFliter() {
	return &(this->pileFliter);
}

bool OHMS::EntityMain::canAttack() {
	std::vector<OHMS::EntityMain*>* entity = &(this->myTeam->entity);
	std::vector<OHMS::EntityMain*>::iterator i = entity->begin(), n = entity->end();
	for (; i != n; ++i) {
		if ((*i)->normal) {
			if (this->entityFilter.checkEntity(*i)) {
				if (this->atkRange.checkEntityInME(*i)) {
					return true;
				}
			}
		}
	}
	return false;
}

bool OHMS::EntityMain::isDied() {
	if (this->attribute.health < 1e-6f) {
		this->IamDying = true;
	}
	if (this->IamDying) {
		if (this->myAnime->playOver()) {
			this->Leave = true;
		}
	}
	return this->IamDying;
}

bool OHMS::EntityMain::HaveToLeave() {
	return this->Leave;
}

void OHMS::EntityMain::LeaveNow() {
	this->Leave = true;
	return;
}

void OHMS::EntityMain::update() {
	if (this->isEnemy && !this->IamDying && !this->canAttack()) {
		this->setPosition(this->position.x - 0.008f * this->speed, this->position.y);
		if (this->position.x < 0.5f) {
			this->myTeam->theBattleGround->life--;
			this->myTeam->theBattleGround->myAudio->addSound(3);
			this->Leave = true;
		}
	}
	return this->STM->update();
}

void OHMS::EntityMain::getReady() {
	this->STM->changeState(OHMS_GAME_STATEMACHINE_SET);
	return;
}

unsigned short OHMS::EntityMain::getCost() {
	if (this->isEnemy) {
		return 9999;
	}
	return 10;
}

void OHMS::EntityMain::veryFirstSetSTM(bool isEnemy) {
	this->STM = new OHMS::EntityStateMachine;
	this->arm = new OHMS::EntityArm;
	this->STM->veryFirstSet(this, isEnemy);
	this->isEnemy = isEnemy;
	this->IamDying = false;
	this->Leave = false;
	this->normal = false;
	this->arm->veryFirstSet(1, this);
	if (isEnemy) {
		this->entityFilter.wantStyle = OHMS::EntityFilter::Style::isOperator;
		this->myAnime->setTurnLeft(true);
	}
	return;
}

void OHMS::EntityMain::changeAnime(const unsigned short aniN) {
	return this->myAnime->changeAnimation(aniN);
}

void OHMS::EntityMain::changeDist(unsigned char ndist) {
	ndist %= 4;
	this->dist = ndist;
	this->atkRange.setDist(this->dist);
	if (ndist > 1) this->myAnime->setTurnLeft(true);
	return;
}

OHMS::AllEntity::AllEntity() {
	this->theBattleGround = nullptr;
	this->entity.clear();
	while (!this->ptrRecycle.empty())
		this->ptrRecycle.pop();
	return;
}

OHMS::AllEntity::~AllEntity() {
	unsigned int j = this->entity.size();
	for (unsigned short i = 0; i < j; ++i) {
		delete this->entity[i];
	}
	while (!this->ptrRecycle.empty()) {
		delete this->ptrRecycle.top();
		this->ptrRecycle.pop();
	}
	return;
}

void OHMS::AllEntity::update() {
	for (unsigned int i = 0, j = this->entity.size(); i < j; ++i) {
		this->entity[i]->update();
	}
	std::vector<OHMS::EntityMain*>::iterator i = this->entity.begin(), j = this->entity.end();
	for (; i != j;) {
		if ((*i)->HaveToLeave()) {
			if ((*i)->isEnemy) {
				this->theBattleGround->enemykilled++;
			}
			else {
				sf::Vector2f tmpv = (*i)->getPosition();
				int kx = (int)tmpv.x, ky = (int)tmpv.y;
				int id = this->theBattleGround->pile.getOpeOn(kx, ky);
				this->theBattleGround->pile.delOpeOn(kx, ky);
				this->theBattleGround->op->comeBack(id);
			}
			(*i)->~EntityMain();
			this->theBattleGround->myGraphic.getAnime()->delAnime((*i)->getAnime());
			i = this->entity.erase(i);
			j = this->entity.end();
		}
		else {
			++i;
		}
	}
	return;
}

OHMS::EntityMain* OHMS::AllEntity::test___getEntity() {
	return this->entity[this->entity.size() - 1];
}

OHMS::EntityMain * OHMS::AllEntity::addEntity() {
	OHMS::EntityMain * flag;
	if (this->ptrRecycle.empty()) {
		flag = new OHMS::EntityMain;
	}
	else {
		flag = ptrRecycle.top();
		ptrRecycle.pop();
	}
	this->entity.push_back(flag);
	return flag;
}

void OHMS::AllEntity::delEntity(OHMS::EntityMain * delObj) {
	unsigned int j = this->entity.size();
	for (unsigned short i = 0; i < j; ++i) {
		if (this->entity[i] == delObj) {
			this->ptrRecycle.push(delObj);
			this->entity[i] = this->entity[j - 1];
			this->entity.pop_back();
			break;
		}
	}
	return;
}

OHMS::AllPile::AllPile() {
	sf::Image im;
	sf::Color clr;
	im.create(32u, 32u, sf::Color(255, 255, 255, 100));
	for (unsigned int i = 0; i < 32; ++i) {
		clr = sf::Color(225, 225, 225, 150);
		im.setPixel(i, 0, clr);
		im.setPixel(i, 31, clr);
		im.setPixel(0, i, clr);
		im.setPixel(31, i, clr);
	}
	this->texture[0].loadFromImage(im);
	this->texture[0].setSmooth(true);
	im.create(32u, 32u, sf::Color(25, 155, 25, 100));
	for (unsigned int i = 0; i < 32; ++i) {
		clr = sf::Color(5, 105, 5, 150);
		im.setPixel(i, 0, clr);
		im.setPixel(i, 31, clr);
		im.setPixel(0, i, clr);
		im.setPixel(31, i, clr);
	}
	this->texture[1].loadFromImage(im);
	this->texture[1].setSmooth(true);
	im.create(32u, 32u, sf::Color(255, 175, 0, 100));
	for (unsigned int i = 0; i < 32; ++i) {
		clr = sf::Color(225, 135, 0, 150);
		im.setPixel(i, 0, clr);
		im.setPixel(i, 31, clr);
		im.setPixel(0, i, clr);
		im.setPixel(31, i, clr);
	}
	this->texture[2].loadFromImage(im);
	this->texture[2].setSmooth(true);
	im.create(32u, 32u, sf::Color(55, 155, 255, 100));
	for (unsigned int i = 0; i < 32; ++i) {
		clr = sf::Color(35, 125, 225, 150);
		im.setPixel(i, 0, clr);
		im.setPixel(i, 31, clr);
		im.setPixel(0, i, clr);
		im.setPixel(31, i, clr);
	}
	this->texture[3].loadFromImage(im);
	this->texture[3].setSmooth(true);
	im.create(32u, 32u, sf::Color(255, 0, 0, 100));
	for (unsigned int i = 0; i < 32; ++i) {
		clr = sf::Color(225, 0, 0, 150);
		im.setPixel(i, 0, clr);
		im.setPixel(i, 31, clr);
		im.setPixel(0, i, clr);
		im.setPixel(31, i, clr);
	}
	this->texture[4].loadFromImage(im);
	this->texture[4].setSmooth(true);
	return;
}

OHMS::AllPile::~AllPile() {
	delete[] this->piles;
	return;
}

void OHMS::AllPile::readFromFile(std::string path, OHMS::BattleGroundMain * theBattleGround) {
	this->myBattleGround = theBattleGround;
	sf::Vector2u bgsize = theBattleGround->getBattleGroundSize();
	char str[50];
	std::string func;
	int num, num1;
	this->piles = new OHMS::PileMain[bgsize.x * bgsize.y];
	this->showType = new int[bgsize.x * bgsize.y];
	std::ifstream fin;
	fin.open(path.append("piles.txt").c_str(), std::ios::in);
	OHMS::PileMain* tmp;
	for (unsigned int j = 0; j < bgsize.y; ++j) {
		for (unsigned int i = 0; i < bgsize.x; ++i) {
			fin >> num;
			tmp = (this->piles + bgsize.x * j + i);
			if (num % 10) {
				if (num % 10 == 2) {
					tmp->high = false;
					tmp->bothHighAndPlain = true;
				}
				else {
					tmp->high = true;
					tmp->bothHighAndPlain = false;
				}
			}
			else {
				tmp->high = false;
				tmp->bothHighAndPlain = false;
			}
			tmp->canPlace = num / 10;
			tmp->compPoint(i, j, &(theBattleGround->myGraphic));
			*(this->showType + bgsize.x * j + i) = -1;
		}
	}
	fin >> str >> num >> num1 >> func;
	while (num != -1) {
		if (func == "protect") {
			(this->piles + num1 * bgsize.x + num)->func = &OHMS::PileMain::func_PROTECT;
		}
		else if (func == "enemyin") {
			;
		}
		fin >> num >> num1 >> func;
	}
	fin.close();
	return;
}

bool OHMS::AllPile::isHighLand(unsigned short x, unsigned short y) {
	if (x < 0 || y < 0 || x >= this->myBattleGround->battleGroundSize.x || y >= this->myBattleGround->battleGroundSize.y)
		return 0;
	return (this->piles + y * this->myBattleGround->battleGroundSize.x + x)->high;
}

bool OHMS::AllPile::canPlace(unsigned short x, unsigned short y) {
	if (x < 0 || y < 0 || x >= this->myBattleGround->battleGroundSize.x || y >= this->myBattleGround->battleGroundSize.y)
		return 0;
	return (this->piles + y * this->myBattleGround->battleGroundSize.x + x)->canPlace && (this->piles + y * this->myBattleGround->battleGroundSize.x + x)->theOpeID == -1;
}

void OHMS::AllPile::setOpeOn(int opeID, OHMS::EntityMain* ptr, unsigned short x, unsigned short y) {
	if (x < 0 || y < 0 || x >= this->myBattleGround->battleGroundSize.x || y >= this->myBattleGround->battleGroundSize.y)
		return;
	(this->piles + this->myBattleGround->battleGroundSize.x * y + x)->theOpeOnMe = ptr;
	(this->piles + this->myBattleGround->battleGroundSize.x * y + x)->theOpeID = opeID;
	return;
}

int OHMS::AllPile::getOpeOn(unsigned short x, unsigned y) {
	if (x < 0 || y < 0 || x >= this->myBattleGround->battleGroundSize.x || y >= this->myBattleGround->battleGroundSize.y)
		return -1;
	return (this->piles + this->myBattleGround->battleGroundSize.x * y + x)->theOpeID;
}

OHMS::EntityMain* OHMS::AllPile::getOpeOnPtr(unsigned short x, unsigned short y) {
	if (x < 0 || y < 0 || x >= this->myBattleGround->battleGroundSize.x || y >= this->myBattleGround->battleGroundSize.y)
		return nullptr;
	return (this->piles + this->myBattleGround->battleGroundSize.x * y + x)->theOpeOnMe;
}

void OHMS::AllPile::delOpeOn(unsigned short x, unsigned short y) {
	if (x < 0 || y < 0 || x >= this->myBattleGround->battleGroundSize.x || y >= this->myBattleGround->battleGroundSize.y)
		return;
	(this->piles + this->myBattleGround->battleGroundSize.x * y + x)->theOpeID = -1;
	(this->piles + this->myBattleGround->battleGroundSize.x * y + x)->theOpeOnMe = nullptr;
	return;
}

OHMS::EntityMain* OHMS::AllPile::delAndgetOpeOnPtr(unsigned short x, unsigned short y) {
	if (x < 0 || y < 0 || x >= this->myBattleGround->battleGroundSize.x || y >= this->myBattleGround->battleGroundSize.y)
		return nullptr;
	OHMS::EntityMain* res = (this->piles + this->myBattleGround->battleGroundSize.x * y + x)->theOpeOnMe;
	(this->piles + this->myBattleGround->battleGroundSize.x * y + x)->theOpeID = -1;
	(this->piles + this->myBattleGround->battleGroundSize.x * y + x)->theOpeOnMe = nullptr;
	return res;
}

void OHMS::AllPile::clearShow() {
	unsigned int x, y;
	x = this->myBattleGround->battleGroundSize.x;
	y = this->myBattleGround->battleGroundSize.y;
	for (unsigned int j = 0; j < y; ++j) {
		for (unsigned int i = 0; i < x; ++i) {
			*(this->showType + x * j + i) = -1;
		}
	}
	return;
}

void OHMS::AllPile::addShow(int type, OHMS::Range* range) {
	this->clearShow();
	unsigned int x, y;
	int typeD;
	x = this->myBattleGround->battleGroundSize.x;
	y = this->myBattleGround->battleGroundSize.y;
	if (range->type == 1) type = OHMS::AllPile::SHOWTYPE::CAN_HEAL;
	for (unsigned int j = 0; j < y; ++j) {
		for (unsigned int i = 0; i < x; ++i) {
			if (range->checkPos2fInME({ i + 0.5f, j + 0.5f })) {
				typeD = type;
			}
			else {
				typeD = -1;
			}
			*(this->showType + x * j + i) = typeD;
		}
	}
	return;
}

void OHMS::AllPile::addShow(int type, OHMS::PileFilter* pile) {
	this->clearShow();
	unsigned int x, y;
	int typeD;
	x = this->myBattleGround->battleGroundSize.x;
	y = this->myBattleGround->battleGroundSize.y;
	OHMS::PileMain* tmp;
	for (unsigned int j = 0; j < y; ++j) {
		for (unsigned int i = 0; i < x; ++i) {
			tmp = (this->piles + x * j + i);
			if (tmp->theOpeID == -1 && tmp->canPlace && pile->checkPile(tmp)) {
				typeD = type;
			}
			else {
				typeD = -1;
			}
			*(this->showType + x * j + i) = typeD;
		}
	}
	return;
}

void OHMS::AllPile::showArrange(sf::RenderWindow& window) {
	unsigned int x, y;
	int type;
	x = this->myBattleGround->battleGroundSize.x;
	y = this->myBattleGround->battleGroundSize.y;
	for (unsigned int j = 0; j < y; ++j) {
		for (unsigned int i = 0; i < x; ++i) {
			type = *(this->showType + x * j + i);
			if (type >= 0) {
				(this->piles + x * j + i)->displayIn(window, this->texture[type]);
			}
		}
	}
	return;
}

OHMS::PileMain* OHMS::AllPile::getPile(sf::Vector2u pos) {
	if (pos.x < 0 || pos.y < 0 || pos.x >= this->myBattleGround->battleGroundSize.x || pos.y >= this->myBattleGround->battleGroundSize.y)
		return nullptr;
	return (this->piles + this->myBattleGround->battleGroundSize.x * pos.y + pos.x);
}

void OHMS::PileMain::func_DEFAULT(OHMS::PileMain * pile, ...) {
	return;
}

void OHMS::PileMain::func_PROTECT(OHMS::PileMain * pile, ...) {
	return;
}

void OHMS::PileMain::func_ENEMYIN(OHMS::PileMain* pile, ...)
{
}

void OHMS::PileMain::specialPileAs(int type)
{
}

OHMS::PileMain::PileMain() {
	this->high = false;
	this->canPlace = true;
	this->func = &func_DEFAULT;
	this->theOpeID = -1;
	this->theOpeOnMe = nullptr;
	return;
}

void OHMS::PileMain::compPoint(unsigned short x, unsigned short y, OHMS::GSbattleGround * gsbg) {
	float left = x, bottom = y;
	float right = left + 1.0f, top = bottom + 1.0f;
	float hight = this->high ? 0.2f : 0.0f;
	this->pos[0][0] = gsbg->posMap3dTo2d({ left, bottom, hight });
	this->pos[1][0] = gsbg->posMap3dTo2d({ right, bottom, hight });
	this->pos[0][1] = gsbg->posMap3dTo2d({ left, top, hight });
	this->pos[1][1] = gsbg->posMap3dTo2d({ right, top, hight });
	this->pos[0][0].x += winWidth / 2.0f;
	this->pos[1][0].x += winWidth / 2.0f;
	this->pos[0][1].x += winWidth / 2.0f;
	this->pos[1][1].x += winWidth / 2.0f;
	this->pos[0][0].y += winHight / 2.0f;
	this->pos[1][0].y += winHight / 2.0f;
	this->pos[0][1].y += winHight / 2.0f;
	this->pos[1][1].y += winHight / 2.0f;
	this->arrage = new sf::VertexArray(sf::Quads, 4);
	this->arrage->operator[](0).texCoords = {0.0f, 0.0f};
	this->arrage->operator[](1).texCoords = {32.0f, 0.0f};
	this->arrage->operator[](2).texCoords = {32.0f, 32.0f};
	this->arrage->operator[](3).texCoords = {0.0f, 32.0f};
	this->arrage->operator[](0).position = this->pos[0][1];
	this->arrage->operator[](1).position = this->pos[1][1];
	this->arrage->operator[](2).position = this->pos[1][0];
	this->arrage->operator[](3).position = this->pos[0][0];
	return;
}

void OHMS::PileMain::displayIn(sf::RenderWindow& window, sf::Texture &texture) {
	return window.draw(*(this->arrage), &texture);
}

OHMS::Range::Range() {
	this->dist = 0;
	this->origin = { 0.0f, 0.0f };
	this->range.clear();
	this->backPile = 0;
	this->type = 0;
	return;
}

bool OHMS::Range::checkEntityInME(OHMS::EntityMain* theEntity) {
	sf::Vector2f pos = theEntity->getPosition();
	return 
	this->checkPos2fInME({ pos.x, pos.y - cloRadius }) ||
	this->checkPos2fInME({ pos.x, pos.y + cloRadius }) ||
	this->checkPos2fInME({ pos.x - cloRadius, pos.y }) ||
	this->checkPos2fInME({ pos.x + cloRadius, pos.y });
}

bool OHMS::Range::checkPos2fInME(sf::Vector2f pos2f) {
	return OHMS::Range::checkPos2fInRange(*this, pos2f);
}

bool OHMS::Range::checkPos2fInRange(OHMS::Range& range, sf::Vector2f pos2f) {
	sf::Vector2f deltaV = pos2f - range.origin;
	if (range.dist % 2) {
		std::swap(deltaV.x, deltaV.y);
	}
	if (range.dist % 4 > 1) {
		deltaV.x = -deltaV.x;
	}
	deltaV.x += 0.5f + range.backPile;
	if (deltaV.x < 0.0f) return false;
	deltaV.y = fabsf(deltaV.y) + 0.5f;
	int dx = (int)deltaV.x, dy = (int)deltaV.y;
	if (dx >= (int)range.range.size()) return false;
	if (dy > range.range[dx]) return false;
	return true;
}

void OHMS::Range::setDist(unsigned char dist) {
	this->dist = dist;
	return;
}

void OHMS::Range::setOrigin(sf::Vector2f pos) {
	this->origin = pos;
	return;
}

void OHMS::Range::setBackPile(int bp) {
	this->backPile = bp;
	return;
}

void OHMS::Range::setRange(int length, ...) {
	this->range.clear();
	va_list ap;
	va_start(ap, length);
	for (int i = 0; i < length; ++i) {
		this->range.push_back(va_arg(ap, int));
	}
	va_end(ap);
	return;
}

void OHMS::Range::setRangePushBack(int nextRange) {
	return this->range.push_back(nextRange);
}

void OHMS::Range::clear() {
	this->dist = 0;
	this->origin = { 0.0f, 0.0f };
	this->range.clear();
	this->backPile = 0;
	this->type = 0;
	return;
}

OHMS::PileFilter::PileFilter() {
	this->wantStyle = OHMS::PileFilter::style::PLAIN;
	return;
}

bool OHMS::PileFilter::checkPile(OHMS::PileMain* pile) {
	if ((this->wantStyle & OHMS::PileFilter::style::PLAIN) && ((!pile->high) || (pile->bothHighAndPlain))) return true;
	if ((this->wantStyle & OHMS::PileFilter::style::HIGH) && ((pile->high) || (pile->bothHighAndPlain))) return true;
	return false;
}

OHMS::EntityFilter::EntityFilter() {
	this->wantStyle = 0;
	return;
}

bool OHMS::EntityFilter::checkEntity(OHMS::EntityMain* entity) {
	if (this->wantStyle & OHMS::EntityFilter::Style::isEnemy) {
		if (!entity->isEnemy) return false;
	}
	if (this->wantStyle & OHMS::EntityFilter::Style::isOperator) {
		if (entity->isEnemy) return false;
	}
	return true;
}

OHMS::EntityArm::EntityArm() {
	this->attackType = 2;
	this->attacked = false;
	this->nice = false;
	return;
}

void OHMS::EntityArm::veryFirstSet(int n, OHMS::EntityMain* theBody) {
	this->getNum = n;
	this->goals = new OHMS::EntityMain * [n];
	this->myBody = theBody;
	return;
}

void OHMS::EntityArm::aNewRound() {
	this->attacked = false;
	return;
}

void OHMS::EntityArm::AttackIt(OHMS::EntityMain* target) {
	float delta = this->myBody->attribute.attack;
	float &health = target->attribute.health;
	switch (this->attackType) {
	case OHMS::EntityArm::Type::Phys_Damage:
		delta = std::max(this->myBody->attribute.attack - target->attribute.defende, this->myBody->attribute.attack * 0.05f);
		break;
	case OHMS::EntityArm::Type::Magi_Damage:
		delta = this->myBody->attribute.attack * std::max( 1.0f - target->attribute.magicalDef, 0.05f);
		break;
	case OHMS::EntityArm::Type::Real_Damage:
	default:
		;
	}
	health -= delta;
	if (health < 0.0f) health = 0.0f;
	if (health > target->attribute.healthMax) health = target->attribute.healthMax;
	return;
}

void OHMS::EntityArm::getGoal() {
	std::vector<OHMS::EntityMain*>* entity = &(this->myBody->myTeam->entity);
	std::vector<OHMS::EntityMain*>::iterator i = entity->begin();
	for (int i = 0; i < this->getNum; ++i) {
		this->goals[i] = nullptr;
	}
	for (int cnt = 0; cnt < this->getNum && i != entity->end(); ++i) {
		if ((*i)->normal) {
			if (this->myBody->entityFilter.checkEntity(*i)) {
				if (this->myBody->atkRange.checkEntityInME(*i)) {
					this->goals[cnt++] = (*i);
					this->nice = true;
				}
			}
		}
	}
	return;
}

bool OHMS::EntityArm::check() {
	return this->nice;
}

void OHMS::EntityArm::update() {
	if (this->getNum < 1) {
		this->nice = false;
		return;
	}
	if (this->goals[0] == nullptr) {
		this->nice = false;
		return;
	}
	sf::Vector2f myPos, goalPos;
	myPos = this->myBody->position;
	goalPos = this->goals[0]->position;
	if (!this->attacked) {
		if (this->myBody->myAnime->playRate() > 0.36f) {
			if (!this->goals[0]->normal || !this->myBody->atkRange.checkEntityInME(this->goals[0])) {
				this->nice = false;
				return;
			}
			for (int i = 0; i < this->getNum; ++i) {
				this->AttackIt(this->goals[i]);
			}
			this->attacked = true;
		}
	}
	if (goalPos.x - myPos.x < 0.0f) {
		this->myBody->myAnime->setTurnLeft(true);
	}
	else {
		this->myBody->myAnime->setTurnLeft(false);
	}
	return;
}

OHMS::EntityAttribute::EntityAttribute() {
	this->health = 100.0f;
	this->healthMax = 100.0f;
	this->attack = 100.0f;
	this->defende = 0.0f;
	this->magicalDef = 0.0f;
	return;
}

OHMS::OperatorReset::OperatorReset() {
	this->opeSetLeftTime = nullptr;
	this->cost = 0;
	this->costLoad = 0;
	this->openum = 0;
	return;
}

OHMS::OperatorReset::~OperatorReset() {
	delete[] this->opeSetLeftTime;
	return;
}

void OHMS::OperatorReset::veryFirstSet(unsigned short n, unsigned int costdelta, unsigned int costFirst, sf::Font * f) {
	this->opeSetLeftTime = new unsigned int[n];
	this->openum = n;
	this->costDelta = costdelta;
	this->cost = costFirst;
	this->dis.setFont(f);
	for (int i = 0; i < n; ++i) {
		this->opeSetLeftTime[i] = 0;
	}
	return;
}

void OHMS::OperatorReset::getBack(unsigned short n) {
	this->opeSetLeftTime[n] = 60 * 30;
	return;
}

bool OHMS::OperatorReset::canGo(unsigned short n) {
	return !(this->opeSetLeftTime[n]);
}

float OHMS::OperatorReset::leftTime(unsigned short n) {
	return (this->opeSetLeftTime[n] / 30.0f);
}

bool OHMS::OperatorReset::giveoutCost(unsigned short k) {
	if (this->cost >= k) {
		this->cost -= k;
		return true;
	}
	return false;
}

unsigned short OHMS::OperatorReset::hasCost() {
	return this->cost;
}

void OHMS::OperatorReset::update() {
	if (this->cost < 99) {
		this->costLoad += this->costDelta;
		if (this->costLoad / 3600) {
			this->cost += this->costLoad / 3600;
			this->costLoad %= 3600;
		}
		this->dis.setNumber(this->cost, this->costLoad / 3600.0f);
	}
	for (int i = 0; i < this->openum; ++i) {
		if (this->opeSetLeftTime[i] > 0)
			this->opeSetLeftTime[i] --;
	}
	return;
}

void OHMS::OperatorReset::displayIn(sf::RenderWindow& window) {
	window.draw(this->dis.getText());
	window.draw(this->dis.getShape());
	return;
}

OHMS::daojishi::daojishi() {
	this->t.setCharacterSize(40);
	this->t.setFillColor(sf::Color::White);
	this->t.setString("0.0");
	sf::FloatRect rect = this->t.getGlobalBounds();
	this->t.setOrigin(rect.width / 2.0f, rect.height / 2.0f);
	return;
}

void OHMS::daojishi::setSecond(float second) {
	char tmp[15];
	sprintf_s(tmp, 15, "%.1lf", second);
	this->t.setString(tmp);
	sf::FloatRect rect = this->t.getGlobalBounds();
	this->t.setOrigin(rect.width / 2.0f, rect.height / 2.0f);
	return;
}

sf::Text& OHMS::daojishi::getText(float x, float y) {
	this->t.setPosition(x, y);
	return this->t;
}

void OHMS::daojishi::setFont(sf::Font* f) {
	this->t.setFont(*f);
	return;
}

OHMS::costDisplay::costDisplay() {
	this->t.setCharacterSize(40);
	this->t.setFillColor(sf::Color::White);
	this->t.setString("0");
	sf::FloatRect rect = this->t.getGlobalBounds();
	this->t.setOrigin(0.0f, rect.height / 2.0f);

	this->s.setFillColor(sf::Color::White);
	this->s.setOutlineThickness(0);
	this->s.setSize({ 100.0f, 5.0f });
	this->s.setOrigin(0.0f, 2.5f);
	this->s.setScale(0.0f, 1.0f);

	this->t.setPosition(winWidth - 100.0f, winHight - canPlayHeadSize - 100.0f);
	this->s.setPosition(winWidth - 100.0f, winHight - canPlayHeadSize - 50.0f);
	return;
}

void OHMS::costDisplay::setNumber(int num, float rate) {
	char tmp[15];
	sprintf_s(tmp, 15, "%d", num);
	this->t.setString(tmp);
	this->s.setScale(rate, 1.0f);
	return;
}

sf::Text& OHMS::costDisplay::getText() {
	return this->t;
}

sf::RectangleShape& OHMS::costDisplay::getShape() {
	return this->s;
}

void OHMS::costDisplay::setFont(sf::Font* f) {
	this->t.setFont(*f);
	return;
}

OHMS::SummonCtrl::SummonCtrl(OHMS::BattleGroundMain* bg) {
	this->cnt = 0;
	this->theBG = bg;
	this->allcnt = 15;
	this->already = 0;
	return;
}

void OHMS::SummonCtrl::update() {
	if (this->already < this->allcnt) {
		this->cnt++;
		if (cnt > 450) {
			int n = rand();
			while (n >= 30000) n = rand();
			n %= 5;
			int m = rand();
			while (m >= 30000) m = rand();
			m %= 4;
			this->theBG->entityJoin(1, m, { 11.5f, n + 0.5f });
			this->already++;
			cnt -= 450;
		}
	}
	return;
}
