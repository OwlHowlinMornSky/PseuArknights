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

// ���ڴ�С
#define winWidth (1280)
#define winHight (900)

// battleʱ����Ĳ�������С
#define canPlayHeadSize (106)

// ʵ�嶯��
#define ENTITY_ANIMATION_DEFAULT 0
#define ENTITY_ANIMATION_SET 1
#define ENTITY_ANIMATION_IDLE 2
#define ENTITY_ANIMATION_ATTACK 3
#define ENTITY_ANIMATION_DIE 4
#define ENTITY_ANIMATION_MOVE 5

namespace OHMS {

	class GSocket; // ͼ�νṹ���
	class GSROOT; // ͼ�νṹ���ڵ�
	class GSsprite; // ͼ�νṹ-sprite
	class GStext; // ͼ�νṹ-text
	class GSbuttonArray; // ͼ�νṹ-��������
	class GSframeAnimaCtrl; // ͼ�νṹ-֡����������
	class GSbattleGround; // ͼ�νṹ-ս��
	
	class GSocket {
		friend class GSROOT;
		friend class GSsprite;
		friend class GStext;
		friend class GSbuttonArray;
		friend class GSframeAnimaCtrl;
		friend class GSbattleGround;
		friend class AllAnime;
	private:
		GSocket* parent; // ���ڵ�
		sf::Vector2f posToParent, posAbs; // ���λ�ã� ����λ��
		sf::Vector2f scaleToParent, scaleAbs; // ������ţ� ��������
		unsigned int alphaAbs, alphaToParent; // ���͸���ȣ� ����͸����
		float rotationToParent, rotationAbs; // �����ת�� ������ת
		GSocket* subGS1, * subGS2; // ���ӽڵ㣻 ���ӽڵ�
		virtual void displayEle(sf::RenderWindow &window); // ��ʾ�Ӿ�Ԫ�أ������������
		virtual void updatePosEle(); // �����Ӿ�Ԫ�ص�λ�ã������������
		void updatePos(); // ����λ��
		virtual void updateAlphaEle(); // �����Ӿ�Ԫ�ص�͸���ȣ������������
		void updateAlpha(); // ����͸����
		virtual void updateScaleEle(); // �����Ӿ�Ԫ�ص����ţ������������
		void updateScale(); // ��������
		virtual void updateRotationEle(); // �����Ӿ�Ԫ�ص���ת�������������
		void updateRotation(); // ������ת
	public:
		GSocket();
		~GSocket();

		void setSubGS(bool num, OHMS::GSocket* gs); // �����ӽڵ㣨��ָ�룩
		OHMS::GSocket* getSubGS(bool num); // ȡ���ӽڵ㣨ָ�룩

		void setPosToParent(sf::Vector2f pos); // �������λ��
		void setScaleToParent(sf::Vector2f scale); // �����������

		void displayIn(sf::RenderWindow& window); // ��window����ʾͼ�νṹ���Ա��ڵ�Ϊ����������
	};

	class GSROOT : public GSocket {
	public:
		void setPosAbs(sf::Vector2f pos); // ע�⣺���ڵ����������������ʹ��setPosToParent
		void setAlphaAbs(unsigned int alpha); // ע�⣺���ڵ������������/*����ʹ��setAlphaToParent*/
	};

	class GSsprite : public GSocket {
	private:
		sf::Texture* texture; // ����ָ�루�Ժ�Ӧ��ʵ�ֲ��ʹ�������ʹ��ָ��ֱ�ӳ�ʼ�������������������new sf::Texture��
		sf::Sprite sprite;
		sf::Vector2f orgRate; // ԭ��λ����
		void displayEle(sf::RenderWindow& window); // ���ظ���
		void updatePosEle(); // ���ظ���
		void updateScaleEle(); // ���ظ���
	public:
		GSsprite(sf::Texture& tx);
		~GSsprite();

		void updateOrg(); // ����ԭ�㣨��ԭ��λ���ʣ�
		void setOrgRate(sf::Vector2f rate); // ����ԭ��λ����

		sf::Sprite* getSprite(); // ȡ��sprite��ָ��
	};

	class GStext : public GSocket {
	private:
		sf::Text text;
		sf::Vector2f orgRate; // ԭ��λ����
		void displayEle(sf::RenderWindow& window); // ���ظ���
		void updatePosEle(); // ���ظ���
		void updateScaleEle(); // ���ظ���
		void updateAlphaEle(); // ���ظ���
	public:
		GStext();
		~GStext();

		void updateOrg(); // ����ԭ�㣨��ԭ��λ���ʣ�
		void setOrgRate(sf::Vector2f rate); // ����ԭ��λ����

		sf::Text* getText(); // ȡ��text��ָ��
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

		unsigned short push_back(OHMS::Button new_button); // �����°�ť�����ر�� 
		unsigned short push_back(sf::RenderWindow* window, sf::Texture& texture); // ������ʹ����°�ť�����ر�� 

		unsigned short size();
		void setStyle(unsigned short button_rank, unsigned char nAhoverRGB, unsigned char nPressRGB, unsigned char nAhoverAlpha = 255, unsigned char nPressAlpha = 255); // ���ø�ʽ����btncore��
		void setPosition(unsigned short button_rank, float x, float y); // ����λ�ã���btncore��
		void setScale(unsigned short button_rank, float x, float y); // �������ţ���btncore��
		void setOrigin(unsigned short button_rank, float x, float y); // ����ԭ�㣨��btncore��
		void setText(unsigned char button_rank, sf::Font& font, sf::String str, unsigned int csize, bool enable_text = true); // ����ǰ���ı�����btncore��
		void setTextPosition(unsigned short button_rank, float x, float y); // ����ǰ���ı�λ�ã���btncore��

		unsigned short everyCheck(sf::Event& event, sf::Vector2i mousepos); // ��� 
		
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

		void setAnimation(const std::string path); // ���ļ�·�����ض������ϸ�Ƕ�룬��ֹ���븴�ã�
		sf::Texture* getTexture(); // ȡ�ò��ʣ�ָ�룩

		void update(unsigned short frameN); // �ƽ�frameN֡
		void update(); // �ƽ�1֡
		void displayIn(sf::RenderWindow& window);
		void reset(); // ����
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
		void veryFirstSet(const std::string path, OHMS::GSocket* par); // ���ļ�·���͸�ָ���ʼ��
		void aNewSet(OHMS::GSframeAnimaCtrl* orgFrame); // �Դ��ڵı�����ʼ����δ��Ӧ����дΪ���캯����
		void changeAnimation(unsigned short aniN); // �ı䲥�ŵĶ���
		unsigned short getAnimation(); // ȡ�����ڲ��ŵı��
		void update(unsigned short frameN); // �ƽ�frameN֡
		void update(); // �ƽ�1֡��
		void setTurnLeft(bool turn); // �������ҷ�ת
		void Turn(); // ���ҷ�ת
		void displayIn(sf::RenderWindow& window);
		void setShadowR(float r); // ������Ӱ��С
		bool playOver(); // ȡ�ò���״̬
		float playRate();
		OHMS::GSframeAnima* getFrameAnima(const unsigned short num); // ȡ�ö�Ӧ��ŵ�GSframeAnima��ָ�룩
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
		OHMS::GSframeAnimaCtrl * addAnime(); // ��Ӷ���
		void delAnime(OHMS::GSframeAnimaCtrl* delObj);
		void setAnime(const unsigned short n, const std::string path); // ���õ�n������
		OHMS::GSframeAnimaCtrl* getAnimeCtrl(const unsigned short n); // ȡ��GSframeAnimaCtrl��ָ�룩
		void update(); // �ƽ�
		void displayIn(sf::RenderWindow &window);
		void setAnimePos(OHMS::GSframeAnimaCtrl * anime, sf::Vector2f pos, bool upLand); // ����һ��������λ��
		void setAnimePos(const unsigned short num, sf::Vector2f pos, bool upLand, bool yeah = true); // ����һ��������λ��
		static bool SORT_COMPARE(OHMS::GSframeAnimaCtrl * pi, OHMS::GSframeAnimaCtrl * pj); // ͼ������ȽϺ���
		void SORT(); // ͼ������ 
	};

	class Lihui { // ����
	private:
		sf::Texture texture;
		//sf::Sprite sprite;
		sf::VertexArray va;
		sf::Transformable transform;
		unsigned int middleX, faceY, middleY;
		unsigned int m_alpha;
	public:
		void veryFirstSet(std::string path); // ���ļ�·����������
		void setPos(sf::Vector2f pos); // �������ĵ�λ��
		void setScale(float rate); // ��������
		void setAlpha(unsigned int alpha); // ����͸����
		void displayIn(sf::RenderWindow &window);
		void setAsBattle(); // ����ս���Ͻ���͸�����ϸ�Ƕ�룬��ֹ���븴�ã�
	};

	class AllLihui {
	private:
		OHMS::Lihui * lihui;
		unsigned short lihuiNum;
	public:
		void veryFirstSet(unsigned short num); // ������������ʼ��
		void setLihui(unsigned short num, std::string path); // ���õ�num������
		void setPos(unsigned short num, sf::Vector2f pos);  // ���õ�num������
		void setScale(unsigned short num, float rate); // ���õ�num������
		void setAlpha(unsigned short num, unsigned int alpha); // ���õ�num������
		void setAsBattle(unsigned short num); // ���õ�num������
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
		void initAnimeORG(const unsigned short num); // ��ʼ��������������
		void setAnimeOrg(const unsigned short num, std::string path); // ���ô�������
		OHMS::GSframeAnimaCtrl * getAnimeOrg(const unsigned short num); // ȡ�ô�������
		void initAllAnime(); // ��ʼ��AllAnime
		OHMS::GSframeAnimaCtrl * animeJoin(const unsigned short num); // �����������
		OHMS::AllAnime * getAnime(); // ȡ��AllAnime��ָ�룩
		void loadCanPlayHead(unsigned short num); // ��ʼ���ײ�������ͷ������������

	private:
		sf::Vector3f cameraPos, cameraFront, cameraX, cameraY; // graphic main info
	public:
		void setCamera(sf::Vector3f cameraPos, float downAngle); // set graphic main info
		sf::Vector2f posMap3dTo2d(const sf::Vector3f pos3d); // ��3ά����ӳ�䵽2ά������setCamera�� 
		sf::Vector2f posMap2dTo3d(const sf::Vector2f pos2d, float z); // ��2ά����ӳ�䵽3ά���߶�Ϊz������setCamera��  

	public:
		OHMS::AllLihui lihui;
		GSbattleGround();
		~GSbattleGround();

		void update();

		void setAnimaPosition(sf::Vector3f pos, OHMS::GSframeAnimaCtrl* object); // ����AllAnime�е�ĳһ������

		void displayGroundIn(sf::RenderWindow & window);
		void displayAnimeIn(sf::RenderWindow & window);
	};

	void initLihuiShader();

}
