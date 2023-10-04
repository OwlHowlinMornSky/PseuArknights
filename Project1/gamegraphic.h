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

#include "OHMS.h"
#include <SFML/Graphics.hpp>
#include <stack>
#include "btncore.h"

// 窗口大小
#define winWidth (1280)
#define winHight (900)

// battle时下面的操作栏大小
#define canPlayHeadSize (106)

// 实体动画
#define ENTITY_ANIMATION_DEFAULT 0
#define ENTITY_ANIMATION_SET 1
#define ENTITY_ANIMATION_IDLE 2
#define ENTITY_ANIMATION_ATTACK 3
#define ENTITY_ANIMATION_DIE 4
#define ENTITY_ANIMATION_MOVE 5

namespace OHMS {

	class GSocket; // 图形结构插槽
	class GSROOT; // 图形结构根节点
	class GSsprite; // 图形结构-sprite
	class GStext; // 图形结构-text
	class GSbuttonArray; // 图形结构-按键阵列
	class GSframeAnimaCtrl; // 图形结构-帧动画控制器
	class GSbattleGround; // 图形结构-战场
	
	class GSocket {
		friend class GSROOT;
		friend class GSsprite;
		friend class GStext;
		friend class GSbuttonArray;
		friend class GSframeAnimaCtrl;
		friend class GSbattleGround;
		friend class AllAnime;
	private:
		GSocket* parent; // 父节点
		sf::Vector2f posToParent, posAbs; // 相对位置； 绝对位置
		sf::Vector2f scaleToParent, scaleAbs; // 相对缩放； 绝对缩放
		unsigned int alphaAbs, alphaToParent; // 相对透明度； 绝对透明度
		float rotationToParent, rotationAbs; // 相对旋转； 绝对旋转
		GSocket* subGS1, * subGS2; // 左子节点； 右子节点
		virtual void displayEle(sf::RenderWindow &window); // 显示视觉元素（由子类决定）
		virtual void updatePosEle(); // 更新视觉元素的位置（由子类决定）
		void updatePos(); // 更新位置
		virtual void updateAlphaEle(); // 更新视觉元素的透明度（由子类决定）
		void updateAlpha(); // 更新透明度
		virtual void updateScaleEle(); // 更新视觉元素的缩放（由子类决定）
		void updateScale(); // 更新缩放
		virtual void updateRotationEle(); // 更新视觉元素的旋转（由子类决定）
		void updateRotation(); // 更新旋转
	public:
		GSocket();
		~GSocket();

		void setSubGS(bool num, OHMS::GSocket* gs); // 设置子节点（用指针）
		OHMS::GSocket* getSubGS(bool num); // 取得子节点（指针）

		void setPosToParent(sf::Vector2f pos); // 设置相对位置
		void setScaleToParent(sf::Vector2f scale); // 设置相对缩放

		void displayIn(sf::RenderWindow& window); // 在window中显示图形结构（以本节点为根的子树）
	};

	class GSROOT : public GSocket {
	public:
		void setPosAbs(sf::Vector2f pos); // 注意：根节点必须调用这个，不能使用setPosToParent
		void setAlphaAbs(unsigned int alpha); // 注意：根节点必须调用这个，/*不能使用setAlphaToParent*/
	};

	class GSsprite : public GSocket {
	private:
		sf::Texture* texture; // 材质指针（以后应当实现材质管理器，使用指针直接初始化，而不是在这个类下new sf::Texture）
		sf::Sprite sprite;
		sf::Vector2f orgRate; // 原点位置率
		void displayEle(sf::RenderWindow& window); // 重载父类
		void updatePosEle(); // 重载父类
		void updateScaleEle(); // 重载父类
	public:
		GSsprite(sf::Texture& tx);
		~GSsprite();

		void updateOrg(); // 更新原点（以原点位置率）
		void setOrgRate(sf::Vector2f rate); // 设置原点位置率

		sf::Sprite* getSprite(); // 取得sprite的指针
	};

	class GStext : public GSocket {
	private:
		sf::Text text;
		sf::Vector2f orgRate; // 原点位置率
		void displayEle(sf::RenderWindow& window); // 重载父类
		void updatePosEle(); // 重载父类
		void updateScaleEle(); // 重载父类
		void updateAlphaEle(); // 重载父类
	public:
		GStext();
		~GStext();

		void updateOrg(); // 更新原点（以原点位置率）
		void setOrgRate(sf::Vector2f rate); // 设置原点位置率

		sf::Text* getText(); // 取得text的指针
	};

	class GSbuttonArray : public GSocket {
	private:
		sf::RenderWindow* window;
		std::vector<OHMS::Button> ButtonList;
		unsigned short lastAhoverOn, lastPressOn;
		unsigned short checkPosOnButton(sf::Vector2i mousepos);
	public:
		bool isActive;

		GSbuttonArray();
		~GSbuttonArray();

		void initialize(sf::RenderWindow* window);

		unsigned short push_back(OHMS::Button new_button); // 推入新按钮，返回编号 
		unsigned short push_back(sf::RenderWindow* window, sf::Texture& texture); // 推入材质构造新按钮，返回编号 

		unsigned short size();
		void setStyle(unsigned short button_rank, unsigned char nAhoverRGB, unsigned char nPressRGB, unsigned char nAhoverAlpha = 255, unsigned char nPressAlpha = 255); // 设置格式（见btncore）
		void setPosition(unsigned short button_rank, float x, float y); // 设置位置（见btncore）
		void setScale(unsigned short button_rank, float x, float y); // 设置缩放（见btncore）
		void setOrigin(unsigned short button_rank, float x, float y); // 设置原点（见btncore）
		void setText(unsigned char button_rank, sf::Font& font, sf::String str, unsigned int csize, bool enable_text = true); // 设置前景文本（见btncore）
		void setTextPosition(unsigned short button_rank, float x, float y); // 设置前景文本位置（见btncore）

		unsigned short everyCheck(sf::Event& event, sf::Vector2i mousepos); // 检查 
		
		void displayEle(sf::RenderWindow& window);
	};

	class GSframeAnima {
		friend class GSframeAnimaCtrl;
	private:
		unsigned short frameCnt, frameFlag;
		sf::Texture texture;
		sf::Sprite sprite;
	public:
		bool over, loop;
		GSframeAnima();
		GSframeAnima(const std::string path);
		~GSframeAnima();

		void setAnimation(const std::string path); // 以文件路径加载动画（严格嵌入，禁止代码复用）
		sf::Texture* getTexture(); // 取得材质（指针）

		void update(unsigned short frameN); // 推进frameN帧
		void update(); // 推进1帧
		void displayIn(sf::RenderWindow& window);
		void reset(); // 重置
	};

	class GSframeAnimaCtrl : public GSocket {
		friend class AllAnime;
		friend class GSbattleGround;
	private:
		OHMS::GSframeAnima* animation;
		unsigned short animaCnt, animaFlag;
		void updatePosEle();
		void updateScaleEle();
		bool turnLeft;
		sf::Texture* ShadowTX;
		sf::Sprite Shadow;
		float shadowR;
		sf::Vector2f BattlePos;
	public:
		GSframeAnimaCtrl();
		~GSframeAnimaCtrl();
		void veryFirstSet(const std::string path, OHMS::GSocket* par); // 以文件路径和父指针初始化
		void aNewSet(OHMS::GSframeAnimaCtrl* orgFrame); // 以存在的变量初始化（未来应当改写为构造函数）
		void changeAnimation(unsigned short aniN); // 改变播放的动画
		unsigned short getAnimation(); // 取得正在播放的编号
		void update(unsigned short frameN); // 推进frameN帧
		void update(); // 推进1帧；
		void setTurnLeft(bool turn); // 设置左右翻转
		void Turn(); // 左右翻转
		void displayIn(sf::RenderWindow& window);
		void setShadowR(float r); // 设置阴影大小
		bool playOver(); // 取得播放状态
		float playRate();
		OHMS::GSframeAnima* getFrameAnima(const unsigned short num); // 取得对应编号的GSframeAnima（指针）
	};

	class AllAnime : public GSocket {
		friend class EntityMain;
		friend class GSbattleGround;
	private:
		OHMS::GSbattleGround *myBattleGround;
		std::vector<OHMS::GSframeAnimaCtrl*> anime;
		std::stack<OHMS::GSframeAnimaCtrl*> ptrRecycle;
		sf::Texture ShadowTX;
		bool needSort;
	public:
		AllAnime();
		~AllAnime();
		OHMS::GSframeAnimaCtrl * addAnime(); // 添加动画
		void delAnime(OHMS::GSframeAnimaCtrl* delObj);
		void setAnime(const unsigned short n, const std::string path); // 设置第n个动画
		OHMS::GSframeAnimaCtrl* getAnimeCtrl(const unsigned short n); // 取得GSframeAnimaCtrl（指针）
		void update(); // 推进
		void displayIn(sf::RenderWindow &window);
		void setAnimePos(OHMS::GSframeAnimaCtrl * anime, sf::Vector2f pos, bool upLand); // 设置一个动画的位置
		void setAnimePos(const unsigned short num, sf::Vector2f pos, bool upLand, bool yeah = true); // 设置一个动画的位置
		static bool SORT_COMPARE(OHMS::GSframeAnimaCtrl * pi, OHMS::GSframeAnimaCtrl * pj); // 图层排序比较函数
		void SORT(); // 图层排序 
	};

	class Lihui { // 立绘
	private:
		sf::Texture texture;
		//sf::Sprite sprite;
		sf::VertexArray va;
		sf::Transformable transform;
		unsigned int middleX, faceY, middleY;
		unsigned int m_alpha;
	public:
		void veryFirstSet(std::string path); // 以文件路径设置立绘
		void setPos(sf::Vector2f pos); // 设置中心点位置
		void setScale(float rate); // 设置缩放
		void setAlpha(unsigned int alpha); // 设置透明度
		void displayIn(sf::RenderWindow &window);
		void setAsBattle(); // 设置战场上渐变透明（严格嵌入，禁止代码复用）
	};

	class AllLihui {
	private:
		OHMS::Lihui * lihui;
		unsigned short lihuiNum;
	public:
		void veryFirstSet(unsigned short num); // 以立绘数量初始化
		void setLihui(unsigned short num, std::string path); // 设置第num个立绘
		void setPos(unsigned short num, sf::Vector2f pos);  // 设置第num个立绘
		void setScale(unsigned short num, float rate); // 设置第num个立绘
		void setAlpha(unsigned short num, unsigned int alpha); // 设置第num个立绘
		void setAsBattle(unsigned short num); // 设置第num个立绘
		void displayIn(unsigned short num, sf::RenderWindow &window);
	};

	class GSbattleGround : public GSocket {
	private:
		sf::Texture levelGroundTX;
		sf::Sprite levelGround;
	public:
		sf::Texture uiTX, finishTX;
		void setBackground(std::string path);

	private:
		OHMS::AllAnime anime;
		OHMS::GSframeAnimaCtrl * animeORG;
		unsigned short animeORGnum;
	public:
		sf::Texture CanPlayHeadTx;
		sf::Sprite * CanPlayHead;
		void initAnimeORG(const unsigned short num); // 初始化待命动画数量
		void setAnimeOrg(const unsigned short num, std::string path); // 设置待命动画
		OHMS::GSframeAnimaCtrl * getAnimeOrg(const unsigned short num); // 取得待命动画
		void initAllAnime(); // 初始化AllAnime
		OHMS::GSframeAnimaCtrl * animeJoin(const unsigned short num); // 加入待命动画
		OHMS::AllAnime * getAnime(); // 取得AllAnime（指针）
		void loadCanPlayHead(unsigned short num); // 初始化底部操作栏头像（以总数量）

	private:
		sf::Vector3f cameraPos, cameraFront, cameraX, cameraY; // graphic main info
	public:
		void setCamera(sf::Vector3f cameraPos, float downAngle); // set graphic main info
		sf::Vector2f posMap3dTo2d(const sf::Vector3f pos3d); // 将3维坐标映射到2维（请先setCamera） 
		sf::Vector2f posMap2dTo3d(const sf::Vector2f pos2d, float z); // 将2维坐标映射到3维，高度为z（请先setCamera）  

	public:
		OHMS::AllLihui lihui;
		GSbattleGround();
		~GSbattleGround();

		void update();

		void setAnimaPosition(sf::Vector3f pos, OHMS::GSframeAnimaCtrl* object); // 设置AllAnime中的某一个动画

		void displayGroundIn(sf::RenderWindow & window);
		void displayAnimeIn(sf::RenderWindow & window);
	};

	void initLihuiShader();

}
