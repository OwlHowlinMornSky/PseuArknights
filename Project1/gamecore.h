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

#define optionDist (3600.0f)

#include "gameaudio.h"
#include "gamegraphic.h"
#include <math.h>
#include "gamestatemachine.h"

#define cloRadius 0.05f

namespace OHMS {
	class EntityStateMachine;

	class EntityMain;
	class EntityFilter;
	class AllEntity;
	class BattleGroundMain;
	class PileMain;
	class AllPile;
	class SummonCtrl;
	class OptionClass;
	class OptionCore;

	class Range {
	public:
		unsigned char dist;
		sf::Vector2f origin;
		std::vector<unsigned char> range;
		int backPile;
		int type;

		Range();

		bool checkEntityInME(OHMS::EntityMain* theEntity);
		bool checkPos2fInME(sf::Vector2f pos2f); // 检查 pos2f 对应坐标是否在 this 内 (使用 checkPos2fInRange ) 
		static bool checkPos2fInRange(OHMS::Range& range, sf::Vector2f pos2f); // 检查 pos2f 对应坐标是否在 range 内

		void setDist(unsigned char dist);
		void setOrigin(sf::Vector2f pos);
		void setBackPile(int bp);
		void setRange(int length, ...);
		void setRangePushBack(int nextRange);
		void clear();
	};

	class PileFilter {
	public:
		class style {
		public:
			static const int PLAIN = 1;
			static const int HIGH = 2;
		};

		int wantStyle;

		PileFilter();

		bool checkPile(OHMS::PileMain * pile);
	};

	class EntityAttribute {
	public:
		float health, healthMax;
		float attack;
		float defende;
		float magicalDef;
		EntityAttribute();
	};

	class EntityFilter {
		friend class EntityMain;
	public:
		class Style {
		public:
			static const int isEnemy = 0x00000001;
			static const int isOperator = 0x00000002;
			static const int isRemoteAtk = 0x00000004;
			static const int isShortAtk = 0x00000008;
		};

		int wantStyle;

		EntityFilter();

		bool checkEntity(OHMS::EntityMain* entity);
	};

	class EntityArm {
	private:
		int getNum;
		OHMS::EntityMain** goals;
		OHMS::EntityMain* myBody;
		bool attacked, nice;
	public:
		class Type {
		public:
			static const int Real_Damage = 0x00000000;
			static const int Phys_Damage = 0x00000001;
			static const int Magi_Damage = 0x00000002;
			static const int Have_Track = 0x00000010;
			static const int NotHave_Track = 0x00000020;
		};
		int attackType;
		EntityArm();

		void veryFirstSet(int n, OHMS::EntityMain* theBody);

		void aNewRound();

		void AttackIt(OHMS::EntityMain* target);

		void getGoal();
		bool check();

		void update();
	};

	class EntityMain {
		friend class BattleGroundMain;
		friend class EntityFilter;
		friend class EntityArm;
		friend class EntityState_ATTACK;
		friend class EntityState_DIE;
		friend class EntityState_SET;
	private:
		OHMS::AllEntity *myTeam;
		OHMS::GSframeAnimaCtrl *myAnime;
		sf::Vector2f position;
		OHMS::EntityStateMachine* STM;
		bool IamDying, Leave, normal;
		OHMS::Range atkRange;
		OHMS::PileFilter pileFliter;
		OHMS::EntityFilter entityFilter;
		unsigned char dist;
		OHMS::EntityArm* arm;
		float speed;
	public:
		int id;
		bool mustStop = false, isEnemy;
		OHMS::EntityAttribute attribute;
		EntityMain();
		~EntityMain();

		void loadFromFile(const std::string & path);

		void initPtr(OHMS::AllEntity *theTeam, OHMS::GSframeAnimaCtrl *theAnime);

		void setPosition(sf::Vector2f pos);
		void setPosition(float x, float y);
		void getPosition(float* x, float* y);
		sf::Vector2f getPosition();

		OHMS::GSframeAnimaCtrl* getAnime();
		OHMS::Range* getRange();
		OHMS::PileFilter* getPileFliter();

		bool canAttack();
		bool isDied();
		bool HaveToLeave();

		void LeaveNow();

		void update();
		void getReady();

		unsigned short getCost();

		void veryFirstSetSTM(bool isEnemy);

		void changeAnime(const unsigned short aniN);
		void changeDist(unsigned char ndist);
	};

	class AllEntity {
		friend class EntityMain;
		friend class BattleGroundMain;
		friend class EntityArm;
	private:
		OHMS::BattleGroundMain *theBattleGround;
		std::vector<OHMS::EntityMain*> entity;
		std::stack<OHMS::EntityMain*> ptrRecycle;
	public:
		AllEntity();
		~AllEntity();

		void update();

		OHMS::EntityMain* test___getEntity();

		OHMS::EntityMain* addEntity();
		void delEntity(OHMS::EntityMain * delObj);
	};

	class PileMain {
	public:
		class Type {
			static const int PROTECT = 1;
			static const int ENEMYIN = 2;
		};
		int type, id;
		bool high, canPlace, bothHighAndPlain;

		sf::Vector2f pos[2][2];
		sf::VertexArray* arrage;

		OHMS::EntityMain* theOpeOnMe;
		int theOpeID;

		void(*func) (OHMS::PileMain * pile, ...);
		static void func_DEFAULT(OHMS::PileMain * pile, ...);
		static void func_PROTECT(OHMS::PileMain * pile, ...);
		static void func_ENEMYIN(OHMS::PileMain * pile, ...);

		void specialPileAs(int type);

		PileMain();

		void compPoint(unsigned short x, unsigned short y, OHMS::GSbattleGround * gsbg);
		void displayIn(sf::RenderWindow& window, sf::Texture &texture);
	};
	 
	class AllPile {
	private:
		OHMS::BattleGroundMain * myBattleGround;
		OHMS::PileMain * piles;
		sf::Texture texture[5];
		int* showType;
		
	public:
		class SHOWTYPE {
		public:
			static const int DEFAULT = 0;
			static const int CAN_PLACE = 1;
			static const int CAN_ATTACK = 2;
			static const int CAN_HEAL = 3;
			static const int SKILL = 4;
		};
		AllPile();
		~AllPile();

		void readFromFile(std::string path, OHMS::BattleGroundMain * theBattleGround);
		bool isHighLand(unsigned short x, unsigned short y);
		bool canPlace(unsigned short x, unsigned short y);

		void setOpeOn(int opeID, OHMS::EntityMain * ptr, unsigned short x, unsigned short y);
		int getOpeOn(unsigned short x, unsigned y);
		OHMS::EntityMain* getOpeOnPtr(unsigned short x, unsigned short y);
		void delOpeOn(unsigned short x, unsigned short y);
		OHMS::EntityMain* delAndgetOpeOnPtr(unsigned short x, unsigned short y);

		void clearShow();
		void addShow(int type, OHMS::Range* range);
		void addShow(int type, OHMS::PileFilter* pile);
		void showArrange(sf::RenderWindow& window);

		OHMS::PileMain* getPile(sf::Vector2u pos);
	};

	class SummonCtrl {
		friend class BattleGroundMain;
	private:
		int cnt, allcnt, already;
		OHMS::BattleGroundMain* theBG;
	public:
		SummonCtrl(OHMS::BattleGroundMain* bg);
		void update();
	};

	class OptionCore {
	public:
		OHMS::Range tmpRange;
		sf::Sprite optingOpe;
		OHMS::OptionClass * myPt;
		sf::Vector2i mousePos, lastPos;
		sf::Sprite chooseBox, choosePoint, chooseDist;
		bool settingOpe, mouseButtonPressed, finalChoose, optingOpeTurn, showOnBG;
		float optingOpeMove, optingOpeMoveReal;
		sf::Vector2f comeFrom, center;
		sf::Vector2u finalPos;
		OptionCore(OHMS::OptionClass * thePt);
		void check(sf::Event event, sf::Vector2i mouPos);
		void update();
		void displayIn(sf::RenderWindow &window);
	};

	class daojishi {
		sf::Text t;
	public:
		daojishi();

		void setSecond(float second);
		sf::Text& getText(float x, float y);

		void setFont(sf::Font* f);
	};

	class costDisplay {
		sf::Text t;
		sf::RectangleShape s;
	public:
		costDisplay();

		void setNumber(int num, float rate);
		sf::Text& getText();
		sf::RectangleShape& getShape();

		void setFont(sf::Font* f);
	};

	class OptionClass {
		friend class BattleGroundMain;
		friend class OptionCore;
	private:
		OHMS::BattleGroundMain * theBG;
		OHMS::GSbattleGround * theGraphic;
		int *CanPlayList, CanPlayFocus;
		unsigned short CanPlayNum;
		bool *CanPlayAble, CanPlayShowInfo, justShowInfo;
		int *CanPlayOnBG;
		OHMS::EntityMain* showIt;
		float CanPlayInfoPos, CanPlayInfoPosReal;
		float *CanPlayHeadPos, *CanPlayHeadPosReal;
		sf::RectangleShape CanPlayHeadBK;
		sf::Texture HeadFTX; sf::Sprite HeadFFF;
		OHMS::daojishi jishi;

		sf::Sprite pauseBtn, speedBtn, setBtn;
	public:
		OHMS::OptionCore * opc;
		OptionClass(OHMS::BattleGroundMain * myBG, OHMS::GSbattleGround * myGraphic);
		~OptionClass();
		void loadCanPlayList();
		bool setFocus(unsigned int n);
		void setCanPlayInfoPos(float pos);
		void setCanPlayInfoPosOrg();
		short isFocus();
		void cclFocus();
		void setCanPlayInfo();
		sf::Texture * getFocusTexture();
		bool setIt(sf::Vector2u pos, unsigned char dist);
		bool canSetIt(sf::Vector2u pos);
		void comeBack(unsigned short n);
		void update();
		void check(sf::Event event, sf::Vector2i moupos);
		void diplayIn(sf::RenderWindow &window);
	private:
		void displayCanP(sf::RenderWindow &window);
		void displayInfo(sf::RenderWindow &window);
		void displayInfoOnBG(sf::RenderWindow &window);
	};

	class OperatorReset {
	private:
		unsigned short openum;
		unsigned int* opeSetLeftTime;
		unsigned short cost;
		unsigned int costLoad, costDelta;
		OHMS::costDisplay dis;
	public:
		OperatorReset();
		~OperatorReset();

		void veryFirstSet(unsigned short n, unsigned int costdelta, unsigned int costFirst, sf::Font* f);

		void getBack(unsigned short n);
		bool canGo(unsigned short n);
		float leftTime(unsigned short n);
		bool giveoutCost(unsigned short k);

		unsigned short hasCost();

		void update();

		void displayIn(sf::RenderWindow& window);
	};

	class BattleGroundMain {
		friend class EntityMain;
		friend class AllPile;
		friend class OptionClass;
		friend class OptionCore;
		friend class AllEntity;
	public:
		OHMS::GSbattleGround myGraphic;
		OHMS::AudioCtrl *myAudio;
		std::string levelName, levelID;
		sf::Font* font;
		void setEntityPos(sf::Vector2f pos2d, OHMS::EntityMain * entity);

		bool nowPause;
		sf::Text pauseText;

	private:
		sf::Vector2u battleGroundSize; // ground info
		OHMS::AllPile pile;
	public:
		void readFromFile(const std::string path);
		sf::Vector2u getBattleGroundSize();
		int rank, life = 3, enemykilled;
		bool over;
		sf::Text headline;

	private:
		OHMS::EntityMain *entityOrgOpe, *entityOrgEne; // entity info
		unsigned short entityOrgOpeCnt, entityOrgEneCnt;
		OHMS::AllEntity entity;
		OHMS::SummonCtrl* summon;
		OHMS::OperatorReset operatorReset;
		bool setOperatorIn(unsigned short opeID, sf::Vector2u pos, unsigned char dist);
		bool canSetOperatorIn(unsigned short opeID, sf::Vector2u pos);
	public:
		OHMS::EntityMain* entityJoin(const bool isEnemy, const unsigned short entityID, sf::Vector2f pos);
		void initEntityOrgOpe(const unsigned short num);
		void initEntityOrgEne(const unsigned short num);
		void initEntityOrg(const unsigned short OpeNum, const unsigned short EneNum);
		void setEntityOrg(const bool isEnemy, const unsigned short entityID, std::string path);
		OHMS::EntityMain* getEntityOrgOpe(const unsigned short num);
		OHMS::EntityMain* getEntityOrgEne(const unsigned short num);

	public:
		OHMS::OptionClass * op;
		void loadCanPlayList();

		OHMS::AllEntity* test___getEntity();

	public:
		unsigned int frameBufferMax;
		bool speed_shift;
		BattleGroundMain();
		~BattleGroundMain();
		void update();
		void displayIn(sf::RenderWindow &window);

		void changePause();

		void setFont(sf::Font* f);

		void setSpeed(unsigned char status);
	};

}
