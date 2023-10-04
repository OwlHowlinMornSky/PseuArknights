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
#include "gamegraphic.h"

#include <memory>

namespace {

std::unique_ptr<sf::Shader> g_lihuishader;

}

void OHMS::initLihuiShader() {
	static const std::string vs =
		"void main() {"\
		" gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;"\
		" gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;"\
		" gl_FrontColor = gl_Color;"\
		"}";
	static const std::string fs =
		"uniform sampler2D texture;"\
		"uniform int asBattle;"\
		"uniform float alpha;"\
		"void main() {"\
		" float ac = min(1.0, max(0.0, 2.0 * (1.0 - gl_FragCoord.x / (0.4 * 1280.0))));"\
		" vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);"\
		" gl_FragColor = gl_Color * pixel;"\
		" gl_FragColor.w = (asBattle == 1) ? (gl_FragColor.w * ac * alpha) : (gl_FragColor.w * alpha);"
		"}";
	g_lihuishader = std::make_unique<sf::Shader>();
	g_lihuishader->loadFromMemory(vs, fs);
	return;
}


void OHMS::GSocket::displayEle(sf::RenderWindow & window) {
	return;
}

void OHMS::GSocket::updatePosEle() {
	return;
}

void OHMS::GSocket::updatePos() {
	if (this->parent != nullptr) 
		this->posAbs = this->posToParent + this->parent->posAbs;
	else 
		this->posAbs = this->posToParent;
	this->updatePosEle();
	if (this->subGS1 != nullptr) this->subGS1->updatePos();
	if (this->subGS2 != nullptr) this->subGS2->updatePos();
	return;
}

void OHMS::GSocket::updateAlphaEle() {
	return;
}

void OHMS::GSocket::updateAlpha() {
	if (this->parent != nullptr)
		this->alphaAbs = this->parent->alphaAbs;
	else
		this->alphaAbs = this->alphaToParent;
	this->updateAlphaEle();
	if (this->subGS1 != nullptr) this->subGS1->updateAlpha();
	if (this->subGS2 != nullptr) this->subGS2->updateAlpha();
	return;
}

void OHMS::GSocket::updateScaleEle() {
	return;
}

void OHMS::GSocket::updateScale() {
	if (this->parent != nullptr) {
		this->scaleAbs.x = this->scaleToParent.x * this->parent->scaleAbs.x;
		this->scaleAbs.y = this->scaleToParent.y * this->parent->scaleAbs.y;
	}
	this->updateScaleEle();
	if (this->subGS1 != nullptr) this->subGS1->updateScale();
	if (this->subGS2 != nullptr) this->subGS2->updateScale();
	return;
}

void OHMS::GSocket::updateRotationEle() {
	return;
}

void OHMS::GSocket::updateRotation() {
	if (this->parent != nullptr) 
		this->rotationAbs = this->parent->rotationAbs + this->rotationToParent;
	this->updateRotationEle();
	if (this->subGS1 != nullptr) this->subGS1->updateRotation();
	if (this->subGS2 != nullptr) this->subGS2->updateRotation();
	return;
}

OHMS::GSocket::GSocket() {
	this->posToParent = { 0.0f, 0.0f };
	this->posAbs = { 0.0f, 0.0f };
	this->scaleToParent = { 1.0f, 1.0f };
	this->scaleAbs = { 1.0f, 1.0f };
	this->rotationToParent = 0.0f;
	this->rotationAbs = 0.0f;
	this->parent = nullptr;
	this->subGS1 = nullptr;
	this->subGS2 = nullptr;
	return;
}

OHMS::GSocket::~GSocket() {
	delete this->subGS1;
	delete this->subGS2;
	return;
}

void OHMS::GSocket::setSubGS(bool num, OHMS::GSocket * gs) {
	if (num) {
		this->subGS2 = gs;
		gs->parent = this;
		return;
	}
	this->subGS1 = gs;
	gs->parent = this;
	return;
}

OHMS::GSocket * OHMS::GSocket::getSubGS(bool num) {
	if (num) return this->subGS2;
	return this->subGS1;
}

void OHMS::GSocket::setPosToParent(sf::Vector2f pos) {
	this->posToParent = pos;
	return this->updatePos();
}

void OHMS::GSocket::setScaleToParent(sf::Vector2f scale) {
	this->scaleToParent = scale;
	return this->updateScale();
}

void OHMS::GSocket::displayIn(sf::RenderWindow & window) {
	this->displayEle(window);
	if (this->subGS1 != nullptr)
		this->subGS1->displayIn(window);
	if (this->subGS2 != nullptr)
		this->subGS2->displayIn(window);
	return;
}

void OHMS::GSsprite::displayEle(sf::RenderWindow & window) {
	return window.draw(this->sprite);
}

void OHMS::GSsprite::updatePosEle() {
	return this->sprite.setPosition(this->posAbs);
}

void OHMS::GSsprite::updateScaleEle() {
	return this->sprite.setScale(this->scaleAbs);
}

OHMS::GSsprite::GSsprite(sf::Texture &tx) {
	this->texture = &tx;
	this->sprite.setTexture(tx);
	return;
}

OHMS::GSsprite::~GSsprite() {
	delete this->texture;
	return;
}

void OHMS::GSsprite::updateOrg() {
	sf::FloatRect fr = this->sprite.getGlobalBounds();
	return this->sprite.setOrigin(fr.width * this->orgRate.x, fr.height * this->orgRate.y);
}

void OHMS::GSsprite::setOrgRate(sf::Vector2f rate) {
	this->orgRate = rate;
	return this->updateOrg();
}

sf::Sprite * OHMS::GSsprite::getSprite() {
	return &(this->sprite);
}

void OHMS::GSROOT::setPosAbs(sf::Vector2f pos) {
	this->posAbs = pos;
	if (this->subGS1 != nullptr)
		this->subGS1->updatePos();
	if (this->subGS2 != nullptr)
		this->subGS2->updatePos();
	return;
}

void OHMS::GSROOT::setAlphaAbs(unsigned int alpha) {
	this->alphaAbs = alpha;
	if (this->subGS1 != nullptr)
		this->subGS1->updateAlpha();
	if (this->subGS2 != nullptr)
		this->subGS2->updateAlpha();
	return;
}

void OHMS::GStext::updateOrg() {
	sf::FloatRect fr = this->text.getGlobalBounds();
	return this->text.setOrigin(fr.width * this->orgRate.x, fr.height * this->orgRate.y);
}

void OHMS::GStext::displayEle(sf::RenderWindow & window) {
	return window.draw(this->text);
}

void OHMS::GStext::updatePosEle() {
	return this->text.setPosition(this->posAbs);
}

void OHMS::GStext::updateScaleEle() {
	return this->text.setScale(this->scaleAbs);
}

void OHMS::GStext::updateAlphaEle() {
	return this->text.setFillColor(sf::Color(255, 255, 255, this->alphaAbs));
}

OHMS::GStext::GStext() {
	this->orgRate = { 0.0f, 0.0f };
	return;
}

OHMS::GStext::~GStext() {
	return;
}

void OHMS::GStext::setOrgRate(sf::Vector2f rate) {
	this->orgRate = rate;
	return this->updateOrg();
}

sf::Text * OHMS::GStext::getText(){
	return &(this->text);
}

OHMS::GSframeAnima::GSframeAnima() {
	return;
}

OHMS::GSframeAnima::GSframeAnima(const std::string path) {
	this->setAnimation(path);
	return;
}

OHMS::GSframeAnima::~GSframeAnima() {
	return;
}

void OHMS::GSframeAnima::setAnimation(const std::string path){
	this->frameCnt = 0;
	this->frameFlag = 0;
	this->over = false;
	this->loop = false;
	OHMS::filescanf(path + "info.txt", "%u", &this->frameCnt);
	this->texture.loadFromFile(path + "tex.png");
	this->texture.setSmooth(true);
	this->sprite.setTexture(this->texture);
	this->sprite.setOrigin(128.0f, 230.0f);
	return;
}
sf::Texture * OHMS::GSframeAnima::getTexture() {
	return &(this->texture);
}

void OHMS::GSframeAnima::update(unsigned short frameN){
	if (this->over)
		return;
	this->frameFlag += frameN;
	if (this->frameFlag >= this->frameCnt) {
		if (this->loop) {
			this->frameFlag %= this->frameCnt;
		}
		else {
			this->frameFlag = this->frameCnt - 1;
			this->over = true;
		}
	}
	return this->sprite.setTextureRect(sf::IntRect((this->frameFlag % 64) * 256, (this->frameFlag / 64) * 256, 256, 256));
}

void OHMS::GSframeAnima::update(){
	if (this->over)
		return;
	++ this->frameFlag;
	if (this->frameFlag >= this->frameCnt) {
		if (this->loop) {
			this->frameFlag %= this->frameCnt;
		}
		else {
			this->frameFlag = this->frameCnt - 1;
			this->over = true;
		}
	}
	return this->sprite.setTextureRect(sf::IntRect((this->frameFlag % 64) * 256, (this->frameFlag / 64) * 256, 256, 256));
}

void OHMS::GSframeAnima::displayIn(sf::RenderWindow & window) {
	return window.draw(this->sprite);
}

void OHMS::GSframeAnima::reset() {
	this->frameFlag = 0;
	this->over = false;
	return this->sprite.setTextureRect(sf::IntRect(0, 0, 256, 256));
}

void OHMS::GSframeAnimaCtrl::updatePosEle() {
	for (unsigned short i = 0; i < this->animaCnt; ++i) {
		this->Shadow.setPosition(this->posAbs.x, this->posAbs.y);
		(this->animation + i)->sprite.setPosition(this->posAbs);
	}
	return;
}

void OHMS::GSframeAnimaCtrl::updateScaleEle() {
	for (unsigned short i = 0; i < this->animaCnt; ++i) {
		this->Shadow.setScale(this->scaleAbs*this->shadowR*0.1f);
		(this->animation + i)->sprite.setScale(this->scaleAbs.x * (this->turnLeft ? -1.0f : 1.0f), this->scaleAbs.y);
	}
	return;
}

OHMS::GSframeAnimaCtrl::GSframeAnimaCtrl() {
	this->shadowR = 1.0f;
	this->animaFlag = 0;
	this->animation = nullptr;
	this->ShadowTX = nullptr;
	this->turnLeft = false;
	return;
}

OHMS::GSframeAnimaCtrl::~GSframeAnimaCtrl() {
	delete[] this->animation;
	return;
}

void OHMS::GSframeAnimaCtrl::veryFirstSet(const std::string path, OHMS::GSocket * par) {
	this->animation = new OHMS::GSframeAnima[6];
	this->animaCnt = 6;
	this->parent = par;
	this->animation->setAnimation(path + "default/");
	this->animation->loop = false;
	(this->animation + 1)->setAnimation(path + "set/");
	(this->animation + 1)->loop = false;
	(this->animation + 2)->setAnimation(path + "idle/");
	(this->animation + 2)->loop = true;
	(this->animation + 3)->setAnimation(path + "attack/");
	(this->animation + 3)->loop = false;
	(this->animation + 4)->setAnimation(path + "die/");
	(this->animation + 4)->loop = false;
	(this->animation + 5)->setAnimation(path + "move/");
	(this->animation + 5)->loop = true;
	this->Shadow.setTexture(*this->ShadowTX);
	sf::Vector2u TXsize = this->ShadowTX->getSize();
	this->Shadow.setOrigin(TXsize.x * 0.5f, TXsize.y * 0.5f);
	this->updatePos();
	this->updateScale();
	return;
}

void OHMS::GSframeAnimaCtrl::aNewSet(OHMS::GSframeAnimaCtrl * orgFrame) {
	this->animation = new OHMS::GSframeAnima[6];
	this->animaCnt = 6;
	this->parent = orgFrame->parent;
	*this->animation = *orgFrame->animation;
	*(this->animation + 1) = *(orgFrame->animation + 1);
	*(this->animation + 2) = *(orgFrame->animation + 2);
	*(this->animation + 3) = *(orgFrame->animation + 3);
	*(this->animation + 4) = *(orgFrame->animation + 4);
	*(this->animation + 5) = *(orgFrame->animation + 5);
	this->Shadow.setTexture(*this->ShadowTX);
	sf::Vector2u TXsize = this->ShadowTX->getSize();
	this->Shadow.setOrigin(TXsize.x * 0.5f, TXsize.y * 0.5f);
	this->turnLeft = false;
	this->updatePos();
	this->updateScale();
	return;
}

void OHMS::GSframeAnimaCtrl::changeAnimation(unsigned short aniN) {
//	if (aniN < this->animaCnt) {
		(this->animation + this->animaFlag)->reset();
		this->animaFlag = aniN;
		(this->animation + this->animaFlag)->reset();
//	}
	return;
}

unsigned short OHMS::GSframeAnimaCtrl::getAnimation() {
	return this->animaFlag;
}

void OHMS::GSframeAnimaCtrl::update(unsigned short frameN) {
	return (this->animation + this->animaFlag)->update(frameN);
}

void OHMS::GSframeAnimaCtrl::update() {
	return (this->animation + this->animaFlag)->update();
}

void OHMS::GSframeAnimaCtrl::setTurnLeft(bool turn) {
	if (this->turnLeft != turn) {
		this->turnLeft = turn;
		for (unsigned short i = 0; i < this->animaCnt; i++) {
			(this->animation + i)->sprite.scale({ -1.0f, 1.0f });
		}
	}
	return;
}

void OHMS::GSframeAnimaCtrl::Turn() {
	this->turnLeft = !this->turnLeft;
	this->updateScale();
	return;
}

void OHMS::GSframeAnimaCtrl::displayIn(sf::RenderWindow & window) {
	window.draw(this->Shadow);
	return (this->animation + this->animaFlag)->displayIn(window);
}

void OHMS::GSframeAnimaCtrl::setShadowR(float r) {
	this->shadowR = r;
	return this->updateScale();
}

bool OHMS::GSframeAnimaCtrl::playOver() {
	return (this->animation + this->animaFlag)->over;
}

float OHMS::GSframeAnimaCtrl::playRate() {
	return 1.0f * (this->animation + this->animaFlag)->frameFlag / (this->animation + this->animaFlag)->frameCnt;
}

OHMS::GSframeAnima * OHMS::GSframeAnimaCtrl::getFrameAnima(const unsigned short num) {
//	if (num < this->animaCnt) {
		return (this->animation + num);
//	}
	return nullptr;
}

OHMS::GSbattleGround::GSbattleGround() {
	this->posAbs = { winWidth / 2.0f, winHight / 2.0f };
	this->animeORGnum = 0;
	this->animeORG = nullptr;
	this->uiTX.loadFromFile("assets/battle/texture/ui.png");
	this->uiTX.generateMipmap();
	this->finishTX.loadFromFile("assets/battle/texture/finish.png");
	this->finishTX.generateMipmap();
	return;
}

OHMS::GSbattleGround::~GSbattleGround() {
	delete[] this->animeORG;
	return;
}

void OHMS::GSbattleGround::update() {
	this->anime.update();
	return;
}

void OHMS::GSbattleGround::setAnimaPosition(sf::Vector3f pos, OHMS::GSframeAnimaCtrl * object) {
	this->anime.needSort = true;
	sf::Vector3f pos0 = pos;
	object->BattlePos = { pos.x, pos.y };
	sf::Vector2f resPos0 = this->posMap3dTo2d(pos0);
	pos0 = { pos.x + 0.1f, pos.y, pos.z };
	sf::Vector2f resPos1 = this->posMap3dTo2d(pos0);
	pos0 = { pos.x - 0.1f, pos.y, pos.z };
	sf::Vector2f resPos2 = this->posMap3dTo2d(pos0);
	resPos1 -= resPos2;
	float len = std::sqrt(resPos1.x * resPos1.x + resPos1.y * resPos1.y) * 0.066f;
	object->setScaleToParent({ len, len });
	return object->setPosToParent(resPos0);
}

void OHMS::GSbattleGround::displayGroundIn(sf::RenderWindow & window) {
	return window.draw(this->levelGround);
}

void OHMS::GSbattleGround::displayAnimeIn(sf::RenderWindow & window) {
	return this->anime.displayIn(window);
}

void OHMS::AllAnime::update() {
	for (unsigned int i = 0, j = this->anime.size(); i < j; ++i) {
		this->anime[i]->update();
	}
	return;
}

void OHMS::AllAnime::displayIn(sf::RenderWindow & window) {
	if (this->needSort) {
		this->SORT();
		this->needSort = false;
	}
	for (std::vector<OHMS::GSframeAnimaCtrl*>::iterator i = this->anime.begin(); i != this->anime.end(); ++i) {
		(*i)->displayIn(window);
	}
	return;
}

void OHMS::AllAnime::setAnimePos(OHMS::GSframeAnimaCtrl * anime, sf::Vector2f pos, bool upLand) {
	this->needSort = true;
	sf::Vector3f pos0 = { pos.x, pos.y, upLand ? 0.2f : 0.0f };
	anime->BattlePos = pos;
	sf::Vector2f resPos0 = this->myBattleGround->posMap3dTo2d(pos0);
	pos0 = { pos.x + 0.1f, pos.y, upLand ? 0.2f : 0.0f };
	sf::Vector2f resPos1 = this->myBattleGround->posMap3dTo2d(pos0);
	pos0 = { pos.x - 0.1f, pos.y, upLand ? 0.2f : 0.0f };
	sf::Vector2f resPos2 = this->myBattleGround->posMap3dTo2d(pos0);
	resPos1 -= resPos2;
	float len = std::sqrt(resPos1.x * resPos1.x + resPos1.y * resPos1.y ) * 0.066f;
	anime->setScaleToParent({ len, len });
	return anime->setPosToParent(resPos0);
}

void OHMS::AllAnime::setAnimePos(const unsigned short num, sf::Vector2f pos, bool upLand, bool yeah) {
	return OHMS::AllAnime::setAnimePos(this->anime[num], pos, upLand);
}

bool OHMS::AllAnime::SORT_COMPARE(OHMS::GSframeAnimaCtrl * pi, OHMS::GSframeAnimaCtrl * pj) {
	return (pi->BattlePos.y > pj->BattlePos.y);
}

void OHMS::AllAnime::SORT() {
	return std::stable_sort(this->anime.begin(), this->anime.end(), SORT_COMPARE);
}

OHMS::AllAnime::AllAnime() {
	this->anime.clear();
	while (!this->ptrRecycle.empty())
		this->ptrRecycle.pop();
	this->subGS1 = nullptr;
	this->subGS2 = nullptr;
	this->ShadowTX.loadFromFile("assets/battle/texture/shadow.png");
	this->needSort = false;
	return;
}

OHMS::AllAnime::~AllAnime() {
	for (unsigned int i = 0, j = this->anime.size(); i < j; ++i) {
		delete this->anime[i];
	}
	while (!this->ptrRecycle.empty()) {
		delete this->ptrRecycle.top();
		this->ptrRecycle.pop();
	}
	return;
}

OHMS::GSframeAnimaCtrl * OHMS::AllAnime::addAnime() {
	OHMS::GSframeAnimaCtrl * flag;
	if (this->ptrRecycle.empty()) {
		flag = new OHMS::GSframeAnimaCtrl;
		flag->ShadowTX = &(this->ShadowTX);
	}
	else {
		flag = ptrRecycle.top();
		ptrRecycle.pop();
	}
	this->anime.push_back(flag);
	return flag;
}

void OHMS::AllAnime::delAnime(OHMS::GSframeAnimaCtrl * delObj) {
	for (unsigned int i = 0, j = this->anime.size(); i < j; ++i) {
		if (this->anime[i] == delObj) {
			this->ptrRecycle.push(delObj);
			this->anime[i] = this->anime[j - 1];
			this->anime.pop_back();
			break;
		}
	}
	return this->SORT();
}

void OHMS::AllAnime::setAnime(const unsigned short n, const std::string path) {
	return this->anime[n]->veryFirstSet(path, this);
}

OHMS::GSframeAnimaCtrl * OHMS::AllAnime::getAnimeCtrl(const unsigned short n) {
	return this->anime[n];
}

void OHMS::GSbattleGround::setCamera(sf::Vector3f cameraPos, float downAngle) {
	this->cameraPos = cameraPos;
	this->cameraFront = { 0.0f, cosf(downAngle), -sinf(downAngle) };
	this->cameraX = { 1.0f, 0.0f, 0.0f };
	this->cameraY = { 0.0f, -sinf(downAngle), -cosf(downAngle) };
	return;
}

sf::Vector2f OHMS::GSbattleGround::posMap3dTo2d(const sf::Vector3f pos3d) {
	sf::Vector3f vectorTo3dPoint, vectorPlain;
	float lengthVector3d, cosToFront, rho, lengthVectorPlain, cosTo2dX, cosTo2dY;
	vectorTo3dPoint = pos3d - this->cameraPos;
	lengthVector3d =
		sqrtf(vectorTo3dPoint.x * vectorTo3dPoint.x
			+ vectorTo3dPoint.y * vectorTo3dPoint.y
			+ vectorTo3dPoint.z * vectorTo3dPoint.z);
	cosToFront =
		(this->cameraFront.x * vectorTo3dPoint.x +
			this->cameraFront.y * vectorTo3dPoint.y +
			this->cameraFront.z * vectorTo3dPoint.z)
		/ 1.0f / lengthVector3d;
	if (cosToFront >= 1.0f) return { 0.0f, 0.0f };
	rho = tanf(std::min(acosf(cosToFront), 1.5707963f)) * 1320.0f;
	vectorPlain =
	{ vectorTo3dPoint.x - this->cameraFront.x * cosToFront * lengthVector3d,
	vectorTo3dPoint.y - this->cameraFront.y * cosToFront * lengthVector3d,
	vectorTo3dPoint.z - this->cameraFront.z * cosToFront * lengthVector3d };
	lengthVectorPlain =
		sqrtf(vectorPlain.x * vectorPlain.x +
			vectorPlain.y * vectorPlain.y +
			vectorPlain.z * vectorPlain.z);
	cosTo2dX =
		(this->cameraX.x * vectorPlain.x + this->cameraX.y * vectorPlain.y + this->cameraX.z * vectorPlain.z)
		/ 1.0f / lengthVectorPlain;
	cosTo2dY =
		(this->cameraY.x * vectorPlain.x + this->cameraY.y * vectorPlain.y + this->cameraY.z * vectorPlain.z)
		/ 1.0f / lengthVectorPlain;
	return { rho * cosTo2dX, rho * cosTo2dY };
}

sf::Vector2f OHMS::GSbattleGround::posMap2dTo3d(const sf::Vector2f pos2d, float z) {
	float rho, tanToFront, stdVl, l = 0.1f, r = 100.0f, m;
	sf::Vector3f stdV, lv = cameraPos, mv = cameraPos, rv;
	sf::Vector2f testV;
	rho = sqrtf(pos2d.x * pos2d.x + pos2d.y * pos2d.y);
	if (rho < 12.0f) {
		stdV = { 0.0f, 0.0f, 0.0f };
		lv += {cameraFront.x*l, cameraFront.y*l, cameraFront.z*l};
		rv += {cameraFront.x*r, cameraFront.y*r, cameraFront.z*r};
		while (fabsf(r - l) > 0.0001220703125f) {
			m = (l + r) / 2.0f;
			mv = cameraPos;
			mv += {cameraFront.x*m, cameraFront.y*m, cameraFront.z*m};
			if (mv.z > z) l = m, lv = mv;
			else r = m, rv = mv;
		}
	}
	else {
		testV = { pos2d.x / rho, pos2d.y / rho };
		tanToFront = rho / 1320.0f;
		stdV = { cameraX.x*testV.x + cameraY.x*testV.y, cameraX.y*testV.x + cameraY.y*testV.y, cameraX.z*testV.x + cameraY.z*testV.y };
		stdVl = sqrtf(stdV.x*stdV.x + stdV.y*stdV.y + stdV.z*stdV.z);
		stdV = { stdV.x / stdVl, stdV.y / stdVl, stdV.z / stdVl };
		lv += {stdV.x*l + cameraFront.x*l / tanToFront, stdV.y*l + cameraFront.y*l / tanToFront, stdV.z*l + cameraFront.z*l / tanToFront};
		rv += {stdV.x*r + cameraFront.x*r / tanToFront, stdV.y*r + cameraFront.y*r / tanToFront, stdV.z*r + cameraFront.z*r / tanToFront};
		while (fabsf(r - l) > 0.0001220703125f) {
			m = (l + r) / 2.0f;
			mv = cameraPos;
			mv += {stdV.x*m + cameraFront.x*m / tanToFront, stdV.y*m + cameraFront.y*m / tanToFront, stdV.z*m + cameraFront.z*m / tanToFront};
			if (mv.z > z) l = m, lv = mv;
			else r = m, rv = mv;
		}
	}
	return { rv.x, rv.y };
}

void OHMS::GSbattleGround::setBackground(std::string path) {
	this->levelGroundTX.loadFromFile(path);
	return this->levelGround.setTexture(this->levelGroundTX);
}

void OHMS::GSbattleGround::initAnimeORG(const unsigned short num) {
	this->animeORGnum = num;
	this->animeORG = new OHMS::GSframeAnimaCtrl[num];
	return;
}

void OHMS::GSbattleGround::setAnimeOrg(const unsigned short num, std::string path) {
//	if (num < this->animeORGnum) {
		(this->animeORG + num)->ShadowTX = &(this->anime.ShadowTX);
//		;
//	}
	return (this->animeORG + num)->veryFirstSet(path, &(this->anime));
}

OHMS::GSframeAnimaCtrl * OHMS::GSbattleGround::getAnimeOrg(const unsigned short num) {
//	if (num < this->animeORGnum) {
		return (this->animeORG + num);
//	}
	return nullptr;
}

void OHMS::GSbattleGround::initAllAnime() {
	this->anime.parent = this;
	this->anime.myBattleGround = this;
	this->anime.updatePos();
	return;
}

OHMS::GSframeAnimaCtrl * OHMS::GSbattleGround::animeJoin(const unsigned short num) {
//	if (num < this->animeORGnum) {
		OHMS::GSframeAnimaCtrl * flag = this->anime.addAnime();
		flag->aNewSet(this->animeORG + num);
		return flag;
//	}
	return nullptr;
}

OHMS::AllAnime * OHMS::GSbattleGround::getAnime() {
	return &(this->anime);
}

void OHMS::GSbattleGround::loadCanPlayHead(unsigned short num) {
	this->CanPlayHead = new sf::Sprite[num];
	this->CanPlayHeadTx.loadFromFile("assets/character/heads/tex.png");
	this->CanPlayHeadTx.setSmooth(true);
	this->CanPlayHeadTx.generateMipmap();
	for (unsigned short i = 0; i < num; ++i) {
		(this->CanPlayHead + i)->setTexture(this->CanPlayHeadTx);
		(this->CanPlayHead + i)->setTextureRect(sf::IntRect(256 * i, 0, 256, 256));
		(this->CanPlayHead + i)->setOrigin(0.0f, 256.0f);
		(this->CanPlayHead + i)->setScale(canPlayHeadSize / 256.0f, canPlayHeadSize / 256.0f);
	}
	return;
}

OHMS::GSbuttonArray::GSbuttonArray() {
	this->window = nullptr;
	this->lastAhoverOn = 0;
	this->lastPressOn = 0;
	this->isActive = false;
	return;
}

OHMS::GSbuttonArray::~GSbuttonArray() {
	this->ButtonList.clear();
	return;
}

void OHMS::GSbuttonArray::initialize(sf::RenderWindow *window) {
	this->ButtonList.clear();
	this->window = window;
	return;
}

unsigned short OHMS::GSbuttonArray::push_back(sf::RenderWindow *window, sf::Texture &texture) {
	this->ButtonList.push_back(OHMS::Button::make_button(texture));
	return (unsigned short)(this->ButtonList.size() - 1);
}
unsigned short OHMS::GSbuttonArray::push_back(OHMS::Button new_button) {
	this->ButtonList.push_back(new_button);
	return (unsigned short)(this->ButtonList.size() - 1);
}

unsigned short OHMS::GSbuttonArray::everyCheck(sf::Event &event, sf::Vector2i mousepos) {
	unsigned short res = 0x0000;
	if (this->isActive) {
		unsigned short nowFoucusCode = this->checkPosOnButton(mousepos);
		bool nowFoucused = (nowFoucusCode & 0x8000);
		unsigned short lastAhoverNum = (this->lastAhoverOn & 0x7FFF);
		unsigned short lastPressNum = (this->lastPressOn & 0x7FFF);
		switch (event.type) {
		case sf::Event::MouseButtonPressed: {
			if (nowFoucused && (event.mouseButton.button == sf::Mouse::Left))
				this->lastPressOn = nowFoucusCode;
		} break;
		case sf::Event::MouseButtonReleased: {
			if (nowFoucused && (lastPressOn == nowFoucusCode) && (event.mouseButton.button == sf::Mouse::Left)) {
				this->ButtonList[lastPressNum].bStatus = 1;
				res = this->lastPressOn;
			}
			this->lastPressOn = lastPressNum;
		} break;
		default: {
			if ((this->lastAhoverOn & 0x8000) && (this->lastAhoverOn != nowFoucusCode))
				this->ButtonList[lastAhoverNum].bStatus = 0;
			if (nowFoucused)
				this->lastAhoverOn = nowFoucusCode;
			else
				this->lastAhoverOn = lastAhoverNum;
		}
		}
		if ((this->lastAhoverOn & 0x8000)) {
			lastAhoverNum = (this->lastAhoverOn & 0x7FFF);
			this->ButtonList[lastAhoverNum].bStatus = 1;
			lastPressNum = (this->lastPressOn & 0x7FFF);
			if ((this->lastPressOn & 0x8000) && (lastAhoverNum == lastPressNum))
				this->ButtonList[lastPressNum].bStatus = 2;
		}
	}
	return res;
}

unsigned short OHMS::GSbuttonArray::checkPosOnButton(sf::Vector2i mousepos) {
	unsigned short ButtonNum = (unsigned short)this->ButtonList.size();
	for (unsigned short i = ButtonNum, j = ButtonNum - 1; i > 0; i--, j--)
		if (this->ButtonList[j].checkOn(mousepos))
			return (0x8000 | j);
	return 0x0000;
}

void OHMS::GSbuttonArray::displayEle(sf::RenderWindow & window) {
	if (!this->isActive)
		return;
	unsigned short ButtonNum = (unsigned short)this->ButtonList.size();
	for (unsigned short i = 0; i < ButtonNum; i++)
		this->ButtonList[i].draw(&window);
	return;
}

unsigned short OHMS::GSbuttonArray::size() {
	return (unsigned short)(this->ButtonList.size());
}
void OHMS::GSbuttonArray::setPosition(unsigned short button_rank, float x, float y) {
	this->ButtonList[button_rank].setPosition(x, y);
	return;
}
void OHMS::GSbuttonArray::setScale(unsigned short button_rank, float x, float y) {
	this->ButtonList[button_rank].setScale(x, y);
	return;
}
void OHMS::GSbuttonArray::setStyle(unsigned short button_rank, unsigned char nAhoverRGB, unsigned char nPressRGB, unsigned char nAhoverAlpha, unsigned char nPressAlpha) {
	this->ButtonList[button_rank].setStyle(nAhoverRGB, nPressRGB, nAhoverAlpha, nPressAlpha);
	return;
}
void OHMS::GSbuttonArray::setOrigin(unsigned short button_rank, float x, float y) {
	this->ButtonList[button_rank].setOrigin(x, y);
	return;
}

void OHMS::GSbuttonArray::setText(unsigned char button_rank, sf::Font &font, sf::String str, unsigned int csize, bool enable_text) {
	this->ButtonList[button_rank].setText(font, str, csize, enable_text);
	return;
}

void OHMS::GSbuttonArray::setTextPosition(unsigned short button_rank, float x, float y) {
	sf::Vector2f pos = this->ButtonList[button_rank].button_sprite.getPosition();
	this->ButtonList[button_rank].button_text.setPosition(pos.x + x, pos.y + y);
	return;
}

void OHMS::Lihui::veryFirstSet(std::string path) {
	this->texture.loadFromFile(path + "picture/main.png");
	this->texture.setSmooth(true);
	OHMS::filescanf(path + "picture/info.txt", "%u%u%u", &this->middleX, &this->middleY, &this->faceY);
	this->transform.setOrigin((float)this->middleX, (float)this->middleY);
	float assistRate = 200.0f / (this->middleY - this->faceY);
	this->transform.setScale(assistRate, assistRate);
	this->transform.setPosition(winWidth / 2.0f - 340.0f, winHight / 2.0f - 40.0f);

	this->m_alpha = 255;
	g_lihuishader->setUniform("alpha", 1.0f);
	g_lihuishader->setUniform("asBattle", 0);

	float width = static_cast<float>(this->texture.getSize().x);
	float height = static_cast<float>(this->texture.getSize().y);

	this->va.clear();
	this->va.append(sf::Vertex(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(0.0f, 0.0f)));
	this->va.append(sf::Vertex(sf::Vector2f(0.0f, 1024.0f), sf::Vector2f(0.0f, 1024.0f)));
	this->va.append(sf::Vertex(sf::Vector2f(512.0f, 0.0f), sf::Vector2f(512.0f, 0.0f)));
	this->va.append(sf::Vertex(sf::Vector2f(512.0f, 1024.0f), sf::Vector2f(512.0f, 1024.0f)));
	this->va.append(sf::Vertex(sf::Vector2f(1024.0f, 0.0f), sf::Vector2f(1024.0f, 0.0f)));
	this->va.append(sf::Vertex(sf::Vector2f(1024.0f, 1024.0f), sf::Vector2f(1024.0f, 1024.0f)));
	this->va.setPrimitiveType(sf::PrimitiveType::TriangleStrip);
	
	return;
}

void OHMS::Lihui::setPos(sf::Vector2f pos) {
	return this->transform.setPosition(pos);
}

void OHMS::Lihui::setScale(float rate) {
	float assistRate = rate * 200.0f / (this->middleY - this->faceY);
	this->transform.setScale(assistRate, assistRate);
	return;
}

void OHMS::Lihui::setAlpha(unsigned int alpha) {
	this->m_alpha = alpha;
	g_lihuishader->setUniform("alpha", alpha / 255.0f);
	return;
}

void OHMS::Lihui::displayIn(sf::RenderWindow & window) {
	sf::RenderStates states;
	states.texture = &(this->texture);
	states.transform = this->transform.getTransform();
	states.shader = g_lihuishader.get();
	return window.draw(this->va, states);
}

void OHMS::Lihui::setAsBattle() {
	g_lihuishader->setUniform("asBattle", 1);
	//this->va[4].color.a = 0;
	//this->va[5].color.a = 0;
	/*
	sf::Image im = this->texture.copyToImage();
	sf::Vector2u size = this->texture.getSize();
	sf::Color pix;
	unsigned int i = this->middleX + (this->middleY - this->faceY) / 7, j;
	float alpha, whole = (this->middleY - this->faceY) * 1.5f;
	for (int k = (int)whole; i < size.x; ++i, --k) {
		if (k < 0) k = 0;
		alpha = k / whole;
		alpha = k / whole;
		for (j = 0; j < size.y; ++j) {
			pix = im.getPixel(i, j);
			pix.a = (unsigned char)(pix.a * alpha);
			im.setPixel(i, j, pix);
		}
	}
	this->texture.loadFromImage(im);*/
	return;
}

void OHMS::AllLihui::veryFirstSet(unsigned short num) {
	this->lihuiNum = num;
	this->lihui = new OHMS::Lihui[num];
	return;
}

void OHMS::AllLihui::setLihui(unsigned short num, std::string path) {
//	if (num < this->lihuiNum) {
		return (this->lihui + num)->veryFirstSet(path);
//	}
	return;
}

void OHMS::AllLihui::setPos(unsigned short num, sf::Vector2f pos) {
//	if (num < this->lihuiNum) {
		return (this->lihui + num)->setPos(pos);
//	}
	return;
}

void OHMS::AllLihui::setScale(unsigned short num, float rate) {
//	if (num < this->lihuiNum) {
		return (this->lihui + num)->setScale(rate);
//	}
	return;
}

void OHMS::AllLihui::setAlpha(unsigned short num, unsigned int alpha) {
///	if (num < this->lihuiNum) {
		return (this->lihui + num)->setAlpha(alpha);
//	}
	return;
}

void OHMS::AllLihui::setAsBattle(unsigned short num) {
//	if (num < this->lihuiNum) {
		return (this->lihui + num)->setAsBattle();
//	}
	return;
}

void OHMS::AllLihui::displayIn(unsigned short num, sf::RenderWindow & window) {
//	if (num < this->lihuiNum) {
		return (this->lihui + num)->displayIn(window);
//	}
	return;
}
